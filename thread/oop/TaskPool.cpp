//
// Created by ziya on 2021/11/10.
//

#include "TaskPool.h"

Task *TaskPool::pop() {
    Task* ret = _head;

    if (_head->_next == _head && _head->_prev == _head) {
        _head = _tail = nullptr;
        _count--;
        return ret;
    }

    _head->_next->_prev = _head->_prev;
    _head->_prev->_next = _head->_next;

    _head = _head->_next;

    _count--;
    return ret;
}

void TaskPool::push(Task *task){
    if (_tail == nullptr && _head == nullptr) {
        _tail = _head = task;
        task->_next = task;
        task->_prev = task;
        _count++;
        return;
    }

    task->_next = _tail->_next;
    task->_next->_prev = task;
    task->_prev = _tail;
    _tail->_next = task;

    _tail = task;

    _count++;
}

TaskPool::TaskPool() {
    _count = 0;
    _tail = nullptr;
    _head = nullptr;

    if (pthread_mutex_init(_lock, NULL) != 0) {
        ERROR_PRINT("mutex init failed!");
        exit(-1);
    }

    if (pthread_cond_init(_cond, NULL) != 0) {
        ERROR_PRINT("cond init failed!");
        exit(-1);
    }
}

void TaskPool::print() {
    stringstream ss;
    ss << "[";

    int num = _count;
    Task* tmp = _head;
    while (num != 0) {
        ss << tmp->_num;
        tmp = tmp->_next;
        num--;
        if (num !=0 ) {
            ss << ", ";
        }
    }
    ss << "]";
    INFO_PRINT("%s", ss.str().c_str());
}
