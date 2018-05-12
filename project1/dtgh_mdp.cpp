/**
��̬�滮�������Ʒ���߹���

**/
/**

1 �򵥵ݹ�ʵ��

**/

/*
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stack>
#define max_vertex_num 20
#define error -1
#define ok 1
#define size 12
using namespace std;
typedef struct arccell
{
    int adj;//����Ĺ�ϵ��Ȩ
    char *info;
} arccell,adjmatrix[max_vertex_num][max_vertex_num];
typedef struct mgraph
{
    int vexs[max_vertex_num];//����������
    adjmatrix arcs;//�ڽӾ���
    int vexnum,arcnum;//ͼ��ǰ�Ķ������ͻ���
    double reward[max_vertex_num];//ÿ��״̬�Ļر�
} mgraph;
bool visited[max_vertex_num];
int matrix[size][size]=
{
    {0,1,0,0,1,0,0,0,0,0,0,0},
    {1,0,1,0,0,0,0,0,0,0,0,0},
    {0,1,0,1,0,0,1,0,0,0,0,0},
    {0,0,1,0,0,0,0,1,0,0,0,0},
    {1,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,1,0,0,1,0},
    {0,0,0,1,0,0,1,0,0,0,0,1},
    {0,0,0,0,1,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,1,0,1,0},
    {0,0,0,0,0,0,1,0,0,1,0,1},
    {0,0,0,0,0,0,0,1,0,0,1,0}
};
double reward[size]=
{
    -0.02,-0.02,-0.02,1,
    -0.02,0,-0.02,-1,
    -0.02,-0.02,-0.02,-0.02
};
double sumreward[size];
double maxreward[size]={0,0,0,0,0,0,0,0,0,0,0,0};
int count=0;
int createudn(mgraph &g);
void dfstraverse(mgraph &g);//��ȱ���ͼ
void dfs(mgraph &g,int i);
stack<int> s;//��ջ����Ϊȫ�ֱ�����ԭ��������������еݹ麯�������ڲ���
//��ÿһ�ζ�������һ��ջ��ÿһ�β���Ҳ���Ƕ�ͬһ��ջ���в���
stack<int>copys;
int main()
{
    mgraph g;
    createudn(g);
    int i=0;
    dfstraverse(g);//��ȱ����͹�ȱ�����ѡ��һ����Ϊ������ȫ�ֱ���
    for(i=0; i<size; i++)
        cout<<maxreward[i]<<"  ";
        cout<<"count="<<count<<endl;
    return 0;
}
int createudn(mgraph &g)
{
    int i,j,k;
    int x,y,w;
    int a[size]= {1,2,3,4,5,6,7,8,9,10,11,12}; //��������
    g.vexnum=size;
    for(i=0; i<g.vexnum; ++i)//��ʼ����������
    {
        g.vexs[i]=a[i];
    }
    cout<<endl;
    for(i=0; i<g.vexnum; i++)//��ʼ��û���κλ��ͻ�����Ϣ
    {
        for(j=0; j<g.vexnum; j++)
        {
            g.arcs[i][j].adj=matrix[i][j];
            g.arcs[i][j].info=NULL;

        }//��ʼ��������Ȩֵ�������Ϣ
        //Ĭ�ϳ�ʼ��ȨֵΪ-1����ϢΪNULL
        g.reward[i]=reward[i];
    }
    return ok;
}
void dfstraverse(mgraph &g)
{
    int i,j;
    for(i=0; i<g.vexnum; i++)
    {
        for(j=0; j<g.vexnum; j++)
            visited[j]=false;//����ÿ��״̬Ϊ��ʼ������ȱ���֮ǰ�����е�״̬������Ϊδ���ʵı�־
        while(!s.empty())
        {
            s.pop();
        }//��������һ��ʹ��ջʱ����ֹδ�ܰ�ջ��գ��������һ��
        dfs(g,i);
        cout<<"*****************************************************"<<endl;
    }
}
void dfs(mgraph &g,int i)
{
    int j,flag;
    int lujin[size];
    visited[i]=true;//���ʹ��󣬰ѷ��ʱ�־��Ϊtrue
    s.push(g.vexs[i]);
    copys=s;
    flag=0;
    for(j=0; j<g.vexnum; j++)
    {
        if(g.arcs[i][j].adj!=0&&!visited[j]&&g.reward[i]!=1&&g.reward[i]!=-1)//�ɵ�һ�δ�������iѡ��һ�����ڵĶ�������ȱ���
            //����һ�ν�������ѡ����һ�����ڵĶ�������ȱ�����ֱ�����������������Ķ��������ڵ�
            //�����ٷ���������������һ��ѭ��
        {
            flag=1;
            dfs(g,j);
            count++;//�ݹ鷵��һ��count++
            visited[s.top()-1]=false;//ֻ�ǰѷ��ʱ�־������Ϊfalse���������
            s.pop();
        }
    }
    j=0;
    if(flag==0)//���ĳ�εݹ����ú���ʱ��δִ�������for����ô���Ǳ�ʾ�ýڵ�û�������ɵ���Ľڵ��ˣ������ǵ����˽����ڵ�
    //��ʱ�����Խ���·����������Ҽ���ջ�����ڵ㾭����·�����ۻ��ر���ͳ�����еĿ���·�������ϵĸ��������ۻ��ر����Լ����ۻ��ر��µ�·��
    {
        while(!copys.empty())
        {
            lujin[j]=copys.top();
            copys.pop();
            j++;
        }
        int start=j-1;
        sumreward[lujin[start]-1]=0;
        for(j; j>0; j--)
        {
            sumreward[lujin[start]-1]+=g.reward[lujin[j-1]-1];
            cout<<lujin[j-1]<<"  ";
        }
        //��sumreward��ջ��Ԫ��Ϊ�µ����������¼ջ��Ԫ�ص��ۻ��ر�
        cout<<sumreward[lujin[start]-1]<<endl<<"-----------------------------------"<<endl;
        if(maxreward[lujin[start]-1]<sumreward[lujin[start]-1])//�����ۻ��ر�
            {
                maxreward[lujin[start]-1]=sumreward[lujin[start]-1];
            }
    }
}
*/

