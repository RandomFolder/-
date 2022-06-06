#include "paintpanel.h"
#include "TXTHandler.h"
#include "XMLHandler.h"
#include "JSONHandler.h"
#include <QMessageBox>
#include <iostream>
#include <cmath>

PaintPanel::PaintPanel()
{
    setFocusPolicy(Qt::StrongFocus);
}

PaintPanel::~PaintPanel()
{
    //  -\_(-.-)_/-
}

float vectorLength(QPoint start, QPoint finish)  //длина вектора
{
    QPoint vector = finish - start;
    return sqrt(pow(vector.x(), 2) + pow(vector.y(), 2));
}

int dotProduct(QPoint edge_start, QPoint edge_finish, QPoint cursor_pos)  //скалярное произведение
{
    QPoint edge_vector = edge_finish - edge_start;
    QPoint cursor_vector = cursor_pos - edge_start;
    return edge_vector.x() * cursor_vector.x() + edge_vector.y() * cursor_vector.y();
}

float Height(QPoint edge_start, QPoint edge_finish, QPoint cursor_pos)  //высота
{
    int dot_product = dotProduct(edge_start, edge_finish, cursor_pos);
    float edge_vector_length = vectorLength(edge_start, edge_finish);
    float cursor_vector_length = vectorLength(edge_start, cursor_pos);

    float cosinus = dot_product / (edge_vector_length * cursor_vector_length);
    float sinus = sqrt(1 - pow(cosinus, 2));

    return sinus * cursor_vector_length;
}

bool onSegment(QPoint p, QPoint q, QPoint r)
{
    if (q.x() <= std::max(p.x(), r.x()) && q.x() >= std::min(p.x(), r.x()) && q.y() <= std::max(p.y(), r.y()) && q.y() >= std::min(p.y(), r.y())) return true;
    return false;
}

int orientation(QPoint p, QPoint q, QPoint r)
{
    int val = (q.y() - p.y()) * (r.x() - q.x()) - (q.x() - p.x()) * (r.y() - q.y());
    if (val == 0) return 0;  // коллинеарны
    return (val > 0)? 1: 2;     // по часовой стрелке или против часовой стрелки
}

bool doIntersect(QPoint p1, QPoint q1, QPoint p2, QPoint q2)
{
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // общий случай
    if (o1 != o2 && o3 != o4)
        return true;

    // Особые случаи
    // p1, q1 и p2 коллинеарны, и p2 лежит на отрезке p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 и q2 коллинеарны, и q2 лежит на отрезке p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 и p1 коллинеарны, и p1 лежит на отрезке p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

     // p2, q2 и q1 коллинеарны, и q1 лежит на отрезке p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;   //ничто из вышеперечисленного
}

bool lineIntersectsWithRect(QPoint start, QPoint end, QRect rect)
{
    QPoint top_left = QPoint(rect.left(), rect.top());
    QPoint bottom_left = QPoint(rect.left(), rect.bottom());
    QPoint top_right = QPoint(rect.right(), rect.top());
    QPoint bottom_right = QPoint(rect.right(), rect.bottom());
    return (rect.contains(start) && rect.contains(end)) || doIntersect(start, end, top_left, bottom_left) || doIntersect(start, end, top_left, top_right) || doIntersect(start, end, top_right, bottom_right) || doIntersect(start, end, bottom_right, bottom_left);
}

bool PaintPanel::nodeIsHighlighted(int index)
{
    for (int i = 0; i < (int)highlighted_nodes_indices.size(); i++)
        if (highlighted_nodes_indices[i] == index) return true;
    return false;
}

bool PaintPanel::edgeIsHighlighted(int index)
{
    for (int i = 0; i < (int)highlighted_edges_indices.size(); i++)
        if (highlighted_edges_indices[i] == index) return true;
    return false;
}

void PaintPanel::paintEvent(QPaintEvent*)
{
    QPainter painter;
    QPen pen(Qt::black, 2);
    QBrush brush(Qt::green);

    painter.begin(this);

    for (int i = 0; i < graph.number_of_edges(); i++)  //рисуем рёбра
    {
        int first_node_ind = graph.get_node_index(graph.edge(i).node1);
        int second_node_ind = graph.get_node_index(graph.edge(i).node2);
        edgeIsHighlighted(i) ? pen.setColor(Qt::blue) : pen.setColor(Qt::black);
        painter.setPen(pen);
        painter.drawLine(graph[first_node_ind].x, graph[first_node_ind].y, graph[second_node_ind].x, graph[second_node_ind].y);
        painter.drawText((graph[first_node_ind].x + graph[second_node_ind].x) / 2, (graph[first_node_ind].y + graph[second_node_ind].y) / 2, QString::number(graph.edge(i).weight));
    }
    pen.setColor(Qt::black);
    for (int i = 0; i < graph.size(); i++)  //рисуем вершины
    {
        (i == connected_nodes_ind[0]) ? (brush.setColor(Qt::red)) : (brush.setColor(Qt::green));
        if (nodeIsHighlighted(i)) brush.setColor(Qt::blue);
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.drawEllipse(QPointF(graph[i].x, graph[i].y), 25, 25);
        painter.drawText(graph[i].x, graph[i].y, graph[i].name);
    }

    pen.setWidth(1);
    painter.setPen(pen);
    brush.setColor(Qt::transparent);
    painter.setBrush(brush);
    painter.drawRect(QRect(left_top_corner, right_bottom_corner)); //рисуем выделяющий прямоугольник
    painter.end();
}

