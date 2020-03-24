#ifndef NORGATE_H
#define NORGATE_H

#include "../Part.h"

class NorGate : public Part
{
public:
    NorGate(Logic* logic);

    QVector<bool> compute(QVector<bool> inputs) override;
    QPainterPath symbolPainterPath(QRect limits) override;
};

#endif // NORGATE_H
