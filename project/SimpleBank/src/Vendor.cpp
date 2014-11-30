//
//  Vendor.cpp
//

#include "Vendor.h"

vector<string> Vendor::Inventory = {
"CHIPS", "GRANOLA BARS", "CANDY", "POPCORN", "COOKIES", "CRACKERS","RAISONS", "PEANUTS",
"MILK","COFFEE","BEER","WINE","JUICE","TEA","WATER","HOT COCA",
"PASTA","CHILI","TOMATO SAUCE","RICE","BEANS","CHICKEN BROTH","TUNA","SALMON",
"CHICKEN","BEEF","PORK","BACON","SAUSAGE","TURKEY","BROCOLI","SPINACH",
};

Vendor::Vendor(){
    loggedOn_ = false;
    log_.setAppName("VENDOR");
}
Vendor::~Vendor(){}

bool Vendor::logon(string uname, string pass)
{
    
    string db_pass = bank_db_.GetPasswordHash(uname);
    if (Utils::HashPassword(pass) == db_pass) {
        loggedUID_ = bank_db_.GetUID(uname);
        loggedUsername_ = uname;
        loggedOn_ = true;
        return true;
    }
    return false;
}

bool Vendor::getLoggedUserData(int &id, string &uname)
{
    if (loggedOn_) {
        id = loggedUID_;
        uname = loggedUsername_;
        return true;
    }
    
    return false;
    
}

void Vendor::logout()
{
    loggedOn_ = false;
    
}

bool Vendor::IsUserCreditValid(string uname)
{
    return bank_db_.IsUserCreditValid(uname);
}

void Vendor::purchaseItem(string desc, double price)
{
    log_.logTrace("Purchase for "+ desc+" approved: Notifying bank system database.");
    if (loggedOn_) {
        bank_db_.NewTransaction(loggedUID_, desc, price, Utils::DateString());
        double cc_bal = bank_db_.GetAccountBalance(loggedUID_, 2); //2 == credit
        cc_bal += price;
        bank_db_.UpdateAccountBalanceDel(loggedUID_, 2, cc_bal);
    }
}