//
//  Utils.cpp
//
//  Created: 2014-09-18
//

#include "Utils.h"

string Utils::HashPassword(string p)
{
    ostringstream os;
    hash<string> hash_fn;
    os << hash_fn(p);
    return os.str();
}

vector<string> Utils::ExplodeString(string s, string delim)
{
    vector<string> vs;
    
    char * cstr_temp = new char[s.length()+1];
    strcpy(cstr_temp, s.c_str());
    
    char * pch;
    pch = strtok(cstr_temp, delim.c_str());
    while (pch!=NULL)
    {
        vs.push_back(pch);
        pch = strtok(NULL, delim.c_str());
        
    }
    delete[] cstr_temp;
    return vs;
}

string Utils::DateString()
{
    time_t rawtime;
    struct tm * timeinfo;
    
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    string s = asctime(timeinfo);
    
    //removew newline
    if(s.at(s.length()-1)=='\n')
    {
        s.erase(s.length() -1);
    }
    
    return s;
}

