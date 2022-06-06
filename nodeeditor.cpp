#include "nodeeditor.h"

NodeEditor::NodeEditor(QWidget * parent) : QDialog(parent)
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
    okButton->setDisabled(false);
    cancelButton = new QPushButton("Cancel", this);
    cancelButton->move(100, 70);

    connect(nameField, &QLineEdit::textChanged, this, &NodeEditor::NameFieldTextChanged);
    connect(okButton, &QPushButton::clicked, this, &NodeEditor::OkButtonPressed);
    connect(cancelButton, &QPushButton::clicked, this, &NodeEditor::CancelButtonPressed);
}

NodeEditor::~NodeEditor()
{
    disconnect(okButton, &QPushButton::clicked, this, &NodeEditor::OkButtonPressed);
    disconnect(cancelButton, &QPushButton::clicked, this, &NodeEditor::CancelButtonPressed);
    disconnect(nameField, &QLineEdit::textChanged, this, &NodeEditor::NameFieldTextChanged);
}

void NodeEditor::setData(int index, Graph *gr)  //входные параметры
{
    ind = index;
    graph = gr;
    defaultText = graph->node(index).name;
    nameField->setText(graph->node(index).name);
    descriptionField->setText(graph->node(index).description);
}

void NodeEditor::OkButtonPressed()
{
    graph->set_node_parameters(ind, nameField->text(), descriptionField->text());
    this->close();
}

void NodeEditor::CancelButtonPressed()
{
    this->close();
}

void NodeEditor::NameFieldTextChanged()
{
    if (nameField->text() == "" || (graph->get_node_index(nameField->text()) != -1 && nameField->text() != defaultText)) okButton->setDisabled(true);
    else okButton->setDisabled(false);
}
