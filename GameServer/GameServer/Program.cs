using System;
using System.Net.Sockets;
using System.Threading;
using System.Text;
using System.Net;
using System.Collections.Generic;

using System.Threading.Tasks;
using System.Diagnostics;
using System.Data;
using System.IO;
using Mono.Data.Sqlite;


namespace GameServer
{

	// TCP: 7578
	// UDP: 7576
	//

	/// <summary>
	/// TODO: 
	/// 	  Have multicast and unicast as two different options
	/// 	  Receive different types of msgs and react to them
	/// 	  
	/// </summary>

	/*
		PRODUCER-CONSUMER
		Queues separate for udp and tcp
		OR
		One shared
		
		Main loop is consumer, receives are producers. Read data in one place, dequeue in another
		Extra points for broadcasting and detecting  a server
		Database 
		Security: xor -> AES key -> TLS/SSL
		Alias key is simple

		Game logic on server: win/lose etc
		
		
	*/

	//TODO: add password to a database


	/// <summary>
	/// Main body of the server
	/// </summary>
	class MainClass
	{
		//--------------------------------------
		//				USERS
		//--------------------------------------

		//Port numbers
		static int UDP_PORT = 7576;
		static int TCP_PORT = 7578;

		//Data structures
		static Dictionary<string, Client> clientDictionary = new Dictionary<string, Client>();	//Dictionary of clients containng a name and connection information

		//Multithreading pattern manager
		static PatternQueue producerConsumerTCP;													//A queue for the prducer/consumer pattern
		static UDPQueue producerConsumerUDP;

		//Race information
		static int highestID = -1;

		//Thread for time
		static Thread thTimer = new Thread(countDown);
		static bool timerIsActive = false;
		static Stopwatch myStopwatch = new Stopwatch();

		//Connections
		static SqliteConnection m_dbConnection;

		//--------------------------------------
		//			LISTENING LOOP
		//--------------------------------------

		//MAIN
		public static void Main(string[] args)
		{
			//Test message
			Console.WriteLine("Hello, I am server! Give me a second to create a database!");

			//DATABASE

			string folder = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
			SqliteConnection.CreateFile("MyDatabase.sqlite");
			m_dbConnection = new SqliteConnection("Data Source=MyDatabase.sqlite;");
			m_dbConnection.Open();

			string sql = "CREATE TABLE highscores (name VARCHAR(20), score INT)";
			SqliteCommand command = new SqliteCommand(sql, m_dbConnection);
			command.ExecuteNonQuery();

			m_dbConnection.Close();

			//DATABASE

			//Initializig a queue of messages which will be past to the producer/consumer manager
			//TCP
			Queue<MessageTCP> msgQueue = new Queue<MessageTCP>();
			producerConsumerTCP = new PatternQueue(msgQueue, new Object());

			//UDP
			Queue<MessageUDP> msgQueueUDP = new Queue<MessageUDP>();
			producerConsumerUDP = new UDPQueue(msgQueueUDP, new Object());


			//Entry point to the networking part
			StartListening();
		}

		//NETWORKING ENTRY POINT
		public static void StartListening()
		{

			//HOST INFORMATION
			IPHostEntry ipHost = Dns.GetHostEntry(Dns.GetHostName());
			IPAddress ipAddress = ipHost.AddressList[0];
			IPEndPoint ipEndPoint = new IPEndPoint(IPAddress.Any, TCP_PORT);

			//CREATE SOCKET TCP
			Socket listenerTCP = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
			listenerTCP.Bind(ipEndPoint);
			listenerTCP.Listen(100);

			//CREATE SOCKET UDP
			Socket listenerUDP = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
			listenerUDP.Bind(new IPEndPoint(IPAddress.Any, UDP_PORT));

			//CONSUMERS
			//Thread for TCP consumer
			var th = new Thread(interpretTCP);
			th.Start();

			//Thread for UDP consumer
			var thUdp = new Thread(interpretUDP);
			thUdp.Start();

			//PRODUCERS
			//LOOP for lisening to the UDP messages
			ReceiveMessagesUDP(listenerUDP);

			//Loop for TCP producer
			while (true)
			{
				allDone.Reset();
				listenerTCP.BeginAccept(new AsyncCallback(AcceptCallbackTCP), listenerTCP);
				allDone.WaitOne();

			}
		}

