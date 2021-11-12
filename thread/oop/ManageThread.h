//
// Created by ziya on 2021/11/10.
//

#ifndef LINUX_MANAGETHREAD_H
#define LINUX_MANAGETHREAD_H

#include "../../include/common.h"

class ManageThread {
public:
    static pthread_mutex_t _lock;
    static pthread_cond_t _cond;
    static bool _is_locked;
public:
    static void init();
    static void run();
};


#endif //LINUX_MANAGETHREAD_H
