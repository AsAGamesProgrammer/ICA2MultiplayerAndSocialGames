using System;
using System.Net;

namespace GameServer
{
	public class MessageUDP : MessageTCP
	{
		public IPEndPoint endPoint;
	}
}
