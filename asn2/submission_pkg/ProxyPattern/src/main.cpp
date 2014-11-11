
/**
 *  Simple Proxy Pattern skeleton code
 *
 */

#include <iostream>
#include "DiscoveryClient.h"
#include "ItemProxy.h"

int main(int argc, const char * argv[]) {
    
    DiscoveryClient dc;
    
    SearchEngine * se = new SearchEngine();
    
    dc.connectToServer(*se);
    
    dc.searchQuery("All files regarding patent XY from 2000 to 2014");
    
    //DisplayResults only list the inexpensive proxy items
    dc.displayResults();
    
    //Explicitly call to open the original items
    dc.openOriginal(2);
    dc.openOriginal(3);
    

    delete se;
    
    return 0;
}
