//
//  Logger.h
//  Simple logging class
//  Uses default file name from config
//
//  Created: 2014-09-16
//

#ifndef __SimpleBank__Logger__
#define __SimpleBank__Logger__

#include <iostream>
#include <fstream>
#include "Utils.h"
#include "CONFIG.h"

using namespace std;
class Logger
{
public:
    Logger();
    ~Logger();
    Logger(const string &);
    
    void logTrace(const string &);
    void logErr(const string &);
    void flushLog();
    void setON(bool n){this->loggerON_ = n;}
    void setAppName(const string & s) {this->appName_ = s;}
    
    bool isON(){return loggerON_;}
    
    string currUserID;
    
private:
    string nowDateString();
    void init(const string &);
    string appName_;
    
    
    bool loggerON_;
    ofstream fileOut_;
    string logFileName_;
    
};
#endif /* defined(__SimpleBank__Logger__) */
