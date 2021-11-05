//
// Created by Administrator on 2021/10/31.
//
#include <iostream>
#include <fstream>
#include <array>
#include <time.h>
#include <math.h>
#include "tmp.cpp"

using namespace std;
#define Ne 500
#define K_c 70
class SA{
public:
    int N{}, K{};
    int iter{};
    double temp = 10.0;
    double temp0 = 10.0;
    double min_temp = 0.0000000000000001;
    double rate = 0.99;
    int steps = 10000;//这里考虑一下如何随着温度的降低调整一下，
    // 一开始可以小一点，理论上应该是温度越低步长越小，那就应该多探索几次
    int f=0,best_f=0;
    int sel_vertex{}, sel_color{};// move
    int delt{};//此次迭代的最优解
    int equ_count = 0;//记录相等解的个数

    array<int,Ne> sol{};
    array<int,Ne> num_adj{};
    array<array<int,K_c>,Ne> adj_color_table{};//领域颜色表
    array<array<bool,Ne>,Ne>Adj;
    array<array<int,Ne-1>,Ne> Nb{};//邻接表，顶点数*(顶点数-1)，保存邻点有哪些
    array<array<int,2>,Ne*(K_c-1)> equ_delt{};//保存相等的解
    SA(const string& fileName);
    ~SA();

    void MainLoop();
    void GetBestState();
    int SelectNode();
    void NextNe();
    void Cool(int num);

private:
    double alpha = 0.01;
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
    for(int i = 0; i < N; i++) sol[i] = rand() % K_c;
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
                    //cout<<f<<endl;
                }
            }
        }
    }
    best_f = f;  //初始化最优f
    cout<<"初始化最优解(冲突边数)"<<best_f<<endl;
    //初始化NbID为N*(N-1)数组和num_adj
    freeArray(num_adj);
    for(auto &it:Nb) freeArray(it);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            if (Adj[i][j]) {
                int tmp_t = num_adj[i];
                num_adj[i]++;
                Nb[i][tmp_t] = j;
            }
        }
    for(auto &it:Adj){
        it.fill(false);
    }
}

void SA::MainLoop() {
    for(int it=0;it<steps; it++) {
        //NextNe();
        delt = 0;//初始为最大整数
        int tmp_delt = 0;
        int c_color = 0;//当前结点颜色
        array<int,K_c>h_tabu{},h_color{};//禁忌表行首指针,邻接颜色表行首指针
        h_color.fill(0);//初始化
        int c_color_table = 0;//当前结点邻接颜色表的值
        //cout<<"zuiyou"<<best_f<<endl;
            int i = rand() % Ne;
            c_color = sol[i];
            h_color = adj_color_table[i]; //当前节点i所有不同颜色邻居的个数
            c_color_table = h_color[c_color];//即adj_color_table[i][sol[i]]的值  ->  i节点颜色为sol[i]的邻居的个数
            if (c_color_table > 0) {  //颜色表此处的值不为0
                    int j = rand() % K_c;
                    if (c_color != j) { //如果颜色不相同
                        tmp_delt = h_color[j] - c_color_table;
                        double gailv = rand()%1000 /1000;
                        //std::default_random_engine e((unsigned int)time(0));
                        //double gailv = 0.8;
                        if (tmp_delt < 0|| (exp(-tmp_delt/temp*100) >gailv)) {
                            //if(f<50) steps*=10;
                            sel_vertex = i;
                            sel_color = j;
                            delt = tmp_delt;
                        }
                }
            }
        //cout<<delt<<endl;
//        if(temp<min_temp) break;
        f = delt + f;
        if (f < best_f) best_f = f;
        int old_color = sol[sel_vertex];
        sol[sel_vertex] = sel_color; //更新颜色
        //还要有数组存下每个顶点的邻边，以及邻边的数量
        array<int,Ne-1> h_NbID = Nb[sel_vertex];
        int num_edge = num_adj[sel_vertex];
        int tmp = 0;
        for (int i = 0; i < num_edge; i++) {//更新邻接颜色表
            tmp = h_NbID[i];//等于nbID[sel_vertex][i]
            adj_color_table[tmp][old_color]--;
            adj_color_table[tmp][sel_color]++;
        }
        //cout<<"chongtu"<<f<<endl;
    }
    //cout<<temp<<endl;

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
    switch (num) {
        case 0:
            temp = temp - alpha;
            break;
        case 1:
            temp = temp * rate;
            break;
        case 2:
            temp = temp/(1+alpha*temp);
        default:
            temp = temp * (1/log(1/num+2));
            break;
    }
}

int SA::SelectNode() {
    for(int i=0;i<Ne;i++){
        for(int j=0;j<Ne-1;j++){

        }
    }
    return 1;
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

int main()
{
    int result_count = 10;//自动计算10次
    while (result_count) {
        auto *Si = new SA("DSJC500.5.col");
        clock_t start, end;
        start = clock();
        Si->steps = 10000;
        while (Si->f)
        {

            Si->MainLoop();
            Si->Cool(1);
            Si->iter++;
            //if(Si->f<50)
                cout<<Si->iter<<":"<<Si->f<<endl;
            if(Si->temp<Si->min_temp) break;
        }
        end = clock();
        cout << "iter=" << Si->iter << "\t time=" << (end - start) << endl;
        result_count--;
    }
    system("pause");
    return 0;
}
