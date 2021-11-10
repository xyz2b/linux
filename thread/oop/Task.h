//
// Created by ziya on 2021/11/10.
//

#ifndef LINUX_TASK_H
#define LINUX_TASK_H


class Task {
public:
    int _num;

    Task* _prev;
    Task* _next;

public:
    Task(int num) {
        _num = num;
        _prev = nullptr;
        _next = nullptr;
    }
};


#endif //LINUX_TASK_H
