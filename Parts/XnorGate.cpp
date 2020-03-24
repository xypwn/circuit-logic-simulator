#include "XnorGate.h"

#include "../Connector.h"

XnorGate::XnorGate(Logic* logic)
    :Part(logic)
{
    addInputs(2);
    addOutputs(1);
    recalculateLayout();
}

QVector<bool> XnorGate::compute(QVector<bool> inputs)
{
    QVector<bool> ret(1);
    ret[0] = !((inputs[0] || inputs[1]) && (!(inputs[0] && inputs[1])));
    return ret;
}

QPainterPath XnorGate::symbolPainterPath(QRect limits)
{
    Q_UNUSED(limits)
    QPainterPath painterPath;
    // 0th Left arc
    painterPath.moveTo(-12, -8);
    painterPath.quadTo(-4, 0, -12, 8);
    // 1st Left arc
    painterPath.moveTo(-10, -8);
    painterPath.quadTo(-2, 0, -10, 8);
    // Lower line
    painterPath.lineTo(-2, 8);
    // Lower arc
    painterPath.quadTo(5, 6, 8, 0);
    //
    painterPath.moveTo(-10, -8);
    // Upper line
    painterPath.lineTo(-2, -8);
    // Upper arc
    painterPath.quadTo(5, -6, 8, 0);
    // Inverting dot
    painterPath.addEllipse(8.f, -1.5f, 3.f, 3.f);
    // Upper input line
    painterPath.moveTo(-14, -4);
    painterPath.lineTo(-7.5f, -4.f);
    // Lower input line
    painterPath.moveTo(-14, 4);
    painterPath.lineTo(-7.5f, 4.f);
    // Output line
    painterPath.moveTo(11.5f, 0.f);
    painterPath.lineTo(14, 0);
    return painterPath;
}
