from multiprocessing.managers import BaseManager

def read(fname):
	f = open(fname, "r")
	nr = int(f.readline())
	nc = int(f.readline())

	A = [[0] * nc for x in range(nr)]
	r = 0
	c = 0
	for i in range(0,nr*nc):
		A[r][c] = float(f.readline())
		c += 1
		if c == nc:
			c = 0
			r += 1

	return A

A = read("A.dat")
X = read("X.dat")


class QueueManager(BaseManager): 
    pass

tasks = 200

QueueManager.register('in_queue')
QueueManager.register('out_queue')
manager = QueueManager(address=('127.0.0.1', 8080), authkey=bytes('blah', 'utf-8'))
manager.connect()
in_queue = manager.in_queue()
out_queue = manager.out_queue()

rest = len(A)%tasks
how_many_per_task = int((len(A)-rest)/tasks)
indexes = []
last_index = 0
for i in range(tasks):
	indexes.append(last_index)
	last_index = last_index + how_many_per_task + 1 if i < rest else last_index + how_many_per_task
	indexes.append(last_index)
for i in range(tasks):
	task = {}
	task['index'] = i
	task['A'] = A[indexes[2*i]:indexes[2*i+1]]
	task['X'] = X
	in_queue.put(task)

print("wlozylem")
gotten = 0
result = [0] * tasks
while(gotten < tasks):
	if out_queue.empty():
		continue
	else:
		gotten_data = out_queue.get()
		print("odebralem " + str(gotten_data['index']))
		result[gotten_data['index']] = gotten_data['result']
		gotten = gotten + 1

final_result = []
for i in range(tasks):
	final_result = final_result + result[i]

print(len(final_result))


