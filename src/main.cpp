#include "ColorGrabber.h"
#include "ColorPickerWidget.h"

#include <QApplication>
#include <QObject>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	ColorPickerWidget widget;
	ColorGrabber color_grabber;
	widget.show();

	QObject::connect(&color_grabber, &ColorGrabber::ColorGrabbed, &widget,
	                 &ColorPickerWidget::SetColor);

	QObject::connect(&widget, &ColorPickerWidget::CustomMouseClicked, &color_grabber,
	                 &ColorGrabber::GrabColor);

	return app.exec();
}
