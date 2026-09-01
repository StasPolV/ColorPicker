#pragma once

#include <QObject>

class QScreen;

class ScreenGrabber : public QObject
{
	Q_OBJECT

public:
	explicit ScreenGrabber(QObject* parent = nullptr);

	QScreen* GetScreen() const;
};
