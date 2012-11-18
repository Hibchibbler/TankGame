#include "LogFile.h"
using namespace tg;

LogFile* LogFile::get(){
    static LogFile instance;
    return &instance;
}

//
void LogFile::log(int level, int code, std::string msg){
    if (initialized){
        time_t t;
        time(&t);
        char str[0x100];
        if(!ctime_s(str, 0x100, &t)){
            str[24] = '\0';//we change \n to \0.
            out << str << ", ";
        }

        out << level << ", " << code << ", " << msg.c_str() << std::endl << std::flush;
    }
}
    
LogFile::~LogFile(){
    if (initialized){
        out.close();
    }
}

LogFile::LogFile(){
    initialized = false;
    out.open("log.txt", std::ios::out |  std::ios::trunc);
    if (out.is_open()){
        initialized = true;
        time_t t;
        time(&t);
        char str[0x100];
        if(!ctime_s(str, 0x100, &t)){
            str[24] = '\0';//we change \n to \0.
            out << str;
        }
        out << "  ========== New Instance ==========" << std::endl << std::flush;
            
    }
}