//
// Created by ziya on 2021/11/10.
//

#include "ThreadPool.h"

ThreadPool::ThreadPool(int core_size, int max_size) {
    _core_size = core_size;
    _max_size = max_size;
    _alive_size = core_size;
    _busy_size = 0;
    _threads = (JavaThread**) malloc(max_size * sizeof(*_threads));

    for (int i = 0; i < core_size; i++) {
        JavaThread* t = new JavaThread(i);
        _threads[i] = t;
        t->run();
    }
}
