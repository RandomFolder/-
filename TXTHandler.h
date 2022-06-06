#ifndef TXTHANDLER_H
#define TXTHANDLER_H
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "Graph.h"

class TXTHandler
{
private:
    QString filepath;
public:
    TXTHandler(QString path)
    {
        filepath = path;
    }

    void Write(Graph & gr)
    {
        QFile saveFile(filepath);

        if (!saveFile.open(QFile::WriteOnly | QFile::Text))
        {
            QMessageBox::critical(0, "Error", "Couldn't create a file");
            return;
        }

        QTextStream out(&saveFile);
        out << gr.size() << '\n';
        for (int i = 0; i < gr.size(); i++)
            out << gr[i].name << ':' << gr[i].x << ':' << gr[i].y << ':' << gr[i].description << '\n';

        for (int i = 0; i < gr.number_of_edges(); i++)
            out << gr.edge(i).node1 << ':' << gr.edge(i).node2 << ':' << gr.edge(i).weight << '\n';

        saveFile.flush();
        saveFile.close();
    }

    void Read(Graph & gr)
    {
        int index = -1, number_of_nodes = 0;
        QFile openFile(filepath);

        if (!openFile.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::critical(0, "Error", "Couldn't open a file");
            return;
        }

        QTextStream in(&openFile);

        while (!in.atEnd())
        {
            if (index == -1)
            {
                number_of_nodes = in.readLine().toInt();
                index++;
            }
            else if (index < number_of_nodes && number_of_nodes > 0)
            {
                QString text = in.readLine();
                QStringList data = text.split(':');
                gr.add_node(data[1].toInt(), data[2].toInt(), data[0], data[3]);
                index++;
            }
            else
            {
                QString text = in.readLine();
                QStringList data = text.split(':');
                gr.add_edge(data[0], data[1], data[2].toInt());
            }
        }
    }
};

#endif // TXTHANDLER_H
