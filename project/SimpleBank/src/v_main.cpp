




#include "VendorClient.h"

//
#include "DBDelegate.h"

//

int main(int argc, const char * argv[]) {
    

    //VendorClient c; c.start();
    
    DBDelegate d;
    cout << d.NewTransaction(2, "chicken", 122.25, "June 2, 2007");
    
    return 0;
}
