

#include "DiscoveryClient.h"

DiscoveryClient::DiscoveryClient(): APPNAME_("DiscoveryClient")
{
    
    this->connected_ = false;

}

void DiscoveryClient::connect()
{
    log("Connecting Discovery Client to Enterprise Search Engine...");
    if (!connected_) {
        connected_ = se_.connectClient() ? true : false;
        log("Succesfully connected to search server");
    }
}

void DiscoveryClient::searchQuery(std::string q)
{
    log("Sending query: " + q);
    cachedResults_ =  this->se_.searchQuery(q);
    log("Total results found: " + to_string(cachedResults_.size()));
}

void DiscoveryClient::displayResults()
{
    
}

void DiscoveryClient::disconnect()
{
    connected_ = false;
}