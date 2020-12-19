#include <list>
#include <limits.h>
#include <algorithm>

#include <vector>
#include "leitor.h"

using namespace std;

class Graph
{
    int V;    // No. of verticesa
    list<int> *adj;    // Pointer to an array containing adjacency lists
    bool isCyclicUtil(int v, bool visited[], bool *rs);  // used by isCyclic()
public:
    Graph(int V);   // Constructor
    void addEdge(int v, int w);   // to add an edge to graph
    bool isCyclic();    // returns true if there is a cycle in this
    void remove(int n);
    bool existArc(int v1, int v2);
    vector<int> nos;
    vector<int> nos2;
    int vant;
    Leitor *rd;
};

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
    vector<int> nos;
    vector<int> nos2;
    vant = -1;

}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); // Add w to v’s list.
}
void Graph::remove(int n){




    for(int i=0;i<n;i++)
        adj[i].clear();

    //        for(int i=0;i<n;i++)
    //            if(adj[i].empty())
    //                cout << "deu certo " << i << endl;





}

// This function is a variation of DFSUytil() in https://www.geeksforgeeks.org/archives/18212
bool Graph::isCyclicUtil(int v, bool visited[], bool *recStack)
{


    if(visited[v] == false)
    {
        // Mark the current node as visited and part of recursion stack
        visited[v] = true;
        recStack[v] = true;

//        bool parou = false;

        // Recur for all the vertices adjacent to this vertex
        list<int>::iterator i;
        for(i = adj[v].begin(); i != adj[v].end(); ++i)
        {

            nos.push_back(v);
            nos2.push_back(*i);
            vant=*i;

            if ( !visited[*i] && isCyclicUtil(*i, visited, recStack) )
                return true;
            else if (recStack[*i])
                return true;

        }

    }
    recStack[v] = false;  // remove the vertex from recursion stack
    return false;
}

// Returns true if the graph contains a cycle, else false.
// This function is a variation of DFS() in https://www.geeksforgeeks.org/archives/18212
bool Graph::isCyclic()
{
    // Mark all the vertices as not visited and not part of recursion
    // stack
    bool *visited = new bool[V];
    bool *recStack = new bool[V];
    for(int i = 0; i < V; i++)
    {
        visited[i] = false;
        recStack[i] = false;
    }

    // Call the recursive helper function to detect cycle in different
    // DFS trees
    for(int i = 0; i < V; i++){
        nos.clear();
        nos2.clear();
        vant=i;
        if (isCyclicUtil(i, visited, recStack))

            return true;
    }
    return false;
}

bool Graph::existArc(int v1, int v2){

    if(find(adj[v1].begin(), adj[v1].end(), v2) != adj[v1].end())
            return true;
    return false;



}
