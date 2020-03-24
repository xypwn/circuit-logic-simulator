#ifndef ANDGATE_H
#define ANDGATE_H

#include "../Part.h"

class AndGate : public Part
{
public:
    AndGate(Logic* logic);

    QVector<bool> compute(QVector<bool> inputs) override;
    QPainterPath symbolPainterPath(QRect limits) override;
};

#endif // ANDGATE_H
