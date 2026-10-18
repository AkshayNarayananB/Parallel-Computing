#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, name_len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(hostname, &name_len);
    
    printf("Rank %d is running on node: %s\n", rank, hostname);
    
    MPI_Finalize();
    return 0;
}
