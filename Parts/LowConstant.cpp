#include "LowConstant.h"

#include "../Connector.h"

LowConstant::LowConstant(Logic* logic)
    :Part(logic, Circle)
{
    addInputs(0);
    addOutputs(1);
    recalculateLayout();
    m_brushColorNormal = Qt::GlobalColor::black;
    m_brushColorSelected = Qt::GlobalColor::black;
}

QVector<bool> LowConstant::compute(QVector<bool> inputs)
{
    QVector<bool> ret(1);
    ret[0] = false;
    return ret;
}

QPainterPath LowConstant::symbolPainterPath(QRect limits)
{
    Q_UNUSED(limits)
    return QPainterPath();
}
