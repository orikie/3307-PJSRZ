//
//  BankClient.cpp
//
//  Created: 2014-09-19
//


#include <iostream>
#include <cstdio>

#include "BankClient.h"


const unsigned int SB::BankClient::PASSWORD_RETRY_LIM = 3;

BankClient::BankClient()
{
    logger_.setAppName("SB_Console Client");
}

BankClient::~BankClient()
{}

void BankClient::start()
{
    log("Starting Console Client");
    welcomeScreen();
}

void BankClient::welcomeScreen()
{
    
    cout << "SimpleBank Client\n" << Utils::DateString() << "\n\n";
    
    string welcome = "Welcome to SimpleBank main console portal!\n Please select an option to continue.\n\n";
    
    cout << welcome;
    menuline();
    printf("1\tLogin\n");
    //printf("2\tHelp\n");
    printf("2\tQuit\n");
    printf(">");
    int res = getIntFromUser(3);
    
    if (res == 1) {
        if (!login()) {
            //welcomeScreen();
        }
        else
        {
            User::UserType ut = userCache_.getUserType();
            
            switch (ut) {
                case User::UserType::CLIENT:
                    clientMenu();
                    break;
                case User::UserType::MGR:
                    mgrMenu();
                    break;
                case User::UserType::MNT:
                    mntMenu();
                    break;
                default:
                    break;
            }
        }
    }else if (res == 2)
    {
        return;
    }
    
    welcomeScreen();
}

void BankClient::clientMenu()
{
    mainMenuInfo();
    menuline();
    cout << "1\tAccess Savings Account\n";
    cout << "2\tAccess Checking Account\n";
    cout << "3\tTransfer funds to\\from account\n";
    cout << "4\tChange Password\n";
    cout << "5\tLogout\n>";
    
    int res = getIntFromUser(5);
    switch (res) {
        case 1:
            accessSavings();
            break;
        case 2:
            accessChecking();
            break;
        case 3:
            transferFunds();
            break;
        case 4:
            changePassword();
        case 5:
            logout();
            return;
        default:
            break;
    }
    
    clientMenu();
}

void BankClient::changePassword()
{
    log("Changing password");
    cout << "Enter new Password: \n>";
    string s = getWordFromUser();
    Client c = getcurrClient();
    c.changePassword(s);
    bankServer_.updateClient(c.getID(), c);
    cout << "Udpated password\n";
    waitForContinue();
}

void BankClient::printClientAccountBalances()
{
    Client c = getcurrClient();
    cout << "Savings (balance): " << c.getSavingsBalance() << "\n";
    cout << "Checking (blance): " << c.getCheckingBalance() << "\n";
}

void BankClient::transferFunds()
{
    cout << "Account fund transfer\n";
    Client c = getcurrClient();
    printClientAccountBalances();
    menuline();
    cout << "1\tTransfer from Savings to Checking\n";
    cout << "2\tTransfer from Checking to Savings\n";
    cout << "3\tExit\n>";
    
    int res = getIntFromUser(3);
    switch (res) {
        case 1:
            savingsToChecking();
            break;
        case 2:
            checkingToSavings();
        case 3:
            return;
        default:
            break;
    }
}

void BankClient::checkingToSavings()
{
    Client c = getcurrClient();
    cout << "Enter funds amount to transfer: ";
    string amt = getWordFromUser();
    double amtd = stod(amt);
    if (amtd > 0) {
        log("Transferring " + amt + " from checking to savings.");
        double sRem = -1;
        double cRem = -1;
        
        if (c.withdrawChecking(amtd, cRem)) {
            if (c.isSavingOpened()) {
                c.depositSaving(amtd, sRem);
            }
            else
            {
                cout << "Error: Savings account is not yet opened. Please go back to access it and create one.\n";
                goto wait;
            }
            
            bankServer_.updateClient(c.getID(), c);
            cout << "Successfully transfered " << amtd <<".\n";
            cout << "Savings (balance): " << c.getSavingsBalance() << "\n";
            cout << "Checking (blance): " << c.getCheckingBalance() << "\n";
        }
        else
        {
            cout << "Unable to transfer. Insufficient checking funds.\n";
        }
    }else
    {
        cout << "Error. Transfer amount must be positive";
    }

wait:
    waitForContinue();

}

