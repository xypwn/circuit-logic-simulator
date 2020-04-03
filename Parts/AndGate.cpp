#include "AndGate.h"

#include "../Connector.h"

AndGate::AndGate(Logic* logic)
    :Part(logic)
{
    addInputs(2);
    addOutputs(1);
    setLabel("And Gate");
    recalculateLayout();
}

QVector<bool> AndGate::compute(QVector<bool> inputs)
{
    QVector<bool> ret(1);
    ret[0] = inputs[0] && inputs[1];
    return ret;
}

QPainterPath AndGate::symbolPainterPath(QRect limits)
{
    Q_UNUSED(limits)
    QPainterPath painterPath;

    // Left vertical line
    painterPath.moveTo(-8, 8);
    painterPath.lineTo(-8, -8);
    // Upper horizontal line
    painterPath.lineTo(-2, -8);
    // Right arc
    painterPath.cubicTo(11, -8, 11, 8, -2, 8);
    // Lower horizontal line
    painterPath.lineTo(-8, 8);
    // Upper input line
    painterPath.moveTo(-14, -4);
    painterPath.lineTo(-8, -4);
    // Lower input line
    painterPath.moveTo(-14, 4);
    painterPath.lineTo(-8, 4);
    // Output line
    painterPath.moveTo(8, 0);
    painterPath.lineTo(14, 0);
    return painterPath;
}
