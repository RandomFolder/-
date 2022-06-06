#ifndef PAINTPANEL_H
#define PAINTPANEL_H
#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include <QMouseEvent>
#include "Graph.h"
#include "nodecreator.h"
#include "nodeeditor.h"
#include "edgeeditor.h"
#include <vector>
#include <iostream>

class PaintPanel : public QWidget
{
    Q_OBJECT

private:
    Graph graph;
    int active_node_index = -1;
    int connected_nodes_ind[2] = {-1, -1};
    int current_tool = 1;
    std::vector<int> highlighted_nodes_indices, highlighted_edges_indices;
    QPoint left_top_corner = QPoint(-100, -100), right_bottom_corner = QPoint(-100, -100);

    void addNodeTool(QMouseEvent * evt);
    void connectNodesTool(QMouseEvent * evt);
    void editNodeTool(QMouseEvent * evt);
    void editEdgeTool(QMouseEvent * evt);
    void highlighterTool(QMouseEvent * evt);
    bool nodeIsHighlighted(int index);
    bool edgeIsHighlighted(int index);

public:
    PaintPanel();
    ~PaintPanel();
    void set_current_tool(int tool);
    void SaveToFile(QString path, QString extension);
    void OpenFile(QString path, QString extension);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* evt) override;
    void mouseMoveEvent(QMouseEvent* evt) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void keyPressEvent(QKeyEvent * evt) override;
};

#endif // PAINTPANEL_H