void BankClient::savingsToChecking()
{
    
    Client c = getcurrClient();
    cout << "Enter funds amount to transfer: ";
    string amt = getWordFromUser();
    double amtd = stod(amt);
    if (amtd > 0) {
        log("Transferring " + amt + " from savings to checking.");
        double sRem = -1;
        double cRem = -1;
        
        if (c.withdrawSaving(amtd, sRem)) {
            if (c.isCheckingOpened()) {
                c.depositChecking(amtd, cRem);
            }
            else
            {
                cout << "Error: Checking account is not yet opened. Please go back to access it and create one.\n";
                goto wait;
            }
            
            bankServer_.updateClient(c.getID(), c);
            cout << "Successfully transfered " << amtd <<".\n";
            cout << "Savings (balance): " << c.getSavingsBalance() << "\n";
            cout << "Checking (blance): " << c.getCheckingBalance() << "\n";
        }
        else
        {
            cout << "Unable to transfer. Insufficient savings funds.\n";
        }
    }else
    {
        cout << "Error. Transfer amount must be positive";
    }
wait:
    waitForContinue();
}

void BankClient::refreshCache()
{
    userCache_ = bankServer_.getUser(userCache_.getID());
}

void BankClient::accessSavings()
{
    log("Accessing Savings Account");
    if (userCache_.getUserType() == User::UserType::CLIENT) {
        Client c = getcurrClient();
        if (c.isSavingOpened()) {
            cout << "\nSavings (balance): $" << c.getSavingsBalance() << "\n";
            //accountAction();
            accountAction(true);
            return;
        }
        else
        {
            cout << "Your do not have a savings account. Would you like to open one now? (y/n)\n>";
            string o = getWordFromUser();
            if ((o=="y") || (o=="Y")) {
                c.activateSaving();
                bankServer_.updateClient(userCache_.getID(), c);
                refreshCache();
                
                cout << "Created Checing Account";
                //waitForContinue();
                
                accessSavings();
                return;
            }
            else
            {
                cout << "An account was not opened\n";
            }
        }
        
    }
    waitForContinue();
}

void BankClient::accessChecking()
{
    log("Accessing Checking Account");
    if (userCache_.getUserType() == User::UserType::CLIENT) {
        Client c = getcurrClient();
        if (c.isCheckingOpened()) {
            cout << "\nChecking (balance): $" << c.getCheckingBalance() << "\n";

            accountAction(false);
            return;
        }
        else
        {
            cout << "Your do not have a checking account. Would you like to open one now? (y/n)\n>";
            string o = getWordFromUser();
            if ((o == "y") || (o == "Y")) {
                c.activateChecking();
                bankServer_.updateClient(userCache_.getID(), c);
                refreshCache();
                
                cout << "Created Checking Account";
                //waitForContinue();
                
                accessChecking();
                return;
            }
            else
            {
                cout << "An account was not opened\n";
            }
        }
        
    }
    
    waitForContinue();
}

void BankClient::accountAction(bool isSaving)
{
    //isSaving = true: access for Savings account
    //isSAving = false: access for Checking account
    menuline();
    cout << "1\tWithdraw\n";
    cout << "2\tDeposit\n";
    cout << "3\tExit\n>";
    
    int res = getIntFromUser(3);
    
    switch (res) {
        case 1:
            withdraw(isSaving);
            break;
        case 2:
            deposit(isSaving);
            break;
        default:
            break;
    }
    
}

