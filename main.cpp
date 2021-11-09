#include "./include/common.h"
#include "./thread/oop/JavaThread.h"

int main() {
    JavaThread* t1 = new JavaThread("t1");
    JavaThread* t2 = new JavaThread("t2");

    t1->run();
    t2->run();

    getchar();
    return 0;
}