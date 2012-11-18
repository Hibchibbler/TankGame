#ifndef LogFile_h_
#define LogFile_h_

#include <fstream>
#include <time.h>

namespace tg
{
    class LogFile
    {
    public:
        static LogFile* get();
        void log(int level, int code, std::string msg);
        ~LogFile();
    private:
        std::ofstream out;
        bool initialized;

        LogFile();

    
    };

};
#endif

