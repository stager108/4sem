#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include <cassert>
#include <ios>
#include <string>

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

class CInBitStream {
    unsigned char Buffer[4];
    int BufferSize = 0;
    int PartOfFile = 0;
    int FileSize = 0;
    int i;
    FILE* InFil;
    std::fstream MyFile;
    int sizeOfFile = 0;
    int readBitCount = 0;
    int CrBit = 0;
    unsigned char CrashedByte[8];
    int ByteCount = 0;
public:

    explicit CInBitStream() { }

    ~CInBitStream() { }

    void OpenDecodingFile(char* filename, char* outfilename)
    {
        PartOfFile = 0;
        char a[20];
        InFil = fopen(filename, "rb");
        if(InFil != nullptr) {
            sizeOfFile = ReadInt();
        }
        else{
            throw __EXCEPTION__;
        }
        MyFile.open(outfilename, std::ios_base::out);
        if(!MyFile){
            throw __EXCEPTION__;
        }
    }

    char ReadChar(){
        char N;
        char e;
        N = getc(InFil);
        return N;
    }

    int ReadInt(){
        int N = 0;
        char n;
        fscanf(InFil, "%d", &N);
        n = getc(InFil);
        return N;
    }

    void CloseFile(){
        fclose(InFil);
        MyFile.close();
    }

    bool IsInFile(){
        return (PartOfFile!=sizeOfFile);
    }

    void Incr(){
        PartOfFile++;
    }

    void ReadBuffer(){
        unsigned char next;
        i = 0;
        for (int i = 0; i<4; i++)
            Buffer[i] = (unsigned char) 0;
        i = 0;
        while ((i<4)) {
            Buffer[i] = getc(InFil);
            i++;
        }
        ByteCount = 0;

    }

    int GetNextBit(){
        int a;
        unsigned char byte;
        if (CrBit==0) {
            byte = getc(InFil);
            for (int ii = 0; ii<8; ii++) {
                a = byte%2;
                byte = byte/2;
                CrashedByte[ii] = a;
            }
            ByteCount++;
            CrBit = 8;
        }

        CrBit--;
        return CrashedByte[CrBit];
    }

    void WriteNextSymbol(char Symbol)
    {
        MyFile.put(Symbol);
    }
};
