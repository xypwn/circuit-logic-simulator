#ifndef NANDGATE_H
#define NANDGATE_H

#include "../Part.h"

class NandGate : public Part
{
public:
    NandGate(Logic* logic);

    QVector<bool> compute(QVector<bool> inputs) override;
    QPainterPath symbolPainterPath(QRect limits) override;
};

#endif // NANDGATE_H
