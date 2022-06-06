#ifndef NODECREATOR_H
#define NODECREATOR_H
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "Graph.h"

class NodeCreator : public QDialog
{
private:
    Graph * graph = nullptr;
    QLineEdit * nameField;
    QLineEdit * descriptionField;
    QPushButton * okButton;
    QPushButton * cancelButton;
    int newNodeX, newNodeY;

private slots:
    void OkButtonPressed();
    void CancelButtonPressed();
    void NameFieldTextChanged();

public:
    NodeCreator(QWidget * parent = nullptr);
    virtual ~NodeCreator();
    void setData(int X, int Y, Graph * gr);

};

#endif // NODECREATOR_H
