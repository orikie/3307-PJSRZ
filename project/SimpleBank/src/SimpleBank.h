//
//  SimpleBank.h
//
//  Created: 2014-09-18
//

#ifndef __SimpleBank__SimpleBank__
#define __SimpleBank__SimpleBank__

#include "ClientDB.h"
#include "User.h"
#include "Logger.h"

using namespace std;

namespace SB {
    struct SBException : public exception
    {
        string s;
        SBException(string msg):s(msg){}
        const char * what(){return s.c_str();}
    };
    
    class SimpleBank
    {
    public:
        SimpleBank();
        ~SimpleBank();
        
        
        bool logon(string, string);
        void logout();
        
        void newUser(string, Client::UserType);
        void deleteUser(const string &);
        bool userExist(const string & uid) {return clientdb_.userExists(uid);}
        
        void updateClient(const string &, Client);
        
        User getUser(const string &);
        Client getClient(const string &);
        void openSavings(Client&);
        void openChecking(Client &);
        
        void closeSavings(Client &);
        void closeChecking(Client &);
        
        bool isLoggedOn();
        User getLoggedUser()    {return this->loggedOnUser_;}
        
        void print();
        string exportInfo()     {return this->clientdb_.formatedExport();}
        DataEntry exportUser(const string& uid) {return clientdb_.getUserEntry(uid);}
        
        void save()  {clientdb_.saveToFile();}
        
        double getCashReserve() {return cashReserve_;}
        unsigned int getTotalUsers() {return clientdb_.getTotalUsers();}
        
        void changePassword(const string &);
        
        //bool isLoggingOn() {return logger_.IsON;}
        //void setLoggingOn(bool s){logger_.setON(s);}
        
        
    private:
        User loggedOnUser_;
        ClientDB clientdb_;
        bool loggedOn_;
        double cashReserve_;
        //Logger logger_;
        
        //void log(const string& m) {logger_.logTrace(m);}
        //void logError(const string& m) {logger_.logErr(m);}
        
    };

}
#endif /* defined(__SimpleBank__SimpleBank__) */
