
#include <mpi.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) std::cout << "Run with at least 2 processes\n";
        MPI_Finalize();
        return 1;
    }

    const int COUNT = 3;

    if (rank == 0) {
        int numbers[COUNT];                       

        int bufsize = COUNT * (sizeof(int) + MPI_BSEND_OVERHEAD);
        void* buf = std::malloc(bufsize);
        MPI_Buffer_attach(buf, bufsize);

        for (int i = 0; i < COUNT; i++) {
            numbers[i] = i * 10;
            MPI_Bsend(&numbers[i], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            std::cout << "Process 0 sent " << numbers[i] << "\n";
        }

        MPI_Buffer_detach(&buf, &bufsize);      
        std::free(buf);
    } else if (rank == 1) {
        int received[COUNT];
        for (int i = 0; i < COUNT; i++) {
            MPI_Recv(&received[i], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Process 1 received " << received[i] << "\n";
        }
    }

    MPI_Finalize();
    return 0;
}