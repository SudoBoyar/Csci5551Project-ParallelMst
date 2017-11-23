//
// Created by Alex Klein on 11/22/17.
//

#ifndef CSCI5551PROJECT_PARALLELMST_GRAPH_H
#define CSCI5551PROJECT_PARALLELMST_GRAPH_H

#include <algorithm>
#include <array>
#include <queue>
#include <vector>

using namespace std;

struct Edge {
    int v1;
    int v2;
    int weight;

    Edge(int source, int dest, int edge_weight) {
        v1 = source;
        v2 = dest;
        weight = edge_weight;
    }

    bool operator<(const Edge other) const {
        return weight < other.weight;
    }

    bool goesTo(int v) {
        return v1 == v || v2 == v;
    }
};

struct Vertex {
    int id;
    bool flag;

    vector<Edge> edges;
    priority_queue<Edge> edges_queue;


    Vertex(int vid) {
        id = vid;
        flag = false;
    }

    void addEdge(Edge edge) {
        edges_queue.push(edge);
        edges.push_back(edge);
    }

    int weight(int v) {
        for (Edge edge : edges) {
            if (edge.goesTo(v)) {
                return edge.weight;
            }
        }

        return NO_EDGE;
    }

    template<typename Functor>
    void visitEdges(Functor f) {
        for_each(edges.begin(), edges.end(), f);
    }
};

struct Graph {
    array<Vertex, VCOUNT> vertices;

    void addEdge(int v1, int v2, int weight) {
        Edge edge = Edge(v1, v2, weight);
        vertices[v1].addEdge(edge);
        vertices[v2].addEdge(edge);
    }

    int weight(int v1, int v2) {
        vertex(v1).weight(v2);
    }

    Vertex vertex(int i) {
        return vertices[i];
    }

    template<typename Functor>
    void visitVertices(Functor f) {
        for_each(vertices.begin(), vertices.end(), f);
    }

    int length() {
        return VCOUNT;
    }
};

#endif //CSCI5551PROJECT_PARALLELMST_GRAPH_H
