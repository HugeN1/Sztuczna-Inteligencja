#include <iostream>
#include <vector>
#include <fstream>

using namespace std;
int inf = 1000000;
int start;
int finish;


vector<vector<int>> build_graph(istream& file) {
    vector<vector<int>> graph;
    while (true) {
        char begin;
        char end;
        int cost;
        file >> begin;
        file >> end;
        file >> cost;
        if (file.eof()) {
            break;
        }
        vector<int> edge = { (int)begin, (int)end, cost };
        graph.push_back(edge);
    }
    return graph;
}

void build_graph_dot(ofstream& file_out, vector<vector<int>> graph) {
    file_out << "graph {\n";
    for (auto e : graph) {
        file_out << (char)e[0]
            << " -- "
            << (char)e[1]
            << "[label="
            << e[2]
            << "];\n";
    }
    file_out << "}";
}

vector<vector<int>> build_graph_dijkstra(istream& file, int& start, int& finish) {
    vector<vector<int>> graph;
    bool first_iteration = true;
    char local_start;
    char local_finish;
    while (true) {
        if (first_iteration) {
            file >> local_start;
            file >> local_finish;
            start = (int) local_start;
            finish = (int)local_finish;
        }
        else {
            char begin;
            char end;
            int cost;
            file >> begin;
            file >> end;
            file >> cost;
            if (file.eof()) {
                break;
            }
            vector<int> edge = { (int)begin, (int)end, cost };
            graph.push_back(edge);
        }
        first_iteration = false;
    }
    return graph;
}

//// funkcja do określania początku i końca szukanej ścieżki oraz znajdywania wszystkich 
//// ścieżek z punktu startowego do każdego wierzchoła i ustawiania ich kosztu dotarcia na inf

vector <vector<int>> build_paths (vector<vector<int>> graph){
    vector<vector<int>> paths;
    for (auto e : graph) {
        vector <int> path;
        //tworzymy listę wszystkich możliwyś ścieżek w grafie z wierzchołka początkowego np. A-B A-C A-D A-F i ustawiamy koszt na inf

            path = { start, e[0], inf };
            if (e[0] == start) path = { start,start, 0, start };
            if (!(find(paths.begin(), paths.end(), path) != paths.end())) paths.push_back(path);

            path = { start, e[1], inf };
            if (e[1] == start) path = { start,start, 0, start };
            if (!(find(paths.begin(), paths.end(), path) != paths.end())) paths.push_back(path);
    }
    return paths;
}

///////funkcja do znajdywania wszystkich wierzchołków w grafie
vector<int> find_all_verticles (vector<vector<int>> graph) {
    vector <int> all_verticles;
    for (auto e : graph) {
        if (!(find(all_verticles.begin(), all_verticles.end(), e[0]) != all_verticles.end())) all_verticles.push_back(e[0]);
        if (!(find(all_verticles.begin(), all_verticles.end(), e[1]) != all_verticles.end())) all_verticles.push_back(e[1]);
    }
    return all_verticles;
}

//////funkcja do usuwania odwiedzonego wierzchołka  
void delete_visited_vertex(vector<int> &not_visited_vertices, int actual_vertex) {
    for (auto e : not_visited_vertices) {
        if (e == actual_vertex) not_visited_vertices.erase(remove(not_visited_vertices.begin(), not_visited_vertices.end(), actual_vertex), not_visited_vertices.end());
    }
}

