//
// Created by ziya on 2021/11/10.
//

#ifndef LINUX_TASKPOOL_H
#define LINUX_TASKPOOL_H

#include "Task.h"
#include "../../include/common.h"

class TaskPool {
public:
    int _count;
    Task* _tail;
    Task* _head;

    pthread_mutex_t _lock[1];
    pthread_cond_t _cond[1];
public:
    TaskPool();

    void push(Task* task);

    Task* pop();

    void print();
};


#endif //LINUX_TASKPOOL_H
