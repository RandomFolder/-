#include "edgeeditor.h"
#include <QIntValidator>

EdgeEditor::EdgeEditor(QWidget * parent) : QDialog(parent)
{
    this->setFixedSize(200, 80);
    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    weightField = new QLineEdit(this);
    weightField->setValidator(new QIntValidator(0, 1000, this));    //можно вводить только числа
    weightField->move(10, 10);
    weightField->setPlaceholderText("Weight");

    okButton = new QPushButton("Ok", this);
    okButton->move(10, 50);
    okButton->setDisabled(false);
    cancelButton = new QPushButton("Cancel", this);
    cancelButton->move(100, 50);

    connect(weightField, &QLineEdit::textChanged, this, &EdgeEditor::WeightFieldValueChanged);
    connect(okButton, &QPushButton::clicked, this, &EdgeEditor::OkButtonPressed);
    connect(cancelButton, &QPushButton::clicked, this, &EdgeEditor::CancelButtonPressed);
}

EdgeEditor::~EdgeEditor()
{
    disconnect(weightField, &QLineEdit::textChanged, this, &EdgeEditor::WeightFieldValueChanged);
    disconnect(okButton, &QPushButton::clicked, this, &EdgeEditor::OkButtonPressed);
    disconnect(cancelButton, &QPushButton::clicked, this, &EdgeEditor::CancelButtonPressed);
}

void EdgeEditor::setData(int index, Graph *gr)
{
    ind = index;
    graph = gr;
    weightField->setText(QString::number(graph->edge(index).weight));
}

void EdgeEditor::OkButtonPressed()
{
    graph->set_edge_weight(ind, weightField->text().toInt());
    this->close();
}

void EdgeEditor::CancelButtonPressed()
{
    this->close();
}

void EdgeEditor::WeightFieldValueChanged()
{
    int value = weightField->text().toInt();
    if (value < 0) okButton->setDisabled(true);
    else okButton->setDisabled(false);
}
