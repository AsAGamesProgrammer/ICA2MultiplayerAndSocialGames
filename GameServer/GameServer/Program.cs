using System;
using System.Net.Sockets;
using System.Threading;
using System.Text;
using System.Net;

namespace GameServer
{

	/// <summary>
	/// Additional class which holds state information
	/// </summary>
	class StateObject
	{
		public Socket workSocket;
		public int bufferSize = 1024;
		public byte[] buffer = new byte[1024];
		public StringBuilder stringB = new StringBuilder();
	};

	/// <summary>
	/// Main body of the server
	/// </summary>
	class MainClass
	{
		public static ManualResetEvent allDone = new ManualResetEvent(false);

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
			IPEndPoint ipEndPoint = new IPEndPoint(ipAddress, 7576);

			//Create socket
			Socket listener = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
			listener.Bind(ipEndPoint);
			listener.Listen(100);

			//Waiting for connections
			while (true)
			{
				allDone.Reset();
				listener.BeginAccept(new AsyncCallback(AcceptCallback), listener);
				allDone.WaitOne();
			}
		}

		//Accept function
		public static void AcceptCallback(IAsyncResult ar)
		{
			allDone.Set();
			Socket listener = (Socket)ar.AsyncState;
			Socket handle = listener.EndAccept(ar);

			StateObject state = new StateObject();
			state.workSocket = handle;
			handle.BeginReceive(state.buffer, 0, state.bufferSize, 0, new AsyncCallback(ReadCallback), state);
		}

		public static void ReadCallback(IAsyncResult ar)
		{
			//Start with an empty msg
			string content = string.Empty;

			StateObject state = (StateObject)ar.AsyncState;
			Socket handle = state.workSocket;
			int readBytes = handle.EndReceive(ar);

			//If there is data to read
			if (readBytes > -1)
			{
				state.stringB.Append(Encoding.ASCII.GetString(state.buffer, 0, state.bufferSize));
				content = state.stringB.ToString();
				Console.WriteLine("Server received: {0}", content); 

				//Reached the end of line
                if(content.IndexOf("\n") > -1)
				{
					//NEW
					state.stringB.Clear();
					Send(handle, content);

				}

			}

			handle.BeginReceive(state.buffer, 0, state.bufferSize, 0, new AsyncCallback(ReadCallback), state);
		}

		public static void Send(Socket socket, String msg)
		{
			byte[] byteData = Encoding.ASCII.GetBytes(msg);
			socket.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallback), socket);
		}

		public static void SendCallback(IAsyncResult ar)
		{
			Socket handle = (Socket)ar.AsyncState;
			int byteSent = handle.EndSend(ar);
			//handle.Shutdown(SocketShutdown.Both);
			//handle.Close();
		}
	}
}
