//
// Created by xyzjiao on 11/9/21.
//

#include "JavaThread.h"

int ret;

void* thread_do(void* arg) {
    JavaThread* Self = (JavaThread*) arg;

    // 模拟初始化
    sleep(3);

    // 模拟向JVM报告创建线程的一些数据
    sleep(3);

    pthread_mutex_lock(Self->_startThread_lock);
    Self->_state = INITIALIZED;
    INFO_PRINT("[%s] 线程阻塞，等待唤醒", Self->_name.c_str());
    pthread_cond_wait(Self->_cond, Self->_startThread_lock);
    pthread_mutex_unlock(Self->_startThread_lock);

    Self->_state = RUNNABLE;

    for (int i = 0; i < 10; i++) {
        INFO_PRINT("%s: %d", Self->_name.c_str(), i);
        usleep(500);
        if (i == 5) {
            Self->_state = ZOMBIE;
            pthread_exit(reinterpret_cast<void *>(1222));
        }
    }

    Self->_state = ZOMBIE;

    return 0;
}

JavaThread::JavaThread(string name) {


    //          JVM main线程 是 PTHREAD_CREATE_JOINABLE 模式的，然后join，该线程执行的逻辑就是JavaMain，该线程是在JDK main线程中创建的（在本例中使用getchar()替代join进行阻塞）
    //              除JVM main线程之外的其他业务线程，都是在JVM main中创建的，都是 PTHREAD_CREATE_DETACHED 模式的，这些线程的创建并不是一步到位的，要阻塞之后再唤醒
    //                  原因：1.业务上来说是因为要在线程真正处理业务逻辑前需要做一些初始化的的工作
    //                       2.技术上来说
    //                          PTHREAD_CREATE_JOINABLE 其他业务线程使用该模式，线程是串行执行的，解决办法使用 PTHREAD_CREATE_DETACHED
    //                          PTHREAD_CREATE_DETACHED 其他业务线程使用该模式，无法join，即无法等待其他业务线程执行完成之后，再退出JVM main线程，JVM main线程可能会先于其他业务线程执行完毕
    //                          实现方式：阻塞唤醒其他业务线程，具体实现：互斥锁 + 条件变量（在其他业务线程(thread_do方法)中阻塞，在JVM main线程(run方法)中唤醒）
    //
    /*
     * PTHREAD_CREATE_JOINABLE：
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t1: 0
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t1: 1
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t1: 2
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t1: 3
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t1: 4
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t1: 5
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t1: 6
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t1: 7
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t1: 8
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t1: 9
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t2: 0
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t2: 1
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t2: 2
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t2: 3
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t2: 4
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t2: 5
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t2: 6
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t2: 7
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t2: 8
        [INFO] (/home/xyzjiao/Desktop/project/linux/thread/oop/JavaThread.cpp:11->thread_do): t2: 9
     */

    _name = name;

    pthread_mutex_init(_startThread_lock, NULL);
    pthread_cond_init(_cond, NULL);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    _state = ALLOCATED;

    pthread_create(_tid, &attr, thread_do, this);

    pthread_attr_destroy(&attr);
}

// 主线程run方法中的唤醒逻辑 可能会比 业务线程thread_do方法中的阻塞逻辑先运行，不确定，所以会造成先唤醒再阻塞的情况
// 解决办法：使用业务线程状态来判断
void JavaThread::run() {

    while (true) {
        if (_state == INITIALIZED) {
            INFO_PRINT("唤醒 [%s]", _name.c_str());
            pthread_cond_signal(_cond);
            break;
        } else {
            INFO_PRINT("[%s]:[%X] 线程状态: %d", _name.c_str(), pthread_self(), _state);
        }

        sleep(1);
    }

}

void JavaThread::join() {
    while (true) {
        if (_state == ZOMBIE) {
            INFO_PRINT("[%s] 执行完毕", _name.c_str());
            break;
        } else {
            INFO_PRINT("[%s]:[%X] 线程状态: %d", _name.c_str(), pthread_self(), _state);
        }

        sleep(1);
    }
}
