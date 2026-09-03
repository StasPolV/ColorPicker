#include "MagnifierWidget.h"

#include <QGuiApplication>
#include <QPainter>
#include <QPaintEvent>
#include <QPoint>
#include <QScreen>

MagnifierWidget::MagnifierWidget(QWidget* parent) : QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_TransparentForMouseEvents);
	setAttribute(Qt::WA_ShowWithoutActivating);
	setFixedSize(kDiameter, kDiameter);
}

void MagnifierWidget::UpdateAt(const QPoint& global_pos)
{
	QScreen* screen = QGuiApplication::screenAt(global_pos);
	if (!screen)
		screen = QGuiApplication::primaryScreen();
	if (!screen)
		return;

	const QRect screen_geom = screen->geometry();

	const QRect wanted(global_pos.x() - kSample / 2, global_pos.y() - kSample / 2, kSample,
	                   kSample);

	const QRect grab_rect = wanted.intersected(screen_geom);
	if (grab_rect.isEmpty())
		return;

	const QPixmap grabbed =
	        screen->grabWindow(0, grab_rect.x() - screen_geom.x(), grab_rect.y() - screen_geom.y(),
	                           grab_rect.width(), grab_rect.height());
	if (grabbed.isNull())
		return;

	m_sample_dpr = grabbed.devicePixelRatio();
	if (m_sample_dpr <= 0.0)
		m_sample_dpr = 1.0;

	m_sample = grabbed.toImage();
	m_sample.setDevicePixelRatio(1.0);

	m_hotspot = QPointF((global_pos.x() - grab_rect.x() + 0.5) * m_sample_dpr,
	                    (global_pos.y() - grab_rect.y() + 0.5) * m_sample_dpr);

	move(global_pos + QPoint(20, 20));
	update();
}

void MagnifierWidget::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	if (m_sample.isNull())
		return;

	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);
	p.setClipRegion(QRegion(rect(), QRegion::Ellipse));

	const qreal cx = width() / 2.0;
	const qreal cy = height() / 2.0;

	const qreal scale = qreal(kDiameter) / (kSample * m_sample_dpr);

	const QPointF top_left(cx - m_hotspot.x() * scale, cy - m_hotspot.y() * scale);

	p.save();
	p.setRenderHint(QPainter::SmoothPixmapTransform, false);
	p.translate(qRound(top_left.x()), qRound(top_left.y()));
	p.scale(scale, scale);
	p.drawImage(0, 0, m_sample);
	p.restore();

	const qreal cell = qreal(kDiameter) / kCells;
	p.setPen(QPen(QColor(0, 0, 0, 60), 1));
	for (int i = 0; i < kCells / 2; ++i)
	{
		const qreal off = (i + 0.5) * cell;
		p.drawLine(QPointF(cx - off, 0), QPointF(cx - off, height()));
		p.drawLine(QPointF(cx + off, 0), QPointF(cx + off, height()));
		p.drawLine(QPointF(0, cy - off), QPointF(width(), cy - off));
		p.drawLine(QPointF(0, cy + off), QPointF(width(), cy + off));
	}

	const QRectF center_cell(cx - cell / 2.0, cy - cell / 2.0, cell, cell);

	p.setClipping(false);
	p.setPen(QPen(Qt::white, 2));
	p.drawRect(center_cell.adjusted(1, 1, -1, -1));
	p.setPen(QPen(Qt::black, 1));
	p.drawRect(center_cell.adjusted(2, 2, -2, -2));

	p.setPen(QPen(Qt::gray, 2));
	p.drawEllipse(QRectF(rect()).adjusted(1, 1, -1, -1));
}
