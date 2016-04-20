// Copyright 2016 Paul Merrill

#include <mpi.h>

#include "Image8.h"
#include "MPIHelper.h"

const MPIHelper mpiHelper;

MPIHelper::MPIHelper() {
    MPI_Init(nullptr, nullptr);
}

MPIHelper::~MPIHelper() {
    MPI_Finalize();
}

int MPIHelper::myProcessId() const {
    int taskId;
    MPI_Comm_rank(MPI_COMM_WORLD, &taskId);
    return taskId;
}

int MPIHelper::processCount() const {
    int groupSize;
    MPI_Comm_size(MPI_COMM_WORLD, &groupSize);
    return groupSize;
}

template<class T>
void MPIHelper::sendToSlave(int slaveProcessId, T data) const {
    MPI_Send(&data, sizeof(T), MPI_BYTE, slaveProcessId, 0, MPI_COMM_WORLD);
}

template<class T>
void MPIHelper::sendToSlave(int slaveProcessId, T *data, size_t count) const {
    MPI_Send(data, sizeof(T) * count, MPI_BYTE, slaveProcessId, 0,
             MPI_COMM_WORLD);
}

template<class T>
void MPIHelper::receiveFromMaster(T *data) const {
    size_t numBytes = sizeof(T);
    MPI_Status status;
    MPI_Recv(data, static_cast<int>(numBytes), MPI_BYTE, MPI_MASTER_PROCESS_ID,
             0, MPI_COMM_WORLD, &status);
}

template<class T>
void MPIHelper::receiveFromMaster(T *data, size_t count) const {
    size_t numBytes = sizeof(T) * count;
    MPI_Status status;
    MPI_Recv(data, static_cast<int>(numBytes), MPI_BYTE, MPI_MASTER_PROCESS_ID,
             0, MPI_COMM_WORLD, &status);
}

template<class T>
void MPIHelper::sendToMaster(T *data, size_t count) const {
    MPI_Send(data, sizeof(T) * count, MPI_BYTE, MPI_MASTER_PROCESS_ID, 0,
             MPI_COMM_WORLD);
}

template<class T>
void MPIHelper::receiveFromSlave(int slaveProcessId, T *data, size_t count) const {
    size_t numBytes = sizeof(T) * count;
    MPI_Status status;
    MPI_Recv(data, static_cast<int>(numBytes), MPI_BYTE, slaveProcessId,
             0, MPI_COMM_WORLD, &status);
}

template void MPIHelper::sendToSlave(int, uint32_t) const;
template void MPIHelper::sendToSlave(int, rgb *, size_t) const;
template void MPIHelper::receiveFromMaster(uint32_t *) const;
template void MPIHelper::receiveFromMaster(rgb *, size_t) const;
template void MPIHelper::sendToMaster(rgb *, size_t) const;
template void MPIHelper::receiveFromSlave(int, rgb *, size_t) const;
