#include <iostream>
#include <list>
#include <queue>
#include "Vector.h"

using namespace std;
 
class Graph
{
    int V;   
    list<int> *adj;

    void All_TSort(Vector<int>& in_degree, Vector<int>& res, bool visited[]);
public:
    Graph(int n); 
	void addEdge(int v, int w);
    void all_tsort();
	void all_tsort_bfs();
};
Graph::Graph(int n)
{
    V = n;
    adj = new list<int>[V];
}
void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); 
}
//回溯法：对某个问题对应的状态空间树进行DFS搜索。那能不能通过BFS来实现呢？
//每一个递归实例纳入一个顶点，直到在某个递归实例中，遍历所有顶点的状态，发现没有顶点可以纳入，则递归回溯。
//这里，在每一次递归回溯之前，先检查一下结果是否完整，若完整，则打印，否则，直接回溯。
//为什么在了解状态空间树之前，死活理解不了为什么这个全拓扑算法实现可以不重不漏地求解出所有的拓扑排序结果，而有了这一概念之后，理解回溯就简单的多？是我没能抽象出实质还是无法理解一些细节？怎么才可以在不需要实现了解状态空间树的情况下，抽象出这一概念或者用尽可能简单的方法理解它？

void Graph::All_TSort(Vector<int>& in_degree, Vector<int>& res, bool visited[]) {
    bool flag = false; 
 
    for (int i = 0; i < V; i++) {                               //查看状态空间树中的该顶点还有没有孩子
        if (in_degree[i] == 0 && !visited[i]) {
            list<int>:: iterator j;
            for (j = adj[i].begin(); j != adj[i].end(); j++)
                in_degree[*j]--;
            res.push_back(i);
            visited[i] = true;
			
            All_TSort(in_degree, res, visited);                 //当该递归函数执行完之后，递归回溯
            visited[i] = false;                                 //恢复结果向量中的尾顶点才算是回溯到状态空间树中的上一个结点
            res.pop_back();
            for (j = adj[i].begin(); j != adj[i].end(); j++)
                in_degree[*j]++;
 
            flag = true;                                        //标记为true表示当前递归实例正在访问的顶点没有被纳入结果向量中。避免在即将递归回溯时，打印了不完整的结果向量。
        }
    }
	
    if (!flag) {
        for (int i = 0; i < (int)res.size(); i++)
            cout << res[i] << " ";
        cout << endl;
    }
}
 
void Graph::all_tsort() {
    bool *visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;
 
	Vector<int> in_degree(V);
	for (int u=0; u<V; u++) {
        list<int>::iterator itr;
        for (itr = adj[u].begin(); itr != adj[u].end(); itr++)
			in_degree[*itr]++;
    }
	
    Vector<int> res;
    All_TSort(in_degree, res, visited);
}
void Graph::all_tsort_bfs() {
	bool *visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;
	Vector<int> res;
	
	queue<int> q;
	q.push(0);
	
	while (!q.empty()) {
		t = q.back(); q.pop(); res.push_back(t); //弹出并纳入结果
		bool flag = true;
		
		for (auto i = adj[t].begin(); i != adj[t].end(); ++i) {
			if (!visited[*i]) {
				visited[*i] = true;
				q.push(*i);
				flag = false; //代表不是遍历树的叶子结点
			}
		}
		if (flag) {
			cout << res << endl;
		}
	}
	
	
}

int main()
{
    Graph g(6);
    g.addEdge(5, 2);
    g.addEdge(5, 0);
    g.addEdge(4, 0);
    g.addEdge(4, 1);
    g.addEdge(2, 3);
    g.addEdge(3, 1);
 
    cout << "All Topological sorts" << endl;
 
    g.all_tsort();
 
    return 0;
}