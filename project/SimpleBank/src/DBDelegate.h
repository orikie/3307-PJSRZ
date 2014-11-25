

#ifndef __SimpleBank__DBDelegate__
#define __SimpleBank__DBDelegate__

#include <sstream>
#include <stdio.h>
#include <vector>

#include "sqlite3.h"
#include "User.h"
#include "Logger.h"


using namespace std;

struct db_user_record
{
    int u_id;
    std::string username;
    std::string password_hash;
    int user_type;
    double creditLimit;
};

struct db_user_record_s
{
    int u_id;
    string username;
};

struct db_account_record
{
    int a_id;
    int owner_id;
    double balance;
    int type;
    bool activated;
};

struct db_transaction_record
{
    int t_id;
    int customer_id;
    double amount;
    std::string description;
    string date;
};

struct db_account_pkg1
{
    int aid;
    int type;
    double balance;
    int credit_option;
};

struct db_credit_record
{
    int aid;
    double balance;
    bool activated;
    double credit_limit;
    int credit_option;
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
    string QueryTextFieldSingle(string q);
    int QueryIntFieldSingle(string q);

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
    void UpdateAccountBalanceDel(int uid, int atype, double newBalance);
    bool NewTransaction(int customer_id, string desc, double amt, string date);
    vector<db_transaction_record> GetTransactionRecords(int customer_id);
    double GetAccountBalance(int uid, int type);
    vector<db_user_record_s> GetEnabledCreditCustomers();
    vector<db_account_pkg1> GetAccountsInfoForUser(int uid);
    db_account_record GetCheckingRecordForUser(int uid);
    db_credit_record GetCreditRecordForUser(int uid);
    void FreezeCreditForUser(int uid);
    void SetAccountActivated(int uid, bool act);
    void ChangePassword(string uid, string pass_real);
    
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
