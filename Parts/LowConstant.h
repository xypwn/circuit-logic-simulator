#ifndef LOWCONSTANT_H
#define LOWCONSTANT_H

#include "../Part.h"

class LowConstant : public Part
{
public:
    LowConstant(Logic* logic);

    QVector<bool> compute(QVector<bool> inputs) override;
    QPainterPath symbolPainterPath(QRect limits) override;
};

#endif // LOWCONSTANT_H
