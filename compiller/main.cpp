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
static string BaseName(string in){
    return in.erase(in.find('.'));
}



void compile(string file){
	string filename = file.substr(0, file.size() - 3);

	string path = "C:\\masm32\\bin\\buildc " + filename;

	system(path.c_str());
	system(string(filename+".exe").c_str());
}


int main(int argc, char *argv[]){
    ofstream *output = 0;
    createKeyWords();
    createOperations();
  //  argc = 2;
//	argv[1] = "-gen";
	
 
//	argv[2] = "Gen_test\\036.in";

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
                ifstream f ;
                f.open(argv[2], ios::in);
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
               // f->close();
//                delete f;

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
                Generator g = *new Generator("");
                Parser pars = *new Parser(a,g);


                pars.ParseProgram();
                pars.print();
                f->close();
                delete f;
            }
            catch (MyException &e){
                e.Print();
            }
        }


        if(!strcmp(argv[1], "-gen")){
            ifstream *f = new ifstream(argv[2], ios::in);
            Scanner scan(argv[2]);
            string name(BaseName(argv[2]));
            Parser parser(scan, *new Generator(name + string(".asm")));

            parser.ParseProgram();
           parser.print();
            parser.GenerateCode();
            string path(argv[0]);
            path = path.substr(0, path.rfind("\\") + 1) + "..\\";
            parser.generator.generate();
			compile(argv[2]);

         } else
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
                        Generator g = *new Generator("");
                        Parser pars = *new Parser(a,g);


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

	

	//system("pause");
	return 0;
	
}