void BankClient::withdraw(bool isSaving)
{
    string accType = isSaving ? "savings" : "checking";
    
    string uid = userCache_.getID();
    
    cout << "Enter amount to withdraw:";
    string amt = getWordFromUser();
    double amtd = stod(amt);
    
    if (amtd < 0) {
        cout << "Error: Unable to process negative values";
        return;
    }
    
    if (amtd > 0) {
        Client c = getcurrClient();
        double remaining = -1;
        
        if (!isSaving) {
            double amtf = c.getCheckingBalance() - amtd;
            if (amtf < 1000) {
                cout << "Warning: Continuing withdraw will leave a balance of less than $1000. A $2 fee will be leived. Continue? (y/n)\n>";
                string s = getWordFromUser();
                if (s == "n") {
                    return;
                }
                else
                {
                    amtd += 2;
                }
            }
        }
        
        bool success = isSaving ? c.withdrawSaving(amtd, remaining) : c.withdrawChecking(amtd, remaining);
        
        if (success) {
            log("Withdrawing " + amt + " from " + accType);
            cout << "New Balance: $" << remaining;
        }
        else
        {
            cout << "Withdraw Failed: Insufficient funds\n";
        }
        
        bankServer_.updateClient(uid, c);
        refreshCache();
        
    }
    else
    {
        cout << "Error. Cannnot withdraw negative values\n";
    }
    
    waitForContinue();
    
}

void BankClient::deposit(bool isSaving)
{
    string accType = isSaving ? "savings" : "checking";
    
    string uid = userCache_.getID();
    
    cout << "Enter amount to deposit:";
    string amt = getWordFromUser();
    double amtd = stod(amt);
    if (amtd > 0) {
        log("Depositing "+ amt+" to " + accType);
        Client c = getcurrClient();
        double remaining = -1;
        bool success = isSaving ? c.depositSaving(amtd, remaining) : c.depositChecking(amtd, remaining);
        
        if (success) {
            log("Depositing " + amt + " to " + accType);
            cout << "New Balance: $" << remaining;
        }
        
        bankServer_.updateClient(uid, c);
        refreshCache();
        
    }
    else
    {
        cout << "Error. Cannnot deposit negative values\n";
    }
    
    waitForContinue();
    
}

void BankClient::logout()
{
    log("Logging out user: " + userCache_.getID());
    bankServer_.logout();
    logger_.currUserID = "SYS";
}

bool BankClient::login()
{
    unsigned int retry = 0;
    bool success = false;
    
    while (!success) {
        cout << "\nUser ID: ";
        string u = getWordFromUser();
        
        cout << "password:";
        string p = getWordFromUser();
        
        success = this->bankServer_.logon(u, p);
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
        
        logger_.currUserID = u;
        log("Succesfully logged in as " + u);
        
        User loggedUser = bankServer_.getLoggedUser();
        
    }
    
    userCache_ = this->bankServer_.getLoggedUser();
    //clearScreen();
    
    return true;
}


void BankClient::mntMenu()
{
    mainMenuInfo();
    menuline();
    
    printf("1\tSet Trace Setting\n");
    printf("2\tLogout\n");
    cout << ">";
    int s = getIntFromUser(2);
    
    switch (s) {
        case 1:
            setTraceSetting();
            break;
        case 2:
            logout();
            return;
        default:
            break;
    }
    
    mntMenu();
}

void BankClient::setTraceSetting()
{
    cout << "Diagnostics tracing\n";
    cout << "Turn on logging to track bank activity.\n";
    string logginSetting = logger_.isON() ? "ON" : "OFF";
    cout << "Logging: " << logginSetting << "\n";
    
    cout << "\nSet Logging Setting:\n";
    cout << "1\t ON\n";
    cout << "2\t OFF\n";
    cout << "3\t EXIT\n>";
    
    int res = getIntFromUser(3);
    
    if (res <=2) {
        string set = (res == 1) ? "ON" : "OFF";
        log("Seting logging to: " + set);
        
    }
    
    switch (res) {
        case 1:
            this->logger_.setON(true);
            break;
        case 2:
            this->logger_.setON(false);
            break;
        case 3:
            return;
        default:
            break;
    }
    
    logginSetting = res == 1 ? "ON" : "OFF";
    string logStr ("Logging set to: " + logginSetting + "\n");
    log(logStr);
    cout << logStr;
    waitForContinue();
    
}


void BankClient::menuline()
{
    string dash (10, '-');
    cout << dash.c_str() << "menu" << dash.c_str()<< "\n";
}

