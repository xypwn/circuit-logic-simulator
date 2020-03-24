#ifndef XORGATE_H
#define XORGATE_H

#include "../Part.h"

class XorGate : public Part
{
public:
    XorGate(Logic* logic);

    QVector<bool> compute(QVector<bool> inputs) override;
    QPainterPath symbolPainterPath(QRect limits) override;
};

#endif // XORGATE_H
