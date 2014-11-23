//
//  Vendor.h
//  SimpleBank
//
//  Created by Joey Lin on 11/22/14.
//
//

#ifndef __SimpleBank__Vendor__
#define __SimpleBank__Vendor__

#include <stdio.h>
#include "DBDelegate.h"
#include <vector>

class Vendor
{
public:
    Vendor();
    ~Vendor();
    
    bool logon(string uname, string pass);
    void logout();
    static vector<string> Inventory;
    bool IsUserCreditValid(string uname);
    
    bool getLoggedUserData(int & id, string & uname);
    void purchaseItem(string desc, double price);
private:
    DBDelegate bank_db_;
    int loggedUID_;
    string loggedUsername_;
    bool loggedOn_;
    
    
    
};
#endif /* defined(__SimpleBank__Vendor__) */