void BankClient::mgrMenu()
{
    mainMenuInfo();
    menuline();
    
    printf("1\tAudit Single User\n");
    printf("2\tList All User Account info\n");
    printf("3\tOpen New Client Account\n");
    printf("4\tClose Existing User Account\n");
    printf("5\tBank Stats\n");
    //printf("6\tChange Passworn\n");
    printf("7\tLogout\n");
    
    cout << "\n>";
    int res = getIntFromUser(7);
    
    switch (res) {
        case 1:
            auditUser();
            break;
        case 2:
            listAllInfo();
            break;
        case 3:
            openNewUserAccount();
            break;
        case 4:
            closeUserAccount();
            break;
        case 5:
            bankStats();
            break;
        case 6:
            changePassword();
        case 7:
            logout();
            return;
        default:
            return;
            break;
    }
    
    waitForContinue();
    mgrMenu();
}


void BankClient::bankStats()
{
    log("Viewing bank statistics");
    cout << "Bank Statistics\n";
    cout << "Cash Reserve: " << bankServer_.getCashReserve();
    cout << "\nTotal Clients: " << bankServer_.getTotalUsers() << "\n";

}

void BankClient::openNewUserAccount()
{
    cout << "Enter new userid: ";
    string s = getWordFromUser();
    
    log("Creating new client account:" + s);
    bankServer_.newUser(s, Client::UserType::CLIENT);
    
    bankServer_.save();
    cout << "Created new user: " << s << endl;
}

void BankClient::closeUserAccount()
{
    cout << "Enter userid to close:";
    string s = getWordFromUser();
    if (bankServer_.userExist(s)) {
        Client c = bankServer_.getClient(s);
        
        if (!c.isCloseable()) {
            cout << "Unable to close client account!\n";
            cout << "Client " << s << " must have 0 balances on both its accounts\n";
            return;
        }
        bankServer_.deleteUser(s);
        bankServer_.save();
        cout << "User " << s << " has been deleted.\n";
        log("Closing client account " + s);
    }
    else
    {
        log("Account " + s + " could not be closed. It does not exist");
        cout << "User " << s << " does not exist.\n";
    }
}

void BankClient::auditUser()
{
    cout << "\nEnter the userid to audit: >";

    string u = getWordFromUser();
    bool found = auditUser(u);
    log("Auditing account info for " + u);
    if (!found) {
        cout << "User "<< u << " was not found!\n";
    }
    return;
}

bool BankClient::auditUser(const string s)
{
    bool found = this->bankServer_.userExist(s);
    if (found) {
        DataEntry de = bankServer_.exportUser(s);
        
        cout << "\nUser ID:\t\t" << de.uid;
        cout << "\nRole:\t\t" << User::usertypeToString((User::UserType)de.utype);
        
        cout << "\nChecking:\t\t" << de.checkingBalance;
        cout << "\nSavings:\t\t" << de.savingsBalance << "\n";
        
    }
    
    return found;
}

void BankClient::listAllInfo()
{
    cout << "----------User Database----------\n";
    log("Listing all user info");
    cout << this->bankServer_.exportInfo();
    cout << "---------------------------------\n";
}

void BankClient::waitForContinue()
{
    cout << "\n\nEnter a key to continue..." << endl;
    getWordFromUser();
}


void BankClient::mainMenuInfo()
{
    if (bankServer_.isLoggedOn()) {
        string uid = userCache_.getID();
        string date = Utils::DateString();
        
        string utype = User::usertypeToString(userCache_.getUserType());
        
        cout << "\nWelcome " << uid << "!\nYour role: " << utype << "\n"<< date << "\n\n";
    }
}

string BankClient::getWordFromUser()
{
    string s {""};
    cin >> s;
    return s;
}

int BankClient::getIntFromUser(const int lim)
{
    int num = -1;
    
    bool success = false;
    
    do {
        cin >> num;
        
        success = !cin.fail();
        
        if (!success || num <= 0 || num > lim) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "\nBad input: please enter options 1 - " << lim << "\n>";
        }
        else
        {
            break;
        }
    } while (1);
    
    return num;
}


void BankClient::clearScreen()
{
    if (system("clear"))
    {
        system("CLS");
    }
}


