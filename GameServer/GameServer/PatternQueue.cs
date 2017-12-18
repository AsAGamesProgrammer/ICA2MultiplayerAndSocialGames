using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace GameServer
{
	public class PatternQueue
	{
		//object locker = new object();
		//Thread[] workers;
		//Queue<T> taskQ = new Queue<T>();

		//public PatternQueue(int workerCount)
		//{
		//	workers = new Thread[workerCount];

		//	// Create and start a separate thread for each worker
		//	for (int i = 0; i < workerCount; i++)
		//		(workers[i] = new Thread(Consume)).Start();
		//}

		//public void Dispose()
		//{
		//	// Enqueue one null task per worker to make each exit.
		//	foreach (Thread worker in workers) EnqueueTask(null);
		//	foreach (Thread worker in workers) worker.Join();
		//}

		//public void EnqueueTask(T task)
		//{
		//	lock (locker)
		//	{
		//		taskQ.Enqueue(task);
		//		Monitor.PulseAll(locker);
		//	}
		//}

		//void Consume()
		//{
		//	while (true)
		//	{
		//		T task;
		//		lock (locker)
		//		{
		//			while (taskQ.Count == 0) Monitor.Wait(locker);
		//			task = taskQ.Dequeue();
		//		}

		//		if (task == null) return;         // This signals our exit
		//		Console.Write(task);

		//		//Thread.Sleep(1000);              // Simulate time-consuming task
		//	}
		//} 	

		Queue<string> _MyQueue;
		private Object _lockObject;

		public PatternQueue(Queue<string> MyQueue, Object lockObject)
		{
			this._MyQueue = MyQueue;
			_lockObject = lockObject;
		}

		public void produce(string message)
		{
			//int msgId = 0;

			lock (_lockObject)
			{
				//while (msgId < 10)
				//{
					//Thread.Sleep(500);
					Monitor.Wait(_lockObject);
					//msgId++;
					//message msg = new message
					//{ Id = msgId, body = "Message " + msgId, sendDate = DateTime.Now };
					//Console.WriteLine("Sending message : {0}", msg.body);

					_MyQueue.Enqueue(message);
					Monitor.Pulse(_lockObject);
				//}


			}
		}

		public void consume()
		{
			lock (_lockObject)
			{
				Monitor.Pulse(_lockObject);
				//while (Monitor.Wait(_lockObject, 1000))
				//{
				if (_MyQueue.Count > 0)
				{
					//continue;
					//}
					string msg = _MyQueue.Dequeue();
					Console.WriteLine
						   (" Message received: {0}", msg);
					Monitor.Pulse(_lockObject);
				}
				//}
			} 
		}  
	}
}
