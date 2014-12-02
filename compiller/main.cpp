\

//
//  main.cpp
//  compiller
//
//  Created by Данил on 17.09.14.
//  Copyright (c) 2014 Данил. All rights reserved.
//

#include "scanner.h"
#include "Parser.h"
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
//    argc = 2;
//    argv[1] = "-pd";
//
//    argv[2] = "TypeChecking/173.in";
//ERROR:Expression must have integral type	 line 4, column 4

//  9  83 97 100 108 137 142  145 147 72
//    приведение типов
//    беда со 170
//    и со 104 - 107
    //
    try{

        if(argc == 1)
            throw MyException("Running without parameters");
        if (!(strcmp(argv[1], "-help"))){
            cout <<"-help" << "-f - write to the file + output to the screen (scan)" << endl <<
            "-s - output to the screen (lexel)" << endl <<
            "-t - write to the file (scan)" << endl;

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
                }
                g->close();
                delete g;
                f->close();
                delete f;

            }
            catch (MyException &e){
                e.Print(output);
                output->close();
            }

        }
        if (!(strcmp(argv[1], "-pd"))){
            try{
                ifstream *f = new ifstream;
                f->open(argv[2], ios::in);
                Scanner a(argv[2]);
                Parser pars = *new Parser(a);


                pars.ParseProgram();
                pars.print_declaration(0);
                f->close();
                delete f;
            }
            catch (MyException &e){
                e.Print();
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


            else
                if (!(strcmp(argv[1], "-pe"))){
                    try{
                        ifstream *f = new ifstream;
                        f->open(argv[2], ios::in);
                        Scanner a(argv[2]);
                        Parser pars = *new Parser(a);


                        pars.ParseExpr()->print();
                        pars.print_declaration(0);
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
