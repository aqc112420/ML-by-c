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

���Իع麯����ʵ�֣�����һ������Իع飬��Сƽ������Ϊ��ʧ��������Ŀ�꺯����һ����Լ����͹���ι滮���⣬
��͹���ι滮����ļ�Сֵ�ڵ���Ϊ0��ȡ�����Ҽ�СֵΪȫ����Сֵ�����б�ʽ�⡣������ѧ���ʽʵ�־���֮���������ò���w��
**/
int regression(Matrix x,Matrix y)
{
    int i=0;
    Matrix xT;
    xT.initMatrix(&xT,x.row,x.col);
    x.transposematrix(x,&xT);
    //xT.print(xT);

    Matrix xTx;
    xTx.initMatrix(&xTx,xT.col,x.row);
    xTx.multsmatrix(&xTx,xT,x);///x��ת�ó���x
    //xTx.print(xTx);

    Matrix xTx_1;
    xTx_1.initMatrix(&xTx_1,xTx.row,xTx.col);
    xTx_1.nimatrix(&xTx_1,xTx);///x��ת�ó���x֮������
    //xTx_1.print(xTx_1);

    Matrix xTx_1xT;
    xTx_1xT.initMatrix(&xTx_1xT,xTx_1.col,xT.row);
    xTx_1xT.multsmatrix(&xTx_1xT,xTx_1,xT);///���ٳ���x��ת��
    //xTx_1.print(xTx_1xT);

    Matrix ws;
    ws.initMatrix(&ws,xTx_1xT.col,y.row);
    ws.multsmatrix(&ws,xTx_1xT,y);///�ٳ���y����Ȩ��Ȩ��������������һ��������

    cout<<"ws"<<endl;
    ws.print(ws);
    return 0;
}
/**
�ֲ���Ȩ���Իع��������Իع�Ļ����϶�ÿһ������������ѵ����ʱ�����ÿһ��ѵ���������������е���������һ����Ȩ��ϣ�
Ȩ�ص�ȷ������ͨ��һ���������㣬���õ��и�˹�ˣ����������Խ����Ȩ��Խ�󣬷�֮ԽС����������ÿһ�����������͵õ��˲�һ����
Ȩ���������������ó���������߲��������Ե��ˣ����������ӵ�ģ�͵ĸ��Ӷ������õ���Ϸ��������ݡ�

**/
//��Ҫע����Ǿֲ���Ȩ���Իع��Ƕ�ÿһ����������Ȩ�ؼ��㣬���Զ���ÿһ����������һ��Ȩ��w����������ĺ���ֻ�Ǿֲ����Իع��һ����Ҫ��������
Matrix locWeightLineReg(Matrix test,Matrix x,Matrix y,double k)
{
    Matrix w;
    w.initMatrix(&w,x.col,x.col);
    double temp=0;
    int i,j;

    /**
    ���ݲ��������������������ľ����Ѿ�ѡ��ĺ�ȷ���ֲ���Ȩ���󣬲��öԽ�����Ϊ�ֲ���Ȩֵ
    **/
    for(i=0;i<x.col;i++)
    {
        temp=0;
        for(j=0;j<x.row;j++)
        {
            temp+=(test.mat[0][j]-x.mat[i][j])*(test.mat[0][j]-x.mat[i][j]);
        }
        w.mat[i][i]=exp(temp/-2.0*k*k);
    }

    Matrix xT;
    xT.initMatrix(&xT,x.row,x.col);
    xT.transposematrix(x,&xT);

    Matrix wx;
    wx.initMatrix(&wx,w.col,x.row);
    wx.multsmatrix(&wx,w,x);

    Matrix xTwx;
    xTwx.initMatrix(&xTwx,xT.col,x.row);
    xTwx.multsmatrix(&xTwx,xT,wx);

    Matrix xTwx_1;
    xTwx_1.initMatrix(&xTwx_1,xTwx.row,xTwx.col);
    xTwx_1.nimatrix(&xTwx_1,xTwx);

    //xTwx_1.print(xTwx_1);

    Matrix xTwx_1xT;
    xTwx_1xT.initMatrix(&xTwx_1xT,xTwx_1.col,xT.row);
    xTwx_1xT.multsmatrix(&xTwx_1xT,xTwx_1,xT);

    Matrix xTwx_1xTw;
    xTwx_1xTw.initMatrix(&xTwx_1xTw,xTwx_1xT.col,w.row);
    xTwx_1xTw.multsmatrix(&xTwx_1xTw,xTwx_1xT,w);

    Matrix ws;
    ws.initMatrix(&ws,xTwx_1xTw.col,y.row);
    ws.multsmatrix(&ws,xTwx_1xTw,y);

    cout<<"ws"<<endl;
    ws.print(ws);
    return ws;
}
/**
ͨ����������ľֲ���Ȩ���Իع麯��ʵ�ֶ�ÿһ����������ȷ�����Ĳ�����ͬ��������������ʽ�洢����n��������������n�У���m��������һ��m�У�
**/