void PaintPanel::addNodeTool(QMouseEvent *evt)  //инструмент для добавления и удаления вершин
{
    if (evt->button() == Qt::LeftButton)
    {
        int hit_node_index = graph.check_if_node_is_hit(evt->x(), evt->y());

        if (hit_node_index == -1)
        {
            NodeCreator nodeCreator(this);
            nodeCreator.setData(evt->x(), evt->y(), &graph);
            nodeCreator.setModal(true);
            nodeCreator.exec();
            repaint();
        }
        else active_node_index = hit_node_index;
    }
    else if (evt->button() == Qt::RightButton)
    {
        int hit_node_index = graph.check_if_node_is_hit(evt->x(), evt->y());
        if (hit_node_index != -1)
        {
            graph.delete_node(hit_node_index);
            repaint();
        }
    }
}

void PaintPanel::connectNodesTool(QMouseEvent *evt)  //инструмент для добавления и удаления рёбер
{
    if (evt->button() == Qt::LeftButton)
    {
        int hit_node_index = graph.check_if_node_is_hit(evt->x(), evt->y());

        if (hit_node_index != -1)
        {
            if (connected_nodes_ind[0] == -1)
            {
                connected_nodes_ind[0] = hit_node_index;
                repaint();
            }
            else
            {
                QString first_node_name = graph[connected_nodes_ind[0]].name;
                QString second_node_name = graph[hit_node_index].name;
                if (!graph.add_edge(first_node_name, second_node_name))
                    QMessageBox::critical(this, "Error", "Attempt to either create a hinge or add an already existing edge");
                connected_nodes_ind[0] = -1;
                connected_nodes_ind[1] = -1;
                repaint();
            }
        }
    }
    else if (evt->button() == Qt::RightButton)
    {
        for (int i = graph.number_of_edges() - 1; i >= 0; i--)
        {
            int node1_index = graph.get_node_index(graph.edge(i).node1);
            int node2_index = graph.get_node_index(graph.edge(i).node2);
            Node node1 = graph[node1_index];
            Node node2 = graph[node2_index];
            int cursorX = evt->x();
            int cursorY = evt->y();

            if (Height(QPoint(node1.x, node1.y), QPoint(node2.x, node2.y), QPoint(cursorX, cursorY)) <= 10.f && ((cursorX >= node1.x && cursorX <= node2.x) || (cursorX >= node2.x && cursorX <= node1.x)) && ((cursorY >= node1.y && cursorY <= node2.y) || (cursorY >= node2.y && cursorY <= node1.y)))
            {
                graph.delete_edge(i);
                repaint();
                break;
            }
        }
    }
}

void PaintPanel::editNodeTool(QMouseEvent *evt)     //инструмент для редактирования параметров узла
{
    if (evt->button() == Qt::LeftButton)
    {
        int hit_node_index = graph.check_if_node_is_hit(evt->x(), evt->y());

        if (hit_node_index != -1)
        {
            NodeEditor node_editor(this);
            node_editor.setData(hit_node_index, &graph);
            node_editor.setModal(true);
            node_editor.exec();
            repaint();
        }
    }
}

void PaintPanel::editEdgeTool(QMouseEvent *evt)     //инструмент для редактирования параметров ребра
{
    if (evt->button() == Qt::LeftButton)
    {
        int hit_edge_index = -1;
        for (int i = 0; i < graph.number_of_edges(); i++)
        {
            int node1_index = graph.get_node_index(graph.edge(i).node1);
            int node2_index = graph.get_node_index(graph.edge(i).node2);
            Node node1 = graph[node1_index];
            Node node2 = graph[node2_index];
            int cursorX = evt->x();
            int cursorY = evt->y();

            if (Height(QPoint(node1.x, node1.y), QPoint(node2.x, node2.y), QPoint(cursorX, cursorY)) <= 10.f && ((cursorX >= node1.x && cursorX <= node2.x) || (cursorX >= node2.x && cursorX <= node1.x)) && ((cursorY >= node1.y && cursorY <= node2.y) || (cursorY >= node2.y && cursorY <= node1.y)))
            {
                hit_edge_index = i;
                break;
            }
        }

        if (hit_edge_index != -1)
        {
            EdgeEditor edge_editor(this);
            edge_editor.setData(hit_edge_index, &graph);
            edge_editor.setModal(true);
            edge_editor.exec();
            repaint();
        }
    }
}

