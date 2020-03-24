#ifndef LAMP_H
#define LAMP_H

#include "../Part.h"

class LightBulb : public Part
{
public:
    friend class IntegratedCircuit;

    LightBulb(Logic* logic);

    QVector<bool> compute(QVector<bool> inputs) override;
    QPainterPath symbolPainterPath(QRect limits) override;

private:
    bool m_state = false;
};


#endif // LAMP_H
