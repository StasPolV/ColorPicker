#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QColor>

class QLineEdit;

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
	QLineEdit* m_color_line_edit;
};
