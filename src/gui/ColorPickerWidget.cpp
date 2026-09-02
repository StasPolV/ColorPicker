#include "ColorPickerWidget.h"

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

ColorPickerWidget::ColorPickerWidget(QWidget* parent) : QWidget(parent) 
{
	setWindowTitle("ColorPicker");
	setMouseTracking(true);

	QLabel* color_label = new QLabel("Hex: ");

	m_color_line_edit = new QLineEdit;
	m_color_line_edit->setModified(false);

	QHBoxLayout* h_layout = new QHBoxLayout(this);

	h_layout->addWidget(color_label);
	h_layout->addWidget(m_color_line_edit);
}

void ColorPickerWidget::mouseMoveEvent(QMouseEvent* event) 
{
	QPoint global_pos = event->globalPos();

	QWidget::mouseMoveEvent(event);
}

void ColorPickerWidget::mousePressEvent(QMouseEvent* event) 
{
	if (event->button() == Qt::LeftButton)
	{
		emit CustomMouseClicked(event->globalPos());
	}

	QWidget::mousePressEvent(event);
}

void ColorPickerWidget::SetColor(QColor color) 
{
	m_color_line_edit->setText(color.name());
}

