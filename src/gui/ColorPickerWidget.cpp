#include "ColorPickerWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

ColorPickerWidget::ColorPickerWidget(QWidget* parent) : QWidget(parent)
{
	setWindowTitle("ColorPicker");
	setWindowFlag(Qt::WindowStaysOnTopHint);

	QLabel* color_label = new QLabel("Hex: ");

	m_color_line_edit = new QLineEdit;
	m_color_line_edit->setModified(false);

	QPushButton* start_grabbing = new QPushButton("Start Grabbing");

	QHBoxLayout* h_layout = new QHBoxLayout(this);

	h_layout->addWidget(color_label);
	h_layout->addWidget(m_color_line_edit);
	h_layout->addWidget(start_grabbing);

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
}

void ColorPickerWidget::SetColor(QColor color)
{
	m_color_line_edit->setText(color.name());
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