// aktualizacja ścieżki
void worth_upadate(vector <vector<int>> &paths, vector <int> edge, int actual) {
    bool worth = false;

    //określamy wierzchołek sąsiadujący z aktualnym 
    int neighbor = 0;
    if (edge[0] == actual) neighbor = edge[1];
    else  neighbor = edge[0];

    //szukamy aktualnego path (od startu do aktualnego wierzchoła)
    vector <int> actual_path;
    for (auto e : paths) {
        if (e[1] == actual) {
            actual_path = e;
        }
    }

    //szukamy aktualnego paths do potencjalnej ścieżki w paths
    vector <int> potential_path;
    for (auto e : paths) {
        if (e[1] == neighbor) {
            potential_path = e;
        }
    }
    if ((actual_path[2] + edge[2]) < potential_path[2]) {
        //znajdujemy indeks na którym znajduje się na potencjalny path
        auto it = find(paths.begin(), paths.end(), potential_path);
        if (it != paths.end())
        {
            int index = it - paths.begin();
            //aktualizuje koszt dotarcia
            paths[index][2] = actual_path[2] + edge[2];
            //aktualizujemy ścieżkę dotarcia do potencjalnego wierzchołka
            int size_of_actual_path =  actual_path.size();

            ///czyściemy starą ścieżkę do potencjalnego wierzchołka
            paths[index].erase(paths[index].begin() + 3, paths[index].end());
            ///przepisujemy ścieżkę z aktualnego wierzchołka
            for (int i = 3; i < size_of_actual_path; i++) {
                paths[index].push_back(actual_path[i]);

            }
            //dodajemy do ścieżki potencjalnego wierzchołka jego samego
            paths[index].push_back(potential_path[1]);
            
        }
        else {
            cout << "Something went wrong" << endl;
        } 
    }
}

////aktualizuj koszty ścieżek w paths
void update_paths(vector <vector<int>> &paths , vector <vector<int>> graph, int actual) {
    for (auto e : graph) {
        if (e[0] == actual || e[1] == actual) {
            worth_upadate(paths, e, actual);
        }
    }
}


// szukaj aktualnie najkrótszej ścieżki w grafie która pomoże nam określić 
//który wierzchołek będzie następny
int shortest_path(vector<vector<int>> paths, vector <int> not_visited_vertices) {
    int next_vertex = 0;
    int min_path = inf;
    for (auto e : paths) {
        if (((e[2] < min_path)) && ((find(not_visited_vertices.begin(), not_visited_vertices.end(), e[1]) != not_visited_vertices.end()))) {
            next_vertex = e[1];
            min_path = e[2];
        }
    }
    return next_vertex;
}


vector <vector<int>> dijkstra(vector<vector<int>> graph_dijkstara, int &start, int &finish) {
    vector<vector<int>> graph = graph_dijkstara;
    bool first_iteration = true;
    vector<vector<int>> paths = build_paths(graph);
    int actual = start;
    vector <int> not_visited_vertices = find_all_verticles(graph);

    while (!not_visited_vertices.empty()) {
        delete_visited_vertex(not_visited_vertices, actual);
        update_paths(paths, graph, actual);
        actual = shortest_path(paths, not_visited_vertices);
    }

    return paths;
}


void build_graph_dijkstra_dot(ofstream& file_out, vector<vector<int>> graph, vector<vector<int>> paths) {
    //szukamy ścieżki wskazanej w pliku
    vector <int> wanted_path;
    string add_color = "";

    for (auto e : paths) {
        if (e[0] == start && e[1] == finish) {
            wanted_path = e;
        }
    }

    file_out << "graph {\n";
    for (auto e : graph) {
        add_color = "";
        for (int i = 3; i < wanted_path.size(); i++) {
            if (i != (wanted_path.size() - 1)) {
                if ((e[0] == wanted_path[i] && e[1] == wanted_path[i + 1]) || (e[0] == wanted_path[i + 1] && e[1] == wanted_path[i])) {
                    add_color = ", color = red";
                }
            }   
        }
        file_out << (char)e[0]
            << " -- "
            << (char)e[1]
            << "[label="
            << e[2]
            << add_color
            << "];\n";
    }
    file_out << "labelloc = \"t\";\n";
    file_out << "label = \"Total cost of the path from the point "
             << (char) start
             << " to "
             << (char) finish
             << " is "
             << wanted_path[2]
             << "\";\n\n";
    file_out << "}";
}


int main(int argc, char** argv) {

    //string infile = argv[1];
    string infile = "input_graph.txt";
    ifstream file_in(infile);
    //vector<vector<int>> graph = build_graph(file_in);

  //  build_graph_dot(file_out, graph);w

    vector<vector<int>> graph = build_graph_dijkstra(file_in, start, finish);

    vector <vector<int>> shortest_paths = dijkstra(graph, start, finish);

    string outfile = "graph_dijkstra.dot";
    //string outfile = argv[2];
    ofstream file_out(outfile);
    
    build_graph_dijkstra_dot(file_out, graph, shortest_paths);
    system("dot graph_dijkstra.dot - Tpng - graph_dijkstra.png");
    return 0;
}
