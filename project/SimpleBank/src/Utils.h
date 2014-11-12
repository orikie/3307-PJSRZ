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

using namespace std;
class Utils
{
public:
    static string HashPassword(string);
    static vector<string> ExplodeString(string, string);
    
    static string DateString();
};

#endif /* defined(__SimpleBank__Utils__) */
