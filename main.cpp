#include "./include/common.h"
#include "./jvm.h"
#include "./thread/oop/Task.h"
#include "./thread/oop/TaskPool.h"
#include "./thread/oop/ThreadPool.h"
#include "./thread/oop/ManageThread.h"

TaskPool taskPool;
ThreadPool threadPool(3, 5);

int main() {
//    int rst = continueInNewThread();
//    INFO_PRINT("%d\n", rst);

//    TaskPool* taskPool = new TaskPool();
//    for(int i = 0; i < 10; i++) {
//        taskPool->push(new Task(i));
//    }
//    taskPool->print();
    ManageThread::init();
    ManageThread::run();

    int v;
    while (true) {
        printf("[主线程] 请输入任务的条数: \n");
        cin >> v;

        for (int i = 0; i < v; i++) {
            taskPool.push(new Task(i));
        }

        // 只有管理线程进行睡眠阻塞之后，才能被唤醒
        // 如果不加判断，则该唤醒逻辑的执行 可能先于 管理线程阻塞的逻辑，导致了没有被阻塞就唤醒的问题
        if (ManageThread::_is_locked) {
            INFO_PRINT("唤醒[管理线程]");
            // 有任务就唤醒管理线程
            pthread_cond_signal(&ManageThread::_cond);
        }

        /**
         * 优化
         * 1.如果任务数超过核心线程数，全部唤醒
         * 2.如果小于核心线程数，有几个任务唤醒几个线程，减少任务争抢的消耗
         * */
        if (taskPool._count >= threadPool._core_size) {
            pthread_cond_broadcast(taskPool._cond);
        } else {
            for (int i = 0; i < taskPool._count; i++) {
                pthread_cond_signal(taskPool._cond);
            }
        }
    }
    getchar();
    return 0;
}