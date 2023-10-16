#pragma once
#include <QWidget>

class RenderWindow : public QWidget
{
	Q_OBJECT

public:
	RenderWindow(QWidget* parent = nullptr);
	~RenderWindow();
	virtual QPaintEngine* paintEngine() const
	{
		return nullptr;
	}
private:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void keyReleaseEvent(QKeyEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
};


