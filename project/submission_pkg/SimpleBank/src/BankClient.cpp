//
//  BankClient.cpp
//
//  Created: 2014-09-19
//


#include <iostream>
#include <cstdio>

#include "BankClient.h"

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
            User::UserType ut = userCache_.getUserType();
            
            switch (ut) {
                case Client::CLIENT:
                    clientMenu();
                    break;
                case Client::MGR:
                    mgrMenu();
                    break;
                case Client::MNT:
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
    Utils::menuline();
    cout << "1\tAccess Savings Account\n";
    cout << "2\tAccess Checking Account\n";
    cout << "3\tTransfer funds to\\from account\n";
    cout << "4\tChange Password\n";
    cout << "5\tView Credit Report\n";
    cout << "6\tChange Credit Paying Option\n";
    cout << "7\tLogout\n>";
    
    int res = Utils::getIntFromUser(6);
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
            break;
        case 5:
            viewCreditPurchases();
            break;
        case 6:
            changeCreditOption();
            break;
        case 7:
            logout();
            return;
        default:
            break;
    }
    
    clientMenu();
}

void BankClient::changeCreditOption()
{
    int opt = bankServer_.GetCreditOption(userCache_.getID());
    string opt_s = opt == PAY_FULL ? "Pay in Full" : "Pay minimum 10%, with 2% interest.\n";
    cout << "Your current payment option: " << opt_s << "\n";
    cout << "Modify your payment option: \n";
    cout << "\t 1 Pay in full\n";
    cout << "\t 2 Pay minium (10% + interest)\n";
    cout << "\t 3 Return to menu\n";
    cout << ">";
    int res = Utils::getIntFromUser(3);
    
    if (res == 3) {
        return;
    }
    
    if (res == 1) {
        bankServer_.ChangeCreditOption(userCache_.getID(), PAY_FULL);
    }
    
    if (res == 2) {
        bankServer_.ChangeCreditOption(userCache_.getID(), PAY_MIN_10);
    }
    
    cout << "Your settings have been updated.\n";
    Utils::waitForContinue();
    
}

void BankClient::viewCreditPurchases()
{
    log("Viewing credit purchase history and report");
    string dash (57, '-');
    
    double total = 0.0;
    Client c = bankServer_.getClient(userCache_.getID());
    if (!c.isCheckingOpened()) {
        cout << "\n**Warning: You do you have a credit account.\nOpen a checking account to obtain one.\n";
    }
    else
    {
        cout << "\n[Monthly Credit Purchase History]\n" << dash;
        vector<db_transaction_record> t_records = bankServer_.GetTransactionRecords(userCache_.del_uid);
        if (t_records.size() > 0) {
            printf("\n%-30s%-20s%-15s\n", "Date","Item","Price");
            auto it = t_records.begin();
            while (it != t_records.end()) {
                db_transaction_record dtr = *it++;
                total += dtr.amount;
                printf("%-30s%-20s$%-.2f\n", dtr.date.c_str(),dtr.description.c_str(),dtr.amount);
            }
            
            cout << dash;
            printf("\n%-50s$%-.2f\n","Total:", total);
            
            
            cout << "\n[CREDIT SUMMARY]\n" << dash;
            double checkingBal = c.getCheckingBalance();
            printf("\n%-30s$%-.2f\n", "Checking Balance", checkingBal);
            
            double creditBal = bankServer_.GetAccountBalance(userCache_.del_uid, SB::Credit);
            printf("%-30s$%-.2f\n", "Unpaid Credit Balance", creditBal);
            
            int pc = -1;
            if (checkingBal > 0) {
                pc = (creditBal / checkingBal) * 100;
            }
            
            string pcs = to_string(pc);
            if (pc == -1) {
                pcs = "N/A";
            }
            printf("%-30s%%%s\n", "Credit to Checking ratio", pcs.c_str());
            
            if (pc >= 75) {
                cout << "\nWarning! Your credit spendings are at %" <<pc<<" of your checking balance!\n";
            }
            
            if (checkingBal < creditBal) {
                cout << "\nWarning! You do not have sufficient checking balance to support your credit spendings!\n";
            }
        }
        
    }
    
    Utils::waitForContinue();
}

