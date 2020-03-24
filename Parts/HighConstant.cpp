#include "HighConstant.h"

#include "../Connector.h"

HighConstant::HighConstant(Logic* logic)
    :Part(logic, Circle)
{
    addInputs(0);
    addOutputs(1);
    recalculateLayout();
    m_brushColorNormal = Qt::GlobalColor::green;
    m_brushColorSelected = Qt::GlobalColor::green;
}

QVector<bool> HighConstant::compute(QVector<bool> inputs)
{
    QVector<bool> ret(1);
    ret[0] = true;
    return ret;
}

QPainterPath HighConstant::symbolPainterPath(QRect limits)
{
    Q_UNUSED(limits)
    return QPainterPath();
}
