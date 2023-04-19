from mpi4py import MPI
import math
import sys


def nwd(a, b):
    if b > a:
        a, b = b, a

    while b != 0:
        c = b
        b = a % b
        a = c

    return a


if __name__ == '__main__':

    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()

    #numbers = [16, 8, 4, 12, 6, 16, 2, 3]

    numbers = [21*3, 21*7, 21*5, 21*5*7, 21*37, 21*11, 21*1721, 21*13]

    steps = int(math.log2(size))
    myValue = numbers[rank]

   # print(f"steps: {steps}")

    for i in range(steps):
        shift = 2 ** i
        send_to = (rank + shift) % size
        receive_from = (rank - shift) % size

        # print(i)

        # if rank == 0:
        #     comm.send(myValue, dest=send_to)
        #     received_value = comm.recv(source=receive_from)
        #     myValue = nwd(myValue, received_value)
        #     continue

        received_value = comm.sendrecv(
            myValue, dest=send_to, source=receive_from)
        myValue = nwd(myValue, received_value)

        # received_value = comm.recv(source=receive_from)
        # myValue = nwd(myValue, received_value)
        # comm.send(myValue, dest=send_to)

        # comm.barrier()
        # print(
        #     f"step: {i} Rank {rank} send to {send_to} reveived from {receive_from} received: {received_value} nwd: {myValue}")

    print(f"Rank {rank} Wynik: {myValue}")
