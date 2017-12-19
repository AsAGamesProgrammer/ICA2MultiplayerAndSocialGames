using System;
using System.Collections.Generic;
using System.Threading;

/*
	TODO
	Insteas of using Message UDP class as a return type, it should be a template. 
	
*/


namespace GameServer
{
	public class UDPQueue
	{
		Queue<MessageUDP> _MyQueue;
		private Object _lockObject;

		//CONSTRUCTOR
		public UDPQueue(Queue<MessageUDP> MyQueue, Object lockObject)
		{
			this._MyQueue = MyQueue;
			_lockObject = lockObject;
		}


		//PRODUCE
		public void produce(MessageUDP message)
		{
			lock (_lockObject)
			{
				Monitor.Wait(_lockObject);
				_MyQueue.Enqueue(message);
				Monitor.Pulse(_lockObject);
			}
		}

		//CONSUME
		public MessageUDP consume()
		{
			lock (_lockObject)
			{
				Monitor.Pulse(_lockObject);

				if (_MyQueue.Count > 0)
				{
					var msg = _MyQueue.Dequeue();

					Monitor.Pulse(_lockObject);
					return msg;
				}
			}

			return null;		
		} 
	}
}
