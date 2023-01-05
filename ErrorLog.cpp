#include "ErrorLog.h"

using namespace std;

void test(int* list,int size)
{
    if(size > 4)
        LOG_MSG("test finish");
    else
        LOG(hzd::Out_Of_Bound,"","list=%x,size=%d",list,size);
}

int main() {
//    hzd::ErrorLog &logger = hzd::ErrorLog::getLogger();
//    logger << "error";
    int list[5] = {9,2,4,5,1};
    test(list,2);
    getchar();

}