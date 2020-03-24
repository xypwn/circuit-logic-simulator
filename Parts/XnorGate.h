#ifndef XNORGATE_H
#define XNORGATE_H

#include "../Part.h"

class XnorGate : public Part
{
public:
    XnorGate(Logic* logic);

    QVector<bool> compute(QVector<bool> inputs) override;
    QPainterPath symbolPainterPath(QRect limits) override;
};

#endif // XNORGATE_H