		///---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		///																						UTILITIES
		///---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//---------------------------------------
		//				TIMER
		//---------------------------------------
		public static void countDown()
		{
			Console.WriteLine("Timer started");

			myStopwatch.Start();

			for (int i = 0; i< 10000; i++)
        	{
            	Thread.Sleep(1);
        	}

			myStopwatch.Stop();
			Console.WriteLine("Time off");
			SendTCP("STR");
		}

		///---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		///																						TCP
		///---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		//TCP
		public static ManualResetEvent allDone = new ManualResetEvent(false);

		//---------------------------------------
		//				REGISTER
		//---------------------------------------
		//TODO: send a client a confirmation that registration is complete
		// In the client wait until a confirmation is received before starting on anyting new
		public static void RegisterClient(Socket sock, String name)
		{

			Console.WriteLine("TCP registartion started");
			if (clientDictionary.ContainsKey(name))
			{
				Client existingInfo = clientDictionary[name];
				if (existingInfo.tcpSock != sock)
				{
					existingInfo.tcpSock = sock;

					clientDictionary[name] = existingInfo;
				}
			}
			else
			{
				Client newClient = new Client();
				newClient.tcpSock = sock;

				clientDictionary.Add(name, newClient);
			}

			//Change this to sendTo
			//SendTCP("TCP registartion complete");
			//Console.WriteLine("Client is registered with TCP");
		}

		//Accept function
		public static void AcceptCallbackTCP(IAsyncResult ar)
		{
			allDone.Set();
			Socket listener = (Socket)ar.AsyncState;
			Socket handle = listener.EndAccept(ar);

			StateObject state = new StateObject();
			state.workSocket = handle;


			//var tcpReceive = new Thread(ReadCallbackTCP);

			handle.BeginReceive(state.buffer, 0, state.bufferSize, 0, new AsyncCallback(ReadCallbackTCP), state);

		}

		//---------------------------------------
		//				JOIN REQUEST
		//---------------------------------------
		public static void Join(string name)
		{ 
			//Increase id as the new player joined
			highestID++;

			//Update player's status
			clientDictionary[name].raceId = highestID;

			//Update everyone that this player is registered
			foreach (string entry in clientDictionary.Keys)
			{
				if(clientDictionary[entry].raceId !=-1)
					SendTCP("JOI " + clientDictionary[entry].raceId.ToString() + entry);
			}

			if (!timerIsActive)
			{
				timerIsActive = true;
				thTimer.Start();
			}
		}

		//---------------------------------------
		//				RECEIVE
		//---------------------------------------
		//Reading data
		public static void ReadCallbackTCP(IAsyncResult ar)
		{
			//Start with an empty msg
			string content = string.Empty;


			StateObject state = (StateObject)ar.AsyncState;
			Socket handle = state.workSocket;
			int readBytes = handle.EndReceive(ar);

			Console.WriteLine(readBytes);

			//If there is data to read
			if (readBytes > 0)
			{

				//state.stringB.Append(Encoding.ASCII.GetString(state.buffer, 0, state.bufferSize));
				content = Encoding.ASCII.GetString(state.buffer, 0, state.bufferSize);
				Console.WriteLine("Server received: '{0}', {1} bytes", content, readBytes);
				state.buffer = new byte[1024];

				//interpretTCP(state, readBytes, content);

				//Produce content and handle?
				MessageTCP msg = new MessageTCP();
				msg.sock = state.workSocket;
				msg.body = content;
				producerConsumerTCP.produce(msg);

				//Loop
				handle.BeginReceive(state.buffer, 0, state.bufferSize, 0, new AsyncCallback(ReadCallbackTCP), state);


				//TEST PRODUCE
				//producerConsumer.produce(content);

			}
			else if (readBytes == 0)
			{
				Console.WriteLine("Socket closed");
			}

		}


