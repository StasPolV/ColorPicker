#include "ColorPickerWidget.h"

#include <QColorDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

ColorPickerWidget::ColorPickerWidget(QWidget* parent) : QWidget(parent)
{
	setWindowTitle("ColorPicker");
	setWindowFlag(Qt::WindowStaysOnTopHint);

	QPushButton* start_grabbing = new QPushButton("Start Grabbing");
	start_grabbing->setMinimumHeight(30);

	m_color_dialog = new QColorDialog;
	m_color_dialog->setOption(QColorDialog::NoButtons, true);

	QVBoxLayout* v_layout = new QVBoxLayout(this);
	v_layout->addWidget(m_color_dialog);
	v_layout->addWidget(start_grabbing);

	connect(start_grabbing, &QPushButton::clicked, this,
	        [this]()
	        {
		        if (!m_is_grabbing)
		        {
			        StartGrabbing();
		        }
		        else
		        {
			        StopGrabbing();
		        }
	        });

	setFixedSize(m_color_dialog->size());
}

void ColorPickerWidget::SetColor(QColor color)
{
	m_color_dialog->setCurrentColor(color);
}

void ColorPickerWidget::mouseMoveEvent(QMouseEvent* event)
{
	QPoint global_pos = event->globalPos();

	QWidget::mouseMoveEvent(event);
}

void ColorPickerWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && m_is_grabbing)
	{
		emit CustomMouseClicked(event->globalPos());
		StopGrabbing();
	}

	QWidget::mousePressEvent(event);
}

void ColorPickerWidget::StartGrabbing()
{
	m_is_grabbing = true;
	grabMouse();
}

void ColorPickerWidget::StopGrabbing()
{
	m_is_grabbing = false;
	releaseMouse();
}
