// Tabu08.cpp: 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include<iostream>
#include<time.h>
#include<string>
#include<fstream>
#include<array>
#include<stdlib.h>

using namespace std;
//int N = 500
int N, K;
int* sol; //保存每个点的颜色
// array<int,N> sol;
int** adj_color_table; //邻域颜色表
//array<array<int,500>,49> test;
int** tabutenure;//禁忌表
//array<array<int,500>,49>tabutenure;
int f, best_f;//冲突数
int **NbID; //顶点数*(顶点数-1)，保存邻点有哪些
int* num_adj; //保存邻点个数

bool** Adj;  //相邻则为true
//array<array<bool,500>,49>Adj;
int** equ_delt;//保存相等的解
//array<array<int,500>,49>equ_delt;

void Initialize(string fileName)
{
	//定义顶点是否相连的数组

	ifstream ifs;
	ifs.open(fileName);
	//char strRead[100];
	string str;
    if(!ifs.is_open()){
        cout<<"failed"<<endl;
        return ;
    }
	//ifs >> strRead;
	while (!ifs.eof())
	{
		if (str == "edge")
		{
			ifs >> N;
			////v = new Vertex[nVertex]; //初始化nV个顶点

			Adj = new bool*[N];
			for (int i = 0; i < N; i++)
			{
				Adj[i] = new bool[N];
			}
			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < N; j++)
					Adj[i][j] = false;
			}

		}
		if (str == "e")
		{
			int m, n;
			//ifs >> n >> vNext;
			//v[n].adjV.push_back(vNext);	
			ifs >> m >> n;
			Adj[m - 1][n - 1] = true;//对称形成邻接矩阵
			Adj[n - 1][m - 1] = true;
		}
		ifs >> str;
		//cout<<++tmp<<endl;
	}

	cout<<"file ready"<<endl;
	ifs.close();

	/*for(int i=0;i<N;i++){ //检查邻接矩阵是否正确
	    for(int j=0;j<N;j++){
	        cout<<Adj[i][j];
	    }
	    cout<<endl;
	}*/

	//开辟equ_delt,最多有N*(K-1)个解.即每个顶点可以产生K-1个delt
	//equ_delt[i][0]存第i个解的顶点，equ_delt[i][2]存第i个解的newcolor
	equ_delt = new int*[N*(K - 1)];
	for (int i = 0; i < N*(K - 1); i++) equ_delt[i] = new int[2];


	sol = new int[N];//每个顶点的颜色
	srand(time(NULL));
	for (int i = 0; i < N; i++)
	{
		sol[i] = rand() % K;  //给每个点随机着色
	}

	///////////开辟adjcolortable数组
	adj_color_table = new int*[N];
	for (int i = 0; i < N; i++)
	{
		adj_color_table[i] = new int[K];
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < K; j++) adj_color_table[i][j] = 0;//
	}
	//初始化adj_color_table
	for (int i = 0; i < N; i++)
	{
		for (int j = i + 1; j < N; j++)
		{
			if (Adj[i][j])  //点i和点j相邻 ,更新邻居边不同颜色的冲突边数
			{
				adj_color_table[i][sol[j]]++; //点i为sol[j]颜色的邻居加1
				adj_color_table[j][sol[i]]++; //点j为sol[i]颜色的邻居加1
				if (sol[i] == sol[j]) //判断两个点的颜色是否相同
				{
					f++;  //两点颜色相等，则冲突数加一
				}
			}
		}
	}
	best_f = f;  //初始化最优f
    cout<<"初始化最优解(冲突边数)"<<best_f<<endl;
	//初始化tabutenure	
	tabutenure = new int*[N];
	for (int i = 0; i < N; i++)
	{
		tabutenure[i] = new int[K]; //禁忌表中保存k个颜色的禁忌期
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < K; j++) tabutenure[i][j] = 0; //开始k个颜色的禁忌期都为0
	}
	//初始化NbID为N*(N-1)数组和num_adj
	num_adj = new int[N];
	for (int i = 0; i < N; i++) num_adj[i] = 0;
	NbID = new int*[N];
	for (int i = 0; i < N; i++) NbID[i] = new int[N - 1];
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N - 1; j++) NbID[i][j] = 0;
	}
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			if (Adj[i][j]) {
				int tmp = num_adj[i];
				num_adj[i]++;
				NbID[i][tmp] = j;  //等于去掉了adj表中的非零元素
			}
		}
	for (int i = 0; i != N; i++)
		delete[] Adj[i];
	delete[] Adj;
}

int delt;//此次迭代的最优解
int equ_count = 0;//记录相等解的个数
long long iter;//迭代次数
int sel_vertex, sel_color;// move

