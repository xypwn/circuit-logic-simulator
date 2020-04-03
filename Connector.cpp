#include "Connector.h"

#include <QPainter>
#include "Part.h"
#include "MainWindow.h"
#include "Wire.h"
#include "eConnectorType.h"
#include "Scene.h"
#include "Label.h"


// PUBLIC
Connector::Connector(Scene* scene, Part *parentPart, ConnectorType::ConnectorType side)
    :m_scene(scene), m_connectorType(side)
{
    setParentItem(parentPart);

    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

Part* Connector::parentPart()
{
    return (Part*)parentItem();
}

void Connector::setLabel(const QString& text)
{
    if(m_label == nullptr)
    {
        m_label = new Label(this);
        // If this Connector is on the left side of a Part
        if(m_connectorType == ConnectorType::Input)
            m_label->alignText(QPointF(0, 0), mkAlignMode(AlignMode::Left, AlignMode::VCenter));
        // If this Connector is on the right side of a Part
        else
            m_label->alignText(QPointF(0, 0), mkAlignMode(AlignMode::Right, AlignMode::VCenter));
    }
    m_label->setText(text);
}

QString Connector::label() const
{
    if(m_label == nullptr)
        return QString();
    else
        return m_label->text();
}


QRectF Connector::boundingRect() const
{
    if(m_connectorType == ConnectorType::Output)
        return QRectF(-1, -4, 15, 8);
    else
        return QRectF(-14, -4, 15, 8);
}

QPainterPath Connector::shape() const
{
    QPainterPath path;
    if(m_connectorType == ConnectorType::Output)
        path.addRect(-1, -4, 15, 8);
    else
        path.addRect(-14, -4, 15, 8);
    return path;
}

void Connector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QPen pen = painter->pen();
    pen.setWidth(2);
    if(parentItem()->isSelected())
        pen.setColor(((Part*)parentItem())->m_penColorSelected);
    else
        pen.setColor(((Part*)parentItem())->m_penColorNormal);
    painter->setPen(pen);
    QBrush brush = painter->brush();
    if(m_selected)
    {
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        brush.setColor(Qt::green);
    }
    painter->setBrush(brush);
    if(m_connectorType == ConnectorType::Output)
    {
        painter->drawLine(2, 0, 6, 0);
        painter->drawEllipse(7, -3, 6, 6);
    }
    else
    {
        painter->drawLine(-6, 0, -2, 0);
        painter->drawEllipse(-13, -3, 6, 6);
    }
}

void Connector::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    m_scene->connectorClicked(this);
}

QVariant Connector::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemScenePositionHasChanged)
    {
        // Update wires
        for(auto i : m_wires)
            i->updateLine();
    }

    return value;
}

ConnectorType::ConnectorType Connector::connectorType()
{
    return m_connectorType;
}

// PRIVATE
void Connector::select()
{
    m_selected = true;
    update();
}

void Connector::unselect()
{
    m_selected = false;
    update();
}
