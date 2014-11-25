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

string Utils::getWordFromUser()
{
    string s {""};
    cin >> s;
    return s;
}

int Utils::getIntFromUser(const int lim)
{
    int num = -1;
    bool success = false;
    do {
        cin >> num;
        success = !cin.fail();
        if (!success || num <= 0 || num > lim) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "\nBad input: please enter options 1 - " << lim << "\n>";
        }
        else
        {
            break;
        }
    } while (1);
    return num;
}

void Utils::waitForContinue()
{
    cout << "\n\nEnter a key to continue...";
    getWordFromUser();
}

void Utils::menuline()
{
    string dash (10, '-');
    cout << dash.c_str() << "menu" << dash.c_str()<< "\n";
}

void Utils::printFileToConsole(const string &fname)
{
    ifstream fileIn;
    fileIn.open(fname,ifstream::in);
    while (fileIn.good()) {
        string s;
        getline(fileIn, s);
        if (s.length()>0) cout << s << endl;
    }
}

int Utils::GetRandomInt(int max)
{
    srand(time(nullptr));
    return rand() % max;
}

