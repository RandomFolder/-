#ifndef EDGEEDITOR_H
#define EDGEEDITOR_H
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "Graph.h"

class EdgeEditor : public QDialog
{
private:
    int ind;
    Graph * graph = nullptr;
    QLineEdit * weightField;
    QPushButton * okButton;
    QPushButton * cancelButton;

private slots:
    void OkButtonPressed();
    void CancelButtonPressed();
    void WeightFieldValueChanged();

public:
    EdgeEditor(QWidget * parent = nullptr);
    virtual ~EdgeEditor();
    void setData(int index, Graph * gr);
};

#endif // EDGEEDITOR_H
