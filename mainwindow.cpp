#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "paintpanel.h"
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800, 600);
    this->setWindowTitle("Grapher");

    toolBar = addToolBar("Tools");
    toolBar->setEnabled(false);
    toolBar->setMovable(false);
    selectAddNodeTool = toolBar->addAction("Add Node");
    selectConnectNodesTool = toolBar->addAction("Connect Nodes");
    selectEditNodeTool = toolBar->addAction("Edit node");
    selectEditEdgeTool = toolBar->addAction("Edit edge");
    selectHighlighter = toolBar->addAction("Highlighter");

    file = new QMenu("&File", menuBar());
    newAction = file->addAction("&New");
    saveAction = file->addAction("&Save"); saveAction->setEnabled(false);
    openAction = file->addAction("&Open");
    exitAction = file->addAction("&Exit");

    newAction->setShortcut(tr("CTRL+N"));
    saveAction->setShortcut(tr("CTRL+S"));
    openAction->setShortcut(tr("CTRL+O"));

    menuBar()->addMenu(file);

    tabWidget = new TabWidget(this);
    this->setCentralWidget(tabWidget);

    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::TabCloseReq);
    connect(newAction, SIGNAL(triggered()), this, SLOT(AddNewWorkspace()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(SaveToFile()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(Open()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(Exit()));
    connect(selectAddNodeTool, SIGNAL(triggered()), this, SLOT(AddNodeToolSelected()));
    connect(selectConnectNodesTool, SIGNAL(triggered()), this, SLOT(ConnectNodesToolSelected()));
    connect(selectEditNodeTool, SIGNAL(triggered()), this, SLOT(EditNodeToolSelected()));
    connect(selectEditEdgeTool, SIGNAL(triggered()), this, SLOT(EditEdgeToolSelected()));
    connect(selectHighlighter, SIGNAL(triggered()), this, SLOT(HighlighterSelected()));
    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::CurrentTabChanged);
}

MainWindow::~MainWindow()
{
    disconnect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::TabCloseReq);
    disconnect(newAction, SIGNAL(triggered()), this, SLOT(AddNewWorkspace()));
    disconnect(saveAction, SIGNAL(triggered()), this, SLOT(SaveToFile()));
    disconnect(openAction, SIGNAL(triggered()), this, SLOT(Open()));
    disconnect(exitAction, SIGNAL(triggered()), this, SLOT(Exit()));
    disconnect(selectAddNodeTool, SIGNAL(triggered()), this, SLOT(AddNodeToolSelected()));
    disconnect(selectConnectNodesTool, SIGNAL(triggered()), this, SLOT(ConnectNodesToolSelected()));
    disconnect(selectEditNodeTool, SIGNAL(triggered()), this, SLOT(EditNodeToolSelected()));
    disconnect(selectEditEdgeTool, SIGNAL(triggered()), this, SLOT(EditEdgeToolSelected()));
    disconnect(selectHighlighter, SIGNAL(triggered()), this, SLOT(HighlighterSelected()));
    disconnect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::CurrentTabChanged);
}

void MainWindow::TabCloseReq(int index)     //при закрытии вкладки
{
    tabWidget->removeTab(index);
    if (tabWidget->count() == 0)
    {
        saveAction->setEnabled(false);
        toolBar->setEnabled(false);
    }
}

void MainWindow::Exit()     //при выборе пункта выхода
{
    this->close();
}

void MainWindow::AddNewWorkspace()      //при выборе пункта New
{
    tabWidget->addTab(new PaintPanel(), "New Tab");
    saveAction->setEnabled(true);
    toolBar->setEnabled(true);
}

void MainWindow::AddNodeToolSelected()      //выбор инструмента Add Node
{
    currently_used_tool = 1;
    PaintPanel * paintP = (PaintPanel*)tabWidget->currentWidget();
    paintP->set_current_tool(currently_used_tool);
}

void MainWindow::ConnectNodesToolSelected()     //выбор инструмента Connect nodes
{
    currently_used_tool = 2;
    PaintPanel * paintP = (PaintPanel*)tabWidget->currentWidget();
    paintP->set_current_tool(currently_used_tool);
}

void MainWindow::EditNodeToolSelected()     //выбор инструмента Edit node
{
    currently_used_tool = 3;
    PaintPanel * paintP = (PaintPanel*)tabWidget->currentWidget();
    paintP->set_current_tool(currently_used_tool);
}

void MainWindow::EditEdgeToolSelected()     //выбор инструмента Edit edge
{
    currently_used_tool = 4;
    PaintPanel * paintP = (PaintPanel*)tabWidget->currentWidget();
    paintP->set_current_tool(currently_used_tool);
}

void MainWindow::HighlighterSelected()      //выбор инструмента highlighter
{
    currently_used_tool = 5;
    PaintPanel * paintP = (PaintPanel*)tabWidget->currentWidget();
    paintP->set_current_tool(currently_used_tool);
}

void MainWindow::CurrentTabChanged()        //смена вкладки
{
    if (tabWidget->count() > 0)
    {
        PaintPanel * paintP = (PaintPanel*)tabWidget->currentWidget();
        paintP->set_current_tool(currently_used_tool);
    }
}

void MainWindow::Open()     //Открытие файла
{
    QFileDialog dialog(this, tr("Open"));
    QStringList filters;
    filters << "TXT file (*.txt)" << "XML file (*.xml)" << "JSON file (*.json)";
    dialog.setNameFilters(filters);
    if (dialog.exec() == QDialog::Accepted)
    {
        //QString selectedFilter = dialog.selectedNameFilter();
        QString filepath = dialog.selectedFiles().at(0);
        QFileInfo info(filepath);
        int new_tab_index = tabWidget->addTab(new PaintPanel(), info.fileName());
        saveAction->setEnabled(true);
        toolBar->setEnabled(true);
        tabWidget->setCurrentIndex(new_tab_index);
        PaintPanel * paintP = (PaintPanel*)tabWidget->widget(new_tab_index);
        paintP->OpenFile(filepath, info.suffix());
    }
}

void MainWindow::SaveToFile()       //сохранение файла
{
    QFileDialog dialog(this, tr("Save as ..."));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList filters;
    filters << "TXT file (*.txt)" << "XML file (*.xml)" << "JSON file (*.json)";
    dialog.setNameFilters(filters);
    if (dialog.exec() == QDialog::Accepted)
    {
        //QString selectedFilter = dialog.selectedNameFilter();
        QString filename = dialog.selectedFiles().at(0);
        PaintPanel * paintP = (PaintPanel*)tabWidget->currentWidget();
        QFileInfo info(filename);
        paintP->SaveToFile(filename, info.suffix());
        int tab_index = tabWidget->currentIndex();
        tabWidget->setTabText(tab_index, info.fileName());
    }
}

void MainWindow::PreloadFile(QString path)      //открытие файла через входной параметр
{
    QFileInfo info(path);
    int new_tab_index = tabWidget->addTab(new PaintPanel(), info.fileName());
    saveAction->setEnabled(true);
    toolBar->setEnabled(true);
    PaintPanel * paintP = (PaintPanel*)tabWidget->widget(new_tab_index);
    paintP->OpenFile(path, info.suffix());
}


