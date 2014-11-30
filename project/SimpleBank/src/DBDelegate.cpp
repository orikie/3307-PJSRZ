
#include "DBDelegate.h"

const std::string DBDelegate::DB_NAME{"bank_del.db"};

const string DBDelegate::CREATE_USER_TABLE =
"CREATE TABLE users(uid INTEGER PRIMARY KEY, username TEXT UNIQUE, password_hash TEXT, user_type INTEGER, credit_limit TEXT, credit_option INTEGER);";

const string DBDelegate::CREATE_ACCOUNT_TABLE =
"CREATE TABLE accounts(aid INTEGER PRIMARY KEY, owner_id INTEGER, balance TEXT,type INTEGER, activated INTEGER);";

const string DBDelegate::CREATE_TRANSACTIONS_TABLE =
"CREATE TABLE transactions (tid INTEGER PRIMARY KEY, customer_id INTEGER, amount TEXT, description TEXT, date TEXT, hidden INTEGER);";

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

void DBDelegate::InitTables()
{
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
            int r5 = sqlite3_bind_int(stm, 5, PAY_FULL);
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
    int res = -1;
    bool suc = false;
    if (connected_) {
        sqlite3_stmt * stm;
        
        res = sqlite3_prepare_v2(dbconn_, INSERT_NEW_ACCOUNT.c_str(),(unsigned int)(INSERT_NEW_ACCOUNT.length() + 1), &stm, NULL);
        if (SQLITE_OK == res) {
            
            int r1 = sqlite3_bind_int(stm, 1, del_id);                  //user id
            int r2 = sqlite3_bind_text(stm, 2, "0", -1, SQLITE_STATIC); //initial balance
            int r3 = sqlite3_bind_int(stm, 3, (int)type);               //Credit type
            int r4 = sqlite3_bind_int(stm, 4, YES);                       //initially activated
            
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

void DBDelegate::UpdateAccountBalanceDel(int uid, int atype, double newBalance)
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

vector<db_user_record_s> DBDelegate::GetEnabledCreditCustomers()
{
    string q = "select uid, username from users inner join accounts on accounts.owner_id = users.uid and type = 2 and activated = 1;";
    vector<db_user_record_s> uid_v;
    int res = -1;
    if (connected_) {
        sqlite3_stmt * stm;
        
        res = sqlite3_prepare_v2(dbconn_, q.c_str(), (unsigned int)(q.length() + 1), &stm, NULL);
        if (SQLITE_OK == res) {
            res = sqlite3_step(stm);
            while (res== SQLITE_ROW) {
                int id = sqlite3_column_int(stm, 0);
                const unsigned char * username = sqlite3_column_text(stm, 1);
                
                db_user_record_s rs = {
                    id,
                    string((char*)username)
                };
                
                uid_v.push_back(rs);
                res = sqlite3_step(stm);
            }
        }
        sqlite3_finalize(stm);
    }
    return uid_v;
}

vector<db_account_pkg1> DBDelegate::GetAccountsInfoForUser(int uid)
{
    string q = "select aid,type, balance, credit_option from accounts inner join users on users.uid = accounts.owner_id where owner_id = " + to_string(uid) + ";";
    
    vector<db_account_pkg1> db_recs;
    int res = -1;
    
    if (connected_) {
        sqlite3_stmt * stm;
        
        res = sqlite3_prepare_v2(dbconn_, q.c_str(),(unsigned int)(q.length() + 1), &stm, NULL);
        if (SQLITE_OK == res) {
            res = sqlite3_step(stm);
            while (res == SQLITE_ROW) {
                
                int aid = sqlite3_column_int(stm, 0);
                int type = sqlite3_column_int(stm, 1);
                const unsigned char * balance = sqlite3_column_text(stm, 2);
                int credit_option = sqlite3_column_int(stm, 3);

                db_account_pkg1 record = {
                    aid,
                    type,
                    stod((char *)balance),
                    credit_option
                };
                
                db_recs.push_back(record);
                res = sqlite3_step(stm);
            }
        }
        sqlite3_finalize(stm);
    }
    return db_recs;
}

db_account_record DBDelegate::GetCheckingRecordForUser(int uid)
{
    string q = "select * from accounts where type = 1 and owner_id = " + to_string(uid) + ";";
    int res = -1;
    db_account_record r;
    if (connected_) {
        sqlite3_stmt * stm;
        
        res = sqlite3_prepare_v2(dbconn_, q.c_str(),(unsigned int)(q.length() + 1), &stm, NULL);
        if (SQLITE_OK == res) {
            res = sqlite3_step(stm);
            if (SQLITE_ROW == res || SQLITE_OK == res) {
                int aid = sqlite3_column_int(stm, 0);
                int owner_id = sqlite3_column_int(stm, 1);
                const unsigned char * balance = sqlite3_column_text(stm, 2);
                int type = sqlite3_column_int(stm, 3);
                int activated = sqlite3_column_int(stm, 4);
                
                r = {
                    aid,
                    owner_id,
                    stod((char *)balance),
                    type,
                    activated == 1 ? true : false
                };
            }
        }
        sqlite3_finalize(stm);
    }
    return r;
}

db_credit_record DBDelegate::GetCreditRecordForUser(int uid)
{
    string q = "select aid, balance, activated, credit_limit, credit_option from accounts inner join users on users.uid = accounts.owner_id and type = 2 and uid = " +to_string(uid)+";";
    
    int res = -1;
    db_credit_record r;
    if (connected_) {
        sqlite3_stmt * stm;
        
        res = sqlite3_prepare_v2(dbconn_, q.c_str(),(unsigned int)(q.length() + 1), &stm, NULL);
        if (SQLITE_OK == res) {
            res = sqlite3_step(stm);
            if (SQLITE_OK == res || SQLITE_ROW == res) {
                
                int aid = sqlite3_column_int(stm, 0);
                const unsigned char * balance = sqlite3_column_text(stm, 1);
                int activated = sqlite3_column_int(stm, 2);
                const unsigned char * credit_limit = sqlite3_column_text(stm, 3);
                int credit_option = sqlite3_column_int(stm, 4);
                
                r = {
                    aid,
                    stod((char *)balance),
                    activated == 1 ? true : false,
                    stod((char *)credit_limit),
                    credit_option == 1? true : false,
                };
            }
        }
        sqlite3_finalize(stm);
    }
    return r;
}

void DBDelegate::FreezeCreditForUser(int uid)
{
    string q = "update accounts set activated = " + string(NO) + " where type = 2 and owner_id = " + to_string(uid)+ ";";
    RunQuery(q);
}

void DBDelegate::SetAccountActivated(int uid, bool act)
{
    string act_s = act == true ? "1" : "0";
    string q = "update accounts set activated = " + act_s + " where type = 2 and owner_id = " + to_string(uid)+ ";";
    RunQuery(q);
}

void DBDelegate::ChangePassword(string uid, string pass_real)
{
    string q = "update users set password_hash = \"" + Utils::HashPassword(pass_real) + "\" where username = \"" + uid + "\";";
    RunQuery(q);
}


