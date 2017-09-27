#pragma once

#include "cinbitstream.h"
#include "coutbitstream.h"
#include "myheap.h"
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
#include <vector>

class Coder{

protected:
    struct Code {
        int length = 0;
        unsigned char code[256];
    };

    COutBitStream OutStream;
    unsigned char a[256];
    Code CodeSeq[256];
    int j = 0;
    int FileSize;
    int SymbolsInText = 0;
    FILE* InFile;

    void getFrequencies(int*& Counter, char* &filename)
    {
        unsigned char sym = 0;
        unsigned char next;
        FileSize = 0;
        InFile = fopen(filename, "rb");
        if(InFile != nullptr) {
            while (!feof(InFile)) {
                next = getc(InFile);
                sym = next;
                Counter[sym]++;
                FileSize++;
            }
            fclose(InFile);
        }
        else{
            throw __exception();
        }
    }

    void createSequensies(Node* NowNode)
    {
        if (NowNode!=nullptr) {
            j++;
            if ((NowNode->Left == nullptr) && (NowNode->Right == nullptr)) {
                for (int ii = 0; ii<j; ii++) {
                    CodeSeq[NowNode->Symbol].code[ii] = a[ii];
                }
                CodeSeq[NowNode->Symbol].length = j;
            }
            if (NowNode->Left!=nullptr) {
                a[j] = 0;
                createSequensies(NowNode->Left);
            }
            if (NowNode->Right!=nullptr) {
                a[j] = 1;
                createSequensies(NowNode->Right);
            }
            j--;
        }
    }

    virtual Node* CreateTree(char* filename) = 0;

    void WriteTree(Node* Nd)
    {

        if ((Nd->Left!=nullptr) && (Nd->Right!=nullptr)) {
            WriteTree(Nd->Left);
            WriteTree(Nd->Right);
            OutStream.WriteByte((unsigned char) 1);
        }
        else {
            OutStream.WriteByte((unsigned char) 0);
            OutStream.WriteByte((unsigned char) (Nd->Symbol));
        }
    }


public:

    Coder() { }

    ~Coder() { }

    void CodeFile(char* filename, char* outfilename)
    {
        unsigned char c;
        Node* root;
        FILE* InFile;
        j = -1;
        root = CreateTree(filename);
        unsigned char next;
        createSequensies(root);
        InFile = fopen(filename, "rb");
        OutStream.OpenBitStream(outfilename);
        OutStream.WriteInt(FileSize);
        OutStream.WriteInt(SymbolsInText);
        WriteTree(root);
        while (!feof(InFile)) {
            next = getc(InFile);
            c = next;
            for (int e = 0; e<CodeSeq[c].length; e++) {
                OutStream.WriteBit(CodeSeq[c].code[e]);
            }
        }
        delete root;
        OutStream.FlushToFile();
        fclose(InFile);

    }
};




class HaffmanCoder : public Coder {

public:

    HaffmanCoder() {}
    ~HaffmanCoder() { }

private:

    Heap HaffmanTree;

protected:

    Node* CreateTree(char* filename) override
    {

        int* Counter = new int[260];
        for (int i = 0; i<260; i++) { Counter[i] = 0; }
        getFrequencies(Counter, filename);
        Node* First = nullptr;
        Node* Second = nullptr;
        Node* NewNode = nullptr;

        int F = 0;
        for(int i = 0; i < 256; i++){
            F += Counter[i];
        }
/*
        long double entr = 0;
        std::cout << FileSize<<std::endl;
        for(int i = 0; i < 256; i++){
            if(Counter[i] > 0){
                std::cout << Counter[i]<<std::endl;
                entr +=  -((double)Counter[i]/F) * log2((double)Counter[i]/F);
                std::cout << entr << std::endl;
            }
        }
        std::cout << " entr " << entr << std::endl;*/

        for (int s = 0; s<256; s++) {
            if (Counter[s]>0) {
                NewNode = new Node;
                NewNode->Frequency = Counter[s];
                NewNode->Symbol = s;
                HaffmanTree.push(NewNode);
                SymbolsInText++;
            }
        }
        for (int i = SymbolsInText-1; i>0; i--) {
            First = HaffmanTree.min();
            HaffmanTree.extractMin();
            Second = HaffmanTree.min();
            HaffmanTree.extractMin();
            NewNode = new Node;
            NewNode->Left = First;
            NewNode->Right = Second;
            NewNode->Symbol = 0;
            NewNode->Frequency = First->Frequency+Second->Frequency;
            HaffmanTree.push(NewNode);
        }
        delete Counter;
        NewNode = HaffmanTree.min();
        HaffmanTree.extractMin();
        return NewNode;
    }
};

class ShannonCoder : public Coder{
public:

    ShannonCoder() { }

    ~ShannonCoder() { }

private:
    Node* Divide(std::vector<Node*>& Arr)
    {
        if (Arr.size()==1) {
            Node* leaf = Arr[0];
            return leaf;
        }
        else {
            double weight = 0;
            std::vector<Node*> FirstPart;
            std::vector<Node*> SecondPart;
            for (int i = 0; i < Arr.size(); i++) {
                weight += Arr[i]->Frequency;
            }
            weight /= 2;

            FirstPart.push_back(Arr[0]);
            double cur_weight = Arr[0]->Frequency;
            int i = 1;
            while (cur_weight + Arr[i]->Frequency < weight) {
                FirstPart.push_back(Arr[i]);
                cur_weight += Arr[i]->Frequency;
                i++;
            }
            for (i; i < Arr.size(); i++) {
                SecondPart.push_back(Arr[i]);
            }
            //std::cout << ((FirstPart.size() > 0) && (SecondPart.size() > 0)) << std::endl;
            Node* brunch = new Node;
            brunch->Left = Divide(FirstPart);
            brunch->Right = Divide(SecondPart);
            FirstPart.clear();
            SecondPart.clear();
            return brunch;
        }
    }

protected:

    Node* CreateTree(char* filename) override
    {

        int* Counter = new int[260];
        for (int i = 0; i < 260; i++) { Counter[i] = 0; }
        getFrequencies(Counter, filename);
        std::vector<Node*> vertices;
        Node* NewNode = nullptr;
        for (int s = 0; s < 256; s++) {
            if (Counter[s] > 0) {
                NewNode = new Node;
                NewNode->Frequency = Counter[s];
                NewNode->Symbol = s;
                vertices.push_back(NewNode);
                SymbolsInText++;
            }
        }
        Node* ans =  Divide(vertices);
        vertices.clear();
        return ans;
    }

};