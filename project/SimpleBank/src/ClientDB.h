//
//  ClientDB.h
//
//  Simple parser for client data
//  User file storage for data persistence
//  Legacy dabatabase first used in asn 1
//  Methods here are migrated to DBDelegate
//  Author: Zhao Lin
//  Created: 2014-09-16
//

#ifndef __SimpleBank__ClientDB__
#define __SimpleBank__ClientDB__

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdlib.h>

#include "Utils.h"
#include "User.h"

using namespace std;
using namespace SB;

namespace SB {

    //  A record stored in file for each Client user
    //  Reprsents a "row" in the csv file
    struct DataEntry{
        string uid;
        string pass;
        int utype; //enum UserType
        double savingsBalance;
        double checkingBalance;
    };
    
    //  Client DB simulates persistent data storage
    //  for user data info using a file
    class ClientDB
    {
    public:
        //  Constuctor: Initialize empty vector storage for DataEntry
        ClientDB();
        
        //  Destructor: Release DataEntry vector from heap
        ~ClientDB();
        
        //  Default user file name
        const static string DB_FILE;
        
        //  Default delimiter for csv file
        const static string DB_DELIM;
        
        //  Add a new user to memory storage
        void addUser(const string uid, const string pass, Client::UserType);
        void addUser(User u);
        
        //  Update user/ client info
        void updateUser(const string &, User);
        void updateClient(const string&, SB::Client);
        
        //  Remove a user from memory storage
        void removeUser(const string&);
        
        //  Check if a userid exsists
        bool userExists(const string&);
        
        //  Get user's password hash
        string getUserPasswordHash(const string &);
        
        //  Get specified userid; Sets User reference; returns true if found
        bool getUser(string, User &);
        bool getClient(string, Client&);
        DataEntry getUserEntry(const string &);
        
        //Dump current users on file
        void print();
        string formatedExport();
        
        //Save users in memory to file
        void saveToFile();
        
        //Load users from default file
        void loadFromFile();
        void loadFromFile(double &);
        
        //Make a new User type
        static User makeUser(const string &uid, const Client::UserType & t);
        
        //Return true if user is in memory
        bool find(const string &, DataEntry &);
        
        //Return total Users
        unsigned int getTotalUsers() {return (unsigned int)this->userTableVec_->size();}
        
    private:
        void updateEntry(const string &, const string&, User::UserType, const double &, const double &);
        
        //User data storage
        vector<DataEntry> * userTableVec_;
        
        //Internal Find method
        vector<DataEntry>::iterator find(const string&);
        
        //Export userTableVec_ info to string, for writing
        string exportClientInfo();
        
        //IO File Streams
        ofstream dbfileOut_;
        ifstream dbfileIn_;
    };
}



#endif /* defined(__SimpleBank__ClientDB__) */
