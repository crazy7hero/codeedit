/****************************************************************************
** 代码来源: Code Editor Example
** 修改说明: 添加了详细注释和中文解读
** 修改作者: [lqy]
** 修改日期: 2026
** 许可证: 遵循 Qt 的 LGPL 许可证
****************************************************************************/
#include "codeeditor.h"
#include <QFont>
#include <QTextEdit>
#include <QTextBlock>
#include <QTextFormat>
#include <QDebug>

LineNumberArea::LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor) {}

QSize LineNumberArea::sizeHint() const
{
    return QSize(codeEditor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    codeEditor->lineNumberAreaPaintEvent(event);
}

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    /*blockCountChanged 和cursorPositionChanged有点类似
    **确保所有情况都被覆盖需要添加两种// updateRequest 能处理：
    **滚动导致的显示更新
    **编辑导致的局部更新
    **整体视图变化

    **updateRequest 不能可靠处理：
    **行数变化但不触发整体视图更新的情况
    **行数从 999→1000（需要增加宽度）
    **在视图外部插入/删除行
    */

    //当文档中的文本块（段落/行）数量改变时发出更新行区域
    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    //当行数改变时，重新计算行号区域宽度
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    //光标选择对进行整行高亮
    connect(this, &CodeEditor::cursorPositionChanged , this, &CodeEditor::highlightCurrentLine);

     //QFont font("Courier New", 12);
     //setFont(font);

     // 禁用自动换行
     setLineWrapMode(QPlainTextEdit::NoWrap);

     //  垂直滚动条：按需显示
     setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

     //  水平滚动条：始终显示（或按需显示）
     setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
     // 或者始终显示：setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

}

//添加行号处理
void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    //行号的矩形框的填充颜色
    painter.fillRect(event->rect(), QColor(Qt::lightGray).lighter(120));
    //行号字体颜色
    painter.setPen(QColor(Qt::gray));
    /*核心代码
     * 获取当前在屏幕上显示的第一个文本块( firstVisibleBlock屏幕上第一行是什么？)
     * 获取这个文本块的编号 (从0开始） (blockNumber 它在文档中是第几行？)
     * 获取这个文本块的编号+1,画在左边
     */
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());


    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignHCenter, number); //居中显示行号 其实向右对齐比较好看 模仿notepad++
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

//行号的宽度设置
int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    //行号的宽度设置
    //加上左右边距（3像素）
    //数字'9'的宽度（最宽的数字）
    //horizontalAdvance() 返回字符的宽度（像素）
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

//当编辑器窗口大小改变时，同步调整行号区域的大小和位置。
void CodeEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::updateLineNumberAreaWidth()
{
    //设置文本编辑区域（视口）的边距，为行号区域留出空间
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        //创建额外选择列表 ExtraSelection 用于临时改变文本的显示效果
        QTextEdit::ExtraSelection selection;
        //lighter(160) 使颜色变亮 60%
        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        //表示高亮整行，不只是文字部分
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}

//保持行号区域和文本编辑区域同步
// 1. 当编辑器滚动时，让行号区域跟着滚动
// 2. 当编辑器内容更新时，刷新行号区域
// 3. 当整体视图改变时，重新计算行号区域宽度
void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    //还有一个间接用处，监视字体变化
    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth();
}
