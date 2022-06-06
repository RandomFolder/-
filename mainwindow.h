#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include "tabwidget.h"
#include "paintpanel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void PreloadFile(QString path);

private slots:
    void TabCloseReq(int index);
    void Exit();
    void AddNewWorkspace();
    void Open();
    void SaveToFile();
    void AddNodeToolSelected();
    void ConnectNodesToolSelected();
    void EditNodeToolSelected();
    void EditEdgeToolSelected();
    void HighlighterSelected();
    void CurrentTabChanged();

private:
    int currently_used_tool = 1;
    QMenu * file;
    TabWidget * tabWidget;
    QToolBar * toolBar;
    QAction * newAction;
    QAction * saveAction;
    QAction * openAction;
    QAction * exitAction;
    QAction * selectAddNodeTool;
    QAction * selectConnectNodesTool;
    QAction * selectEditNodeTool;
    QAction * selectEditEdgeTool;
    QAction * selectHighlighter;
};

#endif // MAINWINDOW_H
