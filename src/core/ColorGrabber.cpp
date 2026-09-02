#include "ColorGrabber.h"

#include "ScreenGrabber.h"

#include <QColor>
#include <QPixmap>
#include <QScreen>
#include <QPoint>
#include <QCursor>
#include <QImage>

ColorGrabber::ColorGrabber(QObject* parent) : QObject(parent) 
{
}

void ColorGrabber::GrabColor() 
{
	QScreen* screen = ScreenGrabber::GetScreen();

	if (!screen)
		return;

	QPixmap pixmap = screen->grabWindow();
	if (pixmap.isNull())
		return;

	QPoint local_logical = QCursor::pos() - screen->geometry().topLeft();

	QImage img = pixmap.toImage();
	qreal dpr = pixmap.devicePixelRatio();

	int px = qRound(local_logical.x() * dpr);
	int py = qRound(local_logical.y() * dpr);

	px = qBound(0, px, img.width() - 1);
	py = qBound(0, py, img.height() - 1);

	emit ColorGrabbed(img.pixelColor(px, py));
}
