#include "NotGate.h"

#include "../Connector.h"

NotGate::NotGate(Logic* logic)
    :Part(logic)
{
    addInputs(1);
    addOutputs(1);
    recalculateLayout();
}

QVector<bool> NotGate::compute(QVector<bool> inputs)
{
    QVector<bool> ret(1);
    ret[0] = !inputs[0];
    return ret;
}

QPainterPath NotGate::symbolPainterPath(QRect limits)
{
    Q_UNUSED(limits)
    QPainterPath painterPath;
    QPolygon triangle;
    triangle.append(QPoint(-8, 8));
    triangle.append(QPoint(-8, -8));
    triangle.append(QPoint(8, 0));
    triangle.append(QPoint(-8, 8));
    painterPath.addPolygon(triangle);
    // Inverting dot
    painterPath.addEllipse(8.f, -1.5f, 3.f, 3.f);
    // Input line
    painterPath.moveTo(-14, 0);
    painterPath.lineTo(-8, 0);
    // Output line
    painterPath.moveTo(11.5f, 0.f);
    painterPath.lineTo(14, 0);
    return painterPath;
}
