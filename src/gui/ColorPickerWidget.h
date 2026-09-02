#pragma once

#include <QColor>
#include <QMouseEvent>
#include <QPoint>
#include <QWidget>

class QColorDialog;

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
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;

private:
	void StartGrabbing();
	void StopGrabbing();

	QColorDialog* m_color_dialog;
	bool m_is_grabbing = false;
};
