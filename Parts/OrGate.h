#ifndef ORGATE_H
#define ORGATE_H

#include "../Part.h"

class OrGate : public Part
{
public:
    OrGate(Logic* logic);

    QVector<bool> compute(QVector<bool> inputs) override;
    QPainterPath symbolPainterPath(QRect limits) override;
};

#endif // ORGATE_H
