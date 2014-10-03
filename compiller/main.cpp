//
//  main.cpp
//  compiller
//
//  Created by Данил on 17.09.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#include "scanner.h"

static string get_out_addr(const string& in){
    string out = in;
    out.erase(out.find('.') + 1);
    out += "out";
    return out;
}

int main(int argc, char *argv[]){
    ofstream *output = 0;
    createKeyWords();
    createOperations();
  //  argv[1] = "-s";
    //  argv[2] = "30.in";

    try{

        if (!(strcmp(argv[1], "-help"))){
            cout <<"-help" << "-f - write to the file + output to the screen (lexer)" << endl <<
            "-s - output to the screen (lexel)" << endl <<
            "-t - write to the file (lexer)" << endl;

        }
        if (!(strcmp(argv[1], "-t"))) {
            try{
                ifstream *f = new ifstream;
                f->open(argv[2], ios::in);
                ofstream *g = new ofstream;
                output = g;
                g->open(get_out_addr(argv[2]), ios::out);
                Scanner a(argv[2]);

                while (!a.isEnd()){
                    a.Next();
                    a.Get()->Print(g);
                    a.Get()->Print();
                }
                g->close();
                delete g;
                f->close();
                delete f;

            }
            catch (MyException &e){
                e.Print();
                e.Print(output);
                output->close();
            }

        }
        if (!(strcmp(argv[1], "-f"))) {
            try{
                ifstream *f = new ifstream;
                f->open(argv[2], ios::in);
                ofstream *g = new ofstream;
                output = g;
                g->open(get_out_addr(argv[2]), ios::out);
                Scanner a(argv[2]);

                while (!a.isEnd()){
                    a.Next();
                    a.Get()->Print(g);
                    a.Get()->Print();
                }
                g->close();
                delete g;
                f->close();
                delete f;

            }
            catch (MyException &e){
                e.Print();
                e.Print(output);
                output->close();
            }
            
        }
        else
            if (!(strcmp(argv[1], "-s"))){
                try{
                    ifstream *f = new ifstream;
                    f->open(argv[2], ios::in);
                    Scanner a(argv[2]);
                    while (!a.isEnd()){
                        a.Next();
                        a.Get()->Print();
                    }

                    f->close();
                    delete f;
                }
                catch (MyException &e){
                    e.Print();
                }
            }

    }
    catch (MyException &e){
        e.Print();
        if (output){
            e.Print(output);
            output->close();
        }
    }

    return 0;
}
