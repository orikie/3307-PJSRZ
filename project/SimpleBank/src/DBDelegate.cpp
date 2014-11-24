

#include "DBDelegate.h"

const std::string DBDelegate::DB_NAME{"bank_del.db"};
//  USER TABLE
//  |  ID  |  USERID  |  PASSWORD_HASH  |  USER_TYPE  |
const string DBDelegate::CREATE_USER_TABLE =
"CREATE TABLE users(uid INTEGER PRIMARY KEY, username TEXT UNIQUE, password_hash TEXT, user_type INTEGER, credit_limit TEXT, credit_option INTEGER);";
//credit_option{0=pay minimum (10%), 1 = pay in full}

//  ACCOUNTS TABLE
//  |  ID  |  OWNER_ID  |  BALANCE  |  ACTIVATED  |
const string DBDelegate::CREATE_ACCOUNT_TABLE =
"CREATE TABLE accounts(aid INTEGER PRIMARY KEY, owner_id INTEGER, balance TEXT,type INTEGER, activated INTEGER);";
//type { Savings = 0, Checking = 1, Credit = 2}
//Activated{True = 1, False =0}

//  TRANSACTIONS TABLE
//  |  TID  |  CUSTOMER_ID  |  AMOUNT  |  DESCRIPTION  |  DATE  |
const string DBDelegate::CREATE_TRANSACTIONS_TABLE =
"CREATE TABLE transactions (tid INTEGER PRIMARY KEY, customer_id INTEGER, amount TEXT, description TEXT, date TEXT, hidden INTEGER);";
//hidden: 0 = false, 1 = true. Hide after end of month event
//

const string DBDelegate::DROP_ALL =
"DROP TABLE users; DROP TABLE account; DROP TABLE transactions;";

const string DBDelegate::INSERT_NEW_ACCOUNT =
"INSERT INTO accounts (owner_id, balance, type, activated) values(?,?,?,?);";

const string DBDelegate::INSERT_NEW_USER =
"INSERT INTO users (username,password_hash,user_type, credit_limit, credit_option) VALUES(?,?,?,?,?);";

const string DBDelegate::INSERT_NEW_TRANSACTION =
"INSERT INTO transactions (customer_id, amount, description, date, hidden) values(?,?,?,?,?);";


DBDelegate::DBDelegate()
{
    connected_ = sqliteopen(DB_NAME);
}

DBDelegate::~DBDelegate()
{
    if (connected_) {
        sqlite3_close(dbconn_);
    }
}

DBDelegate::DBDelegate(string dbname)
{
    connected_ = sqliteopen(dbname);
}

bool DBDelegate::sqliteopen(string name)
{
    connected_ =  (sqlite3_open(DB_NAME.c_str(), &dbconn_)== SQLITE_OK)? true : false;
    return connected_;
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
    RunQuery(CREATE_TRANSACTIONS_TABLE);
    
    tablesInitialized_  = true;
}

