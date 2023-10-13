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
};


