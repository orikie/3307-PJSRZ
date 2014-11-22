//
//  SimpleCheckOut.h
//  SimpleBank
//
//  Created by Joey Lin on 11/22/14.
//
//

#ifndef __SimpleBank__SimpleCheckOut__
#define __SimpleBank__SimpleCheckOut__

#include <stdio.h>
#include "DBDelegate.h"

class SimpleCheckOut
{
public:
    SimpleCheckOut();
    ~SimpleCheckOut();
    
private:
    DBDelegate db;
    
};

#endif /* defined(__SimpleBank__SimpleCheckOut__) */
