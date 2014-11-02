

#include "SearchEngine.h"

const string SearchEngine::APPNAME{"SearchEngine"};

SearchEngine::SearchEngine()
{
    log("Starting " + APPNAME);
}

//Generate sample results in the form of proxy files
vector<ItemProxy *> SearchEngine::searchQuery(string q)
{
    log("Processing query: " + q);
    
    vector<ItemProxy *> results;
    
    for (int i = 0; i < 7; i++) {
        ItemProxy * ip;
        
        if ((i % 3)==0)
        {
            ip = new FileProxy(i, "System File");
        }
        else if ((i % 2)==0)
        {
            ip = new ImageProxy(i, "Image File");
        }
        else if ((i % 1)==0)
        {
            ip = new EmailProxy(i, "Email File");
        }

        results.push_back(ip);
    }
    
    return results;
}
