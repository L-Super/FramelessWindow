#pragma once

#include <QWidget>

class FramelessWindowPrivate;
class FramelessWindow : public QWidget {
  Q_OBJECT
public:
  explicit FramelessWindow(QWidget *parent = nullptr);
  virtual ~FramelessWindow();

  //TODO:add setCenterWidget();

  /**
   * 设置窗口边框宽度
   * 用于鼠标光标是否处于窗口边缘的宽度控制，默认窗口内外宽度为5个像素
   * @param width
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
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

protected:
	void ChangeCursorShape(WhichEdge edge);

private:
  QScopedPointer<FramelessWindowPrivate> d_ptr;
  Q_DECLARE_PRIVATE(FramelessWindow);
};
