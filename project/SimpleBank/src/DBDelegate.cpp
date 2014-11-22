

#include "DBDelegate.h"

const std::string DBDelegate::DB_NAME{"bank_del.db"};
//  USER TABLE
//  |  ID  |  USERID  |  PASSWORD_HASH  |  USER_TYPE  |
const string DBDelegate::CREATE_USER_TABLE =
"CREATE TABLE users(uid INTEGER PRIMARY KEY, username TEXT UNIQUE, password_hash TEXT, user_type INTEGER);";

//  ACCOUNTS TABLE
//  |  ID  |  OWNER_ID  |  BALANCE  |  ACTIVATED  |
const string DBDelegate::CREATE_ACCOUNT_TABLE =
"CREATE TABLE accounts(aid INTEGER PRIMARY KEY, owner_id INTEGER, balance TEXT,type INTEGER, activated INTEGER);";
//type { Savings = 0, Checking = 1, Credit = 2}
//Activated{True = 1, False =0}

const string DBDelegate::DROP_ALL =
"DROP TABLE users; DROP TABLE account;";

const string DBDelegate::CREATE_NEW_ACCOUNT =
"INSERT INTO account (owner_id, balance, activated) values(%d,%d,%d)";


//not used atm
const string DBDelegate::CREATE_NEW_USER =
"INSERT INTO users (username,password_hash,user_type) VALUES(\"%\", \"%s\", %d);";

DBDelegate::DBDelegate()
{
    connected_ =  (sqlite3_open(DB_NAME.c_str(), &dbconn_)== SQLITE_OK)? true : false;
}

DBDelegate::~DBDelegate()
{
    if (connected_) {
        sqlite3_close(dbconn_);
    }
}


int DBDelegate::createTables_cb(void *arg, int argc, char **argv, char **azColName)
{
    if (argc >= 1) {
        //RunQuery(CREATE_USER_TABLE);
        //RunQuery(CREATE_ACCOUNT_TABLE);
    }
    
    return 0;
}

void DBDelegate::InitTables()
{
 
    //string q = "SELECT name FROM sqlite_master WHERE type='table' AND name='users';";
    //RunQuery(q, createTables_cb, NULL);
    RunQuery(CREATE_USER_TABLE);
    RunQuery(CREATE_ACCOUNT_TABLE);
    
}

void DBDelegate::ResetTables()
{
    RunQuery(DROP_ALL);
}

void DBDelegate::RunQuery(string q)
{
    if (connected_) {
        char * errMsg = 0;
        sqlite3_exec(dbconn_, q.c_str(), NULL, NULL, &errMsg);
        //cout << errMsg;
        sqlite3_free(errMsg);
    }
}

void DBDelegate::RunQuery(string q, sqlite3_callback cb, void * cb_arg)
{
    if (connected_) {
        char * errMsg = 0;
        sqlite3_exec(dbconn_, q.c_str(), cb, cb_arg, &errMsg);
        //cout << errMsg;
        sqlite3_free(errMsg);
    }
}

void DBDelegate::createDefaultUsers()
{

}

string DBDelegate::BuildNewUserQuery(string uid, string password_real, SB::User::UserType uType)
{
    ostringstream os;
    os << "INSERT INTO users (username,password_hash,user_type) VALUES(\"" << uid << "\",\"";
    os << Utils::HashPassword(password_real)<< "\"," <<(int)uType << ");";
    
    return os.str();

}

void DBDelegate::NewUser(string uid, string password_real, SB::User::UserType utype)
{
    string q = BuildNewUserQuery(uid, password_real, utype);
    //cout << q;
    RunQuery(q);
}

int DBDelegate::userIdentity_cb(void *arg, int argc, char **argv, char **azColName)
{
    UserIdentity ** u = (UserIdentity **)arg;
    *u = new UserIdentity{1,2,3};
    return 0;
}

void DBDelegate::GetUserIdentity(string uname, UserIdentity **uid)
{
    string q = "SELECT id FROM users WHERE ";
    RunQuery(q, userIdentity_cb, uid);
    
}

int DBDelegate::getuid_cb(void *arg, int argc, char **argv, char **azColName)
{
    if (argc >= 1) {
        //cout << argv[0];
        int ** ip = (int **)arg;
        *ip = new int(stoi(argv[0]));
        
    }
    
    return 0;
}

void DBDelegate::GetUID(string uname, int **uid)
{
    string q = "select DISTINCT uid from users where username = \"" + uname + "\";";
    *uid = new int(-1);
    int * temp_p = *uid;
    RunQuery(q, getuid_cb, uid);
    //If results return cb would modify uid
    //If found, release default -1
    if (**uid != -1) {
        delete temp_p;
    }
}

void DBDelegate::OpenAccount(int del_id, int type)
{
    string q = "insert into accounts (owner_id,balance,type,activated) values(" + to_string(del_id) + ",\"0\","+to_string(type)+",1);";
    RunQuery(q);
}

void DBDelegate::UpdateAccountBalance(int uid, int atype, double newBalance)
{
    string q = "update accounts set balance = "+ to_string(newBalance) + " where owner_id = " +to_string(uid) + " and type = " + to_string(atype) + ";";
    //cout << q;
    RunQuery(q);
}
