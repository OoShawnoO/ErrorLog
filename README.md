# ErrorLog
一个自用的错误日志

```C++
/* 提供两个宏接口 */
/**
* LOG_MSG()
* @params:
* msg  - string
/
LOG("信息");

/**
* LOG()
* @params:
* error - enum ERROR
* msg - string
* format - string format
* args ...
*/
LOG(Out_of_Memory,"内存溢出","xxx使用了地址为%x的内存",address);
