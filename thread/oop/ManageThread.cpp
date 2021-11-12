//
// Created by ziya on 2021/11/10.
//

#include "ManageThread.h"
#include "TaskPool.h"
#include "ThreadPool.h"

pthread_mutex_t ManageThread::_lock;
pthread_cond_t ManageThread::_cond;
bool ManageThread::_is_locked;

extern TaskPool taskPool;
extern ThreadPool threadPool;

void* mange_thread_do(void* arg) {
    INFO_PRINT("[管理线程] 开始执行");

    while (true) {
        // 扩容
        /**
         * 条件
         * 1.有任务
         * 2.所有存活线程都在干活
         * 3.现存线程没有达到最大数
         * */
        if ((taskPool._count != 0) && (threadPool._alive_size == threadPool._busy_size) && (threadPool._alive_size < threadPool._max_size)) {
            INFO_PRINT("[管理线程] 需要扩容: core_size=%d, alive_size=%d, busy_size=%d, max_size=%d", threadPool._core_size, threadPool._alive_size, threadPool._busy_size, threadPool._max_size);

            threadPool.expand();
        } else {
            INFO_PRINT("[管理线程] 不需要扩容: core_size=%d, alive_size=%d, busy_size=%d, max_size=%d", threadPool._core_size, threadPool._alive_size, threadPool._busy_size, threadPool._max_size);
        }

        // 缩容
        /**
         * 1.没有任务了
         * 2.所有线程都空闲了
         * 3.现存线程超过核心线程数
         * */
         if ((taskPool._count == 0) && (threadPool._busy_size == 0) && (threadPool._alive_size > threadPool._core_size)) {
             INFO_PRINT("[管理线程] 需要缩容: core_size=%d, alive_size=%d, busy_size=%d, max_size=%d", threadPool._core_size, threadPool._alive_size, threadPool._busy_size, threadPool._max_size);

             threadPool.shrink();
         } else {
             INFO_PRINT("[管理线程] 不需要缩容: core_size=%d, alive_size=%d, busy_size=%d, max_size=%d", threadPool._core_size, threadPool._alive_size, threadPool._busy_size, threadPool._max_size);
         }

        // 无任务时阻塞睡眠
        /**
         * 1.没有任务
         * 2.没有繁忙线程
         * */
        if (taskPool._count == 0 && threadPool._busy_size == 0) {
            pthread_mutex_lock(&ManageThread::_lock);
            ManageThread::_is_locked = true;
            INFO_PRINT("[管理线程] 进入休眠，等待唤醒");
            pthread_cond_wait(&ManageThread::_cond, &ManageThread::_lock);
            pthread_mutex_unlock(&ManageThread::_lock);
        }

        sleep(1);
    }

    return 0;
}

void ManageThread::run() {
    pthread_t tid[1];

    pthread_create(tid, NULL, mange_thread_do, NULL);
}

void ManageThread::init() {
    ManageThread::_is_locked = false;
    if (pthread_mutex_init(&_lock, NULL) != 0) {
        ERROR_PRINT("mutex init failed!");
        exit(-1);
    }

    if (pthread_cond_init(&_cond, NULL) != 0) {
        ERROR_PRINT("cond init failed!");
        exit(-1);
    }
}
