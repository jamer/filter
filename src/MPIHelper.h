// Copyright 2016 Paul Merrill

#ifndef SRC_MPIHELPER_H_
#define SRC_MPIHELPER_H_

const int MPI_MASTER_PROCESS_ID = 0;

class MPIHelper {
 public:
    MPIHelper();
    ~MPIHelper();

    int myProcessId() const;
    int processCount() const;

    template<class T>
    void sendToSlave(int slaveProcessId, T data) const;

    template<class T>
    void sendToSlave(int slaveProcessId, T *data, size_t count) const;

    template<class T>
    void receiveFromMaster(T *data) const;

    template<class T>
    void receiveFromMaster(T *data, size_t count) const;

    template<class T>
    void sendToMaster(T *data, size_t count) const;

    template<class T>
    void receiveFromSlave(int slaveProcessId, T *data, size_t count) const;
};

extern const MPIHelper mpiHelper;

#endif  // SRC_MPIHELPER_H_
