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

#include "FramelessWindow.h"
#include <QDebug>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QWindow>
#include <QGuiApplication>

class FramelessWindowPrivate
{
public:
	FramelessWindowPrivate(FramelessWindow *parent)
		: q_ptr(parent)
	{}

	using WhichEdge = FramelessWindow::WhichEdge;

	/**
	 * 计算鼠标是否位于左、上、右、下、左上角、左下角、右上角、右下角
	 * @return
	 */
	[[nodiscard]] WhichEdge CursorPosCalculator(const QPoint &mousePos,
												const QRect &frameRect) const;

private:
	FramelessWindow *const q_ptr;

	Q_DECLARE_PUBLIC(FramelessWindow);

	int borderWidth{5};
	bool isResizing{false};
	FramelessWindow::WhichEdge whichEdge;
};

FramelessWindow::WhichEdge
FramelessWindowPrivate::CursorPosCalculator(const QPoint &mousePos,
											const QRect &frameRect) const
{
	int mouseX = mousePos.x();
	int mouseY = mousePos.y();

	int frameX = frameRect.x();
	int frameY = frameRect.y();

	int frameWidth = frameRect.width();
	int frameHeight = frameRect.height();

	WhichEdge edge{WhichEdge::NoneEdge};
	bool left{false};
	bool right{false};
	int top{false};
	int bottom{false};

	if (mouseX >= frameX - borderWidth && mouseX <= frameX + borderWidth) {
		left = true;
		edge = WhichEdge::OnLeftEdge;
	}
	if (mouseX >= frameX + frameWidth - borderWidth &&
		mouseX <= frameX + frameWidth + borderWidth) {
		right = true;
		edge = WhichEdge::OnRightEdge;
	}
	if (mouseY >= frameY - borderWidth && mouseY <= frameY + borderWidth) {
		top = true;
		edge = WhichEdge::OnTopEdge;
	}
	if (mouseY >= frameY + frameHeight - borderWidth &&
		mouseY <= frameY + frameHeight + borderWidth) {
		bottom = true;
		edge = WhichEdge::OnBottomEdge;
	}

	if (left && top)
		edge = WhichEdge::OnLeftTopEdge;
	else if (right && top)
		edge = WhichEdge::OnRightTopEdge;
	else if (right && bottom)
		edge = WhichEdge::OnRightBottomEdge;
	else if (left && bottom)
		edge = WhichEdge::OnLeftBottomEdge;

	return edge;
}

FramelessWindow::FramelessWindow(QWidget *parent)
	: QWidget(parent), d_ptr(new FramelessWindowPrivate(this))
{
	setWindowFlags(Qt::FramelessWindowHint);

	setMouseTracking(true);
	auto layout = new QVBoxLayout(this);
	layout->setContentsMargins(1, 1, 1, 1);
}

FramelessWindow::~FramelessWindow() = default;

void FramelessWindow::ChangeCursorShape(FramelessWindow::WhichEdge edge)
{
	switch (edge) {
	case WhichEdge::OnLeftTopEdge:
	case WhichEdge::OnRightBottomEdge: {
		setCursor(Qt::CursorShape::SizeFDiagCursor);
		break;
	}
	case WhichEdge::OnLeftBottomEdge:
	case WhichEdge::OnRightTopEdge: {
		setCursor(Qt::CursorShape::SizeBDiagCursor);
		break;
	}
	case WhichEdge::OnLeftEdge:
	case WhichEdge::OnRightEdge: {
		setCursor(Qt::CursorShape::SizeHorCursor);
		break;
	}
	case WhichEdge::OnTopEdge:
	case WhichEdge::OnBottomEdge: {
		setCursor(Qt::CursorShape::SizeVerCursor);
		break;
	}
	default:
		setCursor(Qt::CursorShape::ArrowCursor);
		break;
	}
}

void FramelessWindow::mousePressEvent(QMouseEvent *event)
{
	Q_D(FramelessWindow);

	if (event->button() == Qt::MouseButton::LeftButton) {
		d->whichEdge =
			d->CursorPosCalculator(event->globalPos(), this->frameGeometry());
		if (!window()->isMaximized() && d->whichEdge != WhichEdge::NoneEdge) {
			d->isResizing = true;
			qDebug() << __func__ << ":" << __LINE__ << d->whichEdge << d->isResizing;
		}
	}

	QWidget::mousePressEvent(event);
}

// TODO: 向下缩放窗口时，会有抖动
void FramelessWindow::mouseMoveEvent(QMouseEvent *event)
{
	Q_D(FramelessWindow);


//    // Windows 11上若使用Qt::CustomizeWindowHint标志则不需要自行设置光标及窗口大小
//    if (window()->windowFlags() & Qt::CustomizeWindowHint) {
//        return QWidget::mouseMoveEvent(event);
//    }
	if (!window()->isMaximized()) {
		auto isEdge =
			d->CursorPosCalculator(event->globalPos(), this->frameGeometry());
		ChangeCursorShape(isEdge);
	}


	if (!window()->isMaximized()) {
		auto isEdge =
			d->CursorPosCalculator(event->globalPos(), this->frameGeometry());
		ChangeCursorShape(isEdge);
	}


	if ((event->buttons() & Qt::LeftButton) && d->isResizing) {
		qDebug() << __func__ << ":" << __LINE__ << "startSystemResize";

		this->windowHandle()->startSystemResize(CovertEdgeEnum(d->whichEdge));
		// https://bugreports.qt.io/browse/QTBUG-102488
		QMouseEvent e(
			QEvent::MouseButtonRelease,
			window()->windowHandle()->mapFromGlobal(QCursor::pos()),
			Qt::LeftButton,
			QGuiApplication::mouseButtons() ^ Qt::LeftButton,
			QGuiApplication::keyboardModifiers());
		QGuiApplication::sendEvent(window()->windowHandle(), &e);
	}
	QWidget::mouseMoveEvent(event);
}

void FramelessWindow::mouseReleaseEvent(QMouseEvent *event)
{
	Q_D(FramelessWindow);

	d->isResizing = false;
	d->whichEdge = WhichEdge::NoneEdge;
	QWidget::mouseMoveEvent(event);
}

void FramelessWindow::SetBorderWidth(int width)
{
	Q_D(FramelessWindow);
	d->borderWidth = width;
}

int FramelessWindow::GetBorderWidth()
{
	Q_D(FramelessWindow);
	return d->borderWidth;
}

void FramelessWindow::SetCentralWidget(QWidget *widget)
{
	layout()->addWidget(widget);

}
Qt::Edge FramelessWindow::CovertEdgeEnum(FramelessWindow::WhichEdge edge)
{
	Qt::Edge e;
	switch (edge) {
	case WhichEdge::OnLeftTopEdge: {

		break;
	}
	case WhichEdge::OnRightBottomEdge: {
		break;
	}
	case WhichEdge::OnLeftBottomEdge: {

		break;
	}
	case WhichEdge::OnRightTopEdge: {

		break;
	}
	case WhichEdge::OnLeftEdge: {
		e = Qt::LeftEdge;
		break;
	}
	case WhichEdge::OnRightEdge: {
		e = Qt::RightEdge;
		break;
	}
	case WhichEdge::OnTopEdge: {
		e = Qt::TopEdge;
		break;
	}
	case WhichEdge::OnBottomEdge: {
		e = Qt::BottomEdge;
		break;
	}
	default:
		break;
	}
	return e;
}

