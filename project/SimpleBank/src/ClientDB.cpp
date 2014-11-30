
#include "ClientDB.h"

const string SB::ClientDB::DB_FILE{"clientinfo.data"};
const string SB::ClientDB::DB_DELIM{","};

SB::ClientDB::ClientDB()
{
    this->userTableVec_ = new vector<DataEntry>();
}

string SB::ClientDB::getUserPasswordHash(const string & uid)
{
    DataEntry de;
    
    if(find(uid, de))
    {
        return de.pass;
    }
    return "";
}

void ClientDB::addUser(SB::User u)
{
    addUser(u.getID(), u.getPass(), u.getUserType());
}

void ClientDB::addUser(const string uid, const string pass, Client::UserType t)
{
    if (!userExists(uid)) {
        DataEntry de {uid, Utils::HashPassword(pass),(int)t,-1,-1};
        this->userTableVec_->push_back(de);
    }
    else
    {
        std::cout<< uid <<" already exists\n";
    }
}


void ClientDB::removeUser(const string& id)
{
    if (userExists(id)) {
        this->userTableVec_->erase(find(id));
    }
}

bool ClientDB::userExists(const string & uid)
{
    DataEntry dummy;
    return find(uid, dummy);
}

void ClientDB::saveToFile()
{
    string info = exportClientInfo();
    dbfileOut_.open(DB_FILE, ofstream::trunc);
    dbfileOut_ << info;
    dbfileOut_.close();
}

string ClientDB:: exportClientInfo()
{
    ostringstream s;
    auto it = this->userTableVec_->begin();
    while(it != this->userTableVec_->end())
    {
        
        DataEntry de = *it++;
        s << de.uid << DB_DELIM << de.pass << DB_DELIM << de.utype <<DB_DELIM << de.savingsBalance << DB_DELIM << de.checkingBalance<< "\n";
    }
    return s.str();
}

string ClientDB::formatedExport()
{
    ostringstream s;
    auto it = this->userTableVec_->begin();
    while(it != this->userTableVec_->end())
    {
        DataEntry de = *it;
        s << "User ID:\t" <<de.uid << "\n";
        s << "User Type:\t"  << SB::User::usertypeToString((SB::User::UserType)de.utype) << "\n";
        
        if (de.utype == Client::UserType::CLIENT) {
            if (de.savingsBalance < 0) {
                s << "Savings:\t" << "NOT OPENED\n";
            }
            else
            {
                s << "Savings:\t" << de.savingsBalance << "\n";
            }
            
            if(de.checkingBalance < 0) {
                s << "Checking:\t" << "NOT OPENED\n\n";
            }
            else
            {
                s << "Checking:\t" << de.checkingBalance << "\n\n";
            }
        }
        else
        {
            s << "\n";
        }

        it++;
    }
    
    return s.str();
}

void ClientDB::loadFromFile()
{
    double dummy;
    loadFromFile(dummy);
}

void ClientDB::loadFromFile(double & totalCash)
{
    totalCash = 0;
    vector<DataEntry> newClientVec;
    vector<string> line_ary;
    this->dbfileIn_.open(DB_FILE, ifstream::in);
    while (dbfileIn_.good())
    {
        string s;
        getline(dbfileIn_, s);
        
        if(s.length() > 0)
        {
            line_ary = Utils::ExplodeString(s, DB_DELIM);
            double sd = stod(line_ary[3]);
            double cd = stod(line_ary[4]);
            
            double savingVal = sd > 0 ? sd : 0;
            double checkVal = cd > 0 ? cd : 0;
            totalCash += savingVal;
            totalCash += checkVal;
            
            newClientVec.push_back((DataEntry){
                line_ary[0],
                line_ary[1],
                atoi(line_ary[2].c_str()),
                sd, cd
            });
        }
    }
    
    *this->userTableVec_ = newClientVec;
    this->dbfileIn_.close();
}

bool ClientDB::getUser(string uid, User & u)
{
    DataEntry de;

    bool found = find(uid, de);
    if (found) {
        u = ClientDB::makeUser(uid, (Client::UserType)de.utype);
    }
    return found;
}

bool ClientDB::getClient(string uid, SB::Client & c)
{

    DataEntry de;
    bool found = find(uid, de);
    if (found) {
        c = Client(uid,de.pass,de.savingsBalance,de.checkingBalance);
        
    }
    return found;
}

DataEntry ClientDB::getUserEntry(const string & uid)
{
    return *find(uid);
}

void ClientDB::print()
{
    cout << exportClientInfo();
}

User ClientDB::makeUser(const string & uid, const Client::UserType & t)
{
    User u;
    switch (t)
    {
        case Client::UserType::MGR:
            u = Manager{uid};
            break;
        case Client::UserType::MNT:
            u = Maintenance{uid};
            break;
        case Client::UserType::CLIENT:
        default:
            u = Client{uid};
            break;
    }
    return u;
}

void ClientDB::updateUser(const string & oldUser,SB::User newUser)
{
    removeUser(oldUser);
    updateEntry(newUser.getID(), newUser.getPass(), newUser.getUserType(), -1, -1);
}

void ClientDB::updateClient(const string & oldClient, SB::Client newClient)
{
    removeUser(oldClient);
    updateEntry(newClient.getID(), newClient.getPass(),SB::User::UserType::CLIENT, newClient.getSavingsBalance(), newClient.getCheckingBalance());
}

void ClientDB::updateEntry(const string & uid, const string & pass, User::UserType utype, const double & savings, const double & checking)
{
    DataEntry de {uid, pass, (int)utype, savings, checking};
    userTableVec_->push_back(de);
}

vector<DataEntry>::iterator ClientDB::find(const string & id)
{
    vector<DataEntry>::iterator itb = this->userTableVec_->begin();
    while (itb->uid.compare(id)!=0)
    {
        itb++;
    }
    return itb;
}

bool ClientDB::find(const string & id, DataEntry & ret)
{
    bool found = false;
    vector<DataEntry>::iterator itb = this->userTableVec_->begin();
    vector<DataEntry>::iterator ite = this->userTableVec_->end();
    
    while (itb != ite) {
        if (itb->uid.compare(id)==0) {
            found = true;
            ret = *itb;
        }
        
        if (found) {
            break;
        }
        
        itb++;
    }
    
    return found;
}

ClientDB::~ClientDB()
{
    delete this->userTableVec_;
}