void BankClient::changePassword()
{
    log("Changing password");
    cout << "Enter new Password: \n>";
    string s = Utils::getWordFromUser();
    
    if (s.length() > 0) {
        Client c = getcurrClient();
        c.changePassword(s);
        bankServer_.updateClient(c.getID(), c);
        cout << "Udpated password\n";
        bankServer_.ChangePasswordDel(userCache_.getID(), s);
    }
    else
    {
        cout << "Error: password length must be atleast 1.\n";
    }
    
    Utils::waitForContinue();
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
    Utils::menuline();
    cout << "1\tTransfer from Savings to Checking\n";
    cout << "2\tTransfer from Checking to Savings\n";
    cout << "3\tExit\n>";
    
    int res = Utils::getIntFromUser(3);
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
    string amt = Utils::getWordFromUser();
    double amtd = stod(amt);
    if (amtd > 0) {
        log("Transferring " + amt + " from checking to savings.");
        double sRem = -1;
        double cRem = -1;
        
        if (c.withdrawChecking(amtd, cRem)) {
            bankServer_.UpdateAccountBalanceDel(userCache_.del_uid, SB::Checking, cRem);
            if (c.isSavingOpened()) {
                c.depositSaving(amtd, sRem);
                bankServer_.UpdateAccountBalanceDel(userCache_.del_uid, SB::Savings, sRem);
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
    Utils::waitForContinue();

}

void BankClient::savingsToChecking()
{
    
    Client c = getcurrClient();
    cout << "Enter funds amount to transfer: ";
    string amt = Utils::getWordFromUser();
    double amtd = stod(amt);
    if (amtd > 0) {
        log("Transferring " + amt + " from savings to checking.");
        double sRem = -1;
        double cRem = -1;
        
        if (c.withdrawSaving(amtd, sRem)) {
            bankServer_.UpdateAccountBalanceDel(userCache_.del_uid, SB::Savings, sRem);
            
            if (c.isCheckingOpened()) {
                c.depositChecking(amtd, cRem);
                bankServer_.UpdateAccountBalanceDel(userCache_.del_uid, SB::Checking, cRem);
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
    Utils::waitForContinue();
}

void BankClient::refreshCache()
{
    userCache_ = bankServer_.getUser(userCache_.getID());
}

void BankClient::refreshCache(int del_id_c)
{
    userCache_ = bankServer_.getUser(userCache_.getID());
    userCache_.del_uid = del_id_c;
}

void BankClient::accessSavings()
{
    log("Accessing Savings Account");
    if (userCache_.getUserType() == Client::CLIENT) {
        Client c = getcurrClient();
        if (c.isSavingOpened()) {
            cout << "\nSavings (balance): $" << c.getSavingsBalance() << "\n";
            //accountAction();
            accountAction(true);
            return;
        }
        else
        {
            cout << "You do not have a savings account. Would you like to open one now? (y/n)\n>";
            string o = Utils::getWordFromUser();
            if ((o=="y") || (o=="Y")) {
                c.activateSaving();
                
                //Update del db
                bankServer_.OpenAccountDel(userCache_.del_uid, SB::Savings);
                //
                
                bankServer_.updateClient(userCache_.getID(), c);
                refreshCache(userCache_.del_uid);
                
                cout << "Created Savings Account";
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
    Utils::waitForContinue();
}

void BankClient::accessChecking()
{
    log("Accessing Checking Account");
    if (userCache_.getUserType() == Client::CLIENT) {
        Client c = getcurrClient();
        if (c.isCheckingOpened()) {
            cout << "\nChecking (balance): $" << c.getCheckingBalance() << "\n";

            accountAction(false);
            return;
        }
        else
        {
            cout << "You do not have a checking account. Would you like to open one now? (y/n)\n>";
            string o = Utils::getWordFromUser();
            if ((o == "y") || (o == "Y")) {
                c.activateChecking();
                
                //Update del db
                bankServer_.OpenAccountDel(userCache_.del_uid, SB::Checking);
                //Automatic Credit account
                bankServer_.OpenAccountDel(userCache_.del_uid, SB::Credit);
                //
                
                bankServer_.updateClient(userCache_.getID(), c);
                refreshCache(userCache_.del_uid);
                
                cout << "Created Checking Account!\n";
                cout << "You've qualified for Credit. Created Credit Card Account!";
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
    
    Utils::waitForContinue();
}

void BankClient::accountAction(bool isSaving)
{
    //isSaving = true: access for Savings account
    //isSAving = false: access for Checking account
    Utils::menuline();
    cout << "1\tWithdraw\n";
    cout << "2\tDeposit\n";
    cout << "3\tExit\n>";
    
    int res = Utils::getIntFromUser(3);
    
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
    string amt = Utils::getWordFromUser();
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
                string s = Utils::getWordFromUser();
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
            
            //Update del db
            int type = isSaving ? 0 : 1;
            bankServer_.UpdateAccountBalanceDel(userCache_.del_uid, type, remaining);
        }
        else
        {
            cout << "Withdraw Failed: Insufficient funds\n";
        }
        
        bankServer_.updateClient(uid, c);
        refreshCache(userCache_.del_uid);
        
    }
    else
    {
        cout << "Error. Cannnot withdraw negative values\n";
    }
    
    Utils::waitForContinue();
    
}

void BankClient::deposit(bool isSaving)
{
    log("");
    string accType = isSaving ? "savings" : "checking";
    
    string uid = userCache_.getID();
    
    cout << "Enter amount to deposit:$";
    string amt = Utils::getWordFromUser();
    
    double amtd = stod(amt);
    
    if (amtd > 0) {
        log("Depositing "+ amt+" to " + accType);
        Client c = getcurrClient();
        double remaining = -1;
        bool success = isSaving ? c.depositSaving(amtd, remaining) : c.depositChecking(amtd, remaining);
        
        if (success) {
            log("Depositing " + amt + " to " + accType);
            cout << "New Balance: $" << remaining;
            
            //Update del db
            int type = isSaving ? 0 : 1;
            bankServer_.UpdateAccountBalanceDel(userCache_.del_uid, type, remaining);
        }
        
        bankServer_.updateClient(uid, c);
        refreshCache(userCache_.del_uid);
        
        if (bankServer_.payCreditOverDues(userCache_.del_uid)) {
            cout << "\n\nYou have paid your credit card's outstanding balance!\nCredit is now unfrozen\n";
            cout << "\nNew Balance:\t$" << bankServer_.GetAccountBalance(userCache_.del_uid, ACCOUNT_TYPE_CHECKING);
        }
    }
    else
    {
        cout << "Error. Cannnot deposit negative values\n";
    }
    
    Utils::waitForContinue();
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
        string u = Utils::getWordFromUser();
        
        cout << "PIN:";
        string p = Utils::getWordFromUser();
        
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
        
        bankServer_.SetCurrDelID(bankServer_.GetLoggedUID(u));

    }
    
    userCache_ = this->bankServer_.getLoggedUser();
    //clearScreen();
    
    return true;
}


void BankClient::mntMenu()
{
    mainMenuInfo();
    Utils::menuline();
    
    printf("1\tSet Trace Setting\n");
    printf("2\tLogout\n");
    cout << ">";
    int s = Utils::getIntFromUser(2);
    
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
    
    int res = Utils::getIntFromUser(3);
    
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
    Utils::waitForContinue();
    
}


void BankClient::mgrMenu()
{
    mainMenuInfo();
    Utils::menuline();
    
    printf("1\tAudit Single User\n");
    printf("2\tList All User Account info\n");
    printf("3\tOpen New Client Account\n");
    printf("4\tClose Existing User Account\n");
    printf("5\tBank Stats\n");
    printf("6\tTrigger End of Month Event\n");
    printf("7\tView failed credit payments\n");
    printf("8\tLogout\n");
    
    cout << "\n>";
    int res = Utils::getIntFromUser(8);
    
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
            triggerEndofMonth();
            break;
        case 7:
            viewFailedCreditPayments();
            break;
        case 8:
            logout();
            return;
        default:
            return;
            break;
    }
    
    Utils::waitForContinue();
    mgrMenu();
}

void BankClient::viewFailedCreditPayments()
{
    cout << "[Customer Failed Credit Payments Log]\n";
    Utils::printFileToConsole(FAILED_PAYMENT_LOG_NAME);
    Utils::waitForContinue();
    
}

void BankClient::bankStats()
{
    log("Viewing bank statistics");
    cout << "Bank Statistics\n";
    cout << "Cash Reserve: " << bankServer_.getCashReserve();
    cout << "\nTotal Clients: " << bankServer_.getTotalUsers() << "\n";

}

void BankClient::triggerEndofMonth()
{
    string msg = "[Processing End of Month Credit Payments...]\n";
    cout << msg;
    log(msg);
    bankServer_.TriggerEndOfMonth();
}

void BankClient::openNewUserAccount()
{
    cout << "Enter new userid: ";
    string s = Utils::getWordFromUser();
    
    log("Creating new client account:" + s);
    bankServer_.newUser(s, Client::CLIENT);
    
    bankServer_.save();
    cout << "Created new user: " << s << endl;
    
    //db delegate
    bankServer_.newUserDelegate(s, DEFAULT_PASSWORD, Client::CLIENT);
    
}

void BankClient::closeUserAccount()
{
    cout << "Enter userid to close:";
    string s = Utils::getWordFromUser();
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

    string u = Utils::getWordFromUser();
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



void BankClient::mainMenuInfo()
{
    if (bankServer_.isLoggedOn()) {
        string uid = userCache_.getID();
        string date = Utils::DateString();
        
        string utype = User::usertypeToString(userCache_.getUserType());
        
        cout << "\nWelcome " << uid << "!\nYour role: " << utype << "\n"<< date << "\n\n";
        cout <<"Delegate db id: " << userCache_.del_uid << endl;
    }
}



void BankClient::clearScreen()
{
    if (system("clear"))
    {
        system("CLS");
    }
}


