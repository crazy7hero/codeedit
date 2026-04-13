#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QStatusBar>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Code Editor Demo - with Line Numbers");
    resize(800, 600);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QWidget *toolBar = new QWidget();
    QHBoxLayout *toolLayout = new QHBoxLayout(toolBar);

    QPushButton *loadBtn = new QPushButton("Open");
    QPushButton *saveBtn = new QPushButton("Save");
    QPushButton *clearBtn = new QPushButton("Clear");
    QLabel *infoLabel = new QLabel("Current line is highlighted (yellow)");

    toolLayout->addWidget(loadBtn);
    toolLayout->addWidget(saveBtn);
    toolLayout->addWidget(clearBtn);
    toolLayout->addStretch();
    toolLayout->addWidget(infoLabel);

    editor = new CodeEditor(this);

    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::loadFile);
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::saveFile);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::clearEditor);
    connect(editor, &CodeEditor::cursorPositionChanged, this, &MainWindow::updateStatusBar);

    layout->addWidget(toolBar);
    layout->addWidget(editor);

    setCentralWidget(centralWidget);

    statusBar()->showMessage("Ready");
    updateStatusBar();
}

void MainWindow::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "",
                                                    "Text Files (*.txt *.cpp *.h *.py);;All Files (*.*)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            editor->setPlainText(stream.readAll());
            file.close();
            statusBar()->showMessage("Loaded: " + fileName, 3000);
        } else {
            QMessageBox::warning(this, "Error", "Cannot open file: " + fileName);
        }
    }
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "",
                                                    "Text Files (*.txt);;C++ Files (*.cpp);;Header Files (*.h);;Python Files (*.py);;All Files (*.*)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << editor->toPlainText();
            file.close();
            statusBar()->showMessage("Saved: " + fileName, 3000);
        } else {
            QMessageBox::warning(this, "Error", "Cannot save file: " + fileName);
        }
    }
}

void MainWindow::clearEditor()
{
    editor->clear();
    statusBar()->showMessage("Cleared", 2000);
}

void MainWindow::updateStatusBar()
{
    QTextCursor cursor = editor->textCursor();
    int line = cursor.blockNumber() + 1;
    int col = cursor.columnNumber() + 1;
    statusBar()->showMessage(QString("Line: %1  Column: %2").arg(line).arg(col), 2000);
}
