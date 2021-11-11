//
// Created by ziya on 2021/11/10.
//

#include "ManageThread.h"

void* mange_thread_do(void* arg) {
    INFO_PRINT("[管理线程]");



    return 0;
}

void ManageThread::run() {
    pthread_t tid[1];

    pthread_create(tid, NULL, mange_thread_do, NULL);
}
