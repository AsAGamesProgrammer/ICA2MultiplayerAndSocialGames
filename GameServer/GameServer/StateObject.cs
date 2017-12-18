/*
	This class is used to represent all they key TCP information
*/

using System;
using System.Net.Sockets;
using System.Text;

namespace GameServer
{
	//TCP
	class StateObject
	{
		public Socket workSocket;
		public int bufferSize = 1024;
		public byte[] buffer = new byte[1024];
		public StringBuilder stringB = new StringBuilder();
	};
}
