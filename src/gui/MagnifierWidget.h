#pragma once

#include <QImage>
#include <QPoint>
#include <QPointF>
#include <QWidget>

class MagnifierWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MagnifierWidget(QWidget* parent = nullptr);
	void UpdateAt(const QPoint& global_pos);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	static constexpr int kCells = 9;
	static constexpr int kDiameter = 160;
	static constexpr int kSample = kDiameter;

	QImage m_sample;
	QPointF m_hotspot;
	qreal m_sample_dpr = 1.0;
};
