#ifndef __ProxySkeletonArchive__ItemProxy__
#define __ProxySkeletonArchive__ItemProxy__

#include <stdio.h>
#include <string>

class ItemProxy
{
public:
    ItemProxy();
    ItemProxy(const std::string &);
    unsigned int getID(){return this->id_;};

private:
    
    unsigned int id_;
    std::string name_;
    
};

#endif
