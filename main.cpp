#include "./include/common.h"
#include "./jvm.h"
#include "./thread/oop/Task.h"
#include "./thread/oop/TaskPool.h"
#include "./thread/oop/ThreadPool.h"

TaskPool taskPool;

int main() {
//    int rst = continueInNewThread();
//    INFO_PRINT("%d\n", rst);

//    TaskPool* taskPool = new TaskPool();
//    for(int i = 0; i < 10; i++) {
//        taskPool->push(new Task(i));
//    }
//    taskPool->print();

    ThreadPool threadPool(3, 5);

    int v;
    while (true) {
        printf("[主线程] 请输入任务的条数: \n");
        cin >> v;

        for (int i = 0; i < v; i++) {
            taskPool.push(new Task(i));
        }

        pthread_cond_broadcast(taskPool._cond);
    }
    getchar();
    return 0;
}