void findmove() {
	delt = 10000;//初始为最大整数
	int tmp_delt;
	int c_color;//当前结点颜色
	int *h_color;//邻接颜色表行首指针
	int *h_tabu;//禁忌表行首指针
	int c_color_table;//当前结点邻接颜色表的值
	for (int i = 0; i < N; i++) {
		c_color = sol[i];
		h_color = adj_color_table[i]; //当前节点i所有不同颜色邻居的个数
		c_color_table = h_color[c_color];//即adj_color_table[i][sol[i]]的值  ->  i节点颜色为sol[i]的邻居的个数
		if (c_color_table > 0) {  //颜色表此处的值不为0
			h_tabu = tabutenure[i]; //i节点的禁忌表元素
			for (int j = 0; j < K; j++) {
				if (c_color != j) { //如果颜色不相同
					tmp_delt = h_color[j] - c_color_table; //这里的意思就是判断如果将节点i的颜色转换为颜色j时的冲突节点数变化量，因为原本的c_color_table记录的就是当前节点相同颜色
					                                        //邻居的个数，那冲突边就是c_color_table,如果换成其他邻居的颜色，冲突边就变为新颜色的同颜色邻居个数，如果新的冲突边小于
					                                        //原来的，就相当于变换颜色可以实现减少冲突边的效果，那就将减少的量记录为delt，且一直记录减少量最多的（负值，因此取最小）
					if (tmp_delt <= delt && (iter > h_tabu[j] || (tmp_delt + f) < best_f)) { //应该把更常出现的情况写在前面,实测差别不大
					    /*这里的三个条件a&&(b||c)
					     * a条件的含义是当前改变颜色的方式冲突边减少量要大于原本的才能进入判断条件
					     * b条件是指将i节点的颜色改变为j的这种方式是否符合禁忌表的规则，禁忌表的初始化为0，但在第一次实际赋值时的初值是迭代次数，所以相当于禁忌表中的所有禁忌期都含有
					     * 一个大小为iter的偏置，只有大于这个偏置才相当于禁忌期为0，否则这种方式就在禁忌期内
					     * c条件的含义是当前颜色改变方式产生的冲突边减少量
					     */
/*
						if (tmp_delt < delt) {
							equ_count = 1;
							delt = tmp_delt;
							sel_vertex = i;
							sel_color = j;
						}
						int time_count = 100000;
						clock_t start, end;
						start = clock();
						while (time_count) {
							if (rand() % equ_count == 0) {
								sel_vertex = i;
								sel_color = j;
							}
							time_count--;
						}
						end = clock();
						cout << "取余时间为：" << end - start << endl;
							equ_count++;
							*/
						if (tmp_delt < delt) {//当前解小于本次迭代最优解,则重新开始保存解

							equ_count = 0;
							delt = tmp_delt;
						}

						//这里保存解的时候其实就是贪心，每次只保留效果最好的解，但是冲突边减少量最多的解不一定只有一个，所以在后边选择解的时候还是要随机选出一个解，上边的代码是对这个
						//判断语句的深入选择，因为开始的a条件是小于等于，小于时equ_count最大为1，但是出现等于情况时equ_count就会大于1
						equ_delt[equ_count][0] = i;
						equ_delt[equ_count][1] = j;
						equ_count++;//end of another way
					}
				}
			}
		}
	}

	int tmp = rand() % equ_count;//有多个解时，随机选择
	sel_vertex = equ_delt[tmp][0];
	sel_color = equ_delt[tmp][1];
}
/*
 * 更新禁忌表和颜色表
 */
void makemove() {
	f = delt + f;
	if (f < best_f) best_f = f;
	int old_color = sol[sel_vertex];
	sol[sel_vertex] = sel_color; //更新颜色
	tabutenure[sel_vertex][old_color] = iter + f + 0.6*(rand() % 10);//更新禁忌表，是否最后要+1？？
	//还要有数组存下每个顶点的邻边，以及邻边的数量
	int* h_NbID = NbID[sel_vertex];
	int num_edge = num_adj[sel_vertex];
	int tmp;
	for (int i = 0; i < num_edge; i++) {//更新邻接颜色表
		tmp = h_NbID[i];//等于nbID[sel_vertex][i]
		adj_color_table[tmp][old_color]--;
		adj_color_table[tmp][sel_color]++;
	}
	//if(f==1)
	//	cout<<"Vertex="<<sel_vertex << "\t oldcolor=" << old_color << "\t newcolor=" << sel_color <<"\t iter="<<iter<<"\t tt="<< tabutenure[sel_vertex][old_color]<< endl;
    /*if(f==5){
        cout<<"5"<<endl;
    } else if(f==10){
        cout<<"=10"<<endl;
    } else if(f==50){
        cout<<"<50"<<endl;
    }*/
}



int main()
{
	int result_count = 10;//自动计算10次
	while (result_count) {
		K = 49;
        Initialize("DSJC500.5.col");
		iter = 0;
		clock_t start, end;
		start = clock();
		while (f)
		{
			//if (f == 1)
			//	int i = 0;
            /*if(f==5){
                cout<<"5"<<endl;
            } else if(f==10){
                cout<<"=10"<<endl;
            } else if(f==50){
                cout<<"=50"<<endl;
            }*/
			findmove();
			makemove();
			iter++;
			if(iter==50000000)
            {
			    cout<<"迭代次数超过5000w次"<<endl;
            }
			if(iter==100000000){
                cout<<"迭代次数超过1e次"<<endl;
			}
		}
		end = clock();
		cout << "iter=" << iter << "\t time=" << (end - start) << endl;
		for (int i = 0; i < N; i++)
			delete[]adj_color_table[i];
		delete[]adj_color_table;

		delete[]sol;

		for (int i = 0; i < N; i++)
			delete[]tabutenure[i];
		delete[]tabutenure;

		for (int i = 0; i < N; i++)
			delete[]NbID[i];
		delete[]NbID;

		delete[]num_adj;

		for (int i = 0; i < K - 1; i++)
			delete[]equ_delt[i];
		delete[]equ_delt;

		result_count--;
	}
	system("pause");
	return 0;

}

