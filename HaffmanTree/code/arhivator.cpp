

#include <iostream>
#include <sstream>
#include <cstring>
#include <fstream>
#include <stack>
#include "coder.h"
#include "decoder.h"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru");
    if (argv[1][0] == 'h') {
        HaffmanCoder H;
        H.CodeFile(argv[2], argv[3]);
    }else {
        if (argv[1][0] == 's') {
            ShannonCoder S;
            S.CodeFile(argv[2], argv[3]);
        } else {
            if (argv[1][0] == 'd') {
                Decoder D;
                D.DecodeFile(argv[2], argv[3]);
            }
        }
    }
    return 0;
}

