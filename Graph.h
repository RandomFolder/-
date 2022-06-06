#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <QString>
#include <vector>

struct Node
{
    int x;
    int y;
    QString name;
    QString description;
};

struct Edge
{
    QString node1;
    QString node2;
    int weight;
};

class Graph
{
private:
    std::vector<Node> nodes;
    std::vector<Edge> edges;

public:
    void add_node(int X, int Y, QString name, QString des)
    {
        nodes.push_back({X, Y, name, des});
    }

    void delete_node(int index)
    {
        for (int i = edges.size() - 1; i >= 0; i--)
            if (edges[i].node1 == nodes[index].name || edges[i].node2 == nodes[index].name) edges.erase(edges.begin() + i);
        nodes.erase(nodes.begin() + index);
    }

    void set_node_parameters(int index, QString name, QString des)
    {
        for (int i = 0; i < (int)edges.size(); i++)
        {
            if (edges[i].node1 == nodes[index].name) edges[i].node1 = name;
            if (edges[i].node2 == nodes[index].name) edges[i].node2 = name;
        }
        nodes[index].name = name;
        nodes[index].description = des;
    }

    bool add_edge(QString node1_name, QString node2_name, int Weight = 0)
    {
        if (node1_name == node2_name) return false;
        for (int i = 0; i < (int)edges.size(); i++)
            if ((edges[i].node1 == node1_name && edges[i].node2 == node2_name) || (edges[i].node2 == node1_name && edges[i].node1 == node2_name)) return false;
        edges.push_back({node1_name, node2_name, Weight});
        return true;
    }

    void delete_edge(int index)
    {
        edges.erase(edges.begin() + index);
    }

    void set_edge_weight(int index, int weight)
    {
        edges[index].weight = weight;
    }

    Edge & edge(int index)
    {
        return edges[index];
    }

    Node & node(int index)
    {
        return  nodes[index];
    }

    Node & operator[](int index)
    {
        return nodes[index];
    }

    int get_node_index(QString name)
    {
        for (int i = 0; i < (int)nodes.size(); i++)
            if (nodes[i].name == name) return i;
        return -1;
    }

    int number_of_edges()
    {
        return (int)edges.size();
    }

    int size()
    {
        return (int)nodes.size();
    }

    int check_if_node_is_hit(int cursor_pos_x, int cursor_pos_y)
    {
        for (int i = 0; i < (int)nodes.size(); i++)
            if (abs(cursor_pos_x - nodes[i].x) <= 25 && abs(cursor_pos_y - nodes[i].y) <= 25) return i;
        return -1;
    }
};

#endif // GRAPH_H
