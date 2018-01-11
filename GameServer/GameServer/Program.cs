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


	//	PRODUCER-CONSUMER
	//	Queues separate for udp and tcp


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
		static int currentSessionID = 0;

		//Thread for time
		//static Thread thTimer = new Thread(countDown);
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
			prepareDatabase();

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


		//Prepare database and connections
		public static void prepareDatabase()
		{ 
			//Connection
			string folder = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
			//SqliteConnection.CreateFile("MyDatabase.sqlite");
			m_dbConnection = new SqliteConnection("Data Source=MyDatabase.sqlite;");
			m_dbConnection.Open();

			//Lap table
			//Clear laps table
			string sql = "DELETE from laps";
			SqliteCommand command = new SqliteCommand(sql, m_dbConnection);
			command.ExecuteNonQuery();

			//Close connection
			m_dbConnection.Close();
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
		//Timer runs on a separate thread
		//When timer finished the game start on the current session and enties for the next session are open
		public static void countDown()
		{
			Console.WriteLine("Timer started");

			//Start a stopwatch
			myStopwatch.Start();

			//Sleep for 20 seconds
			for (int i = 0; i< 20000; i++)
        	{
            	Thread.Sleep(1);
        	}

			//Stop a stopwatch
			myStopwatch.Stop();

			//provide support information
			Console.WriteLine("Time off");
			SendTCPToRegistered("STR", currentSessionID);

			//open entry for the next session
			currentSessionID++;
			timerIsActive = false;
			highestID = -1;
			Console.WriteLine("New session entry: " + currentSessionID);
		}

		///---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		///																						TCP
		///---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		//TCP
		public static ManualResetEvent allDone = new ManualResetEvent(false);

		//---------------------------------------
		//				REGISTER
		//---------------------------------------
		//Registers a client on the server
		//Adds client information to a local dictionary and its name to a database
		public static void RegisterClient(Socket sock, String name)
		{
			//Debug information
			Console.WriteLine("TCP registartion started");

			//If this client exxists in the dictionary
			if (clientDictionary.ContainsKey(name))
			{
				//Udate clients info
				Client existingInfo = clientDictionary[name];
				if (existingInfo.tcpSock != sock)
				{
					existingInfo.tcpSock = sock;

					clientDictionary[name] = existingInfo;
				}
			}
			else
			{
				//Create a new client and oput in a dictionary
				Client newClient = new Client();
				newClient.tcpSock = sock;

				clientDictionary.Add(name, newClient);
			}

			//Send client a confirmation about successful registration
            SendTCP("REG " + "TCP registered");

			//---------------------------------------
			//				DATABASE
			//---------------------------------------
			//If client played the game before then greet a client

			m_dbConnection.Open();

			//Make a secue commad to query the database
			SqliteCommand command = new SqliteCommand(m_dbConnection);

			command.CommandText ="SELECT * FROM users WHERE name = :Name";

			command.Parameters.Add("Name", DbType.String).Value = name;
			command.ExecuteNonQuery();

			//Start a reader to interpret results
			SqliteDataReader reader = command.ExecuteReader();

			//Flag
			bool userExists = false;

			//If at least one enty was found - then the client exists an should receive a personal tcp msg
			while (reader.Read())
			{
				userExists = true;
				Console.WriteLine("Reader read: " + reader.GetString(0) + " " + reader.GetString(1));
				SendTCPTo(reader.GetString(0), "Welcome back " + reader.GetString(0));

			}

			//If client doesn't exist then add him to a database
			if (!userExists)
			{
				SqliteCommand commandUpdate = new SqliteCommand(m_dbConnection);

				commandUpdate.CommandText = "INSERT into users (name, password) values (:Name, :Password)";

				commandUpdate.Parameters.Add("Name", DbType.String).Value = name;
				commandUpdate.Parameters.Add("Password", DbType.String).Value = "test";
				commandUpdate.ExecuteNonQuery();
			}

			//Close connection
			m_dbConnection.Close();

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
			//DATABASE PART
			m_dbConnection.Open();

			string sql = "insert into laps (name, score) values ('"+ name + "', 0.0)";
			SqliteCommand command = new SqliteCommand(sql, m_dbConnection);
			command.ExecuteNonQuery();

			m_dbConnection.Close();


			//GAMEPLAY PART
			//Increase id as the new player joined
			highestID++;

			//Update player's status
			clientDictionary[name].raceId = highestID;
			clientDictionary[name].sessionID = currentSessionID;

			Console.WriteLine(name + " joined session " + currentSessionID);

			//Update everyone that this player is registered
			foreach (string entry in clientDictionary.Keys)
			{
				if (clientDictionary[entry].raceId != -1 && clientDictionary[entry].sessionID == currentSessionID)
				{
					Console.WriteLine("Sendinginformation about " + entry);
					SendTCPToRegistered("JOI " + clientDictionary[entry].raceId.ToString() + entry, currentSessionID);
				}
			}

			if (!timerIsActive)
			{
				timerIsActive = true;
				Thread thTimer = new Thread(countDown);
				thTimer.Start();
			}

		}

		//---------------------------------------
		//				SCORE
		//---------------------------------------
		public static void ScoreHandle (string name, float score)
		{
			//UPDATE DATABASE
			m_dbConnection.Open();

			SqliteCommand command = new SqliteCommand(m_dbConnection);

			command.CommandText ="update laps set score = :Score where name = :Name";

			command.Parameters.Add("Name", DbType.String).Value = name;
			command.Parameters.Add("Score", DbType.Decimal).Value = score;
			command.ExecuteNonQuery();

			m_dbConnection.Close();

			//UPDATE PLAYERS
			SendTCPToRegistered("SCR " + clientDictionary[name].raceId.ToString() + " " + score + " ", clientDictionary[name].sessionID);
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

				//Get content and print a debug msg
				content = Encoding.ASCII.GetString(state.buffer, 0, state.bufferSize);
				Console.WriteLine("Server received: '{0}', {1} bytes", content, readBytes);
				state.buffer = new byte[1024];


				//Produce content and handle
				MessageTCP msg = new MessageTCP();
				msg.sock = state.workSocket;
				msg.body = content;
				producerConsumerTCP.produce(msg);

				//Loop
				handle.BeginReceive(state.buffer, 0, state.bufferSize, 0, new AsyncCallback(ReadCallbackTCP), state);


			}
			else if (readBytes == 0)
			{
				Console.WriteLine("Socket closed");
			}

		}

		//---------------------------------------
		//				INTERPRET
		//---------------------------------------
		//This runs on a separate thread

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

						string[] elements = newMsg.body.Split(' ');
						string charName = elements[1];
						RegisterClient(newMsg.sock, charName);

                        //SendTCP(newMsg.body);
					}

					//JOIN the race
					if (sub == "JOI")
					{
						
						string[] elements = newMsg.body.Split(' ');
						string charName = elements[1];
                        Join(charName);

					}

					//REPEAT the joining proccess
					if (sub == "REP")
					{ 
						//Send information of all players to all players
						foreach (string entry in clientDictionary.Keys)
						{
							if(clientDictionary[entry].raceId !=-1)
							SendTCPToRegistered("JOI " + clientDictionary[entry].raceId.ToString() + entry, clientDictionary[entry].sessionID);
						}
					}

					if (sub == "SCR")
					{ 
						//insert to database
						string[] elements = newMsg.body.Split(' ');
						string charName = elements[1];
						string score = elements[2];
						float scoreFloat = (float)Convert.ToDouble(score);

						//Handle score, update database and players
						ScoreHandle(charName, scoreFloat);

					}

					if (sub == "BLT")
					{ 
						string[] elements = newMsg.body.Split(' ');
						string charName = elements[1];
						string angle = elements[2];

						SendTCPToRegistered("BLT " + clientDictionary[charName].raceId + " " + angle + " ", clientDictionary[charName].sessionID);
					}

					//Console.WriteLine("Server consumed content of " + newMsg.body);
					//SendTCP(newMsg.body);
				}
			}

		}

		//---------------------------------------
		//				   SEND
		//---------------------------------------

		//SEND TCP TO a user by name
		public static void SendTCPTo(String name, String msg)
		{
			byte[] byteData = Encoding.ASCII.GetBytes(msg);
			Console.WriteLine("Server sending: {0},", msg);

				//Standard 
				//socket.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallbackTCP), socket);

			clientDictionary[name].tcpSock.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallbackTCP), clientDictionary[name].tcpSock);
		}

		//SEND TCP TO ALL
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

		//SEND TCP TO SESSION MATES
		public static void SendTCPToRegistered(string msg, int session)
		{ 
			byte[] byteData = Encoding.ASCII.GetBytes(msg);
			Console.WriteLine("Server sending: {0},", msg);

			foreach (Client entry in clientDictionary.Values)
			{
				Console.WriteLine(entry + " is at session " + entry.sessionID + ", requested session is " + session);

				if (entry.raceId != -1 && entry.sessionID == session)
				{
					Console.WriteLine("Msg sent");
					entry.tcpSock.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallbackTCP), entry.tcpSock);
				}
			}
		}


		//PREPARE TO SEND DATA
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

			//If name exists then update the entry
			if (clientDictionary.ContainsKey(name))
			{

				Client existingInfo = clientDictionary[name];
				existingInfo.udpSocket = sock;
				existingInfo.endPoint = endP;

				clientDictionary[name] = existingInfo;
			}
			else //otherwise create a new entry
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

			//Clear buffer
			((UdpState)ar.AsyncState).buffer = new byte[1024];

			//Produce content and handle
			MessageUDP msg = new MessageUDP();
			msg.sock = sock;
			msg.body = receiveString;
			msg.endPoint = stateUdp.endPoint;
			producerConsumerUDP.produce(msg);


			// Begin receive
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
		//				  INERPRET
		//---------------------------------------
		public static void interpretUDP()
		{ 
			while (true)
			{
				//Consume msg
				MessageUDP newMsg = producerConsumerUDP.consume();

				if (newMsg !=null)
				{
					Console.WriteLine("Server consumed UDP content of " + newMsg.body);

					string sub = newMsg.body.Substring(0, 3);

					//RECEIVED REG for registration
					if (sub == "REG")
					{
						string[] elements = newMsg.body.Split(' ');
						string charName = elements[1];

						RegisterUdp(newMsg.endPoint, newMsg.sock, charName);
						SendUDP(newMsg.body, clientDictionary[charName].sessionID);
					}

					//RECEIVED POS for position update
					if (sub == "POS")
					{
						string[] elements = newMsg.body.Split(' ');

						string name = elements[3];
						string newString = elements[0] + elements[1] + " " + elements[2] + " " + clientDictionary[name].raceId;

						Console.WriteLine(newString);
                        SendUDP(newString, clientDictionary[name].sessionID);
					}

					//SendUDP(newMsg.body);
				}
			}
		}

		//---------------------------------------
		//				   SEND
		//---------------------------------------

		public static void SendCallbackUDP(IAsyncResult ar)
		{

			//Get a state from the AR
			UdpState stateUdp = ((UdpState)(ar.AsyncState));
			Socket sock = stateUdp.workingSocket;
			Console.WriteLine(stateUdp.endPoint.Address);
			int byteSent = sock.EndSendTo(ar);

		}

		//SEND TO SESSION MATES
		public static void SendUDP(String msg, int session)
		{
			byte[] byteData = Encoding.ASCII.GetBytes(msg);

			//Test
			foreach (Client entry in clientDictionary.Values)
			{
				if (entry.raceId != -1 && entry.sessionID == session )
				{

					UdpState stateUdp = new UdpState();
					stateUdp.workingSocket = entry.udpSocket;
					stateUdp.endPoint = entry.endPoint;

					Console.WriteLine("Sending UDP to " + entry.raceId);

					if (stateUdp != null)
						entry.udpSocket.BeginSendTo(byteData, 0, byteData.Length, 0, entry.endPoint, new AsyncCallback(SendCallbackUDP), stateUdp);
				}

			}

		}

	}
}
