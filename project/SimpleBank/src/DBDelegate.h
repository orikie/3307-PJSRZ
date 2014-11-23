

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
    static const string DB_NAME;
    static const string DROP_ALL;
    static const string CREATE_USER_TABLE;
    static const string CREATE_ACCOUNT_TABLE;
    static const string CREATE_TRANSACTIONS_TABLE;
    static const string INSERT_NEW_USER;
    static const string INSERT_NEW_ACCOUNT;
    static const string INSERT_NEW_TRANSACTION;
    
    DBDelegate();
    DBDelegate(string dbname);
    ~DBDelegate();
    
    void RunQuery(string q);
    void RunQuery(string q, sqlite3_callback cb, void * cb_arg);
    bool RunQuery(sqlite3_stmt * stm);
    void log(string s){log_.logTrace(s);}
    
    
    
    void InitTables();
    void ResetTables();
    
    static string BuildNewUserQuery(string uid, string password_real, SB::User::UserType uType);
    //void GetUserIdentity(string uname, UserIdentity ** uid);
    void GetUID(string uname, int ** uid);
    int GetUID(string uname);
    string GetPasswordHash(string uname);
    bool IsUserCreditValid(string uname);
    bool OpenAccount(int del_id, int type);
    bool NewUser(string uid, string password_real, SB::User::UserType utype);
    void UpdateAccountBalance(int uid, int atype, double newBalance);
    bool NewTransaction(int customer_id, string desc, double amt, string date);
    double GetAccountBalance(int uid, int type);
    
    string QueryTextFieldSingle(string q);
    int QueryIntFieldSingle(string q);

    
private:
    sqlite3 * dbconn_;
    bool connected_;
    Logger log_;
    bool tablesInitialized_;
    bool sqliteopen(string name);
    static int createTables_cb(void * arg, int argc, char**argv, char **azColName);
    //static int getPassword_cb(void * arg, int argc, char**argv, char **azColName);
    static int getuid_cb(void * arg, int argc, char**argv, char **azColName);
    
};

#endif /* defined(__SimpleBank__DBDelegate__) */