		public static void interpretTCP()
		{

			while (true)
			{
				MessageTCP newMsg = producerConsumerTCP.consume();

				if (newMsg !=null)
				{
					Console.WriteLine("Server consumed content of " + newMsg.body);

					string sub = newMsg.body.Substring(0, 3);

					//REGISTER to a server
					if (sub == "REG")
					{
						//string charName = (newMsg.body.Substring(4, newMsg.body.Length - 5));

						string[] elements = newMsg.body.Split(' ');
						string charName = elements[1];
						RegisterClient(newMsg.sock, charName);
                        SendTCP(newMsg.body);
					}

					//JOIN the race
					if (sub == "JOI")
					{
						
						string[] elements = newMsg.body.Split(' ');
						string charName = elements[1];
                        Join(charName);


						//SendTCP("JOI " + highestID.ToString() + charName);
					}

					//REPEAT the joining proccess
					if (sub == "REP")
					{ 
						//Send information of all players to all players
						foreach (string entry in clientDictionary.Keys)
						{
							if(clientDictionary[entry].raceId !=-1)
							SendTCP("JOI " + clientDictionary[entry].raceId.ToString() + entry);
						}
					}

					//Console.WriteLine("Server consumed content of " + newMsg.body);
					//SendTCP(newMsg.body);
				}
			}

		}

		//---------------------------------------
		//				   SEND
		//---------------------------------------
		//public static void SendTCPTo(Socket sock, String msg)
		//{
		//	byte[] byteData = Encoding.ASCII.GetBytes(msg);
		//	Console.WriteLine("Server sending: {0},", msg);

		//	foreach (Client entry in clientDictionary.Values)
		//	{
		//		//Standard 
		//		//socket.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallbackTCP), socket);

		//		entry.tcpSock.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallbackTCP), entry.tcpSock);
		//	}
		//		}

