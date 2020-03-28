#include "Part.h"

#include <QDebug>

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneDragDropEvent>
#include "Connector.h"
#include "eConnectorType.h"
#include "MainWindow.h"
#include "Scene.h"
#include "Logic.h"

// PUBLIC
Part::Part(Logic* logic, CircuitItemBaseShape baseShape)
    :m_logic(logic), m_baseShape(baseShape)
{
    // Set flags
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    // Set default colors
    m_penColorNormal=Qt::GlobalColor::color1;
    m_penColorSelected=Qt::GlobalColor::color0;
    m_penColorSymbol=Qt::GlobalColor::white;
    m_brushColorNormal=Qt::GlobalColor::blue;
    m_brushColorSelected=Qt::GlobalColor::blue;
    // Set default width factor (actual width = 20 * width factor)
    setWidth(2);
}

PartType::PartType Part::partType()
{
    return m_partType;
}

QPointF Part::getPos() const
{
    return pos();
}

void Part::setPos(QPointF pos)
{
    QGraphicsItem::setPos(pos);
}

void Part::addInputs(int amount)
{
    for(int i = 0; i < amount; i++)
    {
        Connector *connector = new Connector(m_logic->parentScene(), this, ConnectorType::Input);
        m_inputs.append(connector);
        m_logic->m_inputConnectors.append(connector);
    }
}

void Part::addOutputs(int amount)
{
    for(int i = 0; i < amount; i++)
    {
        Connector *connector = new Connector(m_logic->parentScene(), this, ConnectorType::Output);
        m_outputs.append(connector);
        m_logic->m_outputConnectors.append(connector);
    }
}

void Part::setWidth(int factor)
{
    m_widthFactor = factor;
}

void Part::recalculateLayout()
{
    // Add inputs and outputs and position them correctly
    int yOffsetInputs;
    int yOffsetOutputs;
    if(m_baseShape == RoundedRect)
    {
        if(m_outputs.length() > m_inputs.length())
        {
            yOffsetInputs = 10 + 10 * (m_outputs.length() - m_inputs.length());
            yOffsetOutputs = 10;
        }
        else
        {
            yOffsetOutputs = 10 + 10 * (m_inputs.length() - m_outputs.length());
            yOffsetInputs = 10;
        }
    }
    else // if m_baseShape == Circle
    {
        yOffsetInputs = 20;
        yOffsetOutputs = 20;
    }
    for(int i = 0; i < m_inputs.length(); i++)
    {
        m_inputs[i]->setPos(0, yOffsetInputs + 20 * i);
    }
    for(int i = 0; i < m_outputs.length(); i++)
    {
        m_outputs[i]->setPos(20 * m_widthFactor, yOffsetOutputs + 20 * i);
    }
}

QRectF Part::boundingRect() const
{
    if(m_baseShape == RoundedRect)
    {
        int maxConnections = qMax(m_inputs.length(), m_outputs.length());
        return QRectF(-1, -1, m_widthFactor * 20 + 2, maxConnections * 20 + 2);
    }
    else // if m_baseShape == Circle
        return QRectF(-1, -1, 42, 42);
}

QPainterPath Part::shape() const
{
    QPainterPath path;
    if(m_baseShape == RoundedRect)
    {
        int maxConnections = qMax(m_inputs.length(), m_outputs.length());
        path.addRect(-1, -1, m_widthFactor * 20 + 2, maxConnections * 20 + 2);
    }
    else // if m_baseShape == Circle
        path.addRect(-1, -1, 42, 42);
    return path;
}

void Part::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget)

    QPen symbolPen = painter->pen();
    symbolPen.setColor(m_penColorSymbol);

    QBrush symbolBrush = painter->brush();

    QPen pen = painter->pen();
    pen.setWidth(2);

    QBrush brush = painter->brush();
    brush.setStyle(Qt::SolidPattern);

    // Set colors according to state
    if(item->state & QStyle::State_Selected)
    {
        pen.setColor(m_penColorSelected);
        brush.setColor(m_brushColorSelected);
    }
    else
    {
        pen.setColor(m_penColorNormal);
        brush.setColor(m_brushColorNormal);
    }
    painter->setPen(pen);
    painter->setBrush(brush);

    // Draw base shape
    int maxConnections = qMax(m_inputs.length(), m_outputs.length());
    if(m_baseShape == RoundedRect)
        painter->drawRoundedRect(0, 0, 20 * m_widthFactor, 20 * maxConnections, 5, 5);
    else // if m_baseShape == Circle
        painter->drawEllipse(0, 0, 40, 40);
    // Draw provided symbol
    QPainterPath painterPath = symbolPainterPath(QRect(-20, -10 * maxConnections, 40, 20 * maxConnections));
    painter->setPen(symbolPen);
    painter->setBrush(symbolBrush);
    painter->translate(20, 10 * maxConnections);
    painter->drawPath(painterPath);
}

QVariant Part::itemChange(GraphicsItemChange change, const QVariant & value)
{
    // Update connectors if selection state has changed; this ensures that their pen color changes with that of the CircuitItem
    if (change == ItemSelectedHasChanged)
    {
        for(auto i : m_inputs)
            i->update();
        for(auto o : m_outputs)
            o->update();
    }
    return QGraphicsItem::itemChange(change, value);
}

// PRIVATE
void Part::updateState()
{
    QVector<bool> inputs;
    inputs.reserve(m_inputs.length());
    for(auto connector : m_inputs)
        inputs.append(connector->m_state);

    QVector<bool> outputs = compute(inputs);
    for(int i = 0; i < m_outputs.length(); i++)
    {
        m_outputs[i]->m_state = outputs[i];
    }
}
