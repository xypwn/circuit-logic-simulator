#ifndef NOTGATE_H
#define NOTGATE_H

#include "../Part.h"

class NotGate : public Part
{
public:
    NotGate(Logic* logic);

    QVector<bool> compute(QVector<bool> inputs) override;
    QPainterPath symbolPainterPath(QRect limits) override;
};

#endif // NOTGATE_H