/***
2 ״̬Ϊ16�����

*/

/*
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stack>
#define max_vertex_num 20
#define error -1
#define ok 1
#define overflow -2
#define int_max 9999
#define size 16
using namespace std;
typedef struct arccell
{
    int adj;//����Ĺ�ϵ��Ȩ
    char *info;
} arccell,adjmatrix[max_vertex_num][max_vertex_num];
typedef struct mgraph
{
    int vexs[max_vertex_num];//����������
    adjmatrix arcs;//�ڽӾ���
    int vexnum,arcnum;//ͼ��ǰ�Ķ������ͻ���
    double reward[max_vertex_num];//ÿ��״̬�Ļر�
} mgraph;
bool visited[max_vertex_num];
int matrix[size][size]=
{
    {0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,0},
    {0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0},
    {0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0},
    {0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1},

    {0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0}

};
double reward[size]=
{
    -0.02,-0.02,-0.02,1,
    -0.02,0,-0.02,-1,
    -0.02,-0.02,-0.02,-0.02,
    -0.02,-0.02,-0.02,-0.02
};
double sumreward[size];
double maxreward[size]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int createudn(mgraph &g);
void dfstraverse(mgraph &g);//��ȱ���ͼ
void dfs(mgraph &g,int i);
stack<int> s;//��ջ����Ϊȫ�ֱ�����ԭ��������������еݹ麯�������ڲ���
//��ÿһ�ζ�������һ��ջ��ÿһ�β���Ҳ���Ƕ�ͬһ��ջ���в���
stack<int>copys;
int main()
{
    mgraph g;
    createudn(g);
    int i=0;
    dfstraverse(g);//��ȱ����͹�ȱ�����ѡ��һ����Ϊ������ȫ�ֱ���
    //bfstraverse(g);
    for(i=0; i<size; i++)
        cout<<maxreward[i]<<"  ";
    //int v0=0;//ѡ�񵽸���ĳ�ʼ��
    //shortpath_dij(g,v0);
    return 0;
}
int createudn(mgraph &g)
{
    int i,j,k;
    int x,y,w;
    int a[size]= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; //��������
    g.vexnum=size;
    for(i=0; i<g.vexnum; ++i)//��ʼ����������
    {
        g.vexs[i]=a[i];
        cout<<g.vexs[i]<<"  ";
    }
    cout<<endl;
    for(i=0; i<g.vexnum; i++)//��ʼ��û���κλ��ͻ�����Ϣ
    {
        for(j=0; j<g.vexnum; j++)
        {
            g.arcs[i][j].adj=matrix[i][j];
            g.arcs[i][j].info=NULL;

        }//��ʼ��������Ȩֵ�������Ϣ
        //Ĭ�ϳ�ʼ��ȨֵΪ-1����ϢΪNULL
        g.reward[i]=reward[i];
    }
    return ok;
}
void dfstraverse(mgraph &g)
{
    int i,j;
    for(i=0; i<g.vexnum; i++)
        visited[i]=false;
    for(i=0; i<g.vexnum; i++)
    {
        for(j=0; j<g.vexnum; j++)
            visited[j]=false;
        while(!s.empty())
        {
            s.pop();
        }
        dfs(g,i);
        cout<<"***************************************"<<endl;
    }
}
void dfs(mgraph &g,int i)
{
    int j,flag;
    int lujin[size];
    visited[i]=true;
    s.push(g.vexs[i]);
    copys=s;
    flag=0;
    for(j=0; j<g.vexnum; j++)
    {
        if(g.arcs[i][j].adj!=0&&!visited[j]&&g.reward[i]!=1&&g.reward[i]!=-1)//�ɵ�һ�δ�������iѡ��һ�����ڵĶ�������ȱ���
            //����һ�ν�������ѡ����һ�����ڵĶ�������ȱ�����ֱ�����������������Ķ��������ڵ�
            //�����ٷ���������������һ��ѭ��
        {
            flag=1;
            dfs(g,j);
            visited[s.top()-1]=false;
            s.pop();
        }
    }
    j=0;
    if(flag==0)
    {

        while(!copys.empty())
        {
            lujin[j]=copys.top();
            copys.pop();
            j++;
        }
        int start=j-1;
        sumreward[lujin[start]-1]=0;
        for(j; j>0; j--)
        {
            sumreward[lujin[start]-1]+=g.reward[lujin[j-1]-1];
            cout<<lujin[j-1]<<"  ";
        }
        cout<<sumreward[lujin[start]-1]<<endl<<"-----------------------------------"<<endl;
        if(maxreward[lujin[start]-1]<sumreward[lujin[start]-1])
            {
                maxreward[lujin[start]-1]=sumreward[lujin[start]-1];
                    //cout<<sumreward[lujin[start]-1]<<"&"<<maxreward[lujin[start]-1]<<endl;
            }
    }
}

*/

