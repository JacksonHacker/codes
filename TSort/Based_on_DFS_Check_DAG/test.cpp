// A C++ program to print topological sorting of a DAG
#include <iostream>
#include <list>
#include <stack>
using namespace std;
 
typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;
// Class to represent a graph
class Graph
{
public:
    Graph(int V);   // Constructor
 
	// function to add an edge to graph
    void addEdge(int v, int w);
 
    // prints a Topological Sort of the complete graph
    void tsort(int s);
private:
    int V;    
    list<int> *adj;
 
    // A function used by topologicalSort
    bool TSort(int v, VStatus visited[], stack<int> &Stack);
};
 
Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
}
 
void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); // Add w to v¡¯s list.
}
 
// A recursive function used by topologicalSort
bool Graph::TSort(int v, VStatus visited[], stack<int> &Stack)
{
    visited[v] = DISCOVERED;
 
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        switch (visited[*i]) {
	      case UNDISCOVERED:
			  if (!TSort(*i, visited, Stack))
				  return false;
			  break;
		  case DISCOVERED:
			  return false;
		  default:
			  break;
		}
    visited[v] = VISITED; Stack.push(v);
	return true;
}

void Graph::tsort(int s)
{
    stack<int> Stack;

    VStatus *visited = new VStatus[V];
    for (int i = 0; i < V; i++)
        visited[i] = UNDISCOVERED;

    int v = s;
	do {
		if (visited[v] == UNDISCOVERED)
			if (!TSort(v, visited, Stack)) {
				while (!Stack.empty())
					Stack.pop(); 
				break;
			}
	} while (s != (v = (++v % V)));
	
	while (Stack.empty() == false)
    { cout << Stack.top() << " "; Stack.pop(); }
}
 
// Driver program to test above functions
int main()
{
    // Create a graph given in the above diagram
    Graph g(6);
    g.addEdge(5, 2);
    g.addEdge(5, 0);
    g.addEdge(4, 0);
    g.addEdge(4, 1);
    g.addEdge(2, 3);
    g.addEdge(3, 1);
 
    cout << "Following is a Topological Sort of the given graph \n";
    g.tsort(0);
 
    return 0;
}