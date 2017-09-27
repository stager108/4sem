#pragma once

class COutBitStream {
private:
    int BitCount = 0;
    unsigned char Buffer[1024];
    int BufferSize;
    std::ofstream OutFile;
public:
    COutBitStream()
    {
        BitCount = 0;
    }

    void OpenBitStream(char* filename)
    {
        char a[20];
        OutFile.open(filename,  std::ios_base::out);
        if(!OutFile){
            throw __EXCEPTION__;
        }
        for (int e = 0; e<1024; e++) {
            Buffer[e] = (unsigned char) 0;
        }
        BitCount = 0;
    }

    ~COutBitStream()
    {
    }

    void FlushToFile()
    {
        for (int i = 0; i<1024; i++) {
            OutFile << Buffer[i];
        }

        for (int e = 0; e<1024; e++) {
            Buffer[e] = (unsigned char) 0;
        }
        BitCount = 0;

    }

    void FlushAllToFile()
    {
        int n = BitCount/8;
        for (int i = 0; i<n; i++) {
            OutFile << Buffer[i];
        }
        n = BitCount%8;
        if (n>0) {
            OutFile << Buffer[BitCount/8];
        }
    }

    void WriteBit(unsigned char bit)
    {
        if (BitCount==1024*8) {
            FlushToFile();
        }
        if (bit>0) {
            Buffer[BitCount/8] += (1 << (7-BitCount%8));
        }
        BitCount++;
    }

    void WriteByte(unsigned char byte)
    {
        unsigned char CrashedByte[8];
        int a;
        for (int ii = 0; ii<8; ii++) {
            a = byte%2;
            byte = byte/2;
            CrashedByte[7-ii] = a;
        }
        for (int ii = 0; ii<8; ii++) {
            WriteBit(CrashedByte[ii]);
        }

    }

    void WriteInt(int s)
    {
        OutFile << s << " ";
    }
};
