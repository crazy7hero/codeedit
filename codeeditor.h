/****************************************************************************
** 代码来源: Code Editor Example
** 修改说明: 添加了详细注释和中文解读
** 修改作者: [lqy]
** 修改日期: 2026
** 许可证: 遵循 Qt 的 LGPL 许可证
****************************************************************************/
#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QPainter>

class CodeEditor;

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor);
    QSize sizeHint() const override;
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    CodeEditor *codeEditor;
};

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = nullptr);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
protected:
    void resizeEvent(QResizeEvent *event) override;
private slots:
    void updateLineNumberAreaWidth();
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);
private:
    QWidget *lineNumberArea;
    friend class LineNumberArea;
};

#endif // CODEEDITOR_H
