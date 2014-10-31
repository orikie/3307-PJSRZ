

#ifndef __ProxySkeletonArchive__SearchEngine__
#define __ProxySkeletonArchive__SearchEngine__

#include <stdio.h>
#include <vector>
#include <iostream>

#include "ItemProxy.h"
using namespace std;

class SearchEngine
{
public:
    SearchEngine();
    
    bool connectClient();
    std::vector<ItemProxy> searchQuery(std::string);
    void log(string l){cout << this->APPNAME_ << ":" << l << "\n";};
    ~SearchEngine(){};
private:
    const string APPNAME_;
    
};

#endif
