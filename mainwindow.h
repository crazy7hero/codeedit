#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "codeeditor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
private slots:
    void loadFile();
    void saveFile();
    void clearEditor();
    void updateStatusBar();
private:
    CodeEditor *editor;
};

#endif // MAINWINDOW_H
