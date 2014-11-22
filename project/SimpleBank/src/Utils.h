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
};

#endif /* defined(__SimpleBank__Utils__) */
