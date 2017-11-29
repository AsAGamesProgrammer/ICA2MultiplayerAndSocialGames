using System;
using System.Net.Sockets;
using System.Threading;
using System.Text;
using System.Net;

namespace GameServer
{

	//TODO: UDP server

	// TCP: 7578
	// UDP: 7576
	//

	/// <summary>
	/// Additional class which holds state information
	/// </summary>

	//--------------------------------------
	//				STATE OBJECT
	//--------------------------------------

	//TCP
	class StateObject
	{
		public Socket workSocket;
		public int bufferSize = 1024;
		public byte[] buffer = new byte[1024];
		public StringBuilder stringB = new StringBuilder();
	};

	//UDP
	class UdpState
	{
		public IPEndPoint endPoint;
		public UdpClient udpClient;
	}

	/// <summary>
	/// Main body of the server
	/// </summary>
	class MainClass
	{

		//--------------------------------------
		//			LISTENING LOOP
		//--------------------------------------

		//Starting point of the server
		public static void Main(string[] args)
		{
			Console.WriteLine("Hello World!");
			StartListening();
		}

		//Starts the server
		public static void StartListening()
		{
			byte[] bytes = new byte[1024];

			//Get host information
			IPHostEntry ipHost = Dns.GetHostEntry(Dns.GetHostName());
			IPAddress ipAddress = ipHost.AddressList[0];
			IPEndPoint ipEndPoint = new IPEndPoint(ipAddress, 7578);

			//Create socket TCP
			Socket listenerTCP = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

			listenerTCP.Bind(ipEndPoint);
			listenerTCP.Listen(100);

			//Create socket UDP
			//****** BIND???
			Socket listenerUDP = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
			listenerUDP.Bind(new IPEndPoint(IPAddress.Any, 7579));

			//Listen to UDP
            ReceiveMessagesUDP();

			//Waiting for connections
			while (true)
			{
				//listenerUDP.BeginAccept(new AsyncCallback(ReceiveMessagesUDP), listenerUDP);

				allDone.Reset();
				listenerTCP.BeginAccept(new AsyncCallback(AcceptCallbackTCP), listenerTCP);
				allDone.WaitOne();

			}
		}

		//--------------------------------------
		//				TCP
		//--------------------------------------

		//TCP
		public static ManualResetEvent allDone = new ManualResetEvent(false);

		//Accept function
		public static void AcceptCallbackTCP(IAsyncResult ar)
		{
			allDone.Set();
			Socket listener = (Socket)ar.AsyncState;
			Socket handle = listener.EndAccept(ar);

			StateObject state = new StateObject();
			state.workSocket = handle;
			handle.BeginReceive(state.buffer, 0, state.bufferSize, 0, new AsyncCallback(ReadCallbackTCP), state);
		}


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

				//Dont output empty messages
				Console.WriteLine("Server received: '{0}', {1} bytes", content, readBytes);

				//Reached the end of line
				if (content.IndexOf("\n") > -1)
				{
					SendTCP(handle, content);
					state.buffer = new byte[1024];
				}

				handle.BeginReceive(state.buffer, 0, state.bufferSize, 0, new AsyncCallback(ReadCallbackTCP), state);
			}
			else if (readBytes == 0)
			{
				Console.WriteLine("Socket closed");
			}

		}

		//Sending data
		public static void SendTCP(Socket socket, String msg)
		{
			byte[] byteData = Encoding.ASCII.GetBytes(msg);

			socket.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallbackTCP), socket);
		}


		//Preparing to send data
		public static void SendCallbackTCP(IAsyncResult ar)
		{
			Socket handle = (Socket)ar.AsyncState;
			int byteSent = handle.EndSend(ar);
			//handle.Shutdown(SocketShutdown.Both);
			//handle.Close();
		}

		//--------------------------------------
		//				UDP
		//--------------------------------------

		//UDP
		public static bool messageReceived = false;


		public static void ReceiveCallbackUDP(IAsyncResult ar)
		{
			UdpClient uClient = (UdpClient)((UdpState)(ar.AsyncState)).udpClient;

			IPEndPoint ePoint = (IPEndPoint)((UdpState)(ar.AsyncState)).endPoint;
			Byte[] receiveBytes = uClient.EndReceive(ar, ref ePoint);
			string receiveString = Encoding.ASCII.GetString(receiveBytes);
			Console.WriteLine("Received: {0}", receiveString);

			// The message then needs to be handleed
			messageReceived = true;
		}

		public static void ReceiveMessagesUDP()

		{
			//******************************
			//How to receive many messages???
			// Receive a message and write it to the console.
			IPEndPoint ePoint = new IPEndPoint(IPAddress.Any, 7576);
			//*******************************
			UdpClient uClient = new UdpClient(ePoint);
			UdpState stateUdp = new UdpState();
			stateUdp.endPoint = ePoint;
			stateUdp.udpClient = uClient;
			Console.WriteLine("listening for messages");
			uClient.BeginReceive(new AsyncCallback(ReceiveCallbackUDP), stateUdp);
			// Do some work while we wait for a message.

			//while (!messageReceived)
			//{
			//	// Do something

			//}

			//if (messageReceived)
			//{ 
			//	Console.WriteLine("Something as received (UDP)");
			//}
		}
	}
}
