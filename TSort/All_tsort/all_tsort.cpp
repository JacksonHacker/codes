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
//���ݷ�����ĳ�������Ӧ��״̬�ռ�������DFS���������ܲ���ͨ��BFS��ʵ���أ�
//ÿһ���ݹ�ʵ������һ�����㣬ֱ����ĳ���ݹ�ʵ���У��������ж����״̬������û�ж���������룬��ݹ���ݡ�
//�����ÿһ�εݹ����֮ǰ���ȼ��һ�½���Ƿ������������������ӡ������ֱ�ӻ��ݡ�
//Ϊʲô���˽�״̬�ռ���֮ǰ��������ⲻ��Ϊʲô���ȫ�����㷨ʵ�ֿ��Բ��ز�©���������е���������������������һ����֮�������ݾͼ򵥵Ķࣿ����û�ܳ����ʵ�ʻ����޷����һЩϸ�ڣ���ô�ſ����ڲ���Ҫʵ���˽�״̬�ռ���������£��������һ��������þ����ܼ򵥵ķ����������

void Graph::All_TSort(Vector<int>& in_degree, Vector<int>& res, bool visited[]) {
    bool flag = false; 
 
    for (int i = 0; i < V; i++) {                               //�鿴״̬�ռ����еĸö��㻹��û�к���
        if (in_degree[i] == 0 && !visited[i]) {
            list<int>:: iterator j;
            for (j = adj[i].begin(); j != adj[i].end(); j++)
                in_degree[*j]--;
            res.push_back(i);
            visited[i] = true;
			
            All_TSort(in_degree, res, visited);                 //���õݹ麯��ִ����֮�󣬵ݹ����
            visited[i] = false;                                 //�ָ���������е�β��������ǻ��ݵ�״̬�ռ����е���һ�����
            res.pop_back();
            for (j = adj[i].begin(); j != adj[i].end(); j++)
                in_degree[*j]++;
 
            flag = true;                                        //���Ϊtrue��ʾ��ǰ�ݹ�ʵ�����ڷ��ʵĶ���û�б������������С������ڼ����ݹ����ʱ����ӡ�˲������Ľ��������
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
		t = q.back(); q.pop(); res.push_back(t); //������������
		bool flag = true;
		
		for (auto i = adj[t].begin(); i != adj[t].end(); ++i) {
			if (!visited[*i]) {
				visited[*i] = true;
				q.push(*i);
				flag = false; //�����Ǳ�������Ҷ�ӽ��
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