using System;
using System.Net.Sockets;
using System.Threading;
using System.Text;
using System.Net;
using System.Collections.Generic;

using System.Threading.Tasks;

namespace GameServer
{

	//TODO: MEssage system which will support test messages ana tcp registration

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

	//TEST
	//public class message
	//{
	//	public int Id;
	//	public string body;
	//	public DateTime sendDate;
	//}

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

		//--------------------------------------
		//			LISTENING LOOP
		//--------------------------------------

		//MAIN
		public static void Main(string[] args)
		{
			//Test message
			Console.WriteLine("Hello, I am server!");

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


			//LOOP for lisening to the UDP messages
			ReceiveMessagesUDP(listenerUDP);

			//Thread for TCP consumer
			var th = new Thread(interpretTCP);
			th.Start();

			//Loop for TCP producer
			while (true)
			{
				allDone.Reset();
				listenerTCP.BeginAccept(new AsyncCallback(AcceptCallbackTCP), listenerTCP);
				allDone.WaitOne();

			}
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
				//Read TCP and make sense of it

				//Temp register
				//RegisterClient(state.workSocket, "Test");

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
					string sub = newMsg.body.Substring(0, 3);

					if (sub == "REG")
					{
						string charName = (newMsg.body.Substring(0, newMsg.body.Length - 1));

						RegisterClient(newMsg.sock, charName);
					}

					Console.WriteLine("Server consumed content of " + newMsg.body);
					SendTCP(newMsg.body);
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

			//----------------RECEIVED-----------------
			//SEND BACK
			if (receiveString.IndexOf("\n") > -1)
			{
				string sub = receiveString.Substring(0, 3);

				//Registartion check
				if (sub == "REG")
				{
					string charName = receiveString.Substring(0, receiveString.Length - 1);
					RegisterUdp(stateUdp.endPoint, sock, charName);
				}

				//ECHO
				SendUDP(sock, receiveString, stateUdp);
				stateUdp.buffer = new byte[1024];
			}
			//----------------RECEIVED-----------------

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

		public static void SendCallbackUDP(IAsyncResult ar)
		{

			//Get a state from the AR
			UdpState stateUdp = ((UdpState)(ar.AsyncState));
			Socket sock = stateUdp.workingSocket;
			Console.WriteLine(stateUdp.endPoint.Address);
			int byteSent = sock.EndSendTo(ar);

		}

		//Sending data
		public static void SendUDP(Socket socket, String msg, UdpState stateUdp)
		{
			byte[] byteData = Encoding.ASCII.GetBytes(msg);

			//Test
			foreach (Client entry in clientDictionary.Values)
			{
				stateUdp.workingSocket = entry.udpSocket;
				stateUdp.endPoint = entry.endPoint;
				entry.udpSocket.BeginSendTo(byteData, 0, byteData.Length, 0, entry.endPoint, new AsyncCallback(SendCallbackUDP), stateUdp);
			}

		}

	}
}
