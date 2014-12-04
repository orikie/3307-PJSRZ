
//
//  DBDelegate.h
//
//  This delegates database requests to sqlite3 database
//  This retires the ClientDB implemented in assignment 1
//  The vendor system can share the same db using this interface
//
//  Author: Zhao Lin
//  Created: 2014-11-22
//

#ifndef __SimpleBank__DBDelegate__
#define __SimpleBank__DBDelegate__

#include <sstream>
#include <stdio.h>
#include <vector>

#include "sqlite3.h"
#include "User.h"
#include "Logger.h"


using namespace std;

//  USER TABLE
//  |  UID  |  USERNAME  |  PASSWORD_HASH  |  USER_TYPE  | CREDIT_LIMIT | CREDIT_OPTION

//  TRANSACTIONS TABLE
//  |  TID  |  CUSTOMER_ID  |  AMOUNT  |  DESCRIPTION  |  DATE  |

//  ACCOUNTS TABLE
//  |  ID  |  OWNER_ID  |  BALANCE  |  ACTIVATED  |

//  User table row info
struct db_user_record
{
    int u_id;
    std::string username;
    std::string password_hash;
    int user_type;
    double creditLimit;
};

//  User table row info small version
struct db_user_record_s
{
    int u_id;
    string username;
};

//  Account table info
struct db_account_record
{
    int a_id;
    int owner_id;
    double balance;
    int type;
    bool activated;
};

//  Transaction table info
struct db_transaction_record
{
    int t_id;
    int customer_id;
    double amount;
    std::string description;
    string date;
};

//  Account + User join view 1
struct db_account_pkg1
{
    int aid;
    int type;
    double balance;
    int credit_option;
};

//  Account + User join view 2
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
    static const string DROP_ALL;                   //  Query: Drop all tables, reset
    static const string CREATE_USER_TABLE;          //  Query: Create users table
    static const string CREATE_ACCOUNT_TABLE;       //  Query: Create accounts table
    static const string CREATE_TRANSACTIONS_TABLE;  //  Query: Create transactions table
    static const string INSERT_NEW_USER;            //  Query: Add new user
    static const string INSERT_NEW_ACCOUNT;         //  Query: Add new account
    static const string INSERT_NEW_TRANSACTION;     //  Query: Add new transaction
    
    DBDelegate();
    DBDelegate(string dbname);
    ~DBDelegate();
    
    //  Run query interfaces
    void RunQuery(string q);
    void RunQuery(string q, sqlite3_callback cb, void * cb_arg);
    bool RunQuery(sqlite3_stmt * stm);
    
    //  Return single field select queries
    string QueryTextFieldSingle(string q);
    int QueryIntFieldSingle(string q);
    
    //  Log to default file log
    void log(string s){log_.logTrace(s);}
    
    //  Initialize tables if not already
    void InitTables();
    
    //  Drop all tables and existing data
    void ResetTables();
    
    //  deprecated: builds new user query. Use INSERT_NEW_USER and prepare instead
    static string BuildNewUserQuery(string uid, string password_real, SB::User::UserType uType);

    //  Get user id in users table
    void GetUID(string uname, int ** uid);
    int GetUID(string uname);
    
    //  Get current password hash
    string GetPasswordHash(string uname);
    
    //  Check if user's credit card is frozen, i.e. valid
    bool IsUserCreditValid(string uname);
    
    //  Open new bank account
    bool OpenAccount(int del_id, int type);
    
    //  New bank/vendor user
    bool NewUser(string uid, string password_real, SB::User::UserType utype);
    
    //  Update bank account balance
    void UpdateAccountBalanceDel(int uid, int atype, double newBalance);
    
    //  New transaction from vendor
    bool NewTransaction(int customer_id, string desc, double amt, string date);
    
    //  Audit full list of trnasaction records that are not hidden
    vector<db_transaction_record> GetTransactionRecords(int customer_id);
    
    //  Return current account balance
    double GetAccountBalance(int uid, int type);
    
    //  List of Enabled credit customers
    vector<db_user_record_s> GetEnabledCreditCustomers();
    
    //  Get all accounts info for a customer
    vector<db_account_pkg1> GetAccountsInfoForUser(int uid);
    
    //  Get Checking records for a customer
    db_account_record GetCheckingRecordForUser(int uid);
    
    //  Get Credit records for a customer
    db_credit_record GetCreditRecordForUser(int uid);
    
    //  Freeze an user's credit card
    void FreezeCreditForUser(int uid);
    
    //  Activate a bank account
    void SetAccountActivated(int uid, bool act);
    
    //  Change user password
    void ChangePassword(string uid, string pass_real);
    
    //  Get Credit option for user
    int GetCreditOption(string uid);
    
    //  Change Credit option for user
    void ChangeCreditOption(string uid, int val);
    
private:
    //  sqlite3 connection
    sqlite3 * dbconn_;
    bool connected_;
    Logger log_;
    bool tablesInitialized_;
    bool sqliteopen(string name);
    static int createTables_cb(void * arg, int argc, char**argv, char **azColName);
    static int getuid_cb(void * arg, int argc, char**argv, char **azColName);
    
    
};

#endif /* defined(__SimpleBank__DBDelegate__) */
