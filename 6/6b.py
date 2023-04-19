from random import Random
from mpi4py import MPI


def genData(size):
    random = Random()
    result = [0]*size
    for i in range(len(result)):
        result[i] = random.randint(0, 9)
    return result


if __name__ == '__main__':

    comm = MPI.COMM_WORLD
    size = comm.Get_size()
    rank = comm.Get_rank()
    cartcomm = comm.Create_cart(dims=[size], periods=[False], reorder=True)
    dim = cartcomm.Get_dim()
    # print(dim)
    final_result = []
    # print(cartcomm.Get_coords(comm.Get_rank()))

    data_size = 1000000
    chunk_size = 1000
    chunks = int(data_size/chunk_size)

    # print(
    #     f"pozycja: {cartcomm.Get_coords(comm.Get_rank())} shift={cartcomm.Shift(direction=0,disp=1)}")
    if rank == 0:
        data = genData(data_size)
        for i in range(chunks):
            comm.send(data[i*chunk_size:(i+1)*chunk_size],
                      dest=cartcomm.Shift(direction=0, disp=1)[1])
    else:
        shift = cartcomm.Shift(direction=0, disp=1)
        for i in range(chunks):
            received = comm.recv(source=shift[0])
            if shift[1] != -1:
                comm.send(received, dest=shift[1])
            else:
                final_result += received
        if shift[1] == -1:
            print(len(final_result))
