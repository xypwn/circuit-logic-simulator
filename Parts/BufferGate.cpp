#include "BufferGate.h"

#include "../Connector.h"

BufferGate::BufferGate(Logic* logic)
    :Part(logic)
{
    addInputs(1);
    addOutputs(1);
    setLabel("Buffer Gate");
    recalculateLayout();
}

QVector<bool> BufferGate::compute(QVector<bool> inputs)
{
    QVector<bool> ret(1);
    ret[0] = inputs[0];

    return ret;
}

QPainterPath BufferGate::symbolPainterPath(QRect limits)
{
    Q_UNUSED(limits)
    QPainterPath painterPath;
    QPolygon triangle;
    triangle.append(QPoint(-8, 8));
    triangle.append(QPoint(-8, -8));
    triangle.append(QPoint(8, 0));
    triangle.append(QPoint(-8, 8));
    painterPath.addPolygon(triangle);
    // Input line
    painterPath.moveTo(-14, 0);
    painterPath.lineTo(-8, 0);
    // Output line
    painterPath.moveTo(8, 0);
    painterPath.lineTo(14, 0);
    return painterPath;
}
