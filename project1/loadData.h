#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <fstream>
#include <sstream>
#include <stack>
#include <string>
using namespace std;
#define MAX_SIZE_OF_TRAINING_SET 1000
#define ATTR_NUM 1000


struct Data
{
    //�����ù���������
    int id;
    double attr_double[ATTR_NUM];//������ֵ������
    string attr_string[ATTR_NUM];//������ֵ������
    double weight;
    Data *next;
};
class dataToMatrix
{
public:
    Data *dataSet;
    int col;
    int row;
public:

    /**
    ������ֵ����������
    */
    int loadData(dataToMatrix *dtm,char *file)
    {
        int i=0,j=0;
        ifstream infile;
        string tmpstrline;
        Data *p;
        dtm->dataSet=new Data;
        dtm->dataSet->next=NULL;
        p=dtm->dataSet;
        Data *datatmp;
        dtm->col=0;
        cout<<file<<endl;
        infile.open(file,ios::in);
        while(!infile.eof()&&i<MAX_SIZE_OF_TRAINING_SET)
        {
            getline(infile,tmpstrline,'\n');//��ȡ�ļ���һ�е����ݣ�����Ϊstring����
            stringstream input(tmpstrline);
            if(tmpstrline!="\0")////���ڶ�ȡ�ļ�������ͬ��������ò���
            {
                datatmp=new Data;
                datatmp->id=i;
                datatmp->next=NULL;
                j=0;
                while(input>>datatmp->attr_double[j])j++;
                p->next=datatmp;
                p=p->next;
                dtm->col++;
            }
        }
        dtm->row=j;
        infile.close();
        return 0;
    }
    /**
    �����ַ�����������
    */
    int loadData(dataToMatrix *dtm,char *file,int type)
    {
        int i=0,j=0;
        ifstream infile;
        string tmpstrline;
        Data *p;
        dtm->dataSet=new Data;
        dtm->dataSet->next=NULL;
        p=dtm->dataSet;
        Data *datatmp;
        dtm->col=1;
        cout<<file<<endl;
        infile.open(file,ios::in);
        datatmp=new Data;
        datatmp->next=NULL;
        while(!infile.eof()&&i<MAX_SIZE_OF_TRAINING_SET)
        {
            getline(infile,tmpstrline,'\n');//��ȡ�ļ���һ�е����ݣ�����Ϊstring����
            stringstream input(tmpstrline);
            if(tmpstrline!="\0")////���ڶ�ȡ�ļ�������ͬ��������ò���
            {
                while(input>>datatmp->attr_string[j]){j++;};
            }
        }
        p->next=datatmp;
        dtm->row=j;
        infile.close();
        return 0;
    }
    int print(dataToMatrix dtm)
    {
        //������ݼ����Ƿ���ȷ
        int i,j;
        Data *p=dtm.dataSet->next;
        for(i=0; i<dtm.col&&p!=NULL; i++)
        {
            for(j=0; j<dtm.row; j++)
            {
                if(p->attr_string[j]!="")
                    cout<<p->attr_string[j]<<"  ";
                else
                    cout<<p->attr_double[j]<<"  ";
            }
            p=p->next;
            cout<<endl;
        }
        return i;
    }
};
