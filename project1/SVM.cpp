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
//SMO�����ṹ��
struct OS
{
    Matrix x;
    Matrix y;
    double C;
    double soft;
    int m;
    Matrix alphas;
    double b;
    Matrix eCache;
    Matrix kernel;
    bool k;
};
//�˺����Ľṹ��
struct kTup
{
    int type;//0,1
    double arg;
};
class SMOP
{
    //�ǳ�ֵ��ע�����svm��ѵ�������������У���ÿһ����һ�����������Ե���w��������
public:
    OS os;
public:
    /**
    ���ݲ����������ɲ�ͬ�ĺ˺���
    */
    Matrix kernelTran(Matrix x,Matrix xOneCol,kTup ktup)
    {
        Matrix K;
        K.initMatrix(&K,x.col,1);
        Matrix xOneColT;
        xOneColT.initMatrix(&xOneColT,xOneCol.row,xOneCol.col);
        xOneColT.transposematrix(xOneCol,&xOneColT);
        if(ktup.type==1)
        {
            K.multsmatrix(&K,x,xOneColT);
        }
        if(ktup.type==2)
        {
            //��˹��
        }
        return K;
    }
    /**
    �ṹ��OS�ĳ�ʼ�������ڱ�������SMO�㷨����Ҫ�õ��Ĳ���
    */
    int initOs(Matrix x,Matrix y,double C,double soft,kTup ktup)
    {
        os.x.initMatrix(&os.x,x.col,x.row);
        os.x.copy(x,&os.x);
        os.y.initMatrix(&os.y,y.col,y.row);
        os.y.copy(y,&os.y);
        os.eCache.initMatrix(&os.eCache,x.col,2);
        os.alphas.initMatrix(&os.alphas,x.col,1);

        os.b=0;
        os.C=C;
        os.m=x.col;
        os.kernel.initMatrix(&os.kernel,os.m,os.m);
        os.soft=soft;
        if(ktup.type!=0)
        {
            int i=0,j=0;
            Matrix xOneCol;
            xOneCol.initMatrix(&xOneCol,1,os.x.row);
            Matrix kOneRow;
            kOneRow.initMatrix(&kOneRow,os.m,1);
            cout<<"-----------"<<endl;
            for(i=0; i<os.m; i++)
            {
                xOneCol=xOneCol.getOneCol(x,i);
                kOneRow=kernelTran(x,xOneCol,ktup);
                for(j=0; j<os.m; j++)
                    os.kernel.mat[j][i]=kOneRow.mat[j][0];
            }
            os.k=true;
            return 0;
        }
        os.k=false;
        return 0;
    }
    /**
    �����޼�������
    */
    double clipAlpha(double alpha,double L,double H)
    {
        if(alpha>H)
            return H;
        if(alpha<L)
            return L;
        return alpha;
    }
    /**
    ���ֵ�ļ���Ҳ�Ǹ�����ѧ���ʽ��ʵ�ֵ�

    **/
    double calcEk(int k)
    {
        Matrix ay;
        ay.initMatrix(&ay,os.x.col,1);
        int i;
        for(i=0; i<os.m; i++)
        {
            ay.mat[i][0]=os.alphas.mat[i][0]*os.y.mat[i][0];
        }
        Matrix ayT;
        ayT.initMatrix(&ayT,ay.row,ay.col);
        ayT.transposematrix(ay,&ayT);
        Matrix f;
        f.initMatrix(&f,1,1);
        if(!os.k)
        {
            Matrix ayTx;
            ayTx.initMatrix(&ayTx,ayT.col,os.x.row);
            ayTx.multsmatrix(&ayTx,ayT,os.x);

            Matrix xOneCol;
            xOneCol.initMatrix(&xOneCol,1,os.x.row);
            xOneCol=xOneCol.getOneCol(os.x,k);

            Matrix xOneColT;
            xOneColT.initMatrix(&xOneColT,xOneCol.row,xOneCol.col);
            xOneColT.transposematrix(xOneCol,&xOneColT);
            f.multsmatrix(&f,ayTx,xOneColT);//ֵ����ȶ
        }
        else
        {
            Matrix kOneRow;
            kOneRow.initMatrix(&kOneRow,os.m,1);
            kOneRow=kOneRow.getOneRow(os.kernel,k);
            f.multsmatrix(&f,ayT,kOneRow);//ֵ����ȶ
        }
        double fXK=f.mat[0][0]+os.b-os.y.mat[k][0];
        return fXK;
    }
    ///����������
    void updataEk(int k)
    {
        double Ek;
        Ek=calcEk(k);//�������ֵ
        os.eCache.mat[k][0]=1;
        os.eCache.mat[k][1]=Ek;
    }
    /**
    ���ڵڶ���alpha��ѡ�����Ei-Ej�Ĵ�С���к�����ͬʱ��������һ�����������治���E�����ټ�����
    ѡ������ʵڶ���alpha2�����ڵ�һ�ν������������ֵE�ľ����Ȼû��һ����Ч�ģ����Բ����������
    ���ѡ�������ʵ��������ѡ�񣬾���û��ʵ�֣����ǲ��õȼ۷�ʽʵ�֣�����Ҫע������Խ������
    **/
    int selectJ(int i,double Ei)
    {
        int maxK=-1;
        double Ek;
        double Ej;
        double deltaE;
        double maxDeltaE=0;
        int k;
        for(k=0; k<os.alphas.col; k++)
        {
            if(os.eCache.mat[k][0]==1&&k!=i)
            {
                Ek=calcEk(k);
                deltaE=abs(Ei-Ek);
                if(deltaE>maxDeltaE)
                {
                    maxK=k;
                    maxDeltaE=deltaE;
                    Ej=Ek;
                }
            }
            //���ѡ��һ��j, ������ֻ�е�һ��ʱ���е�ek��û�������������ѡһ������
            if(maxK==-1)
            {
                maxK=(i*2+1)%100;
                Ej=calcEk(maxK);
            }
        }
        return maxK;
    }

