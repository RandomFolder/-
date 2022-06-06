#ifndef XMLHANDLER_H
#define XMLHANDLER_H
#include <QtCore>
#include <QtXml>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDebug>
#include <QMessageBox>
#include "Graph.h"

class XMLHandler
{
private:
    QString filepath;
public:
    XMLHandler(QString path)
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

        QXmlStreamWriter xmlWriter(&saveFile);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();

        xmlWriter.writeStartElement("root");

        xmlWriter.writeStartElement("Nodes");
        for (int i = 0; i < gr.size(); i++)
        {
            xmlWriter.writeStartElement("Node");
            xmlWriter.writeTextElement("Name", gr[i].name);
            xmlWriter.writeTextElement("X", QString::number(gr[i].x));
            xmlWriter.writeTextElement("Y", QString::number(gr[i].y));
            xmlWriter.writeTextElement("Description", gr[i].description);
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("Edges");
        for (int i = 0; i < gr.number_of_edges(); i++)
        {
            xmlWriter.writeStartElement("Edge");
            xmlWriter.writeTextElement("Node1", gr.edge(i).node1);
            xmlWriter.writeTextElement("Node2", gr.edge(i).node2);
            xmlWriter.writeTextElement("Weight", QString::number(gr.edge(i).weight));
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();

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

        QXmlStreamReader xmlReader(&openFile);

        if (xmlReader.readNextStartElement())
        {
            if (xmlReader.name() == "root")
            {
                while (xmlReader.readNextStartElement())
                {
                    if (xmlReader.name() == "Nodes")
                    {
                        while (xmlReader.readNextStartElement())
                        {
                            if (xmlReader.name() == "Node")
                            {
                                QString name, des;
                                int X, Y;
                                while (xmlReader.readNextStartElement())
                                {
                                    if (xmlReader.name() == "Name") name = xmlReader.readElementText();
                                    if (xmlReader.name() == "X") X = xmlReader.readElementText().toInt();
                                    if (xmlReader.name() == "Y") Y = xmlReader.readElementText().toInt();
                                    if (xmlReader.name() == "Description") des = xmlReader.readElementText();
                                }
                                gr.add_node(X, Y, name, des);
                            }
                        }
                    }
                    else if (xmlReader.name() == "Edges")
                    {
                        while (xmlReader.readNextStartElement())
                        {
                            if (xmlReader.name() == "Edge")
                            {
                                QString node1, node2;
                                int Weight;
                                while (xmlReader.readNextStartElement())
                                {
                                    if (xmlReader.name() == "Node1") node1 = xmlReader.readElementText();
                                    if (xmlReader.name() == "Node2") node2 = xmlReader.readElementText();
                                    if (xmlReader.name() == "Weight") Weight = xmlReader.readElementText().toInt();
                                }
                                gr.add_edge(node1, node2, Weight);
                            }
                        }
                    }
                }
            }
        }

        openFile.close();
    }
};

#endif // XMLHANDLER_H
