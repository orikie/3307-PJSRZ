

#include <iostream>
#include "DiscoveryClient.h"

int main(int argc, const char * argv[]) {

    DiscoveryClient dc;
    dc.connect();
    dc.searchQuery("All files regarding patent XYZ from Department ABC");
    
    dc.displayResults();
    dc.disconnect();
    
    return 0;
}