void PaintPanel::highlighterTool(QMouseEvent *evt)      //инструмент для выделения узлов и рёбер
{
    if (evt->button() == Qt::LeftButton)
    {
        highlighted_edges_indices.clear();
        highlighted_nodes_indices.clear();
        left_top_corner = QPoint(evt->x(), evt->y());
        right_bottom_corner = QPoint(evt->x(), evt->y());
        repaint();
    }
}

void PaintPanel::SaveToFile(QString path, QString extension)    //Сохранение файла
{
    if (extension == "txt")
    {
        TXTHandler txt_handler(path);
        txt_handler.Write(graph);
    }
    if (extension == "xml")
    {
        XMLHandler xml_handler(path);
        xml_handler.Write(graph);
    }
    if (extension == "json")
    {
        JSONHandler json_handler(path);
        json_handler.Write(graph);
    }
}

void PaintPanel::OpenFile(QString path, QString extension)  //Чтение файла
{
    if (extension == "txt")
    {
        TXTHandler txt_handler(path);
        txt_handler.Read(graph);
    }
    if (extension == "xml")
    {
        XMLHandler xml_handler(path);
        xml_handler.Read(graph);
    }
    if (extension == "json")
    {
        JSONHandler json_handler(path);
        json_handler.Read(graph);
    }
}

void PaintPanel::mousePressEvent(QMouseEvent *evt)  //при нажатии мыши
{
    switch(current_tool)
    {
        case 1: addNodeTool(evt); break;
        case 2: connectNodesTool(evt); break;
        case 3: editNodeTool(evt); break;
        case 4: editEdgeTool(evt); break;
        case 5: highlighterTool(evt); break;
    }
}

void PaintPanel::mouseMoveEvent(QMouseEvent *evt)   //при перетаскивании мыши
{
    switch(current_tool)
    {
    case 1:
        if (active_node_index > -1)
        {
            graph[active_node_index].x = evt->x();
            graph[active_node_index].y = evt->y();
            for (int i = 0; i < graph.size(); i++)
                if (abs(graph[i].x - graph[active_node_index].x) < 50 && abs(graph[i].y - graph[active_node_index].y) < 50 && i != active_node_index)   //если активная вершина находится близко к любой другой вершине
                {
                    int offset_x = graph[i].x - graph[active_node_index].x;
                    int offset_y = graph[i].y - graph[active_node_index].y;
                    graph[i].x += offset_x;
                    graph[i].y += offset_y;
                }
            repaint();
        }
        break;
    case 5:
        right_bottom_corner = QPoint(evt->x(), evt->y());
        repaint();
        break;
    }
}

void PaintPanel::mouseReleaseEvent(QMouseEvent*)    //когда мышь отпустили
{
    for (int i = 0; i < graph.size(); i++)
        if (graph[i].x + 25 >= std::min(left_top_corner.x(), right_bottom_corner.x()) && graph[i].x - 25 <= std::max(left_top_corner.x(), right_bottom_corner.x()) && graph[i].y + 25 >= std::min(left_top_corner.y(), right_bottom_corner.y()) && graph[i].y - 25 <= std::max(left_top_corner.y(), right_bottom_corner.y()) && !nodeIsHighlighted(i))
            highlighted_nodes_indices.push_back(i);

    for (int i = 0; i < graph.number_of_edges(); i++)
    {
        int first_node_ind = graph.get_node_index(graph.edge(i).node1);
        int second_node_ind = graph.get_node_index(graph.edge(i).node2);
        if (lineIntersectsWithRect(QPoint(graph[first_node_ind].x, graph[first_node_ind].y), QPoint(graph[second_node_ind].x, graph[second_node_ind].y), QRect(left_top_corner, right_bottom_corner)) && !edgeIsHighlighted(i)) highlighted_edges_indices.push_back(i);
    }

    active_node_index = -1;
    left_top_corner = QPoint(-100, -100);
    right_bottom_corner = QPoint(-100, -100);
    repaint();
}

void PaintPanel::keyPressEvent(QKeyEvent *evt)      // нажатие клавиши
{
    if (evt->key() == Qt::Key_D)
    {
        for (int i = (int)highlighted_edges_indices.size() - 1; i >= 0; i--) graph.delete_edge(highlighted_edges_indices[i]);
        for (int i = (int)highlighted_nodes_indices.size() - 1; i >= 0; i--) graph.delete_node(highlighted_nodes_indices[i]);
        highlighted_edges_indices.clear();
        highlighted_nodes_indices.clear();
        repaint();
    }
}

void PaintPanel::set_current_tool(int tool)
{
    current_tool = tool;
    active_node_index = -1;
    connected_nodes_ind[0] = -1;
    connected_nodes_ind[1] = -1;
    highlighted_edges_indices.clear();
    highlighted_nodes_indices.clear();
    repaint();
}
