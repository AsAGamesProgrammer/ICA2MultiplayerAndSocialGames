using System;
using System.Net.Sockets;
using System.Net;

namespace GameServer
{
//Client class
	class Client
	{
		//Tcp
		public Socket tcpSock;

		//Udp
		public IPEndPoint endPoint;
		public Socket udpSocket;

		//Race id
		public int sessionID = -1;
		public int raceId = -1;
	}
}
