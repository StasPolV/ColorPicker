#include "ScreenGrabber.h"

#include <QCursor>
#include <QGuiApplication>
#include <QPoint>
#include <QScreen>

ScreenGrabber::ScreenGrabber(QObject* parent) : QObject(parent) {}

QScreen* ScreenGrabber::GetScreen()
{
	QPoint cursor_pos = QCursor::pos();

	QScreen* screen = QGuiApplication::screenAt(cursor_pos);

	return screen;
}
