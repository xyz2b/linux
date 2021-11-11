//
// Created by ziya on 2021/11/10.
//

#ifndef LINUX_TASK_H
#define LINUX_TASK_H

#include "../../include/common.h"

class Task {
public:
    // 任务编号
    int _num;

    Task* _prev;
    Task* _next;

    void doit() {
        INFO_PRINT("任务编号 [%d]", _num);
    }

public:
    Task(int num) {
        _num = num;
        _prev = nullptr;
        _next = nullptr;
    }
};


#endif //LINUX_TASK_H
