

#include "DiscoveryClient.h"

const string DiscoveryClient::APPNAME{"DiscoveryClient"};

DiscoveryClient::DiscoveryClient()
{
    log("Starting Discovery System");
    
    currentCase_ = {234, "Proxy PA Ltd vs. Observer PD Corp. "};
    
    log("Initiating case # " + to_string(currentCase_.caseNum));
    log("Active case: " + currentCase_.caseDesc);
    
}

void DiscoveryClient::connectToServer(SearchEngine &se)
{
    log("Connected to " + se.APPNAME);
    se_ = &se;
}

void DiscoveryClient::displayResults()
{
    log("Displaying Proxy Results:");
    
    for (int i = 0; i< ipResultsCache_.size(); i++) {
        log(ipResultsCache_[i]->getInfo());
    }
    
}

void DiscoveryClient::searchQuery(string q)
{
    ipResultsCache_ = se_->searchQuery(q);
    log("Successfully received results: " + to_string(ipResultsCache_.size()));
    
}

void DiscoveryClient::openOriginal(int i)
{
    log("Requesting full restore of item " + to_string(i));
    ipResultsCache_[i]->openOriginal();

}

void DiscoveryClient::log(string l)
{
    cout << APPNAME << ":\t" << l << "\n";
}

DiscoveryClient::~DiscoveryClient()
{
    for (int i = 0; i< ipResultsCache_.size(); i++) {
        delete ipResultsCache_[i];
    }
}