from multiprocessing.managers import BaseManager
from multiprocessing import Process

class QueueManager(BaseManager):
    pass

QueueManager.register('in_queue')
QueueManager.register('out_queue')
manager = QueueManager(address=('127.0.0.1', 8080), authkey=bytes('blah', 'utf-8'))
manager.connect()
in_queue = manager.in_queue()
out_queue = manager.out_queue()

def mnoz(dane):
	A = dane[0]
	X = dane[1]

	nrows = len(A)
	ncols = len(A[0])
	y = []
	for i in range(nrows):
		s = 0
		for c in range(0, ncols):
			s += A[i][c] * X[c][0]
			#time.sleep(0.1)

		y.append(s)

	return y

def start_process(data):
	print("proces")
	out_data = {'index': data['index'], 'result': mnoz([data['A'],data['X']])}
	out_queue.put(out_data)


if __name__ == '__main__':
	while(True):
		if(in_queue.empty()):
			continue
		else:
			data = in_queue.get()
			p = Process(target=start_process, args=(data,))
			p.start()




