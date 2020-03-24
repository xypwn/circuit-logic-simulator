#ifndef INTEGRATEDCIRCUIT_H
#define INTEGRATEDCIRCUIT_H

#include <QString>

#include "../Part.h"

class Logic;
class ToggleButton;
class LightBulb;

class IntegratedCircuit : public Part
{
public:
    IntegratedCircuit(Logic* logic, QString filename = "");

    QString filename();

    QVector<bool> compute(QVector<bool> inputs) override;
    QPainterPath symbolPainterPath(QRect limits) override;

private:
    QString m_filename;

    Logic* m_icLogic;

    QVector<ToggleButton*> m_icLogicInputs;
    QVector<LightBulb*> m_icLogicOutputs;
};

#endif // INTEGRATEDCIRCUIT_H
