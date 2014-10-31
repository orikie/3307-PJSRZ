

#ifndef __ProxySkeletonArchive__DiscoveryClient__
#define __ProxySkeletonArchive__DiscoveryClient__

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include "SearchEngine.h"

using namespace std;



class DiscoveryClient
{
public:
    
    DiscoveryClient();
    
    void connect();
    
    void disconnect();
    
    void searchQuery(std::string q);
    void displayResults();
    void log(string l){cout << this->APPNAME_ << ":" << l << "\n";};
    
    ~DiscoveryClient();
    
private:
    const string APPNAME_;
    bool connected_;
    SearchEngine se_;
    
    std::vector<ItemProxy> cachedResults_;

};

#endif
