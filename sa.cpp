//
// Created by Administrator on 2021/10/31.
//
#include <iostream>
#include <fstream>
#include <array>
#include <time.h>
#include "tmp.cpp"
using namespace std;
#define Ne 500
#define K_c 47
class SA{
public:
    int N{}, K{};
    int iter{};
    double temp = 1.0;
    double temp0 = 1.0;
    double min_temp = 0.01;
    int steps = 100000;
    int min_color = 47;
    int f,best_f;
    double rate = 0.99;


    array<int,Ne> sol{};
    array<int,Ne> num_adj{};
    array<array<int,K_c>,Ne> adj_color_table{};//领域颜色表
    array<array<bool,Ne>,Ne-1>Adj;
    SA(const string& fileName);
    ~SA();

    void MainLoop();
    void GetBestState();
    void NextNe();
    void Cool(int num);

private:
    double alpha = 0.02;
    int coolMode;
};

SA::~SA() {
    cout<<"end"<<endl;
}

SA::SA(const string& fileName) {
    ifstream ifs;
    ifs.open(fileName);
    cout<<"init"<<endl;
    srand(time(NULL));
    string _tmp;
    if(!ifs.is_open()){
        cout<<"failed"<<endl;
        return ;
    }
    while(!ifs.eof()){
        if (_tmp=="edge"){
            ifs >> N;
            for(auto &it:Adj) it.fill(false);
        }
        if(_tmp=="e") {
            int m,n;
            ifs >> m >> n;
            Adj[m - 1][n - 1] = true;//对称形成邻接矩阵
            Adj[n - 1][m - 1] = true;
        }
        ifs >> _tmp;
    }
    ifs.close();
    for(int i = 0; i < N; i++) sol[i] = rand() % K;
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
}

void SA::MainLoop() {

    for(int i=0;i<steps; i++) {
        //NextNe();
        int c_node = rand() % K;
        int c_color = rand() % K;
        Cool(i);
        if(temp<min_temp) break;

        /*if(f>best_f) {
            best = f;
        }*/
    }
}

void SA::GetBestState() {

}

int findFirstCollision() {

    int pos,pos_col;
    bool collision = false;

    return 1;
}
void SA::NextNe() {
    int node = findFirstCollision(); //node 是


}

void SA::Cool(int num) {
    switch (coolMode) {
        case 0:
            temp = temp - alpha;
            break;
        case 1:
            temp = temp * alpha;
            break;
        default:
            temp = temp0 * (1/log(num+2));
            break;
    }
}

void test() {
    SA *s = new SA("111.col");
    s->MainLoop();
    s->GetBestState();

}


/*
 * individual的chromosome 相当于sol
 *              n_genes相当于 节点数
 *              individual的初始化相当于随机生成一个初始解，只不过后续的解
 *              的变化也在individual上
 *
 */
