#include <iostream>
#include <time.h>
#include<string>
#include<fstream>
using namespace std;

class Ds {

public:
    int **graph=nullptr;
    int *colored=nullptr;
    int *degree=nullptr;
    int *saturation=nullptr;
    int isColor[500] = {0};
    int colors=1;
    int n=0;

    void DegreeMinusOne(int Node) const;
    void SaturationPlusOne(int Node) const;
    void setColor(int Node);
    int MaxSaturationNode();
};



int main()
{ 
	//读文件 
	ifstream ifs;
    ifs.open("../DSJC500.5.col");
    string str;
    ifs>>str;
    Ds *dsatur = new Ds;
    while(!ifs.eof())
    {
		if(str=="edge")
		//获取点的数量完成图数组、颜色数组、度数组及饱和度数组的初始化 
        {
            ifs>>dsatur->n;
            dsatur->colored= (int*)malloc(dsatur->n*sizeof(int*));
            dsatur->degree= (int*)malloc(dsatur->n*sizeof(int*));
            dsatur->saturation= (int*)malloc(dsatur->n*sizeof(int*));
            dsatur->graph = (int**)malloc(dsatur->n*sizeof(int*));
	        for(int i=0;i<dsatur->n;i++)
		    {
                dsatur->graph[i] = (int*)malloc(dsatur->n*sizeof(int));
		        for(int j=0;j<dsatur->n;j++)
		        {
                    dsatur->graph[i][j]=0;
        		}
                dsatur->colored[i]=0;
                dsatur->degree[i]=0;
                dsatur->saturation[i]=0;
	        }
        }
		//根据所给数据初始化图数组
        if(str=="e")
        {
			int m,n;
            ifs>>m>>n;
            dsatur->graph[m-1][n-1]=1;
            dsatur->graph[n-1][m-1]=1;
        }
        ifs>>str;
    }
    ifs.close();
    //初始化度数组
	for(int i=0;i<dsatur->n;i++)
    {
        int idegree=0;
		for(int j=0;j<dsatur->n;j++)
        {
		    if(dsatur->graph[i][j]!=0)
                idegree++;
		}
        dsatur->degree[i]=idegree;
    } 

 
	clock_t start, end;
	//开始计时 
    start = clock();
    
    //选定初始节点即度最高的节点 
    int Initialnode;
    int max=0; int Node=0;
    for(int i=0;i<dsatur->n;i++)
    {
        int nd=dsatur->degree[i];
        if(nd>max)
        {
            max=nd;
            Node=i;
        }
    }
	Initialnode=Node;
	//给初始节点指定第一个颜色 
    dsatur->setColor(Initialnode);
    //printf("NODE %3d     COLOR %2d\n",Initialnode,dsatur->colored[Initialnode]);
    dsatur->DegreeMinusOne(Node);
    dsatur->SaturationPlusOne(Node);
    
    int Dyed=1;
    //迭代直到每个节点都有颜色 
    while (Dyed<dsatur->n)
	{
    	//选取未被染色节点中饱和度最大的节点
		//若出现多个节点则选其中度最大的节点
		int MaxNode=0;

        MaxNode=dsatur->MaxSaturationNode();
        
        //为上述操作最终得到的节点染色 
        dsatur->setColor(MaxNode);
        //printf("NODE %3d     COLOR %2d\n",MaxNode,dsatur->colored[MaxNode]);
        dsatur->DegreeMinusOne(Node);
        dsatur->SaturationPlusOne(Node);
        Dyed++;
        /*printf("%d",dsatur->Dyed);
        printf("%s",dsatur->saturation);*/
	}
	end = clock();
    printf("color is %d\n",dsatur->colors);
	double sec = (double)(end-start) / CLOCKS_PER_SEC;
	printf("\ntime is %lf s\n",sec);
	free(dsatur->graph);
	free(dsatur->colored);
	delete dsatur;
	return 0;
}

void Ds::DegreeMinusOne(int Node) const
{
	for(int i=0;i<this->n;i++)
    {
        if(this->graph[Node][i]==1)
            degree[i]=degree[i]-1;
    }
}

void Ds::SaturationPlusOne(int Node) const
{
	for(int i=0;i<this->n;i++)
    {
        if(this->graph[Node][i]==1)
            this->saturation[i]=this->saturation[i]+1;
    }
} 

void Ds::setColor(int Node)
{
    bool flag=true;
    int colorNode=1;
    while(flag)
    {
        int count=0;
        for(int i=0;i<this->n;i++)
        {
            if(this->graph[Node][i]==1 && this->colored[i]==colorNode)
                count++;
        }
        if(count==0)
        {
            flag=false;
        }
        else
        {
            colorNode++;
        }
    }
    this->colored[Node]=colorNode;
    if(colorNode>this->colors)
        this->colors=colorNode;
    this->isColor[Node] = 1;
}

int Ds::MaxSaturationNode()
{
    int maxSat=0; int Node=0;
    for(int i=0;i<500;i++){
        if(this->isColor[i]==0){
            Node = i;
            break;
        }
    }
    for(int i=0;i<this->n;i++)
    {
         if(this->colored[i]==0)
        {
            int nd=this->saturation[i];
            if(nd > maxSat)
            {
                maxSat=nd;
                Node=i;
            }
            else if(nd==maxSat)
            {
                if(this->degree[i]>this->degree[Node])
                Node=i;
            }
        }
    }
    return Node;
}
