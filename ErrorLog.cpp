#include "ErrorLog.h"

using namespace std;

void test(int* list,int size)
{
    int x = 0;
    for(;x <= size;x++)
    {
        if(x == size) {
            LOG(hzd::Error::Out_Of_Bound,"数组越界","list = %x,size = %d,x = %d",list,size,x);
        }
        _sleep(1000);
        LOG_MSG(to_string(list[x]));
    }

    int* y = nullptr;
    if(!y){
        LOG(hzd::Pointer_To_Null,"操作空指针","y=%x",y);
    }
}

int main() {
//    hzd::ErrorLog &logger = hzd::ErrorLog::getLogger();
//    logger << "error";
    int list[5] = {9,2,4,5,1};
    test(list,5);
    getchar();

}