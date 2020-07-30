// Philip Knott
// CSCI 2275: Data Structures
// Boulder Campus GPS
// November 2019

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
using namespace std;

const double DISTANCE_CONST = 1.883978; // represents meters/pixel
const double WALKING_SPEED = 1.6931598; // represents meters/second

struct adjVertex; // Pre-definition so it can be used by 'vertex' struct

/* Vertex struct */
struct vertex {
    string name;
    int loc[2]; // [x-pos, y-pos]
    int distance;
    string path;
    bool solved;
    vector<adjVertex> adjacency;
    
    vertex() {
        distance = INT_MAX;
        solved = false;
        path = "";
    }
    
    vertex(string name, int x, int y) {
        this->name = name;
        loc[0] = x;
        loc[1] = y;
        distance = INT_MAX;
        solved = false;
        path = "";
    }
};

/* Adjacent vertex struct */
struct adjVertex {
    vertex *v;
    int weight; // distance between two vertices
};

/* Graph class */
class Graph {
    vector<vertex> vertices; // contains all vertices
    vector<vector<string> > edgeNames; // contains names of edges
    
public:
    /* Constructor, with filename of adjacency list as argument */
    Graph(string fileName) {
        build(fileName);
        cout << vertices.size() << " vertices loaded" << endl;
        
        // menu loop
        while (true) {
            cout << "====MENU====" << endl;
            cout << "1. Print all vertices" << endl;
            cout << "2. Find best path" << endl;
            cout << "3. Quit" << endl;
            
            // get input as integer 'c'
            string sc;
            cin >> sc;
            int c = stoi(sc);
            
            if (c == 1) { // print all vertices
                print();
                cout << endl;
            }
            else if (c == 2) { // find best path
                string name1, name2;
                cout << "Start location: ";
                cin >> name1;
                cout << "End location: ";
                cin >> name2;
                
                if (findIdx(name1) == -1 || findIdx(name2) == -1) { // if either vertices don't exist
                    cout << "Location non-existent." << endl;
                    continue;
                }
                else if (name1 == name2) { // if both vertices are the same
                    cout << "You're already there." << endl;
                    continue;
                }
                
                dijkstras(name1, name2);
                resetProperties();
            }
            else if (c == 3) { // quit program
                cout << "Goodbye!" << endl;
                exit(1);
            }
        }
    }
    
    /* Reads adjacency list in .txt tile with given filename, and creates graph. */
    void build(string fileName) {
        ifstream input;
        input.open(fileName);
        
        if (!input.is_open()) {
            cout << "Error reading file" << endl;
            exit(0);
        }
        
        string line;
        while (getline(input, line)) {
            stringstream ss(line);
            
            // get name
            string name;
            getline(ss, name, ':');
            
            // get x and y
            string x, y;
            getline(ss, x, 'x');
            getline(ss, y, '-');
            
            vertex v(name, stoi(x), stoi(y));
            vertices.push_back(v);
            
            vector<string> names;
            // get adjVertices
            while (getline(ss, name, ',')) {
                names.push_back(name);
            }
            edgeNames.push_back(names);
        }
        
        // fill adjacent vertices (edges) from edgeNames vector
        for (int i = 0; i < vertices.size(); i++) {
            for (int j = 0; j < edgeNames.at(i).size(); j++) {
                int e_idx = findIdx(edgeNames.at(i).at(j));
                
                // add adjacent vector to parent vector's 'adjacency' vector
                adjVertex av;
                av.v = &vertices.at(e_idx);
                av.weight = getWeight(vertices.at(i), *av.v);
                vertices.at(i).adjacency.push_back(av);
            }
        }
    }
    
    /* Prints each vertex and their properties (name, coordinates, adjacent vertices) */
    void print() {
        for (int i = 0; i < vertices.size(); i++) {
            cout << endl;
            
            vertex v = vertices.at(i);
            cout << "Vertex: " << v.name << " [" << v.loc[0] << 'x' << v.loc[1] << "]" << endl;
            
            cout << "Adjacency: ";
            for (int j = 0; j < v.adjacency.size(); j++) {
                cout << v.adjacency.at(j).v->name << ", ";
            }
            cout << endl;
        }
    }
    
    /* Returns index of vertex in vetices vector with given name, or -1 if not found. */
    int findIdx(string name) {
        for (int i = 0; i < vertices.size(); i++) {
            if (vertices.at(i).name == name) return i;
        }
        return -1;
    }
    
    /* Returns distance (hypotenuse) between to coordinates given by vectors. */
    int getWeight(vertex v1, vertex v2) {
        int dx = v1.loc[0] - v2.loc[0];
        int dy = v1.loc[1] - v2.loc[1];
        return floor(sqrt(pow(dx, 2) + pow(dy, 2)));
    }
    
    /* Finds shortest path between two vertices. */
    void dijkstras(string start, string end) {
        
        // "initiate" starting vertex
        int c = findIdx(start);
        vertices.at(c).distance = 0;
        vertices.at(c).path = vertices.at(c).name + " => ";
        
        while (true) {
            int min = INT_MAX; // minimum distance
            vertex temp;
            
            // find unsolved vertex with smallest distance
            for (int i = 0; i < vertices.size(); i++) {
                if (vertices.at(i).solved == false && vertices.at(i).distance < min) {
                    temp = vertices.at(i);
                    min = temp.distance;
                }
            }
            
            for (int i = 0; i < vertices.size(); i++) {
                if (temp.name == vertices.at(i).name) { // go back to vertex with smallest distance
                    vertices.at(i).solved = true;
                    
                    // if this vertex is the end vertex, done!
                    // output all answers
                    if (vertices.at(i).name == end) {
                        cout << endl << "Best route:" << endl;
                        
                        // output path
                        vertices.at(i).path = vertices.at(i).path.substr(0, vertices.at(i).path.size() - 4);
                        cout << vertices.at(i).path << endl;
                        
                        // output distance (in meters and miles)
                        cout << vertices.at(i).distance * DISTANCE_CONST << " meters (" <<
                        vertices.at(i).distance * DISTANCE_CONST / 1609 << " miles)" << endl;

                        // output time (in minutes and seconds)
                        int seconds = (int)(vertices.at(i).distance * DISTANCE_CONST / WALKING_SPEED);
                        cout << seconds / 60 << " minutes " << seconds % 60 << " seconds." << endl;
                        
                        cout << endl; // formatting
                        return;
                    }
                    
                    // update all adjacent vertices' distances
                    vector<adjVertex> ad = vertices.at(i).adjacency;
                    for (int j = 0; j < ad.size(); j++) {
                        if (ad.at(j).v->distance > vertices.at(i).distance + ad.at(j).weight) {
                            ad.at(j).v->distance = vertices.at(i).distance + ad.at(j).weight;
                            ad.at(j).v->path = vertices.at(i).path + ad.at(j).v->name + " => ";
                        }
                    }
                }
            }
        }
    }
    
    /* Resets distance, solved, and path variables in vertices changed by dijkstras. */
    void resetProperties() {
        for (int i = 0; i < vertices.size(); i++) {
            vertices.at(i).distance = INT_MAX;
            vertices.at(i).solved = false;
            vertices.at(i).path = "";
        }
    }
};

/* Main */
int main(int argc, char * argv[]) {
    Graph g(argv[1]);
    
    return 0;
}
