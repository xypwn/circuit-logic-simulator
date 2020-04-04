#include "Label.h"

#include <QKeyEvent>
#include <QGraphicsScene>
#include <QTextCursor>

Label::Label(QGraphicsItem* parent)
    :m_parentItem(parent)
{
    setParentItem((QGraphicsItem*)m_parentItem);
    if(parent->scene() != nullptr)
        parent->scene()->addItem(this);
}

void Label::setEditable(bool value)
{
    if(value)
        setTextInteractionFlags(Qt::TextInteractionFlag::TextEditorInteraction);
    else
        setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
}

void Label::setText(const QString& text)
{
    setPlainText(text);
    recalculateTextAlignment();
}

QString Label::text() const
{
    return toPlainText();
}

void Label::alignText(QPointF anchorPos, AlignMode::AlignMode mode)
{
    m_textAnchor = anchorPos;
    m_alignMode = mode;
    recalculateTextAlignment();
}

void Label::keyPressEvent(QKeyEvent* event)
{
    QGraphicsTextItem::keyPressEvent(event);

    if(event->isAccepted())
        recalculateTextAlignment();
}

void Label::focusOutEvent(QFocusEvent* event)
{
    QTextCursor tc = textCursor();
    tc.clearSelection();
    setTextCursor(tc);
    QGraphicsTextItem::focusOutEvent(event);
}

void Label::recalculateTextAlignment()
{
    if(m_alignMode & AlignMode::Right)
        setX( - boundingRect().width());
    else if(m_alignMode & AlignMode::HCenter)
        setX( - boundingRect().width() / 2.f);
    else //usually if(m_alignMode & AlignMode::Left)
        setX(0);

    if(m_alignMode & AlignMode::Bottom)
        setY( - boundingRect().height());
    else if(m_alignMode & AlignMode::VCenter)
        setY( - boundingRect().height() / 2.f);
    else //usually if(m_alignMode & AlignMode::Top)
        setY(0);

    moveBy(m_textAnchor.x(), m_textAnchor.y());
}
