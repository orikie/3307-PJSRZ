
#include "SearchEngine.h"

SearchEngine::SearchEngine():APPNAME_("SearchEngine")
{

}

bool SearchEngine::connectClient()
{
    
    return true;
}

std::vector<ItemProxy> SearchEngine::searchQuery(std::string q)
{
    std::cout << "In engine";
    ItemProxy i("Item one");
    std::vector<ItemProxy> r;
    return r;
}