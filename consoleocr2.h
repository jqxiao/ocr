#ifndef CONSOLEOCR_H_INCLUDED
#define CONSOLEOCR_H_INCLUDED
#include<fstream>
#include<sstream>
using namespace std;
#define DIGITS 10
#define ROWS 28
#define COLS 28
#define TRAIN 60000
#define TEST 10000
typedef long double DTYPE;
typedef long int ITYPE;
DTYPE feature[DIGITS][ROWS][COLS];
ITYPE sum[DIGITS][ROWS][COLS];
ITYPE counter60k[DIGITS];
ITYPE counter10k[DIGITS];
ITYPE errorAccept[DIGITS];
ITYPE errorReject[DIGITS];
DTYPE testTemp[ROWS][COLS];
DTYPE testArr[DIGITS];
DTYPE dtemp;
ITYPE correct[DIGITS];
//DTYPE testPossibility;
struct Image28
{
    unsigned char data[ROWS][COLS];

};
void trainSample()
{
    ifstream ifslbl,ifsimg;
    ifslbl.open("train-labels.idx1-ubyte");
    ifsimg.open("train-images.idx3-ubyte");
    ifslbl.seekg(8,ios::beg);
    ifsimg.seekg(16,ios::beg);
    Image28 img28;
    unsigned char lbl28;
    for(int i=0; i<TRAIN; i++)
    {
        counter60k[lbl28]++;

        for(int j=0; j<ROWS; j++)
            for(int k=0; k<COLS; k++)
            {
                ifslbl.read((char*)&lbl28,sizeof(unsigned char));
                ifsimg.read((char*)&img28,sizeof(Image28));
                if((img28.data)[j][k]>128)
                    sum[lbl28][j][k]++;
            }

    }

    ifslbl.close();
    ifsimg.close();
    for(int i2=0; i2<DIGITS; i2++)
        for(int j2=0; j2<ROWS; j2++)
            for(int k2=0; k2<COLS; k2++)
                feature[i2][j2][k2]=sum[i2][j2][k2]/(DTYPE)counter60k[i2];

}
void testAll()
{
    ifstream ifslbl;
    ifstream ifsimg;
    ifsimg.open("t10k-images.idx3-ubyte");
    ifslbl.open("t10k-labels.idx1-ubyte");
    ifslbl.seekg(8,ios::beg);
    ifsimg.seekg(16,ios::beg);
    unsigned char lbl28;
    Image28 img28;
    for(int i0=0; i0<DIGITS; i0++)
        correct[i0]=0;
    for(int i=0; i<TEST; i++)
    {
        ifslbl.read((char*)&lbl28,sizeof(unsigned char));
        ifsimg.read((char*)&img28,sizeof(Image28));
        counter10k[lbl28]++;
        for(int j0=0; j0<DIGITS; j0++)
            testArr[j0]=0;
        for(int m=0; m<DIGITS; m++)
        {
            for(int j=0; j<ROWS; j++)
                for(int k=0; k<COLS; k++)
                {
                    if((img28.data)[j][k]>128)
                        testArr[m]+=feature[m][j][k];
                    else
                        testArr[m]+=(1-feature[m][j][k]);

                }
        }
        unsigned char index=0;
        DTYPE maxval=testArr[index];
        for(unsigned char j2=1; j2<DIGITS; j2++)
            if(maxval<testArr[j2])
            {
                index=j2;
                maxval=testArr[index];
            }
        if(index==lbl28)
            correct[lbl28]++;
    }
    int tmp=0;
    int tmp2=0;
    for(int r=0; r<DIGITS; r++)
    {
        cout<<"input:"<<r<<" "<<"total:"<<counter10k[r]<<" "<<"correct"<<correct[r]<<endl;
        tmp+=counter10k[r];
        tmp2+=counter60k[r];
    }
    cout<<tmp<<" "<<tmp2<<endl;
    ifslbl.close();
    ifsimg.close();

}


#endif // CONSOLEOCR_H_INCLUDED