    /**
    �ڲ�ѭ��ʵ������alpha���Ż��������ѭ�������ṩalpha1�Ŀ�ѡ���ϣ���������������֧����������
    ���������ϵ�ÿһ��alpha����������ж��Ƿ�Υ��KKT���������Υ����ѡΪ��һ��alpha1��
    ͬ������selctJ����������ʽѡ��ʹ��alpha1�ı�����alpha2��
    ѡ�������ij֮�󣬻��ø��ݵ�ǰalpha1��alpha2��ȷ���Ż��������ޡ�
    Ȼ��ͨ��������ʽ��������alpha1��alpha2�������ʱ���ж��Ƿ���ú˷�����
    ���Լ��������������޼���������ȷ���Ż����alpha1��alpha2��ͬʱ�Ե�ǰ�Ż�������bҲ�����޸�
    ���ǵ�ÿһ��ѡ��ڶ���alpha2ʱ������Ҫ�Ƚ�һ�����ֵ����������ֵ��ÿ���Ż������ж�ֻ�ж�Ӧ�޸�alpha�����ֵ�б仯
    ���������ǲ���ģ�������һ��������������Ч�����ֵ��
    */
    int innerL(int i)
    {
        double Ei;
        double Ej;
        int j;
        double alphaIOld;
        double alphaJOld;
        double L;
        double H;
        double eta;
        int n;
        double temp[3];//ii��jj��ij
        double b1,b2;
        if(os.y.mat[i][0]*(Ei-os.y.mat[i][0]*os.soft)<0&&os.alphas.mat[i][0]<os.C||os.y.mat[i][0]*(Ei-os.y.mat[i][0]*os.soft)>0&&os.alphas.mat[i][0]>0)
        {
            Ei=calcEk(i);
            j=selectJ(i,Ei);
            Ej=calcEk(j);
            alphaIOld=os.alphas.mat[i][0];
            alphaJOld=os.alphas.mat[j][0];
            if(os.y.mat[i][0]!=os.y.mat[j][0])
            {
                L=max(0.0,os.alphas.mat[j][0]-os.alphas.mat[i][0]);
                H=min(os.C,os.alphas.mat[j][0]-os.alphas.mat[i][0]+os.C);
            }
            else
            {
                L=max(0.0,os.alphas.mat[j][0]+os.alphas.mat[i][0]-os.C);
                H=min(os.C,os.alphas.mat[j][0]+os.alphas.mat[i][0]);
            }
            if(L==H)
            {
                cout<<"l=h------------------"<<endl;
                return 0;
            }
            if(!os.k)
            {
                temp[0]=0;
                temp[1]=0;
                temp[2]=0;
                for(n=0; n<os.x.row; n++)
                {
                    temp[0]+=pow(os.x.mat[i][n],2);
                    temp[1]+=pow(os.x.mat[j][n],2);
                    temp[2]+=os.x.mat[i][n]*os.x.mat[j][n];
                }
            }
            else
            {
                temp[0]=os.kernel.mat[i][i];
                temp[1]=os.kernel.mat[j][j];
                temp[2]=os.kernel.mat[i][j];
                eta=temp[0]+temp[1]-2*temp[2];
            }
            eta=temp[0]+temp[1]-2*temp[2];
            if(eta<0)
            {
                cout<<"eta<0-----------------"<<endl;
                return 0;
            }
            os.alphas.mat[j][0]+=os.y.mat[j][0]*(Ei-Ej)/eta;
            os.alphas.mat[j][0]=clipAlpha(os.alphas.mat[j][0],L,H);//��alpha���м���
            updataEk(j);//�������ֵ
            if(fabs(os.alphas.mat[j][0]-alphaJOld)<0.000001)
            {
                cout<<"j not moving enough---------------"<<endl;
                return 0;
            }
            os.alphas.mat[i][0]+=os.y.mat[i][0]*os.y.mat[j][0]*(alphaJOld-os.alphas.mat[j][0]);
            updataEk(i);//�������ֵ

            b1=os.b-Ei-os.y.mat[i][0]*(os.alphas.mat[i][0]-alphaIOld)*temp[0]-os.alphas.mat[j][0]*(os.alphas.mat[j][0]-alphaJOld)*temp[2];
            b2=os.b-Ej-os.y.mat[i][0]*(os.alphas.mat[i][0]-alphaIOld)*temp[2]-os.alphas.mat[j][0]*(os.alphas.mat[j][0]-alphaJOld)*temp[1];
            if(0<os.alphas.mat[i][0]&&os.C>os.alphas.mat[i][0])
                os.b=b1;
            else if(0<os.alphas.mat[j][0]&&os.C>os.alphas.mat[j][0])
                os.b=b2;
            else
                os.b=(b1+b2)/2;
            return 1;
        }
        cout<<"kkt--------------------------"<<endl;
        return 0;
    }

