using System;
using System.Net.Sockets;
using System.Net;

namespace GameServer
{
//UDP
	class UdpState
	{
		public Socket workingSocket;
		public IPEndPoint endPoint;
		public UdpClient udpClient;
		public int bufferSize = 1024;
		public byte[] buffer = new byte[1024];

		//Constructor
		public UdpState()
		{
			udpClient = new UdpClient();
			udpClient.EnableBroadcast = true;
		}	
	}
}
