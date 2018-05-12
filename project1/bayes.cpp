#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <fstream>
#include <sstream>
#include <stack>
#include "matrix.h"
#define EMAILSUM 50 //�ǳ��ر��һ�����������÷�Χ����
#define FILE_MAX_NUM 99
#define FILE_NAME_LEN 30
#define FILE_WORD_LEN 1000
#define CLASS_SUM 2
#define VEC_LEN 1000
using namespace std;

struct Bayes
{
    Matrix pXY;
    Matrix pY;
    Matrix pX;
};
Bayes bayes;//����bayes���������еĲ���
char file[CLASS_SUM][FILE_MAX_NUM][FILE_NAME_LEN];//�������ݵ��ļ���

/**
���ɵ����ֵ�
*/
dataToMatrix createVocabList(dataToMatrix *dtm)
{
    int i,j,k,vl;
    int vec_len=0;
    string dic[VEC_LEN];
    Data *p;
    for(k=0; k<EMAILSUM; k++)
    {
        for(i=0; i<dtm[k].col; i++)
        {
            p=dtm[k].dataSet->next;
            for(j=0; j<dtm[k].row; j++)
            {
                for(vl=0; vl<vec_len; vl++)
                {
                    if(!p->attr_string[j].compare(dic[vl]))
                        break;
                }
                if(vl==vec_len)
                {
                    dic[vec_len]=p->attr_string[j];
                    vec_len++;
                }
            }
            p=p->next;
        }
    }
    dataToMatrix dicMat;
    dicMat.dataSet=new Data;
    dicMat.dataSet->next=NULL;
    p=dicMat.dataSet;
    Data *datatmp=new Data;
    datatmp->next=NULL;
    for(i=0; i<vec_len; i++)
    {
        datatmp->attr_string[i]=dic[i];
    }
    p->next=datatmp;
    dicMat.col=1;
    dicMat.row=vec_len;
    cout<<"vec_len="<<vec_len<<endl;
    return dicMat;
}
/**
����ѵ����������
**/
Matrix createFectVec(dataToMatrix *dtm,dataToMatrix dicMat)
{
    int i,j,k=0,vl;
    Data *p;
    Data *q;
    q=dicMat.dataSet->next;//�ֵ�ͬ��Ҳ��һ��
    Matrix vecX;
    vecX.initMatrix(&vecX,EMAILSUM,dicMat.row);
    for(k=0; k<EMAILSUM; k++)
    {
        for(i=0; i<dtm[k].col; i++)//ֻ��һ��
        {
            p=dtm[k].dataSet->next;
            for(j=0; j<dtm[k].row; j++)
            {
                for(vl=0; vl<dicMat.row; vl++)
                {
                    if(!p->attr_string[j].compare(q->attr_string[vl]))
                    {
                        vecX.mat[k][vl]=1;
                        break;
                    }
                }
            }
        }
    }
    /*
    for(i=0;i<vecX.col;i++)
    {
        for(j=0;j<vecX.row;j++)
            cout<<vecX.mat[i][j]<<"  ";
        cout<<endl;
    }*/
    return vecX;
}

/**
���ɲ���������������
**/
Matrix createTestVec(dataToMatrix testX,dataToMatrix dicMat)
{
    int i,j,vl;
    Data *p;
    Data *q;
    p=testX.dataSet->next;
    q=dicMat.dataSet->next;//�ֵ�ͬ��Ҳ��һ��
    Matrix testVecX;
    testVecX.initMatrix(&testVecX,testX.col,dicMat.row);
    for(i=0; i<testX.col; i++)//ֻ��һ��
    {
        for(j=0; j<testX.row; j++)
        {
            for(vl=0; vl<dicMat.row; vl++)
            {
                if(!p->attr_string[j].compare(q->attr_string[vl]))
                {
                    testVecX.mat[0][vl]=1;
                    break;
                }
            }
        }
    }
    return testVecX;
}

