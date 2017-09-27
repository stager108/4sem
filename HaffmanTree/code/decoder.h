#pragma once

#include "cinbitstream.h"
#include "coutbitstream.h"
#include <vector>

class Decoder {
public:
    CInBitStream InFile;
    struct Node {
        Node* Left = nullptr;
        Node* Right = nullptr;
        char Symbol;
    };

    Decoder() { }

    ~Decoder() { }

    Node* ReadTree()
    {
        std::vector<Node*> TreeMaker;
        Node* NewNode = nullptr;
        Node* First;
        Node* Second;
        int treesize;
        char u;

        treesize = InFile.ReadInt();
        treesize = treesize * 2 - 1;
        for (int m = 0; m < treesize; m++) {
            u = InFile.ReadChar();
            if (u == (unsigned char) 0) {
                NewNode = new Node;
                u = InFile.ReadChar();
                NewNode->Symbol = u;
                TreeMaker.push_back(NewNode);
            } else {
                NewNode = new Node;
                First = TreeMaker.back();
                TreeMaker.pop_back();
                Second = TreeMaker.back();
                TreeMaker.pop_back();
                NewNode->Symbol = '0';
                NewNode->Left = Second;
                NewNode->Right = First;
                TreeMaker.push_back(NewNode);
            }
        }
        return TreeMaker.back();
    }

    void DecodeFile(char* filename, char* outfilename)
    {
        int newbit;
        InFile.OpenDecodingFile(filename, outfilename);
        Node* Tree = ReadTree();
        Node* TreeList = Tree;
        while (InFile.IsInFile()) {
            newbit = InFile.GetNextBit();
            if (newbit == 0) {
                if (TreeList->Left == nullptr) {
                    InFile.WriteNextSymbol(TreeList->Symbol);
                    InFile.Incr();
                    TreeList = Tree;
                }
                TreeList = TreeList->Left;
            } else {
                if (TreeList->Right == nullptr) {
                    InFile.WriteNextSymbol(TreeList->Symbol);
                    InFile.Incr();
                    TreeList = Tree;
                }
                TreeList = TreeList->Right;
            }
        }
        InFile.Incr();
        InFile.CloseFile();
    }
};