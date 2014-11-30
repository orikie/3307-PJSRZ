//
//  VendorClient.h
//  SimpleBank
//
//  Created by Joey Lin on 11/22/14.
//
//

#ifndef __SimpleBank__VendorClient__
#define __SimpleBank__VendorClient__

#include <stdio.h>


#include "Utils.h"
#include "Vendor.h"
#include "Logger.h"

class VendorClient
{
public:
    VendorClient();
    VendorClient(string name);
    ~VendorClient();
    
    void start();
    string vendorName;
    
private:
    
    void welcomeScreen();
    bool login();
    void logout();
    void mainScreen();
    void viewSpecial();
    
    void log(string l){log_.logTrace(l);}
    

    Vendor vendor_;
    Logger log_;
    
    
};

#endif /* defined(__SimpleBank__VendorClient__) */
