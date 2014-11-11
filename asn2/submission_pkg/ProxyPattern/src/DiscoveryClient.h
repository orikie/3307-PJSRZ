
#ifndef __ProxyFile__DiscoveryClient__
#define __ProxyFile__DiscoveryClient__

#include <stdio.h>
#include <string>
#include <vector>
#include "SearchEngine.h"
using namespace std;

struct LegalCase
{
    int caseNum;
    string caseDesc;
};

class DiscoveryClient
{
public:
    static const string APPNAME;
    
    DiscoveryClient();
    ~DiscoveryClient();
    void connectToServer(SearchEngine & se);
    void searchQuery(string q);
    void log(string l);
    void displayResults();
    void openOriginal(int i);
    
private:
    SearchEngine  * se_;
    vector<ItemProxy *> ipResultsCache_;
    LegalCase currentCase_; 
    
};


#endif /* defined(__ProxyFile__DiscoveryClient__) */
