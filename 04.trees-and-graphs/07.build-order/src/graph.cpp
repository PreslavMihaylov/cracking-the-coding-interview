#include <graph.h>
#include <algorithm>
#include <climits>
#include <iostream>
#include <sstream>
#include <cassert>

using namespace std;

bool routeExists(GraphNode& node1, GraphNode& node2)
{
    if (node1.id == UINT_MAX) return false;
    if (node1.id == node2.id) return true;

    uint32_t tmp = node1.id;
    node1.id = UINT_MAX;
    
    bool pathFound = false;
    for (auto& neighbor: node1.neighbors)
    {
        pathFound = routeExists(*neighbor, node2);
        if (pathFound) break;
    }

    node1.id = tmp;
    return pathFound;
}

bool nodeExists(vector<shared_ptr<GraphNode>>& adjacencyList, uint32_t nodeId)
{
    return find_if(adjacencyList.begin(), 
                   adjacencyList.end(), 
                   [&](const shared_ptr<GraphNode>& n) { return n->id == nodeId; }) != adjacencyList.end();
}

shared_ptr<GraphNode> findNode(vector<shared_ptr<GraphNode>>& adjacencyList, uint32_t nodeId)
{
    return *find_if(adjacencyList.begin(), 
                   adjacencyList.end(), 
                   [&](const shared_ptr<GraphNode>& n) { return n->id == nodeId; });
}

void appendNeighbor(vector<shared_ptr<GraphNode>>& adjacencyList, uint32_t nodeId, uint32_t neighborId)
{
    assert(nodeExists(adjacencyList, nodeId));

    if (!nodeExists(adjacencyList, neighborId))
    {
        adjacencyList.push_back(make_shared<GraphNode>(neighborId));
    }

    shared_ptr<GraphNode> node = findNode(adjacencyList, nodeId);
    vector<shared_ptr<GraphNode>>& neighbors = node->neighbors;
    shared_ptr<GraphNode> newNeighbor = findNode(adjacencyList, neighborId);

    if (!nodeExists(neighbors, newNeighbor->id))
    {
        neighbors.push_back(newNeighbor);
        // uncomment for undirected graph
        // newNeighbor->neighbors.push_back(node);
    }
}

void readAdjacencyList(vector<shared_ptr<GraphNode>>& adjacencyList)
{
    string line;
    int linesCnt;

    cin >> linesCnt;
    getline(cin, line);
    for (int i = 0; i < linesCnt; i++)
    {
        getline(cin, line);

        istringstream iss(line);
        uint32_t nodeId;
        string arrowSymbol;
        uint32_t adjacentNodeId;

        iss >> nodeId;

        // discard arrow symbol
        iss >> arrowSymbol;

        if (!nodeExists(adjacencyList, nodeId))
        {
            adjacencyList.push_back(make_shared<GraphNode>(nodeId));
        }

        while (!iss.eof())
        {
            iss >> adjacentNodeId;
            appendNeighbor(adjacencyList, nodeId, adjacentNodeId);
        }
    }
}

void printAdjacencyList(vector<shared_ptr<GraphNode>>& adjacencyList)
{
    for (shared_ptr<GraphNode>& node : adjacencyList)
    {
        cout << node->id << " -> ";
        for (shared_ptr<GraphNode>& neighbor : node->neighbors)
        {
            cout << neighbor->id << " ";
        }

        cout << endl;
    }
}
