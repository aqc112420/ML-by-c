/**

Kmeans�㷨�����ľ��Ǿ۳ɿ���ĳ��֣�ֵ�÷���һ��kmeansʲô����»���ֿ��࣬
�ó�����δ�Դ�����������϶����ǻ������������ģ��Ժ��ٷ���
**/
#include <iostream>
#include <stdlib.h>
using namespace std;
#include "matrix.h"
#define MAX 1000000
#define MIN -100000
#define random(x) (rand()%x)
#define MAXK 10

/**
Kmeans�����������ݽṹ
*/
struct CenAndDis
{
    Matrix cen;//��������
    Matrix dis;//ÿһ����������������ĵľ��룬��Ϊһ��m�е�������
};

///����һ��������ĳһ���������ĵľ���
double distances(Matrix xOne,Matrix kCen)
{
    int i,j;
    double dis=0;
    for(i=0; i<xOne.row; i++)
    {
        dis+=pow((xOne.mat[0][i]-kCen.mat[0][i]),2);
    }
    return dis;
}

/**
�����ʼ��һ���������ģ���ʼ���ķ�ʽ�����֣�һ���������������ѡһ����Ϊ�������ģ�
һ�����������ռ��ڳ�ʼ��һ���������ģ����Բ��Ǿ���ĳ�������������ĵĳ�ʼ���ǳ��ؼ�

������õ��Ǵ�������ѡ����Ϊ�������ģ���������ѡȡ��������ͬ��Ҳ����־۳ɿ�������󣬶Դ˽���취��

**/
Matrix randCent(Matrix x,int kNum)
{
    int i,j,k;
    Matrix initmeans;
    initmeans.initMatrix(&initmeans,kNum,x.row);

    double min[2];
    double max[2];

    /**
    �������ռ��������ʼ��k�������ģ�δд��
    **/
    for(j=0; j<x.row; j++)
    {
        min[j]=MAX;
        max[j]=MIN;
        for(i=0; i<x.col; i++)
        {
            if(x.mat[i][j]<min[j])
            {
                min[j]=x.mat[i][j];
            }
            if(x.mat[i][j]>max[j])
            {
                max[j]=x.mat[i][j];
            }
        }
    }
    /**
    �����������ѡ��k��������Ϊ������
    */
    for(k=0; k<kNum; k++)
    {
        for(j=0; j<x.row; j++)
        {
            //�ı��ֵ��ѡȡ���õ��ľ������Ļ᲻һ�£��ɼ���ֵ��ѡȡ�Ĺؼ���
            initmeans.mat[k][j]=x.mat[k][j];//min[j]+(max[j]-min[j])*(random(10)/10.0);
            cout<<initmeans.mat[k][j]<<"  ";
        }
        cout<<endl;
    }
    return initmeans;
}
CenAndDis kMeans(Matrix x,int kNum)
{
    int i,j,k,d;
    double dis[MAXK];///��¼һ��������k�������ĵľ��룬���Բ��þ���̬������ã�����Ĭ�����MAXK��������

    Matrix kmeans;//��������
    kmeans.initMatrix(&kmeans,kNum,x.row);

    kmeans=randCent(x,kNum);//�����ʼ��k��������

    Matrix xOne;//�洢һ������
    xOne.initMatrix(&xOne,1,x.row);

    Matrix kOne;//�洢һ��������������
    kOne.initMatrix(&kOne,1,x.row);

    Matrix kLabel;//�洢ÿ�����������
    kLabel.initMatrix(&kLabel,x.col,1);

    double minDis=MAX;//��С�ľ���
    int dnum=10;//������������Ϊ�����ٶȺܿ�

    for(d=0; d<dnum; d++)
    {
        //����k���������Ļ������ǩ
        for(i=0; i<x.col; i++)
        {
            xOne=xOne.getOneCol(x,i);
            minDis=MAX;
            for(k=0; k<kNum; k++)
            {
                kOne=kOne.getOneCol(kmeans,k);
                dis[k]=distances(xOne,kOne);
                if(dis[k]<minDis)
                {
                    kLabel.mat[i][0]=k;
                    minDis=dis[k];
                }
            }
        }
        //kmeans���㣬�����������Ĳ���+=����ʽ����Ҫ���㣬���������kmeansͬ���ǲ���+=����ʽ����Ϊ�ұ���һ�����kmeans
        for(k=0; k<kNum; k++)
        {
            for(j=0; j<x.row; j++)
            {
                kmeans.mat[k][j]=0;
            }
        }
        //����kmeans
        for(i=0; i<x.col; i++)
        {
            k=kLabel.mat[i][0];
            for(j=0; j<x.row; j++)
            {
                kmeans.mat[k][j]=(kmeans.mat[k][j]*(i)+x.mat[i][j])/(i+1);//�����ۼӷ�ʽ���ֵ���÷������Ӽ�����
                //��ȻҲ���԰�һ�����������ȡ�����������ȡ��ֵ������û����������������һ����һ��
            }
        }
        //�����ǰ��EM��ľ�������
        cout<<"--------------------"<<endl;
        for(k=0; k<kNum; k++)
        {
            for(j=0; j<x.row; j++)
            {
                cout<<kmeans.mat[k][j]<<"  ";
            }
            cout<<endl;
        }
    }
    /**
    �����������浽�ṹ����
    */
    CenAndDis cendis;
    cendis.cen.initMatrix(&cendis.cen,kNum,x.row);
    cendis.dis.initMatrix(&cendis.dis,x.col,1);
    cendis.cen.copy(kmeans,&cendis.cen);

    ///���������������������ĵľ��뵽�ṹ����
    for(i=0; i<x.col; i++)
    {
        k=kLabel.mat[i][0];
        xOne=xOne.getOneCol(x,i);
        kOne=kOne.getOneCol(kmeans,k);
        cendis.dis.mat[i][0]=distances(xOne,kOne);
    }
    //
    double sum=0;
    for(i=0;i<x.col;i++)
    {
        sum+=cendis.dis.mat[i][0];
    }
    cout<<"sum="<<sum<<endl;
    return cendis;
}


