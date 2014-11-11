
#ifndef __ProxyFile__ItemProxy__
#define __ProxyFile__ItemProxy__

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

//Top level item proxy
class ItemProxy
{
public:
    ItemProxy(){id_=1;};
    ItemProxy(int i, string desc){id_ = i; typeDesc_ = desc;};
    virtual ~ItemProxy(){};
    
    //Request original archived file
    virtual void openOriginal(){cout << "ItemProxy openOriginal\n";};
    
    int getID(){return id_;};
    string getInfo(){return string("id: "+to_string(id_)+" "+typeDesc_);}
    
protected:
    void log(string l) {cout << getID() << ":\t\t" <<l<< "\n";}
    int id_;
    string typeDesc_;
};

/*
 *  System File
 */
class SystemFile
{
public:
    SystemFile(int name){name_ = name;}
    void openOriginal()
    {
        cout << "Opening original System File: "<<name_<<"\n";
    }
private:
    int name_;
};

class FileProxy: public ItemProxy
{
public:
    FileProxy(int i, string desc): ItemProxy(i, desc){}
    void openOriginal(){
        log("Requesting original system file from proxy...");
        SystemFile(id_).openOriginal();
    };
};

/*
 *  Image files
 */
class ImageFile
{
public:
    ImageFile(int name){name_ = name;}
    void openOriginal()
    {
        cout << "Opening original Image File: "<< name_ <<"\n";
    }
private:
    int name_;
    
};

class ImageProxy: public ItemProxy
{
public:
    ImageProxy(int i, string desc): ItemProxy(i, desc){}
    void openOriginal()
    {
        log("Requesting original image file from proxy...");
        ImageFile(id_).openOriginal();
        
    };
};

/*
 *  Enterprise email
 */

class EnterpriseMail
{
public:
    EnterpriseMail(int name){name_ = name;}
    void openOriginal()
    {
        cout << "Opening original mail: "<< name_<<"\n";
    }
private:
    int name_;
};

class EmailProxy: public ItemProxy
{
public:
    EmailProxy(int i, string desc): ItemProxy(i, desc){}
    void openOriginal()
    {
        log("Requesting original enterprise mail from proxy...");
        EnterpriseMail(id_).openOriginal();
    
    };
};



#endif /* defined(__ProxyFile__ItemProxy__) */
