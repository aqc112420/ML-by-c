#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include "matrix.h"
using namespace std;

/***���ݹ�һ������data[i][j]-min[j]/range[j]**/
int autoNorm(Matrix x)
{
    int j=0,i=0;

    Matrix minVals;
    minVals.initMatrix(&minVals,1,x.row,MAX);

    Matrix maxVals;
    maxVals.initMatrix(&maxVals,1,x.row,MIN);

    Matrix ranges;
    ranges.initMatrix(&ranges,1,x.row);

    for(j=0; j<x.row; j++)
    {
        minVals.mat[0][j]=x.mat[0][j];
        maxVals.mat[0][j]=minVals.mat[0][j];
    }
    for(i=0; i<x.col; i++)
    {
        for(j=0; j<x.row; j++)
        {
            if(x.mat[i][j]<minVals.mat[0][j])
                minVals.mat[0][j]=x.mat[i][j];
            if(x.mat[i][j]>maxVals.mat[0][j])
                maxVals.mat[0][j]=x.mat[i][j];
        }
    }
    for(i=0; i<x.row; i++)
        ranges.mat[0][i]=maxVals.mat[0][i]-minVals.mat[0][i];
    for(i=0; i<x.col; i++)
    {
        for(j=0; j<x.row; j++)
        {
            x.mat[i][j]-=minVals.mat[0][j];
            x.mat[i][j]/=ranges.mat[0][j];
            //cout<<x.mat[i][j]<<"  ";
        }
        //cout<<endl;
    }
    return 0;
}
/**����ÿ������������ѵ�������ľ��룬������distance������**/
Matrix cdistances(Matrix test,Matrix x)
{
    int i,j;
    Matrix distances;
    distances.initMatrix(&distances,x.col,1,0);
    for(i=0; i<x.col; i++)
    {
        for(j=0; j<x.row; j++)
        {
            distances.mat[i][0]+=pow((x.mat[i][j]-test.mat[0][j]),2);
        }
        distances.mat[i][0]=sqrt(distances.mat[i][0]);
        //cout<<"dis="<<distances.mat[i][0]<<endl;
    }
    return distances;
}
/***ѡ���K������**/
Matrix getK(Matrix oneTest,Matrix x,int K)
{
    int i,j,k;
    //Ϊÿһ������������ʼ��k������Ϊǰk��ѵ������������¼���ڵ�id
    Matrix distances;
    distances.initMatrix(&distances,x.col,1);
    distances=cdistances(oneTest,x);

    Matrix Kdistances;
    Kdistances.initMatrix(&Kdistances,K,2);
    double Max=-1;
    int Maxi=-1;
    for(i=0; i<K; i++)
    {
        Kdistances.mat[i][0]=distances.mat[i][0];
        Kdistances.mat[i][1]=i;//��¼���ڵ�id
        if(Kdistances.mat[i][0]>Max)
        {
            Max=Kdistances.mat[i][0];
            Maxi=i;//ѡ����ǰk������������һ��
        }
    }
    //Ϊÿһ�����������ӵ�K��ѵ�������б��������µ�k������
    for(i=K; i<x.col; i++)
    {
        if(distances.mat[i][0]<Max)
        {
            Kdistances.mat[Maxi][0]=distances.mat[i][0];
            Max=distances.mat[i][0];//��ʱ���µ�ǰ�滻�ľ���Ϊ�����룬��Ϊ�Ѿ�������֮ǰ����������
            Kdistances.mat[Maxi][1]=i;//��¼���ڵ�id
            for(k=0; k<K; k++)
            {
                if(Kdistances.mat[k][0]>Max)
                {
                    Max=Kdistances.mat[k][0];
                    Maxi=k;//ѡ����ǰk������������һ��
                }
            }
        }
    }
    return Kdistances;
}
//KNN�������������ƽ��Ȩ��ͶƱ���Ҳ�������������������
/**
1��knn������ߺ��������ȶ�ѵ�����ݺͲ������ݽ��й�һ����
2������ÿһ������������m��ѵ�������ľ���
3����m��������ѡ����С��k�����룬����¼��k����С���������
4����k��������ȨͶƱ�õ����յľ������

***/
int classfiy(Matrix testData,Matrix testDatay,Matrix x,Matrix y,int K)
{
    int i,j,k;
    int sumz=0,sumf=0;
    Matrix knn;
    knn.initMatrix(&knn,K,2);

    autoNorm(x);
    autoNorm(testData);
    for(i=0; i<testData.col; i++)
    {
        sumz=0;
        sumf=0;

        Matrix oneTest;
        oneTest.initMatrix(&oneTest,1,x.row);
        oneTest=oneTest.getOneCol(testData,i);

        knn=getK(oneTest,x,10);

        for(j=0; j<K; j++)
        {

            if(y.mat[int(knn.mat[j][1])][0]==1)
                sumz++;
            else
                sumf++;
            cout<<y.mat[int(knn.mat[j][1])][0]<<"  ";
        }
        if(sumz>sumf)
            cout<<"juece="<<"1"<<"&"<<"shiji="<<testDatay.mat[i][0]<<endl;
        else
            cout<<"juece="<<"-1"<<"&"<<"shiji="<<testDatay.mat[i][0]<<endl;
    }
    return 0;
}
int main()
{
    dataToMatrix dtm;
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    char file[20]="data\\knn.txt";
    dtm.loadData(&dtm,file);
    //dtm.print(dtm);

    Matrix x;
    x.loadMatrix(&x,dtm);

    Matrix y;
    y.initMatrix(&y,x.col,1);
    y=y.getOneRow(x,x.row);

    x.deleteOneRow(&x,x.row);


    dataToMatrix dtmTest;
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    char testFile[20]="data\\knnTest.txt";
    dtmTest.loadData(&dtmTest,testFile);
    //dtmTest.print(dtmTest);

    Matrix testData;
    testData.loadMatrix(&testData,dtmTest);

    Matrix testDatay;
    testDatay.initMatrix(&testDatay,x.col,1);
    testDatay=testDatay.getOneRow(testData,testData.row);

    testData.deleteOneRow(&testData,testData.row);


    classfiy(testData,testDatay,x,y,10);
    return 0;
}
