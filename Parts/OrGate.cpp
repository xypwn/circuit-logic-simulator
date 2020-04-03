#include "OrGate.h"

#include "../Connector.h"

OrGate::OrGate(Logic* logic)
    :Part(logic)
{
    addInputs(2);
    addOutputs(1);
    setLabel("Or Gate");
    recalculateLayout();
}

QVector<bool> OrGate::compute(QVector<bool> inputs)
{
    QVector<bool> ret(1);
    ret[0] = inputs[0] || inputs[1];
    return ret;
}

QPainterPath OrGate::symbolPainterPath(QRect limits)
{
    Q_UNUSED(limits)
    QPainterPath painterPath;
    // Left arc
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
    // Upper input line
    painterPath.moveTo(-14, -4);
    painterPath.lineTo(-7.5f, -4.f);
    // Lower input line
    painterPath.moveTo(-14, 4);
    painterPath.lineTo(-7.5f, 4.f);
    // Output line
    painterPath.moveTo(8, 0);
    painterPath.lineTo(14, 0);
    return painterPath;
}