int lwlrtest(Matrix x,Matrix y,double k)
{
    int i,j;
    Matrix test;
    test.initMatrix(&test,1,x.row);
    Matrix wsOne;
    wsOne.initMatrix(&wsOne,x.row,1);
    Matrix ws;
    ws.initMatrix(&ws,x.row,x.col);

    /**
    ÿһ�������������þֲ���Ȩ���Իع�
    **/
    for(i=0;i<ws.row;i++)///ws����Ϊһ��Ȩ�ز���������m�б�ʾm������
    {
        test=test.getOneCol(x,i);
        wsOne=locWeightLineReg(test,x,y,k);
        for(j=0;j<ws.col;j++)
        {
            ws.mat[j][i]=wsOne.mat[j][0];
        }
    }
    for(i=0;i<ws.row;i++)
    {
        for(j=0;j<ws.col;j++)
        {
            cout<<ws.mat[j][i]<<"  ";
        }
        cout<<endl;
    }

    /**��֤�ֲ���Ȩ���Իع����ȷ��*/
    double yy;
    for(i=0;i<ws.row;i++)
    {
        yy=0;
        for(j=0;j<ws.col;j++)
        {
            yy+=ws.mat[j][i]*x.mat[i][j];
        }
        cout<<"y="<<yy<<endl;
    }
    return 0;
}
/**
�������ع麯��ֻ����һ������Իع麯���Ļ������ڶԽ�������������ĸ�������н�����󲻿�������ԣ�ͬ��Ҳ���������Ŀ�ģ�
���ó��õĶ������͵õ���ֱ������lam����ʽ��
**/

int ridgeRegres(Matrix x,Matrix y,double lam)
{
    int i=0;
    Matrix xT;
    xT.initMatrix(&xT,x.row,x.col);
    x.transposematrix(x,&xT);
    xT.print(xT);

    Matrix xTx;
    xTx.initMatrix(&xTx,xT.col,x.row);
    xTx.multsmatrix(&xTx,xT,x);
    xTx.print(xTx);

    Matrix denom;
    denom.initMatrix(&denom,xTx.col,xTx.row,lam);
    xTx.addmatrix(&xTx,xTx,denom);
    xTx.print(xTx);

    Matrix xTx_1;
    xTx_1.initMatrix(&xTx_1,xTx.row,xTx.col);
    xTx_1.nimatrix(&xTx_1,xTx);

    Matrix xTx_1xT;
    xTx_1xT.initMatrix(&xTx_1xT,xTx_1.col,xT.row);
    xTx_1xT.multsmatrix(&xTx_1xT,xTx_1,xT);

    Matrix ws;
    ws.initMatrix(&ws,xTx_1xT.col,y.row);
    ws.multsmatrix(&ws,xTx_1xT,y);

    cout<<"ws"<<endl;
    ws.print(ws);
    return 0;
}
int main()
{
    dataToMatrix dtm;
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    char file[20]="data\\lineReg.txt";
    dtm.loadData(&dtm,file);
    dtm.print(dtm);

    cout<<"x,y"<<endl;
    cout<<"----------------------"<<endl;
    Matrix x;
    x.loadMatrix(&x,dtm);
    Matrix y;
    y.initMatrix(&y,x.col,1);
    y=y.getOneRow(x,x.row);
    y.print(y);
    x.deleteOneRow(&x,x.row);
    x.print(x);

    regression(x,y);///�������Իع麯��
    //lwlrtest(x,y,1.0);///���þֲ���Ȩ���Իع麯��
    //ridgeRegres(x,y,0.1);///������ع麯��
    return 0;
}
