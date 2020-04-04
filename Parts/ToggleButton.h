#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include "../Part.h"

class ToggleButton : public Part
{
public:
    friend class IntegratedCircuit;

    ToggleButton(Logic* logic);

    QVector<bool> compute(QVector<bool> inputs) override;
    QPainterPath symbolPainterPath(QRect limits) override;

private:
    void partClickedEvent() override;

    bool m_toggleState = false;
};

#endif // TOGGLEBUTTON_H
