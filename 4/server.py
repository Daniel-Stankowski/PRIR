from multiprocessing.managers import BaseManager
import queue


class QueueManager(BaseManager):
    pass

in_queue = queue.Queue()
out_queue = queue.Queue()
QueueManager.register('in_queue', callable=lambda:in_queue)
QueueManager.register('out_queue', callable=lambda:out_queue)
manager = QueueManager(address=('127.0.0.1', 8080), authkey=bytes('blah','utf-8'))
server = manager.get_server()
server.serve_forever()
