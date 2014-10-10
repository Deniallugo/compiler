//
//  MyException.cpp
//  compiller
//
//  Created by Данил on 09.10.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#include "MyException.h"



void MyException::Print(ofstream *f) const{
    if (line != -1)
        *f <<"ERROR:"<< massage << "\t line " << line << ", column " << col << endl;
    else
        *f <<"ERROR:" << massage << endl;
}

void MyException::Print() const{
    if (line != -1)
        cout <<"ERROR:" << massage << "\t line " << line << ", column " << col << endl;
    else
        cout <<"ERROR:"<< massage << endl;
}
