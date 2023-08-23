// Copyright (c) 2023. Listening
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the “Software”), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#include <QWidget>

class FramelessWindowPrivate;

class FramelessWindow : public QWidget {
Q_OBJECT
public:
    explicit FramelessWindow(QWidget* parent = nullptr);

    virtual ~FramelessWindow();

    /**
     * 设置中央widget
     * @param widget
     */
    void SetCentralWidget(QWidget* widget);
    /**
     * 设置窗口边框宽度
     * 用于鼠标光标是否处于窗口边缘的宽度控制，默认窗口内外宽度为5个像素
     * @param width 宽度
     */
    void SetBorderWidth(int width);

    int GetBorderWidth();

public:
    enum WhichEdge {
        NoneEdge,
        OnLeftEdge,
        OnLeftTopEdge,
        OnTopEdge,
        OnRightTopEdge,
        OnRightEdge,
        OnRightBottomEdge,
        OnBottomEdge,
        OnLeftBottomEdge
    };

    Q_ENUM(WhichEdge)

protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

protected:
    void ChangeCursorShape(WhichEdge edge);

private:
    QScopedPointer<FramelessWindowPrivate> d_ptr;

    Q_DECLARE_PRIVATE(FramelessWindow);
};