    /**

    SMO�㷨����ں���������Ҫ�����ǳ�ʼ��SMO���еĲ������ṹ��OS
    ȷ������������־����������������֧��������ѭ����ѡ�����alpha1������innerȷ��alpha1��
    ���ѭ��ÿһ���ǵ����Ż�һ��alpha,�ڲ�ѭ����������ʵ�ֵ����Ż�һ��alpha
    ��󣬶�smo�㷨��ʵ�ֽ��м�飬ͨ��alpha����w��b������ѵ�������ϱȽ���Ԥ��ֵ��ʵ��ֵ�Ĳ��졣
    �ð취ֻ�ܼ��SMO�㷨ʵ�ֵ���ȷ�ԣ����ܼ��SVM�����ܡ�
    */

    int smoP(Matrix x,Matrix y,double C,double soft,int maxIter,kTup ktup)
    {
        int iter=0;
        int i,j;
        initOs(x,y,C,soft,ktup);///��ʼ��OS�ṹ�壬OS�ṹ���ж�����SMOP�㷨��Ҫ�Ĵ󲿷ֲ���
        bool entireSet=true;//��־�����жϵ�ǰ�������������alpha���������0-C֮��Ĳ���alpha��������ʵ��һ��alpha������ʽѡ�񣬼�ѡ������֧�������ϵĵ�
        int alphaPairsChanged=0;//�ò����ж���ȷ����һ������֮���Ƿ���ѡ�������Ҫ��ĵڶ�alpha������ֵΪ1��ʾ�ܣ�0Ϊ����
        for(iter=0; iter<maxIter&&(alphaPairsChanged>0||entireSet); iter++)
        {//ѭ��������־Ϊ���������ѵ�Ԥ��ֵ�������ǲ����ټ����Ż����������е�֧�������ϵĵ㶼�Ҳ����ڶ���alpha�Ե�һ��alpha�����Ż��������ٱ������еĵ�Ѱ�ҿ��Ż��Ĳ����ԣ�
            //�����Ҳ������ٴα���֧�������ϵĵ㣬��α�ȻҲ���Ҳ������Ž�������
            alphaPairsChanged=0;
            if(entireSet)
            {
                for(i=0; i<os.m; i++)
                {
                    alphaPairsChanged+=innerL(i);
                    cout<<"iter="<<iter<<"  i="<<i<<"  alphachanged="<<alphaPairsChanged<<endl;
                }
                iter++;
            }
            else
            {
                for(i=0; i<os.m; i++)
                {
                    if(os.alphas.mat[i][0]>0&&os.alphas.mat[i][0]<os.C)//ֻѡ��֧�������ϵĵ�
                        continue;
                    alphaPairsChanged+=innerL(i);
                    cout<<"iter="<<iter<<"  i="<<i<<"  alphachanged="<<alphaPairsChanged<<endl;
                }
                iter++;
            }
            if(entireSet)
                entireSet=false;
            else if(alphaPairsChanged==0)
                entireSet=true;
        }
        ///��SMO�㷨ʵ�ֵ���ȷ�Խ�����֤�����Ԥ��ֵ��ʵ��ֵ�Ĳ��ȫΪ0��ʾ��ѵ��������Ԥ��ȫ��
        Matrix ay;
        ay.initMatrix(&ay,os.x.col,1);
        for(i=0; i<os.m; i++)
        {
            ay.mat[i][0]=os.alphas.mat[i][0]*os.y.mat[i][0];
        }

        Matrix xT;
        xT.initMatrix(&xT,os.x.row,os.x.col);
        xT.transposematrix(x,&xT);

        Matrix w;
        w.initMatrix(&w,os.x.row,1);
        w.multsmatrix(&w,xT,ay);

        Matrix label;
        label.initMatrix(&label,os.x.col,1);
        label.multsmatrix(&label,os.x,w);
        cout<<os.b<<"  ";
        for(i=0; i<os.x.row; i++)
        {
            cout<<w.mat[i][0]<<"  ";
        }
        cout<<endl;
        ///��֤ѵ���������ݣ���֤SVMʵ�ֵ���ȷ��
        for(i=0; i<os.m; i++)
        {
            if((label.mat[i][0]+os.b)>0)
                cout<<1-os.y.mat[i][0]<<endl;
            else
                cout<<-1-os.y.mat[i][0]<<endl;
        }
        return 0;
    }

};
int main()
{
    ///���������ļ����浽����dtm�ľ���Ԫ����
    dataToMatrix dtm;
    char file[20]="data\\svm.txt";
    dtm.loadData(&dtm,file);

    ///ͨ����������е�load������ʼ��������������������x,y
    Matrix x;
    x.loadMatrix(&x,dtm);
    Matrix y;
    y.initMatrix(&y,x.col,1);
    y=y.getOneRow(x,x.row);
    x.deleteOneRow(&x,x.row);

    ///����SMO�������SVMģ��
    cout<<"SVM"<<endl;
    SMOP smop;
    kTup ktup;//�˺����Ķ��壬����typeԪ��Ϊ0��ʾ�����ú˺�������0�ֱ��Ӧ��ͬ�ĺ˺���
    ktup.type=1;
    ktup.arg=1.0;
    smop.smoP(x,y,0.6,0.001,40,ktup);//
    return 0;
}
