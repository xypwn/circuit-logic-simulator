#include "IntegratedCircuit.h"

#include "../Scene.h"
#include "../Logic.h"
#include "ToggleButton.h"
#include "LightBulb.h"

static bool compareToggleButtons(const ToggleButton* a, const ToggleButton* b) { return a->y() < b->y(); }
static bool compareLightBulbs(const LightBulb* a, const LightBulb* b) { return a->y() < b->y(); }

IntegratedCircuit::IntegratedCircuit(Logic* logic, QString filename)
    :Part(logic), m_filename(filename), m_icLogic(new Logic)
{
    setWidth(4);

    m_icLogic->loadFromFile(filename);

    for(auto part : m_icLogic->m_parts)
    {
        if(part->partType() == PartType::IOToggleButton)
            m_icLogicInputs.append((ToggleButton*)part);
        else if(part->partType() == PartType::IOLightBulb)
            m_icLogicOutputs.append((LightBulb*)part);
    }

    addInputs(m_icLogicInputs.length());
    addOutputs(m_icLogicOutputs.length());

    // Sort according to y position
    std::sort(m_icLogicInputs.begin(), m_icLogicInputs.end(), compareToggleButtons);
    std::sort(m_icLogicOutputs.begin(), m_icLogicOutputs.end(), compareLightBulbs);

    recalculateLayout();
}

QString IntegratedCircuit::filename()
{
    return m_filename;
}

QVector<bool> IntegratedCircuit::compute(QVector<bool> inputs)
{
    for(int i = 0; i < inputs.length(); i++)
        m_icLogicInputs[i]->m_toggleState = inputs[i];
    m_icLogic->doLogicStep();
    QVector<bool> ret(m_icLogicOutputs.length());
    for(int i = 0; i < m_icLogicOutputs.length(); i++)
        ret[i] = m_icLogicOutputs[i]->m_state;
    return ret;
}

QPainterPath IntegratedCircuit::symbolPainterPath(QRect limits)
{
    Q_UNUSED(limits)
    return QPainterPath();
}