		//Sending data to all
		public static void SendTCP(String msg)
		{
			byte[] byteData = Encoding.ASCII.GetBytes(msg);
			Console.WriteLine("Server sending: {0},", msg);

			foreach (Client entry in clientDictionary.Values)
			{
				//Standard 
				//socket.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallbackTCP), socket);

				entry.tcpSock.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallbackTCP), entry.tcpSock);
			}
		}

		//Sending data to registered
		public static void SendTCPToRegistered(string msg)
		{ 
			byte[] byteData = Encoding.ASCII.GetBytes(msg);
			Console.WriteLine("Server sending: {0},", msg);

			foreach (Client entry in clientDictionary.Values)
			{
				if(entry.raceId !=-1)
					entry.tcpSock.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallbackTCP), entry.tcpSock);
			}
		}


		//Preparing to send data
		public static void SendCallbackTCP(IAsyncResult ar)
		{
			Socket handle = (Socket)ar.AsyncState;
			int byteSent = handle.EndSend(ar);
			//handle.Shutdown(SocketShutdown.Both);
			//handle.Close();
		}


		//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//																						UDP
		//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		//UDP
		public static bool messageReceived = false;

		//---------------------------------------
		//				REGISTER
		//---------------------------------------
		//Register
		public static void RegisterUdp(IPEndPoint endP, Socket sock, String name)
		{
			Console.WriteLine("UDP registration started");
			if (clientDictionary.ContainsKey(name))
			{

				Client existingInfo = clientDictionary[name];
				existingInfo.udpSocket = sock;
				existingInfo.endPoint = endP;

				clientDictionary[name] = existingInfo;
			}
			else
			{
				Client newClient = new Client();
				newClient.udpSocket = sock;
				newClient.endPoint = endP;

				clientDictionary.Add(name, newClient);
			}

			Console.WriteLine("UDP registration is complete");
		}

		//---------------------------------------
		//				RECEIVE
		//---------------------------------------

		public static void ReceiveCallbackUDP(IAsyncResult ar)
		{
			//Start receiving
			Console.WriteLine("Receiving UDP...");

			//Get a state from the AR
			UdpState stateUdp = ((UdpState)(ar.AsyncState));

			//Get main parametres
			UdpClient uClient = stateUdp.udpClient;
			Socket sock = stateUdp.workingSocket;


			//Issue?
			EndPoint ePoint = stateUdp.endPoint;
			//EndPoint ePoint = sock.RemoteEndPoint;

			//Read data into a buffer
			int receiveBytes = sock.EndReceiveFrom(ar, ref ePoint);

			stateUdp.endPoint = (IPEndPoint)ePoint;

			string receiveString = Encoding.ASCII.GetString(((UdpState)ar.AsyncState).buffer);
			Console.WriteLine("UDP received: {0}", receiveString);

			//TEST
			((UdpState)ar.AsyncState).buffer = new byte[1024];

			//TEMP
			//Produce content and handle?
			MessageUDP msg = new MessageUDP();
			msg.sock = sock;
			msg.body = receiveString;
			msg.endPoint = stateUdp.endPoint;
			producerConsumerUDP.produce(msg);



			// The message then needs to be handleed
			//messageReceived = true;
			sock.BeginReceiveFrom(stateUdp.buffer, 0, stateUdp.bufferSize, 0, ref ePoint, new AsyncCallback(ReceiveCallbackUDP), stateUdp);

		}

		public static void ReceiveMessagesUDP(Socket udpSocket)
		{
			EndPoint ePoint = new IPEndPoint(IPAddress.Any, 0);
			UdpClient uClient = new UdpClient((IPEndPoint)ePoint);
			UdpState stateUdp = new UdpState();

			stateUdp.endPoint = (IPEndPoint)ePoint;
			stateUdp.udpClient = uClient;


			stateUdp.workingSocket = udpSocket;

			Console.WriteLine("Listening for messages udp");

			//EndPoint ep = new EndPoint();
			udpSocket.BeginReceiveFrom(stateUdp.buffer, 0, stateUdp.bufferSize, 0, ref ePoint, new AsyncCallback(ReceiveCallbackUDP), stateUdp);
		}

		//---------------------------------------
		//				   SEND
		//---------------------------------------

		public static void interpretUDP()
		{ 
			while (true)
			{
				MessageUDP newMsg = producerConsumerUDP.consume();

				if (newMsg !=null)
				{
					Console.WriteLine("Server consumed UDP content of " + newMsg.body);

					string sub = newMsg.body.Substring(0, 3);

					if (sub == "REG")
					{
						//string charName = (newMsg.body.Substring(4, newMsg.body.Length - 5));

						string[] elements = newMsg.body.Split(' ');
						string charName = elements[1];

						RegisterUdp(newMsg.endPoint, newMsg.sock, charName);
                        SendUDP(newMsg.body);
					}

					if (sub == "POS")
					{
						string[] elements = newMsg.body.Split(' ');

						string name = elements[3];
						string newString = elements[0] + elements[1] + " " + elements[2] + " " + clientDictionary[name].raceId;

						Console.WriteLine(newString);
                        SendUDP(newString);
					}

					//SendUDP(newMsg.body);
				}
			}
		}

		public static void SendCallbackUDP(IAsyncResult ar)
		{

			//Get a state from the AR
			UdpState stateUdp = ((UdpState)(ar.AsyncState));
			Socket sock = stateUdp.workingSocket;
			Console.WriteLine(stateUdp.endPoint.Address);
			int byteSent = sock.EndSendTo(ar);

		}

		//Sending data
		public static void SendUDP(String msg)
		{
			byte[] byteData = Encoding.ASCII.GetBytes(msg);

			//Test
			foreach (Client entry in clientDictionary.Values)
			{
				UdpState stateUdp = new UdpState();
				stateUdp.workingSocket = entry.udpSocket;
				stateUdp.endPoint = entry.endPoint;

				Console.WriteLine("Sending UDP to "+ entry.raceId);

				if(stateUdp !=null)
					entry.udpSocket.BeginSendTo(byteData, 0, byteData.Length, 0, entry.endPoint, new AsyncCallback(SendCallbackUDP), stateUdp);

			}

		}

	}
}
