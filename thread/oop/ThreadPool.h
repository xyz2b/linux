//
// Created by ziya on 2021/11/10.
//

#ifndef LINUX_THREADPOOL_H
#define LINUX_THREADPOOL_H

#include "JavaThread.h"

class ThreadPool {
public:
    int _core_size;
    int _max_size;
    int _alive_size;
    int _busy_size;

    pthread_mutex_t _lock[1];

    JavaThread** _threads;
public:
    ThreadPool(int core, int max);

    void expand();
    void shrink();
};


#endif //LINUX_THREADPOOL_H
