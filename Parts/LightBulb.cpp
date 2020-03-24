#include "LightBulb.h"

#include "../Connector.h"

LightBulb::LightBulb(Logic* logic)
    :Part(logic, Circle)
{
    addInputs(1);
    addOutputs(0);
    recalculateLayout();
    m_brushColorNormal = Qt::GlobalColor::black;
    m_brushColorSelected = Qt::GlobalColor::black;
}

QVector<bool> LightBulb::compute(QVector<bool> inputs)
{
    m_state = inputs[0];

    // Set color according to input state (on or off)
    if(m_state)
    {
        m_brushColorNormal = Qt::GlobalColor::green;
        m_brushColorSelected = Qt::GlobalColor::green;
    }
    else
    {
        m_brushColorNormal = Qt::GlobalColor::black;
        m_brushColorSelected = Qt::GlobalColor::black;
    }

    return QVector<bool>();
}

QPainterPath LightBulb::symbolPainterPath(QRect limits)
{
    Q_UNUSED(limits)
    return QPainterPath();
}
