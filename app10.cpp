#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
#define UNDEFINED -1

int popSmallest(vector<int>& qSet, vector<double>& dist);
void printPath(vector<int> path);
vector<string> splitBySpace(const string& str);

vector<string> splitBySpace(const string& str) {
    istringstream buffer(str);
    vector<string> ret((istream_iterator<string>(buffer)),
        istream_iterator<string>());
    return ret;
}

struct Graph {
    struct Node {
        int x;
        int y;
    };
    vector<Node> nodes;
    vector<vector<uint8_t>> adjMatrix;

    double edgeLength(int v1, int v2);
    vector<int> reconstructPath(vector<int> prev, int current);

    vector<int> Dijkstra(int src, int target);

    double h(int cId, int target);

    vector<int> AStar(int src, int target);

    static Graph* fromFile(string fileName);
    void toFile(string fileName);
};

Graph* Graph::fromFile(string fileName) {
    ifstream file(fileName);

    if (!file.is_open()) {
        return nullptr;
    }

    Graph* g = new Graph();

    string line = "";
    getline(file, line);
    int numNodes = stoi(line);

    g->nodes.resize(numNodes);
    g->adjMatrix.resize(numNodes);
    for (size_t i = 0; i < numNodes; i++) {
        g->adjMatrix[i].resize(numNodes, 0);
        g->adjMatrix[i][i] = 1;
    }

    for (int nodeId = 0; nodeId < numNodes; nodeId++) {
        getline(file, line);
        Node& n = g->nodes[nodeId];
        auto values = splitBySpace(line);

        n.x = stoi(values[0]);
        n.y = stoi(values[1]);

        for (int i = 2; i < values.size(); i++) {
            int neighbourId = stoi(values[i]);
            g->adjMatrix[nodeId][neighbourId] = g->adjMatrix[neighbourId][nodeId] = 1;
        }
    }

    return g;
}

double Graph::edgeLength(int v1, int v2) {
    Node& a = nodes[v1];
    Node& b = nodes[v2];
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (b.y - b.y));
}

double Graph::h(int cId, int target) { return edgeLength(cId, target); }

vector<int> Graph::reconstructPath(vector<int> prev, int current) {
    std::vector<int> path;
    while (prev[current] != -1) {
        path.push_back(current);
        current = prev[current];
    }
    return path;
}

vector<int> Graph::AStar(int src, int target) {
    if (target < 0 || target > nodes.size()) {
        return {};
    }

    vector<int> openSet;
    openSet.push_back(src);

    vector<int> prev(nodes.size(), UNDEFINED);

    vector<double> neighborDist(nodes.size(), INFINITY);
    neighborDist[src] = 0;

    vector<double> targetDist(nodes.size(), INFINITY);
    targetDist[src] = h(src, target);

    while (openSet.size() > 0) {
        int current = popSmallest(openSet, targetDist);
        if (current == target) {
            vector<int> path = reconstructPath(prev, current);
            path.push_back(src);
            return path;
        }

        for (int neighbor = 0; neighbor < nodes.size(); neighbor++) {
            if (neighbor != current && adjMatrix[current][neighbor]) {
                double tentativeGScore =
                    neighborDist[current] + edgeLength(current, neighbor);
                if (tentativeGScore < neighborDist[neighbor]) {
                    prev[neighbor] = current;
                    neighborDist[neighbor] = tentativeGScore;
                    targetDist[neighbor] = tentativeGScore + h(neighbor, target);

                    bool addToOpenSet = true;
                    for (int i = 0; i < openSet.size(); i++) {
                        if (openSet[i] == neighbor) {
                            addToOpenSet = false;
                            break;
                        }
                    }

                    if (addToOpenSet) {
                        openSet.push_back(neighbor);
                    }
                }
            }
        }
    }

    return {};
}

int popSmallest(vector<int>& qSet, vector<double>& dist) {
    double minDist = INFINITY;
    int smallest = -1;
    for (int i = 0; i < qSet.size(); i++) {
        if (dist[qSet[i]] < minDist) {
            smallest = i;
            minDist = dist[qSet[i]];
        }
    }
    if (smallest == -1) {
        return -1;
    }
    int retVal = qSet[smallest];
    qSet.erase(qSet.begin() + smallest);
    return retVal;
}

void printPath(vector<int> path) {
    string strPath = "";
    while (path.size() > 0) {
        int v = path.back();
        path.pop_back();
        strPath.append(to_string(v) + " ");
    }
    cout << strPath << endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return -1;
    }

    string fileName(argv[1]);
    string graphName = fileName.substr(0, fileName.find('.'));

    auto g = Graph::fromFile(fileName);
    if (g == nullptr) {
        return -1;
    }

    if (argc == 4) {
        int src = stoi(argv[2]);
        int target = stoi(argv[3]);

        clock_t t;
        double timeRunning;

        t = clock();
        vector<int> path = g->AStar(src, target);
        t = clock() - t;
        timeRunning = (double)t / CLOCKS_PER_SEC;
        cout << "AStar - " << timeRunning << "s:\n";
        printPath(path);
    }

    delete g;

    return 0;
}
