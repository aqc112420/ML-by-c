#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include "matrix.h"
#include <fstream>
#include <sstream>
#include <stack>
using namespace std;
#define MAX 1000000
#define MIN -100000

/**
sigmad����,����û��ֱ�ӽ�Ԥ�����ת��Ϊ���ͣ����ǻ��Ƿ���һ��doubleֵ
*/
double sigmoid(double z)
{
    return 1.0/(1+exp(-z));
}
/**
�ݶ��½��㷨����Ҫ��ȷ�����ݶȷ��򣬲��������õ�����˼�����n��������
��Ŀ�꺯����͹�滮���⣬��ô�ֲ���Сֵ����ȫ����Сֵ

**/
int gradAscent(Matrix x,Matrix y)
{
    Matrix weights;
    weights.initMatrix(&weights,x.row,1,1);///��ʼ���Ż���������Ϊ1

    Matrix xT;
    xT.initMatrix(&xT,x.row,x.col);
    xT.transposematrix(x,&xT);

    Matrix z;
    z.initMatrix(&z,x.col,1);

    Matrix w1;
    w1.initMatrix(&w1,x.row,y.row);

    double alpha=0.001;///��������
    double error;///��¼������
    int k,c=0;
    int i,j;
    for(c=0; c<1000; c++)
    {
        z.multsmatrix(&z,x,weights);
        for(i=0; i<x.col; i++)
            z.mat[i][0]=sigmoid(z.mat[i][0]);///Ԥ�����
        z.submatrix(&z,y,z);///���㸺�ݶȷ���ͬʱ������ΪԤ���������ʵ�������
        error=0;
        for(k=0; k<x.col; k++)///ͳ�ƴ�����
            error+=z.mat[k][0];
        cout<<"error="<<error<<endl;
        if(error<2&&error>-2)///���ô�����С��һ��ֵʱ�˳�����
            break;
        w1.multsmatrix(&w1,xT,z);///���㸺�ݶȷ���
        for(j=0; j<x.row; j++)
            w1.mat[j][0]*=alpha;///���ݶȷ����벽���ĳ˻�ȷ������ֵ
        weights.addmatrix(&weights,weights,w1);///�����ݶȷ�����һ������

    }
    /**
    ��֤�㷨����ȷ��
    **/
    Matrix test;
    test.initMatrix(&test,y.col,y.row);
    test.multsmatrix(&test,x,weights);
    cout<<"test"<<endl;
    for(i=0; i<y.col; i++)
    {
        if(test.mat[i][0]>0)
            cout<<1-y.mat[i][0]<<endl;
        else
            cout<<0-y.mat[i][0]<<endl;
    }
}
/**
����ݶ��½����ݶ��½�����ͬ�����ڸ��ݶȷ����ȷ�����ݶ��½��Ǹ������е�������ȷ�����ݶȷ���
������ݶ��½�ÿ��ֻ��һ����������ȷ�����ݶȷ�����Ȼ����ȫ���ţ������ŵ����������ӣ�ͬ������

**/
int stoGradAscent(Matrix x,Matrix y)//����ݶ��½�ÿһ��ѡ��m�������������ݶ��½����򣬸ô�����ֻѡ��һ��������������ݶ��½���������ֵ
{
    int i,j,c=0;
    Matrix weights;
    weights.initMatrix(&weights,x.row,1,1);

    Matrix z;
    z.initMatrix(&z,1,1);

    Matrix xOne;
    xOne.initMatrix(&xOne,1,x.row);

    Matrix xOneT;
    xOneT.initMatrix(&xOneT,xOne.row,xOne.col);

    Matrix w1;
    w1.initMatrix(&w1,x.row,y.row);

    double alpha=0.01;///����
    double error;
    for(c=0; c<500; c++)
    {
        for(i=0; i<x.col; i++)
        {
            xOne=xOne.getOneCol(x,i);///���ѡ��һ�������㣬����û�������ѡ�񣬶��ǰ���ѡ��
            z.multsmatrix(&z,xOne,weights);
            z.mat[0][0]=sigmoid(z.mat[0][0]);
            z.mat[0][0]=y.mat[i][0]-z.mat[0][0];
            xOneT.transposematrix(xOne,&xOneT);
            w1.multsmatrix(&w1,xOneT,z);///����һ��������Ԥ�������ȷ�����ݶȷ���
            for(j=0; j<w1.row; j++)
                w1.mat[j][0]*=alpha;
            weights.addmatrix(&weights,weights,w1);///����
        }
        cout<<"weights"<<endl;
        weights.print(weights);
    }
    /**
    ��֤�㷨����ȷ��
    */
    Matrix test;
        test.initMatrix(&test,y.col,y.row);
        test.multsmatrix(&test,x,weights);
        for(i=0; i<y.col; i++)
        {
            if(test.mat[i][0]>0)
                cout<<1-y.mat[i][0]<<endl;
            else
                cout<<0-y.mat[i][0]<<endl;
        }
}

/**
�߼��ع飬������Ҫ�����䳣�õ�����������㷨��һ�����ݶ��½���һ��������ݶ��½�

*/

int main()
{
    dataToMatrix dtm;
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    char file[20]="data\\logReg.txt";
    dtm.loadData(&dtm,file);

    Matrix x;
    x.loadMatrix(&x,dtm);

    Matrix y;
    y.initMatrix(&y,x.col,1);
    y=y.getOneRow(x,x.row);

    x.deleteOneRow(&x,x.row);

    //gradAscent(x,y);
    stoGradAscent(x,y);
    return 0;
}