/**
ѵ��������ʵ�֣�ע��Բ�������ƽ������
**/
int trainNB(Matrix vecX,Matrix vecY)
{
    bayes.pY.initMatrix(&bayes.pY,2,1);//�����ʼ��Ϊ2�е�������
    bayes.pX.initMatrix(&bayes.pX,1,vecX.row);
    bayes.pXY.initMatrix(&bayes.pXY,2,vecX.row);

    double dicY[CLASS_SUM]= {0,0};
    int i,j,k,len=0;
    for(k=0; k<CLASS_SUM; k++)
    {
        for(j=0; j<vecX.row; j++)
        {
            bayes.pXY.mat[k][j]=1;//ƽ������,Ĭ�ϳ���һ�Σ����ڹ�һ��ʱ�����������ĳ���Ҳ���ǽ�ȥ,�����ƽ����ָÿһ���ֵ��������ֵ�ģ�δ�漰����
            //�����е�δ�����
            bayes.pX.mat[0][j]=0;
        }
    }
    int wordsum=0;
    for(i=0; i<vecX.col; i++)
    {
        if(vecY.mat[i][0]==1)
        {
            bayes.pY.mat[0][0]++;
            for(j=0; j<vecX.row; j++)
            {
                bayes.pXY.mat[0][j]+=vecX.mat[i][j];
                dicY[0]+=vecX.mat[i][j];
                wordsum+=vecX.mat[i][j];
                bayes.pX.mat[0][j]+=vecX.mat[i][j];
            }
        }
        else
        {
            bayes.pY.mat[1][0]++;
            for(j=0; j<vecX.row; j++)
            {
                bayes.pXY.mat[1][j]+=vecX.mat[i][j];
                dicY[1]+=vecX.mat[i][j];
                wordsum+=vecX.mat[i][j];
                bayes.pX.mat[0][j]+=vecX.mat[i][j];
            }
        }
    }
    //�����PY����ĸ���
    for(i=0; i<CLASS_SUM; i++)
    {
        bayes.pY.mat[i][0]/=vecX.col;
    }

    double s=0;
    for(k=0; k<CLASS_SUM; k++)
    {
        for(i=0; i<vecX.row; i++)
        {
            bayes.pXY.mat[k][i]/=dicY[k]+vecX.row;
            s+=bayes.pXY.mat[k][i];
        }
    }
    double ss=0;
    for(i=0; i<vecX.row; i++)
    {
        bayes.pX.mat[0][i]/=wordsum;//+vec_len;
        ss+=bayes.pX.mat[0][i];
    }
    /*
    cout<<"pY="<<bayes.pY.mat[0][0]<<endl;
    cout<<"dicy"<<dicY[0]<<"  "<<dicY[1]<<endl;
    cout<<"wordsum="<<wordsum<<endl;
    cout<<"ss="<<ss<<endl;
    cout<<"s="<<s<<endl;*/
}

/**
������ߺ���
**/
int classifyNB(Matrix testVecX)
{
    double p0=1.0,p1=1.0;
    int i,j;
    for(i=0; i<testVecX.row ; i++)
    {
        if(testVecX.mat[0][i]!=0)
        {
            p0*=(bayes.pXY.mat[0][i]/bayes.pX.mat[0][i]);
            p1*=(bayes.pXY.mat[1][i]/bayes.pX.mat[0][i]);
            cout<<p0<<endl;
            cout<<p1<<endl;
        }
    }
    p0*=bayes.pY.mat[0][0];
    p1*=bayes.pY.mat[1][0];
    cout<<"p0="<<p0<<"&"<<"p1="<<p1<<endl;
    if(p0<p1)
        cout<<"class="<<1;
    else
        cout<<"class="<<0;
    return 0;

}

/**
    �������ݵ��ļ���
*/
void getFileName()
{
    int i=0,j=0,k=0,l=0;
    string path[2]= {"data\\email\\ham\\*.txt","data\\email\\spam\\*.txt"};
    for(k=0; k<CLASS_SUM; k++)
    {
        for(i=0; i<25&&i<FILE_MAX_NUM; i++)
        {
            for(j=0,l=0; j<FILE_NAME_LEN; j++,l++)
            {
                if(path[k][j]!='*')
                    file[k][i][j]=path[k][l];
                else
                {
                    if((i+1)/10>0)
                    {
                        file[k][i][j]=48+((i+1)/10);
                        file[k][i][j+1]=48+(i+1)%10;
                        j++;
                    }
                    else
                    {
                        file[k][i][j]=48+i+1;
                    }
                }
            }
            //cout<<file[k][i]<<"  "<<i<<"  "<<i/10<<endl;
        }
    }
}

int main()
{

    int k,i,j;
    getFileName();///����ѵ�������ļ������ɺ���

    /**
    ������������
    **/
    dataToMatrix dtm[EMAILSUM];
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    for(k=0; k<CLASS_SUM; k++)
    {
        for(i=0; i<25&&i<FILE_MAX_NUM; i++)
        {
            dtm[25*k+i].loadData(&dtm[25*k+i],file[k][i],1);
        }
    }

    dataToMatrix dicMat;// �ֵ�
    dicMat=createVocabList(dtm);
    dicMat.print(dicMat);

    /**
    ������������
    **/
    Matrix vecX;
    vecX.initMatrix(&vecX,EMAILSUM,dicMat.row);
    vecX=createFectVec(dtm,dicMat);

    /**
    ����������ǩ
    **/
    Matrix vecY;
    vecY.initMatrix(&vecY,EMAILSUM,1);
    for(i=0; i<EMAILSUM; i++)
    {
        if(i<25)
            vecY.mat[i][0]=1;
        else
            vecY.mat[i][0]=-1;
    }

    trainNB(vecX,vecY);///ͨ����������ͱ�ǩ������ò�����⺯�����bayes���ߵĲ���

    /**
    ���ɲ�������
    */
    dataToMatrix dtmTest;
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    char testfile[30]="data\\email\\test.txt";
    dtmTest.loadData(&dtmTest,testfile,1);

    /**
    ���ɲ���������������
    **/
    Matrix testVecX;
    testVecX.initMatrix(&testVecX,1,dicMat.row);
    testVecX=createTestVec(dtmTest,dicMat);
    testVecX.print(testVecX);

    classifyNB(testVecX);///�������
}
