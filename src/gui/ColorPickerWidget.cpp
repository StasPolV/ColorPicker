#include "ColorPickerWidget.h"

#include "MagnifierWidget.h"

#include <QCloseEvent>
#include <QColorDialog>
#include <QCursor>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPushButton>
#include <QVBoxLayout>

#ifdef Q_OS_WIN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace
{
	bool isPickButtonDown()
	{
#ifdef Q_OS_WIN
		const int vk = GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON;
		return (GetAsyncKeyState(vk) & 0x8000) != 0;
#else
		return (QGuiApplication::mouseButtons() & Qt::LeftButton) != 0;
#endif
	}

}  // namespace

ColorPickerWidget::ColorPickerWidget(QWidget* parent) : QWidget(parent)
{
	setWindowTitle("ColorPicker");
	setWindowFlag(Qt::WindowStaysOnTopHint);

	m_magnifier = new MagnifierWidget(this);

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

	connect(&m_poll_timer, &QTimer::timeout, this, &ColorPickerWidget::PollCursor);
	m_poll_timer.setInterval(16);

	setFixedSize(m_color_dialog->size());
}

void ColorPickerWidget::SetColor(QColor color)
{
	m_color_dialog->setCurrentColor(color);
}

void ColorPickerWidget::closeEvent(QCloseEvent* event)
{
	StopGrabbing();

	QWidget::closeEvent(event);
}

void ColorPickerWidget::mousePressEvent(QMouseEvent* event)
{
	if (m_is_grabbing && event->button() == Qt::LeftButton)
	{
		PickAt(QCursor::pos());
		event->accept();
		return;
	}

	QWidget::mousePressEvent(event);
}

void ColorPickerWidget::keyPressEvent(QKeyEvent* event)
{
	if (m_is_grabbing && event->key() == Qt::Key_Escape)
	{
		StopGrabbing();
		event->accept();
		return;
	}

	QWidget::keyPressEvent(event);
}

void ColorPickerWidget::PickAt(QPoint global_pos)
{
	emit CustomMouseClicked(global_pos);
	StopGrabbing();
}

void ColorPickerWidget::PollCursor()
{
	if (!m_is_grabbing)
		return;

	const QPoint global_pos = QCursor::pos();
	m_magnifier->UpdateAt(global_pos);

	const bool is_pressed = isPickButtonDown();
	if (is_pressed && !m_was_pressed)
	{
		m_was_pressed = true;
		PickAt(global_pos);
		return;
	}
	m_was_pressed = is_pressed;
}

void ColorPickerWidget::StartGrabbing()
{
	m_is_grabbing = true;
	m_was_pressed = isPickButtonDown();

	m_magnifier->show();
	m_magnifier->UpdateAt(QCursor::pos());

	grabMouse(Qt::CrossCursor);
	grabKeyboard();

	m_poll_timer.start();
}

void ColorPickerWidget::StopGrabbing()
{
	m_is_grabbing = false;
	m_poll_timer.stop();
	releaseKeyboard();
	releaseMouse();
	m_magnifier->hide();
}
