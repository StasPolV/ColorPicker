#pragma once

#include <QColor>
#include <QPoint>
#include <QTimer>
#include <QWidget>

class QCloseEvent;
class QColorDialog;
class QKeyEvent;
class QMouseEvent;
class MagnifierWidget;

class ColorPickerWidget : public QWidget
{
	Q_OBJECT

signals:
	void CustomMouseClicked(QPoint global_pos);

public slots:
	void SetColor(QColor color);

public:
	explicit ColorPickerWidget(QWidget* parent = nullptr);

protected:
	void closeEvent(QCloseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;

private:
	void StartGrabbing();
	void StopGrabbing();
	void PickAt(QPoint global_pos);
	void PollCursor();

	QColorDialog* m_color_dialog;
	bool m_is_grabbing = false;
	bool m_was_pressed = false;
	MagnifierWidget* m_magnifier;
	QTimer m_poll_timer;
};
