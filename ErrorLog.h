#ifndef ERRORLOG_ERRORLOG_H
#define ERRORLOG_ERRORLOG_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <unordered_map>
namespace hzd {
#ifdef _WIN32

#include <windows.h>
    #include <cstdint>
    #include <cstdlib>

    //   控制台前景颜色
    enum ConsoleForegroundColor {
        CFC_Red = FOREGROUND_RED,
        CFC_RedI = FOREGROUND_INTENSITY | FOREGROUND_RED,
        CFC_Green = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
        CFC_Blue = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
        CFC_Yellow = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
        CFC_Purple = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
        CFC_Cyan = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
        CFC_Gray = FOREGROUND_INTENSITY,
        CFC_White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        CFC_HighWhite = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        CFC_Black = 0,
    };

    // 背景色枚举值
    enum ConsoleBackGroundColor {
        CBC_Red = BACKGROUND_INTENSITY | BACKGROUND_RED,
        CBC_Green = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
        CBC_Blue = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
        CBC_Yellow = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
        CBC_Purple = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
        CBC_Cyan = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
        CBC_White = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
        CBC_HighWhite = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
        CBC_Black = 0,
    };

    // 设置颜色函数
    void SetConsoleColor(ConsoleForegroundColor foreColor = CFC_White, ConsoleBackGroundColor backColor = CBC_Black) {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handle, foreColor | backColor);
    }

    //调用函数
    void ShowMessage(std::string msg, ConsoleForegroundColor foreColor = CFC_White,
                     ConsoleBackGroundColor backColor = CBC_Black) {
        SetConsoleColor(foreColor, backColor);
        std::cout << msg << std::endl;
        SetConsoleColor(CFC_White, CBC_Black);
    }

#endif

#ifdef __unix__

#define RESET       "\033[0m"
#define BLACK       "\033[30m"             /* Black */
#define RED         "\033[31m"             /* Red */
#define GREEN       "\033[32m"             /* Green */
#define YELLOW      "\033[33m"             /* Yellow */
#define BLUE        "\033[34m"             /* Blue */
#define MAGENTA     "\033[35m"             /* Magenta */
#define CYAN        "\033[36m"             /* Cyan */
#define WHITE       "\033[37m"             /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#endif

    enum Error {
        None = 0,
        Out_Of_Bound,
        Pointer_To_Null,
        Bad_Malloc,
    };

    static std::string getTime() {
        time_t t = time(0);
        char timeTable[64] = {0};
        strftime(timeTable, sizeof(timeTable), "[%Y-%m-%d %H:%M:%S]", localtime(&t));
        return std::string(timeTable);
    }

    #define GET_LOGGER() hzd::ErrorLog& logger = hzd::ErrorLog::getLogger()

    #define FORMAT_ARGS(format,...) do{ \
        size_t __length = std::snprintf(nullptr,0,format,__VA_ARGS__);   \
        if(__length >= 0)                                                \
        {                                                                \
            char* __buf = new char[__length + 1];                        \
            std::snprintf(__buf,__length+1,format,__VA_ARGS__);          \
            __str = __buf;                                               \
            delete []__buf;                                              \
        }                                                                \
    }while(0)

    #define LOG_MSG(msg) do {           \
        GET_LOGGER();                   \
        string logmsg = hzd::getTime() + "[INFO]:Line:" + std::to_string(__LINE__) + "\t" + "Function:" + __FUNCTION__ + "\tMsg:" + (msg);                     \
        logger << logmsg;               \
    }while(0)

    #define LOG(error,msg,format,...) do{ \
        std::string __str;                      \
        FORMAT_ARGS(format,__VA_ARGS__);        \
        GET_LOGGER();                           \
        logger[error];                          \
        string logmsg = hzd::getTime() + "[ERROR]:" + logger.errorMsg[error] + "\tLine:" + std::to_string(__LINE__) + "\t" + "Function:" + __FUNCTION__ + "\tMsg:" + (msg) + "\tArgs={" + __str + "}";                     \
        logger << logmsg;             \
    }while(0)

    class ErrorLog {
        class Watcher
        {
        public:
            ~Watcher()
            {
                if(logger)
                {
                    delete logger;
                }
            }
        };

        static ErrorLog* logger;
        static Watcher watcher;
        std::ofstream out;
        Error error;

        ErrorLog()
        {
            out.open("log.log",std::ios::out);
            error = None;
        }

    public:
        std::unordered_map<Error,std::string> errorMsg
                {
                        {None,"* No Error *"},
                        {Out_Of_Bound,"* Out of Bound *"},
                        {Pointer_To_Null,"* Pointer to null *"},
                        {Bad_Malloc,"* Bad Malloc *"},
                };
        static ErrorLog& getLogger()
        {
            if(!logger)
            {
                logger = new ErrorLog;
            }
            return *logger;
        }

        void operator <<(const std::string msg)
        {
            ShowMessage(msg,error == None ? CFC_Cyan : CFC_Red);
            out << msg << std::endl;
            error = None;
        }

        void operator[](Error error_)
        {
            error = error_;
        }

        ~ErrorLog()
        {
            out.close();
        }
    };

    ErrorLog* ErrorLog::logger = nullptr;
    ErrorLog::Watcher ErrorLog::watcher;
}



#endif //ERRORLOG_ERRORLOG_H
