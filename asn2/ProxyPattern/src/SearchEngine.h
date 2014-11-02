
#ifndef __ProxyFile__SearchEngine__
#define __ProxyFile__SearchEngine__

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include "ItemProxy.h"


using namespace std;

class SearchEngine
{
public:
    static const string APPNAME;
    SearchEngine();
    
    //Search the indexed data for query
    vector<ItemProxy *> searchQuery(string q);
    
private:
    void log(string l) {cout << APPNAME << ":\t\t" <<l<< "\n";}
};

#endif /* defined(__ProxyFile__SearchEngine__) */
