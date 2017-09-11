#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "SimpleGraph.h"

using std::cout;	using std::endl;
using std::cin;
using std::vector;

// Constant number
const double kPi = 3.14159265358979323;
const double kRepel = 1e-3;
const double kAttract = 1e-3;

void Welcome();
void InitGraph(SimpleGraph &graph);
void OpenUserFile(std::ifstream &input);
void LoadGraph(SimpleGraph &graph ,std::ifstream &input);
void ForceDirected(SimpleGraph &graph, int seconds);

// Main method
int main() {
    Welcome();
    SimpleGraph graph;
    InitGraph(graph);
    int seconds;
    while (1) {
        cout << "Please enter the number of seconds to run FR algorithm: ";
        std::cin >> seconds;

        if (seconds > 0) break;

        cout << "Sorry, you should enter a positive number" << endl;
    }
    ForceDirected(graph, seconds);


    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}

// Open user file
void OpenUserFile(std::ifstream &input) {
    while(true) {
        cout << "Please enter filename to read: ";
        std::string file_name;
        std::getline(std::cin, file_name);

        input.open(file_name.c_str());
        if(input.is_open()) return;

        cout << "Sorry, I can't find the file " << file_name << endl;
        input.clear();
    }
}

// Initialize the graph
void InitGraph(SimpleGraph &graph) {
    InitGraphVisualizer(graph);
    std::ifstream input;
    OpenUserFile(input);
    LoadGraph(graph, input);
    DrawGraph(graph);
}

// Load graph, including nodes and edges
void LoadGraph(SimpleGraph &graph, std::ifstream &input) {
    int nodes_num;
    input >> nodes_num;

    for (int i = 0; i < nodes_num; ++i) {
        double x = std::cos(2 * kPi * i / nodes_num);
        double y = std::sin(2 * kPi * i / nodes_num);

        struct Node node = {x, y};
        graph.nodes.push_back(node);
    }

    size_t start, end;
    while(input >> start >> end) {
        struct Edge edge = {start, end};
        graph.edges.push_back(edge);
    }
}
// Implement the Force Directed algorithm
void ForceDirected(SimpleGraph &graph, int seconds) {
    time_t startTime = time(NULL);
    while(true) {
        size_t size = graph.nodes.size();
        vector<double> deltXs(size, 0);
        vector<double> deltYs(size, 0);
        for (size_t i = 0; i < size - 1; ++i) {
            for (size_t j = i + 1; j < size ; ++j) {
                double x0 = graph.nodes[i].x;
                double y0 = graph.nodes[i].y;
                double x1 = graph.nodes[j].x;
                double y1 = graph.nodes[j].y;
                double Frepel = kRepel / sqrt(pow(y1 - y0, 2) + pow(x1 - x0, 2));
                double theta = atan2(y1 - y0, x1 - x0);
                deltXs[i] -= Frepel * cos(theta);
                deltYs[i] -= Frepel * sin(theta);
                deltXs[j] += Frepel * cos(theta);
                deltYs[j] += Frepel * sin(theta);
            }
        }

        size = graph.edges.size();
        for (size_t i = 0; i < size; ++i) {
            size_t start = graph.edges[i].start;
            size_t end = graph.edges[i].end;
            double x0 = graph.nodes[start].x;
            double y0 = graph.nodes[start].y;
            double x1 = graph.nodes[end].x;
            double y1 = graph.nodes[end].y;
            double Fattract = kAttract * (pow(y1 - y0, 2) + pow(x1 - x0, 2));
            double theta = atan2(y1 - y0, x1 - x0);
            deltXs[start] += Fattract * cos(theta);
            deltYs[start] += Fattract * sin(theta);
            deltXs[end] -= Fattract * cos(theta);
            deltYs[end] -= Fattract * sin(theta);
        }

        size = graph.nodes.size();
        for (size_t i = 0; i < size; ++i) {
            graph.nodes[i].x += deltXs[i];
            graph.nodes[i].y += deltYs[i];
        }

        DrawGraph(graph);
        double elapsedTime = difftime(time(NULL), startTime);
        if (elapsedTime >= seconds)
            return ;
    }
}