Matrix subMatrix(Matrix x,Matrix clusterAssment,int label)
{
    int i=0,j=0,k=0;
    Matrix submatrix;
    submatrix.initMatrix(&submatrix,x.col,x.row);
    for(i=0; i<x.col; i++)
    {
        if(clusterAssment.mat[i][0]==label)
        {
            for(j=0; j<x.row; j++)
            {
                submatrix.mat[k][j]=x.mat[i][j];
            }
            k++;
        }
    }
    submatrix.col=k;
    return submatrix;
}
/***
���־����˼����ÿһ�ζ�����һ�����͵Ķ�����࣬����ѡ����Ǹ�������ж��ֵ����۱�׼��SSE��
��������һ��ʹ��SSE��С�ͷ�����һ�࣬SSE����Сƽ��������ʹ�þ������ľ�����Сƽ���������С
�Ͷ���һ����ж��֣�ֱ�����ټ���SSE��������һ��Ϊ���࣬���ߵ���Ԥ��ľ�ΪK��

**/
int biKmeans(Matrix x,int kNum)
{
    int i,j,k,d;
    Matrix kmeans;
    kmeans.initMatrix(&kmeans,1,x.row);///��ʼ����Ϊһ�࣬

    Matrix xOne;//һ������
    xOne.initMatrix(&xOne,1,x.row);

    Matrix kOne;//һ����������
    kOne.initMatrix(&kOne,1,x.row);

    Matrix clusterAssment;///����ĵ�һ�б����䵽������𣬵ڶ��б����������������ľ���
    clusterAssment.initMatrix(&clusterAssment,x.col,2);

    CenAndDis cenanddis;///������
    cenanddis.cen.initMatrix(&cenanddis.cen,kNum,x.row);
    cenanddis.dis.initMatrix(&cenanddis.dis,x.col,1);

    CenAndDis bestCenanddis;///��¼��ǰ��õľ�����
    bestCenanddis.cen.initMatrix(&bestCenanddis.cen,kNum,x.row);
    bestCenanddis.dis.initMatrix(&bestCenanddis.dis,x.col,1);

    for(i=0; i<x.col; i++)
    {
        for(j=0; j<x.row; j++)
        {
            kmeans.mat[0][j]=(kmeans.mat[0][j]*i+x.mat[i][j])/(i+1);
        }
        clusterAssment.mat[i][0]=0;///��ʼ����Ϊһ�࣬������Ϊ֮ǰ��ʼ��Ϊһ���������
    }
    for(i=0; i<x.col; i++)
    {
        xOne=xOne.getOneCol(x,i);
        clusterAssment.mat[i][1]=distances(xOne,kmeans);///��ʼ��Ϊһ���������������������ľ���
    }

    Matrix submatrix;///���Ѻ���Ӽ�����
    submatrix.initMatrix(&submatrix,x.col,x.row);

    double lowestSSE=MAX;///��¼��ǰ��������SSEֵ
    double sseSplit=0;///��¼���Ѻ��Ӽ���SSEֵ
    double sseNosplit=0;///��¼û�з��ѵ��Լ���SSEֵ
    int bestCentToSplit;///��¼Ԥ�����Ӽ������
    double dis[2];///��¼���Ѻ������Ӽ���SSEֵ

    for(d=1; d<kNum; d++)
    {
        lowestSSE=MAX;//��������ʼ�������óɵ�ǰ�������µ��ܾ��룬��ô���ܱ�֤һ���õ�kNum��������
        for(k=0; k<d; k++)
        {
            cout<<"k="<<k<<endl;
            submatrix=subMatrix(x,clusterAssment,k);///���ڱ�������k����Ӽ���������kmeans���Խ��л���
            cout<<submatrix.col<<endl;
            submatrix.print(submatrix);
            cenanddis=kMeans(submatrix,2);///������kmeans���ж���
            sseSplit=0;
            sseNosplit=0;
            /**
            ��������SSEֵ��
            */
            for(i=0;i<submatrix.col;i++)
            {
                sseSplit+=cenanddis.dis.mat[i][1];
            }
            /**
            δ���ѵ�����SSEֵ��
            */
            for(i=0;i<x.col;i++)
            {
                if(clusterAssment.mat[i][0]!=k)
                {
                     sseNosplit+=clusterAssment.mat[i][1];
                }
            }
            ///����С��SSE���бȽϣ�ѡ�����ѵķ�����
            if(sseSplit+sseNosplit<lowestSSE)//�������k���ؽ��ж����ָ��ã����¼������������lowsetSSE
            {
                bestCentToSplit=k;
                bestCenanddis=cenanddis;
                lowestSSE=sseSplit+sseNosplit;
            }
        }
        ///���ȷ��ѡ����õĶ����ִ�֮�����濪ʼ��x�����Ͻ��и��¾������ĺ;��룬clusterAssment����
        for(i=0;i<x.col;i++)
        {
            if(clusterAssment.mat[i][0]==bestCentToSplit)
            {
                xOne=xOne.getOneCol(x,i);
                for(k=0;k<2;k++)
                {
                    kOne=kOne.getOneCol(bestCenanddis.cen,k);
                    dis[k]=distances(kOne,xOne);
                }
                if(dis[0]<dis[1])
                {
                    clusterAssment.mat[i][0]=bestCentToSplit;///���Ѻ�������࣬��һ���������ǵ�ǰ���ֵ
                    clusterAssment.mat[i][1]=dis[0];
                }
                else
                {
                    clusterAssment.mat[i][0]=bestCentToSplit+1;///���Ѻ�������࣬�ڶ������������һ���µ����ֵ��һֱ���ϼ�1
                    clusterAssment.mat[i][1]=dis[1];
                }
            }
        }
        ///�������յ�SSEֵ
        double sum=0;
        for(i=0;i<x.col;i++)
        {
            sum+=clusterAssment.mat[i][1];
            cout<<clusterAssment.mat[i][1]<<"   ";
        }
        cout<<"sum="<<sum<<endl;
    }
}

int main()
{
    dataToMatrix dtm;
    char file[20]="data\\kmeans.txt";
    dtm.loadData(&dtm,file);
    Matrix x;
    x.loadMatrix(&x,dtm);
    //x.print(x);
    //kMeans(x,3);
    biKmeans(x,3);
}