void DBDelegate::ResetTables()
{
    RunQuery(DROP_ALL);
    tablesInitialized_ = false;
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

bool DBDelegate::RunQuery(sqlite3_stmt * stm)
{
    int res = sqlite3_step(stm);
    sqlite3_finalize(stm);
    return res == SQLITE_DONE ? true : false;
    
}


string DBDelegate::BuildNewUserQuery(string uid, string password_real, SB::User::UserType uType)
{
    ostringstream os;
    os << "INSERT INTO users (username,password_hash,user_type,credit_limit) VALUES(\"" << uid << "\",\"";
    os << Utils::HashPassword(password_real)<< "\"," <<(int)uType << ",\"2000\");";
    
    return os.str();
}

bool DBDelegate::NewUser(string uid, string password_real, SB::User::UserType utype)
{
    int res = -1;
    bool suc = false;
    if (connected_) {
        sqlite3_stmt * stm;
        
        res = sqlite3_prepare_v2(dbconn_, INSERT_NEW_USER.c_str(),(unsigned int)(INSERT_NEW_USER.length() + 1), &stm, NULL);
        if (SQLITE_OK == res) {
            
            
            int r1 = sqlite3_bind_text(stm, 1, uid.c_str(), -1, SQLITE_STATIC);
            int r2 = sqlite3_bind_text(stm, 2, Utils::HashPassword(password_real).c_str(), -1, SQLITE_STATIC);
            int r3 = sqlite3_bind_int(stm, 3, (int)utype);
            int r4 = sqlite3_bind_int(stm, 4, DEFAULT_CREDIT_LIMIT);
            int r5 = sqlite3_bind_int(stm, 5, PAY_MIN_10);
            if ((r1!=SQLITE_OK)||(r2!=SQLITE_OK)||(r3!=SQLITE_OK)||(r4!=SQLITE_OK)||(r5!=SQLITE_OK)) {
                log("Error inserting transaction record");
            }
            else
            {
                suc = RunQuery(stm);
            }
        }
    }
    return suc;
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
    string q = "select DISTINCT uid from users where username = " + uname + ";";
    *uid = new int(-1);
    int * temp_p = *uid;
    RunQuery(q, getuid_cb, uid);
    //If results return cb would modify uid
    //If found, release default -1
    if (**uid != -1) {
        delete temp_p;
    }
}

double DBDelegate::GetAccountBalance(int uid, int type)
{
    string q = "select balance from accounts where owner_id = " + to_string(uid) + " and type = " + to_string(type) + ";";
    return stod(QueryTextFieldSingle(q));
}

bool DBDelegate::OpenAccount(int del_id, int type)
{
    //string q = "insert into accounts (owner_id,balance,type,activated) values(" + to_string(del_id) + ",\"0\","+to_string(type)+",1);";
    //RunQuery(q);
    
    int res = -1;
    bool suc = false;
    if (connected_) {
        sqlite3_stmt * stm;
        
        res = sqlite3_prepare_v2(dbconn_, INSERT_NEW_ACCOUNT.c_str(),(unsigned int)(INSERT_NEW_ACCOUNT.length() + 1), &stm, NULL);
        if (SQLITE_OK == res) {
            
            int r1 = sqlite3_bind_int(stm, 1, del_id);//user id
            int r2 = sqlite3_bind_text(stm, 2, "0", -1, SQLITE_STATIC);//initial balance
            int r3 = sqlite3_bind_int(stm, 3, (int)type); //Credit type
            int r4 = sqlite3_bind_int(stm, 4, 1); //initially activated
            
            if ((r1!=SQLITE_OK)||(r2!=SQLITE_OK)||(r3!=SQLITE_OK)||(r4!=SQLITE_OK)) {
                log("Error inserting transaction record");
            }
            else
            {
                suc = RunQuery(stm);
            }
        }
    }
    return suc;
}

void DBDelegate::UpdateAccountBalance(int uid, int atype, double newBalance)
{
    string q = "update accounts set balance = "+ to_string(newBalance) + " where owner_id = " +to_string(uid) + " and type = " + to_string(atype) + ";";
    //cout << q;
    RunQuery(q);
}

string DBDelegate::GetPasswordHash(string uname)
{
    string q = "select password_hash from users where username = \"" + uname + "\";";
    return QueryTextFieldSingle(q);
}

int DBDelegate::GetUID(string uname)
{
    string q = "select DISTINCT uid from users where username = \"" + uname + "\";";
    return QueryIntFieldSingle(q);
}

string DBDelegate::QueryTextFieldSingle(string q)
{
    string rets = "";
    sqlite3_stmt * stm;
    int ret = SQLITE_OK;
    if (connected_) {
        ret = sqlite3_prepare_v2(dbconn_, q.c_str(), (unsigned int)(q.size() + 1), &stm, NULL);
        if (ret == SQLITE_OK) {
            ret = sqlite3_step(stm);
            if (ret == SQLITE_ROW) {
                const unsigned char * p = sqlite3_column_text(stm, 0);
                rets = string((char *)p);
            }
        }
    }
    sqlite3_finalize(stm);
    return rets;
}

int DBDelegate::QueryIntFieldSingle(string q)
{
    int rets = -1;
    sqlite3_stmt * stm;
    int ret = SQLITE_OK;
    if (connected_) {
        ret = sqlite3_prepare_v2(dbconn_, q.c_str(), (unsigned int)(q.size() + 1), &stm, NULL);
        if (ret == SQLITE_OK) {
            ret = sqlite3_step(stm);
            if (ret == SQLITE_ROW) {
                rets = sqlite3_column_int(stm, 0);
            }
        }
    }
    
    sqlite3_finalize(stm);
    return rets;
}

bool DBDelegate::IsUserCreditValid(string uname)
{
    string q = "select activated from accounts inner join users on users.uid = accounts.owner_id and type = 2 and username  = \"" + uname + "\";";
    return QueryIntFieldSingle(q) == 1 ? true: false;
}

bool DBDelegate::NewTransaction(int customer_id, string desc, double amt, string date)
{
    int res = -1;
    bool suc = false;
    if (connected_) {
        sqlite3_stmt * stm;
        
        res = sqlite3_prepare_v2(dbconn_, INSERT_NEW_TRANSACTION.c_str(),(unsigned int)(INSERT_NEW_TRANSACTION.length() + 1), &stm, NULL);
        if (SQLITE_OK == res) {
            
            int r1 = sqlite3_bind_int(stm, 1, customer_id);
            int r2 = sqlite3_bind_text(stm, 2, to_string(amt).c_str(), -1, SQLITE_STATIC);
            int r3 = sqlite3_bind_text(stm, 3, desc.c_str(), -1, SQLITE_STATIC);
            int r4 = sqlite3_bind_text(stm, 4, date.c_str(), -1, SQLITE_STATIC);
            int r5 = sqlite3_bind_int(stm, 5, HIDE_TRANSACTIONS_OFF);
            
            if ((r1!=SQLITE_OK)||(r2!=SQLITE_OK)||(r3!=SQLITE_OK)||(r4!=SQLITE_OK)||(r5!=SQLITE_OK)) {
                log("Error inserting transaction record");
            }
            else
            {
                suc = RunQuery(stm);
            }
        }
    }
    return suc;
}

vector<db_transaction_record> DBDelegate::GetTransactionRecords(int customer_id)
{
    string q = "select * from transactions where customer_id = " + to_string(customer_id) + " and hidden = 0;";
    vector<db_transaction_record> db_recs;
    int res = -1;
    
    if (connected_) {
        sqlite3_stmt * stm;
        
        res = sqlite3_prepare_v2(dbconn_, q.c_str(),(unsigned int)(q.length() + 1), &stm, NULL);
        if (SQLITE_OK == res) {
            res = sqlite3_step(stm);
            while (res == SQLITE_ROW) {
                int tid = sqlite3_column_int(stm, 0);
                int customer_id = sqlite3_column_int(stm, 1);
                const unsigned char * amount = sqlite3_column_text(stm, 2);
                const unsigned char * desc = sqlite3_column_text(stm, 3);
                const unsigned char * date = sqlite3_column_text(stm, 4);
                
                db_transaction_record record = {
                    tid,
                    customer_id,
                    stod((char *)amount),
                    string((char*)desc),
                    string((char*)date)
                };
        
                db_recs.push_back(record);
                res = sqlite3_step(stm);
            }
        }
        sqlite3_finalize(stm);
    }
    return db_recs;
}

//int DBDelegate::getPassword_cb(void *arg, int argc, char **argv, char **azColName)
//{
//    if (argc > 0) {
//        string ** s = (string **)arg;
//        *s = new string(argv[0]);
//    }
//    
//    return 0;
//
//}









