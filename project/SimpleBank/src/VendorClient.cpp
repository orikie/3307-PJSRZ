//
//  VendorClient.cpp
//  SimpleBank
//
//  Created by Joey Lin on 11/22/14.
//
//

#include "VendorClient.h"

VendorClient::VendorClient(){
    log_.setAppName("VENDOR");
    vendorName = "G8'S GROCER";
}

VendorClient::VendorClient(string name)
{
    log_.setAppName("VENDOR");
    vendorName = name;
}

VendorClient::~VendorClient(){}

void VendorClient::start()
{
    welcomeScreen();
    log_.logTrace("Starting console vendor client");
}

void VendorClient::welcomeScreen()
{
    string welcome = "[Welcome to " +vendorName+ " self checkout!]\n\nYOUR EVERYDAY FOODS | CHECKOUT\n"+Utils::DateString()+"\n<Please select an option to continue>\n\n";
    
    cout << welcome;
    Utils::menuline();
    printf("1\tLogin\n");
    //printf("2\tHelp\n");
    printf("2\tQuit\n");
    printf(">");
    int res = Utils::getIntFromUser(2);
    
    if (res == 1) {
        if (!login()) {
            //welcomeScreen();
        }
        else
        {
            mainScreen();
        }
    }else if (res == 2)
    {
        return;
    }
    
    welcomeScreen();
}

bool VendorClient::login()
{
    log_.logTrace("Authenticating user...");
    unsigned int retry = 0;
    bool success = false;
    
    while (!success) {
        cout << "\nUser ID: ";
        string u = Utils::getWordFromUser();
        
        cout << "PIN:";
        string p = Utils::getWordFromUser();
        
        success = this->vendor_.logon(u, p);
        log("Attempting to login as " + u);
        
        if (!success) {
            log_.logTrace("Login failed.");
            log("Login failed for user " + u + "Attempt: " + to_string(retry));
            ++ retry;
            cout << "Incorrect username or password.\n\n";
            if (retry >= PASSWORD_RETRY_LIM ) {
                cout << "Maxium retries encounted. Redirecting..\n\n";
                return false;
            }
        }
        
        log("Successfully logged in as " + u);
    }
    return true;
}

void VendorClient::logout()
{
    vendor_.logout();
    log("Logging out vendor");
}

void VendorClient::mainScreen()
{
    int uid;
    string uname;
    
    vendor_.getLoggedUserData(uid, uname);
    cout << "[Welcome " + uname + "]\n";
    cout << "\n" << Utils::DateString() << "\n";
    

    if (!vendor_.IsUserCreditValid(uname)) {
        string msg = "Your Credit Card is not valid or is frozen!\nPlease contact your bank for assistance.\n";
        cout << msg;
        log(msg);
        Utils::waitForContinue();
        return;
    }
    else
    {
        log_.logTrace("Credit card approved for user");
        cout << "\nYou have a valid credit card from SimpleBank!\n\n";
        printf("1\tBuy today's special!\n");
        printf("2\tLogout\n");
        printf(">");
        int res = Utils::getIntFromUser(2);
        if (res == 1) {
            viewSpecial();
        }else if (res ==2)
        {
            logout();
            return;
        }
    }
    
    mainScreen();
}


void VendorClient::viewSpecial()
{
    unsigned int ranItemIndex = Utils::GetRandomInt((int)vendor_.Inventory.size());
    
    int randomPrice = Utils::GetRandomInt(MAX_RANDOM_PRICE);
    if (randomPrice == 0) randomPrice = MAX_RANDOM_PRICE;
    
    cout << "\n[Special Item for sale!]\n";
    string item = vendor_.Inventory[ranItemIndex];
    printf("Item Name:\t%s \nPrice:\t$%d\n", item.c_str(),randomPrice);
    cout << "\nContinue with purchase? [y\\n]\n>";
    string res = Utils::getWordFromUser();
    if (res == "y" || res == "Y") {
        vendor_.purchaseItem(item, randomPrice);
    }
    log("Displaying special item");
}