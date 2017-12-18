using System;
using System.Net.Sockets;

namespace GameServer
{
	public class MessageBase
	{
		public string body;
		public Socket sock;
	}
}
