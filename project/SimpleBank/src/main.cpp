 



#include "BankClient.h"
#include "DBDelegate.h"
using namespace std;

int main(int argc, const char * argv[])
{

    
    //DBDelegate d;d.ResetTables();

    BankClient c;c.start();
    //d.NewUser("joe", "you", SB::User::UserType::MNT);
    /*
    DBDelegate d;
    UserIdentity * uid;
    d.GetUserIdentity("zlin45", &uid);
    delete uid;
    cout << uid->uid << uid->checkingID << endl;*/
    
    
    return 0;

}

