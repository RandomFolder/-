#ifndef JSONHANDLER_H
#define JSONHANDLER_H
#include <QFile>
#include <QMessageBox>
#include <QByteArray>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "Graph.h"

class JSONHandler
{
private:
    QString filepath;
public:
    JSONHandler(QString path)
    {
        filepath = path;
    }

    void Write(Graph & gr)
    {
        QFile saveFile(filepath);
        if (!saveFile.open(QIODevice::WriteOnly))
        {
            QMessageBox::critical(0, "Error", "Couldn't create a file");
            return;
        }
        QJsonDocument savedContent;

        QJsonObject content;

        QJsonArray nodes, edges;

        for (int i = 0; i < (int)gr.size(); i++)
        {
            QJsonObject node;
            node.insert("name", gr[i].name);
            node.insert("X", gr[i].x);
            node.insert("Y", gr[i].y);
            node.insert("description", gr[i].description);
            nodes.append(node);
        }

        content.insert("Nodes", nodes);

        for (int i = 0; i < (int)gr.number_of_edges(); i++)
        {
            QJsonObject edge;
            edge.insert("node1", gr.edge(i).node1);
            edge.insert("node2", gr.edge(i).node2);
            edge.insert("weight", gr.edge(i).weight);
            edges.append(edge);
        }

        content.insert("Edges", edges);

        savedContent.setObject(content);

        saveFile.write(savedContent.toJson());
        saveFile.close();
    }

    void Read(Graph & gr)
    {
        QFile openFile(filepath);
        if (!openFile.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(0, "Error", "Couldn't open a file");
            return;
        }

        QByteArray jsonData = openFile.readAll();
        openFile.close();

        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        QJsonObject content = doc.object();

        QJsonValue value1 = content.value("Nodes");
        QJsonArray nodes = value1.toArray();
        for (int i = 0; i < nodes.size(); i++)
        {
            int X = nodes[i].toObject().value("X").toInt();
            int Y = nodes[i].toObject().value("Y").toInt();
            QString Name = nodes[i].toObject().value("name").toString();
            QString des = nodes[i].toObject().value("description").toString();
            gr.add_node(X, Y, Name, des);
        }

        QJsonValue value2 = content.value("Edges");
        QJsonArray edges = value2.toArray();
        for (int i = 0; i < edges.size(); i++)
        {
            QString node1 = edges[i].toObject().value("node1").toString();
            QString node2 = edges[i].toObject().value("node2").toString();
            int Weight = edges[i].toObject().value("weight").toInt();
            gr.add_edge(node1, node2, Weight);
        }
    }
};

#endif // JSONHANDLER_H
