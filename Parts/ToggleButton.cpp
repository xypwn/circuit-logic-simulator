#include "ToggleButton.h"

#include <QGraphicsSceneMouseEvent>

ToggleButton::ToggleButton(Logic* logic)
    :Part(logic, Circle)
{
    addInputs(0);
    addOutputs(1);
    setLabel("Toggle Button");
    recalculateLayout();
    m_brushColorNormal = Qt::GlobalColor::red;
    m_brushColorSelected = Qt::GlobalColor::red;
}

QVector<bool> ToggleButton::compute(QVector<bool> inputs)
{
    Q_UNUSED(inputs)
    QVector<bool> ret(1);
    ret[0] = m_toggleState;
    return ret;
}

QPainterPath ToggleButton::symbolPainterPath(QRect limits)
{
    Q_UNUSED(limits)
    return QPainterPath();
}

void ToggleButton::partClickedEvent()
{
    m_toggleState = !m_toggleState;
    if(m_toggleState)
    {
        m_brushColorNormal = Qt::GlobalColor::darkRed;
        m_brushColorSelected = Qt::GlobalColor::darkRed;
    }
    else
    {
        m_brushColorNormal = Qt::GlobalColor::red;
        m_brushColorSelected = Qt::GlobalColor::red;
    }
    update();
}