/***

3 �ݹ龫���

***/
/*

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stack>
#define max_vertex_num 20
#define error -1
#define ok 1
#define overflow -2
#define int_max 9999
#define size 12
using namespace std;
typedef struct arccell
{
    int adj;//����Ĺ�ϵ��Ȩ
    char *info;
} arccell,adjmatrix[max_vertex_num][max_vertex_num];
typedef struct mgraph
{
    int vexs[max_vertex_num];//����������
    adjmatrix arcs;//�ڽӾ���
    int vexnum,arcnum;//ͼ��ǰ�Ķ������ͻ���
    double reward[max_vertex_num];//ÿ��״̬�Ļر�
} mgraph;
bool visited[max_vertex_num];
int matrix[size][size]=
{
    {0,1,0,0,1,0,0,0,0,0,0,0},
    {1,0,1,0,0,0,0,0,0,0,0,0},
    {0,1,0,1,0,0,1,0,0,0,0,0},
    {0,0,1,0,0,0,0,1,0,0,0,0},
    {1,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,1,0,0,1,0},
    {0,0,0,1,0,0,1,0,0,0,0,1},
    {0,0,0,0,1,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,1,0,1,0},
    {0,0,0,0,0,0,1,0,0,1,0,1},
    {0,0,0,0,0,0,0,1,0,0,1,0}
};
double reward[size]=
{
    -0.02,-0.02,-0.02,1,
    -0.02,0,-0.02,-1,
    -0.02,-0.02,-0.02,-0.02
};
double sumreward[size];
double maxreward[size]= {0,0,0,0,0,0,0,0,0,0,0,0};
int count=0;
int createudn(mgraph &g);
void dfstraverse(mgraph &g);//��ȱ���ͼ
void dfs(mgraph &g,int i);
stack<int> s;//��ջ����Ϊȫ�ֱ�����ԭ��������������еݹ麯�������ڲ���
//��ÿһ�ζ�������һ��ջ��ÿһ�β���Ҳ���Ƕ�ͬһ��ջ���в���
stack<int>copys;
int maiwSum=0;n()
{
    mgraph g;
    createudn(g);
    int i=0;
    dfstraverse(g);//��ȱ����͹�ȱ�����ѡ��һ����Ϊ������ȫ�ֱ���
    for(i=0; i<size; i++)
        cout<<maxreward[i]<<"  ";
    cout<<"count="<<count<<endl;
    return 0;
}
int createudn(mgraph &g)
{
    int i,j,k;
    int x,y,w;
    int a[size]= {1,2,3,4,5,6,7,8,9,10,11,12}; //��������
    g.vexnum=size;
    for(i=0; i<g.vexnum; ++i)//��ʼ����������
    {
        g.vexs[i]=a[i];
    }
    for(i=0; i<g.vexnum; i++)//��ʼ��û���κλ��ͻ�����Ϣ
    {
        for(j=0; j<g.vexnum; j++)
        {
            g.arcs[i][j].adj=matrix[i][j];
            g.arcs[i][j].info=NULL;

        }//��ʼ��������Ȩֵ�������Ϣ
        //Ĭ�ϳ�ʼ��ȨֵΪ-1����ϢΪNULL
        g.reward[i]=reward[i];
    }
    return ok;
}
int printf(mgraph &g)
{
    int i=0,j=0;
    for(i=0; i<g.vexnum; i++)
    {
        for(j=0; j<g.vexnum; j++)
            cout<<g.arcs[i][j].adj<<"  ";
        cout<<endl;
    }
    return 0;
}
void dfstraverse(mgraph &g)
{
    int i,j;
    for(i=0; i<g.vexnum; i++)
        visited[i]=false;
    for(i=0; i<g.vexnum; i++)
    {
        if(maxreward[i]>0+0.001)
            continue;
        for(j=0; j<g.vexnum; j++)
            visited[j]=false;
        while(!s.empty())
        {
            s.pop();
        }
        dfs(g,i);
        cout<<endl<<"***************************************"<<endl;
    }
}
void dfs(mgraph &g,int i)
{
    int j,k,flag=0;
    int lujin[size];
    visited[i]=true;
    s.push(g.vexs[i]);
    copys=s;
    for(j=0; j<g.vexnum; j++)
    {
        if(g.arcs[i][j].adj!=0&&!visited[j]&&g.reward[i]!=1&&g.reward[i]!=-1)//�ɵ�һ�δ�������iѡ��һ�����ڵĶ�������ȱ���
            //����һ�ν�������ѡ����һ�����ڵĶ�������ȱ�����ֱ�����������������Ķ��������ڵ�
            //�����ٷ���������������һ��ѭ��
        {
            flag=1;
            dfs(g,j);
            count++;
            visited[s.top()-1]=false;
            s.pop();
        }
    }
    j=0;
    if(flag==0)//ͬ�ϣ�ֱ���ݹ鵽ĳһ���ڵ��߲���ȥ�ˣ���ô��·���͵����յ㣬���������·�����ۻ��ر�
    {
        while(!copys.empty())
        {
            lujin[j]=copys.top();
            copys.pop();
            j++;
        }
        int start=j-1;
        int l=0;
        sumreward[lujin[start]-1]=0;
        for(i=0; i<size; i++)
            sumreward[i]=0;
        for(k=0; k<=start; k++)
        {
            for(j=start-k; j>=0; j--)
            {
                sumreward[lujin[start-k]-1]+=g.reward[lujin[j]-1];//��¼�ۻ��ر�
                //cout<<lujin[j]<<"  ";
            }
            //cout<<sumreward[lujin[k]-1]<<endl<<"-----------------------------------"<<endl;
        }
        if(maxreward[lujin[start]-1]<sumreward[lujin[start]-1])//�����ۻ��ر������Ұ�����·���ϣ���ÿһ��״̬Ϊ��ʼ״̬���ۻ��ر���������������ջ��Ԫ��Ϊ���ŵĻ�
        //��ô����·�������еĽڵ㶼������·��Ϊ���ߴﵽ�����ۻ��ر�
        {
            //��¼����·��������״̬Ϊ��ʼ״̬�߸�·�����ۻ��ر�
            for(l=0; l<=start; l++)
            {
                maxreward[lujin[start-l]-1]=sumreward[lujin[start-l]-1];
                //cout<<maxreward[lujin[start-l]-1]<<"   ";
            }
        }
        for(k=0; k<=start; k++)
        {
            for(j=start-k; j>=0; j--)
            {
                //sumreward[lujin[start-k]-1]+=g.reward[lujin[j]-1];
                cout<<lujin[j]<<"  ";
            }
            cout<<maxreward[lujin[start-k]-1]<<endl;
        }
    }
}

*/

