#include "./include/common.h"
#include "./jvm.h"

int main() {
    int rst = continueInNewThread();
    INFO_PRINT("%d\n", rst);

    return 0;
}