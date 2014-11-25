//
//  Utils.h
//
//  Created: 2014-09-18
//

#ifndef __SimpleBank__Utils__
#define __SimpleBank__Utils__

#include <sstream>
#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;
class Utils
{
public:
    static string HashPassword(string);
    static vector<string> ExplodeString(string, string);
    
    static string DateString();
    static int getIntFromUser(const int lim);
    static string getWordFromUser();
    static void waitForContinue();
    static void menuline();
    static void printFileToConsole(const string & fname);
    static int GetRandomInt(int max);
};

#endif /* defined(__SimpleBank__Utils__) */
