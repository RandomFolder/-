#ifndef NODEEDITOR_H
#define NODEEDITOR_H
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "Graph.h"

class NodeEditor : public QDialog
{
private:
    int ind;
    Graph * graph = nullptr;
    QLineEdit * nameField;
    QLineEdit * descriptionField;
    QPushButton * okButton;
    QPushButton * cancelButton;
    QString defaultText;

private slots:
    void OkButtonPressed();
    void CancelButtonPressed();
    void NameFieldTextChanged();

public:
    NodeEditor(QWidget * parent = nullptr);
    virtual ~NodeEditor();
    void setData(int index, Graph * gr);
};

#endif // NODEEDITOR_H