/**

4 �ǵݹ鶯̬�滮

������״̬���������ù�ȱ������ϵĸ�������һ״̬���ۻ��ر�
*/

/*

#include <iostream>
using namespace std;
#include <stdlib.h>
#define size 12
typedef struct subset
{
    int data;
    struct subset* next;
} subset;
int main()
{
    int matrix[size][size]=
    {
        {0,1,0,0,1,0,0,0,0,0,0,0},
        {1,0,1,0,0,0,0,0,0,0,0,0},
        {0,1,0,1,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,1,0,0,0,0,1,0,0,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,1,0,0},
        {0,0,0,0,0,0,0,0,1,0,1,0},
        {0,0,0,0,0,0,1,0,0,1,0,1},
        {0,0,0,0,0,0,0,1,0,0,1,0}
    };
    double reward[size]=
    {
        -0.02,-0.02,-0.02,1,
        -0.02,0,-0.02,-1,
        -0.02,-0.02,-0.02,-0.02
    };
    double maxreward[size]= {0,0,0,0,0,0,0,0,0,0,0,0};
    int i=0,j=0,count=0;
    struct subset *maxsubset;
    maxsubset=(subset *)malloc(sizeof(subset)*1);
    maxsubset->data=-1;
    maxsubset->next=NULL;
    struct subset *p,*q;
    for(i=0; i<size; i++)//��ʼ��0�������ر�Ϊ�ۻ��ر�
    {
        maxreward[i]=reward[i];
        if(reward[i]==1)
        {
            p=(subset *)malloc(sizeof(subset)*1);
            p->data=i;
            p->next=NULL;
            q=maxsubset;
            while((q->next)!=NULL)
                q=q->next;
            q->next=p;
        }
    }
    q=maxsubset->next;
    while(q!=NULL)//����ͼ�Ĺ�ȱ���û���õ����У���Ҳ�õ��˶��е�˼��
    //���ڵ�ǰ���ܵ���Ľڵ�����������������Ȼ���𽥽�����һ�����ܵ���Ľڵ��������������У���ͬ����һ���Ƚ��ȳ�˼��
    {
        for(i=0; i<size; i++) //���ڲ��ǲ��Ե�����ֻ�ܱ������е�״̬���ҳ��ܵ��ģ��Ҹ��ŵ�
        {
            if(matrix[i][q->data]==1&&maxreward[i]<0)//double���ͱȽϴ�С��ƫ�����һ��С����Ϊ����
            {
                maxreward[i]=reward[i]+maxreward[q->data];
                p=(subset *)malloc(sizeof(subset)*1);
                p->data=i;
                p->next=NULL;
                q=maxsubset;
                while((q->next)!=NULL)
                    q=q->next;
                q->next=p;
            }
            count++;
        }
        maxsubset->next=maxsubset->next->next;//ɾ����ǰ�ڵ㣬����ǰ�����ܵ���Ľڵ㶼�Ѿ������ˣ��ɳ�������
        q=maxsubset->next;//
    }
    for(i=0; i<size; i++)
        cout<<maxreward[i]<<"  ";
    cout<<endl<<"count="<<count<<endl;
    return 0;
}

*/
