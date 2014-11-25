//
//  VendorClient.cpp
//  SimpleBank
//
//  Created by Joey Lin on 11/22/14.
//
//

#include "VendorClient.h"

const int VendorClient::PASSWORD_RETRY_LIM = 3;

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
            log("Login failed for user " + u + "Attempt: " + to_string(retry));
            ++ retry;
            cout << "Incorrect username or password.\n\n";
            if (retry >= PASSWORD_RETRY_LIM ) {
                cout << "Maxium retries encounted. Redirecting..\n\n";
                return false;
            }
        }
        
        log("Succesfully logged in as " + u);
    }
    return true;
}

void VendorClient::logout()
{
    vendor_.logout();
}

void VendorClient::mainScreen()
{
    int uid;
    string uname;
    
    vendor_.getLoggedUserData(uid, uname);
    cout << "[Welcome " + uname + "]\n";
    cout << "\n" << Utils::DateString() << "\n";
    

    if (!vendor_.IsUserCreditValid(uname)) {
        
        cout << "Your Credit Card is not valid or is frozen!\nPlease contact your bank for assistance.\n";
        Utils::waitForContinue();
        return;
    }
    else
    {
        cout << "\nYou have a valid credit card from SimpleBank!\n\n";
        printf("1\tBuy today's special!\n");
        printf("2\tLogout\n");
        printf(">");
        int res = Utils::getIntFromUser(2);
        if (res == 1) {
            viewSpecial();
        }else if (res ==2)
        {
            vendor_.logout();
            return;
        }
    }
    
    mainScreen();
}


void VendorClient::viewSpecial()
{
    int ranItemIndex = Utils::GetRandomInt(vendor_.Inventory.size());
    
    int randomPrice = Utils::GetRandomInt(100);
    if (randomPrice == 0) randomPrice = 100;
    
    cout << "\n[Special Item for sale!]\n";
    string item = vendor_.Inventory[ranItemIndex];
    printf("Item Name:\t%s \nPrice:\t$%d\n", item.c_str(),randomPrice);
    cout << "\nContinue with purchase? [y\\n]\n>";
    string res = Utils::getWordFromUser();
    if (res == "y" || res == "Y") {
        vendor_.purchaseItem(item, randomPrice);
    }
}