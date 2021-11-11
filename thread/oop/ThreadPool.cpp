//
// Created by ziya on 2021/11/10.
//

#include "ThreadPool.h"
#include "TaskPool.h"

extern TaskPool taskPool;

ThreadPool::ThreadPool(int core_size, int max_size) {
    _core_size = core_size;
    _max_size = max_size;
    _alive_size = core_size;
    _busy_size = 0;
    _threads = (JavaThread**) calloc(max_size, sizeof(*_threads));

    if (pthread_mutex_init(_lock, NULL) != 0) {
        ERROR_PRINT("mutex init failed!");
        exit(-1);
    }

    for (int i = 0; i < core_size; i++) {
        JavaThread* t = new JavaThread(i);
        _threads[i] = t;
        t->run();
    }
}

void ThreadPool::expand() {
    for (int i = _alive_size; i < _max_size; i++) {
        JavaThread* thread = new JavaThread(i);
        _threads[i] = thread;
        thread->run();

        _alive_size++;
    }
}

void ThreadPool::shrink() {
    pthread_cond_signal(taskPool._cond);
}
