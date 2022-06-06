#include "nodecreator.h"

NodeCreator::NodeCreator(QWidget * parent) : QDialog(parent)
{
    this->setFixedSize(200, 100);
    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    nameField = new QLineEdit(this);
    nameField->setMaxLength(3);     //в названии вершины может быть максимум 3 символа
    nameField->move(10, 10);
    nameField->setPlaceholderText("Node's name");

    descriptionField = new QLineEdit(this);
    descriptionField->move(10, 40);
    descriptionField->setPlaceholderText("Node's description");

    okButton = new QPushButton("Ok", this);
    okButton->move(10, 70);
    okButton->setDisabled(true);
    cancelButton = new QPushButton("Cancel", this);
    cancelButton->move(100, 70);

    connect(nameField, &QLineEdit::textChanged, this, &NodeCreator::NameFieldTextChanged);
    connect(okButton, &QPushButton::clicked, this, &NodeCreator::OkButtonPressed);
    connect(cancelButton, &QPushButton::clicked, this, &NodeCreator::CancelButtonPressed);
}

NodeCreator::~NodeCreator()
{
    disconnect(okButton, &QPushButton::clicked, this, &NodeCreator::OkButtonPressed);
    disconnect(cancelButton, &QPushButton::clicked, this, &NodeCreator::CancelButtonPressed);
    disconnect(nameField, &QLineEdit::textChanged, this, &NodeCreator::NameFieldTextChanged);
}

void NodeCreator::setData(int X, int Y, Graph *gr)  //входные параметры
{
    graph = gr;
    newNodeX = X;
    newNodeY = Y;
}

void NodeCreator::OkButtonPressed()
{
    graph->add_node(newNodeX, newNodeY, nameField->text(), descriptionField->text());
    this->close();
}

void NodeCreator::CancelButtonPressed()
{
    this->close();
}

void NodeCreator::NameFieldTextChanged()
{
    if (nameField->text() == "" || graph->get_node_index(nameField->text()) != -1) okButton->setDisabled(true);
    else okButton->setDisabled(false);
}
