//
//  Logger.cpp
//
//  Created: 2014-09-16
//

#include "Logger.h"

Logger::Logger()
{
    init(LOG_FILENAME);
}

Logger::Logger(const string& f)
{
    init(f);
}

void Logger::init(const string & f)
{
    logFileName_ = f;
    this->fileOut_.open(logFileName_, std::ofstream::app);
    this->loggerON_ = true;
    currUserID = "SYS";
}

void Logger::flushLog()
{
    this->fileOut_.flush();
}

void Logger::logTrace(const string & s)
{
    if(this->fileOut_.is_open() && loggerON_)
    {
        string log;
        log += Utils::DateString() + "|" + currUserID + "|\t"+ s + "\n";
        this->fileOut_ << log;
    }
}

void Logger::logErr(const string & s)
{
    logTrace("Error: " + s);
}

string Logger::nowDateString()
{
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    string s = asctime(timeinfo);

    //removew newline
    if(s.at(s.length()-1)=='\n')
    {
        s.erase(s.length() -1);
    }

    return s;
}

Logger::~Logger(){
    if(this->fileOut_.is_open())
    {
        this->fileOut_.close();
    }
}
