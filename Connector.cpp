#include "Connector.h"

#include <QPainter>
#include "Part.h"
#include "MainWindow.h"
#include "Wire.h"
#include "eConnectorType.h"
#include "Scene.h"


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
