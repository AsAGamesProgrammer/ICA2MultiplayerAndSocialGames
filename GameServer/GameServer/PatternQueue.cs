using System;
using System.Collections.Generic;
using System.Threading;


//----------------------------
//			TCP
//----------------------------

namespace GameServer
{
	public class PatternQueue
	{
		Queue<MessageTCP> _MyQueue;
		private Object _lockObject;

		//CONSTRUCTOR
		public PatternQueue(Queue<MessageTCP> MyQueue, Object lockObject)
		{
			this._MyQueue = MyQueue;
			_lockObject = lockObject;
		}


		//PRODUCE
		public void produce(MessageTCP message)
		{
			Console.WriteLine("PRODUCER TCP: " + message);
			lock (_lockObject)
			{
				Monitor.Wait(_lockObject);
				_MyQueue.Enqueue(message);
				Monitor.Pulse(_lockObject);
			}
		}

		//CONSUME
		public MessageTCP consume()
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
