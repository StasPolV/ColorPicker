#pragma once

#include <QObject>
#include <QColor>

class ColorGrabber : public QObject 
{
	Q_OBJECT

signals:
	void ColorGrabbed(QColor color);

public slots:
	void GrabColor();

public:
	explicit ColorGrabber(QObject* parent = nullptr);
};
