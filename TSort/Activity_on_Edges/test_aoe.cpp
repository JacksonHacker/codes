#include <iostream>
#include <string>
#include <list>
#include <cassert>
#include "Vector.h"

using namespace std;

class aoe_graph {
public:
	aoe_graph(int n, Vector<Vector<int>> g) : V(n), graph(g) { }
	const int cedge(int, int) const;
	int edge(int, int);
	void event_early();
private:
	int V;
	Vector<Vector<int>> graph;
	
	Vector<int> ve;//event early
	Vector<int> vl;//event late
	Vector<int> ae;//activity early
	Vector<int> al;//activity late
};

const int aoe_graph::cedge(int v, int w) const {
	assert (v >= 0 && v < V && w >= 0 && w < V);
	return graph[v][w];
}
int aoe_graph::edge(int v, int w) {
	assert (v >= 0 && v < V && w >= 0 && w < V);
	return graph[v][w];
}
void aoe_graph::event_early() {
	ve.push_back(0); //默认0号顶点入度为零，为活动网络的起点
	
	
}

int main() {
	
	Vector<Vector<int>> g = {{0,45,50,15, 0, 0},
                             {0, 0, 5, 0,20,15},
						     {0, 0, 0, 0, 0, 0},
							 {0,10, 0, 0,79, 0},
                             {0,30, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0},
							};
							
	aoe_graph aoe0(6, g);
	int v = 3, w = 4;
    cout << v << "->" << w << " 's weight: " << aoe0.cedge(v, w) << endl;
	
	
	return 0;
}