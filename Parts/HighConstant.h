#ifndef HIGHCONSTANT_H
#define HIGHCONSTANT_H

#include "../Part.h"

class HighConstant : public Part
{
public:
    HighConstant(Logic* logic);

    QVector<bool> compute(QVector<bool> inputs) override;
    QPainterPath symbolPainterPath(QRect limits) override;
};

#endif // HIGHCONSTANT_H
