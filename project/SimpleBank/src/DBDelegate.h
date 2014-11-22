

#ifndef __SimpleBank__DBDelegate__
#define __SimpleBank__DBDelegate__

#include <sstream>
#include <stdio.h>
#include "sqlite3.h"
#include "User.h"
#include "Logger.h"


using namespace std;

struct UserIdentity{
    int uid;
    int savingsID;
    int checkingID;
    
    //string username;
    //double savingBalance;
    //double checkingBalance;
    
};

class DBDelegate
{
public:
    static const std::string DB_NAME;
    static const string CREATE_USER_TABLE;
    static const string DROP_ALL;
    static const string CREATE_ACCOUNT_TABLE;
    static const string CREATE_NEW_USER;
    static const string CREATE_NEW_ACCOUNT;
    static const string CREATE_TRANSACTIONS_TABLE;
    
    DBDelegate();
    ~DBDelegate();
    
    void RunQuery(string q);
    void RunQuery(string q, sqlite3_callback cb, void * cb_arg);
    void log(string s){log_.logTrace(s);}
    
    
    void NewUser(string uid, string password_real, SB::User::UserType utype);
    void InitTables();
    void ResetTables();
    
    //temp public
    static string BuildNewUserQuery(string uid, string password_real, SB::User::UserType uType);
    void GetUserIdentity(string uname, UserIdentity ** uid);
    void GetUID(string uname, int ** uid);
    void OpenAccount(int del_id, int type);
    void UpdateAccountBalance(int uid, int atype, double newBalance);
    
private:
    sqlite3 * dbconn_;
    bool connected_;
    Logger log_;
    
    static int createTables_cb(void * arg, int argc, char**argv, char **azColName);
    static int userIdentity_cb(void * arg, int argc, char**argv, char **azColName);
    
    void createDefaultUsers();
    static int getuid_cb(void * arg, int argc, char**argv, char **azColName);
    
};

#endif /* defined(__SimpleBank__DBDelegate__) */
