#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <string>
#include <string.h>
#include <math.h>
#include <queue>
#include <time.h>
#include <fstream>
#include <set>
using namespace std;
#define maxN 800//点数量
#define maxE 250000//边数量
#define delta 0.98//温度衰减率
#define EPS  1e-8//终止温度
#define LIMIT 1000//概率选择上限
#define OLOOP 20//外循环次数
#define ILOOP 100//内循环次数

struct Edge
{
	int x, y;
}edge[maxE + 10];
int G[maxN][maxN];//存图的关系 

vector<int> Curs, News, Best;
int CurN = 0;//当前解集合中的点的数量
int Vn, En;//|V|和|E|
int in[maxN], ans[maxN], tmp[maxN];
//一个点能不能加入集合并让集合保持完全子图性质  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!CurN初始化 in初始化
bool canadd(vector<int> a, int n)////
{
	int len = a.size();
	for (int i = 0; i < len; ++i)
	{
		if (G[n][a[i]] == 0)
			return false;
	}
	return true;
}

//初始化图 
void input()
{
	int i, j, flag = 1;
	char c;
	char s[20];
	memset(edge, 0, sizeof edge);
	cin >> c >> s >> Vn >> En;
	//	scanf("%d%d", &Vn, &En);
	memset(G, 0, sizeof G);
	for (i = 0; i < En; ++i)
	{
		int x1, x2;
		cin >> c >> x1 >> x2;
		//	scanf("%d%d", &x1, &x2);
		edge[i].x = x1, edge[i].y = x2;
		G[x1][x2] = 1;
		G[x2][x1] = 1;
	}
	Best.clear();
}

void init(int num)
{
	int x = rand() % En;
	int x1, x2;
	memset(in, 0, sizeof in);
	News.clear();
	Curs.clear();
	x1 = edge[x].x, x2 = edge[x].y;
	in[x1] = in[x2] = 1;
	if (x1 == x2)
		Curs.push_back(x1);
	else
	{
		Curs.push_back(x1);
		Curs.push_back(x2);
	}
	News = Curs;
}

//随机向集合里面加入一个可以保持完全子图的点
int randadd() //应该随机
{
	int now = 0;
	memset(tmp, 0, sizeof tmp);
	News = Curs;
	for (int i = 1; i <= Vn; ++i)
		if (in[i] == 0)
		{
			if (canadd(News, i))
			{
				tmp[now++] = i;
			}
		}
	if (now == 0)return 0;
	int n = rand() % now;//
	News.push_back(tmp[n]);
	return tmp[n];
}

int delet() {
	News = Curs;
	int len = News.size();
	if (len == 0) return 0;
	int x = rand() % len;
	int y = News[x];
	vector<int>::iterator it = News.begin() + x;
	News.erase(it);
	return y;
}

int getnew() {
	if (Curs.size() == 0)
		return randadd();
	if (rand() % 2 == 0)
	{
		return randadd();
	}
	else
	{
		return delet();
	}
}
int main() {
	srand((unsigned)(time(NULL)));
	while (1)
	{
		int  num = 10;
		double T = 100;//温度
		input();
		while (num--)//执行十次模拟退火算法 初始化！！
		{
			int P_L = 0;
			int P_F = 0;
			T = 100;
			init(num);
			while (1)//温度降低 
			{
				for (int i = 0; i < ILOOP; i++)//内循环，寻找在一定温度下的最优值
				{
					int num = getnew();
					double dE = int((News.size()) - (Curs.size()));
					if (dE > 0)//找到更优值
					{
						Curs = News;
						in[num] = 1;
						P_L = 0;
						P_F = 0;
					}
					else
					{
						double rd = rand() / (RAND_MAX + 1.0);//如果找到比当前更差的解，以一定概率接受该解，并且这个概率会越来越小
						if (exp(dE / T) > rd && exp(dE / T) < 1)
						{
							Curs = News;
							in[num] = 0;
						}
						P_L++;
					}
					if (P_L > LIMIT)
					{
						P_F++;
						break;
					}
				}
				if (Curs.size() > Best.size())
					Best = Curs;
				if (P_F > OLOOP || T < EPS)
					break;
				T *= delta;
			}
		}
		//这么多次循环后的最大值就是最大团的解 
		int anslen = Best.size();
		for (int i = 0; i < anslen; i++)
			ans[i] = Best[i];
		sort(ans, ans + anslen);
		cout << anslen << endl << ans[0];
		for (int i = 1; i < anslen; i++)
			cout << ' ' << ans[i];
		cout << endl;
	}
	return 0;
}
/*
//在同一个温度下做四十次
//循环里每次在原来的解上随机增加一个随机的可以保持完全图的点或者删去一个随机的点，都要随机选。
//然后贪心地在新解下尽可能多的加入点，作为这个解的值，来和当前解的值比较。
//如果 得到的新解更优则新解变为当前解，如果更差则在一定概率下接受更差的解。
*/