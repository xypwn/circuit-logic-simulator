#ifndef LABEL_H
#define LABEL_H

#include <QGraphicsTextItem>
#include <QPointF>

#include "eAlignMode.h"

QT_BEGIN_NAMESPACE
class QFocusEvent;
QT_END_NAMESPACE

class Label : private QGraphicsTextItem
{
public:
    Label(QGraphicsItem* parent);

    void setEditable(bool value);
    void setText(const QString& text);
    QString text() const;
    void alignText(QPointF anchorPos, AlignMode::AlignMode mode = AlignMode::Default);

private:
    QGraphicsItem* m_parentItem;

    QPointF m_textAnchor;
    AlignMode::AlignMode m_alignMode;

    void keyPressEvent(QKeyEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void recalculateTextAlignment();
};

#endif // LABEL_H
