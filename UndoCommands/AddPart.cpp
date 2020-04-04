#include "AddPart.h"

#include "../Part.h"
#include "../Connector.h"
#include "../Scene.h"
#include "../Logic.h"
#include "../Parts/IntegratedCircuit.h"

#include "RemoveWire.h"

AddPart::AddPart(Scene* scene, PartType::PartType partType, QPointF pos, QString icFilename)
    :m_scene(scene), m_partType(partType), m_pos(pos), m_icFilename(icFilename)
{
    setText("Add Part");
}

AddPart::~AddPart()
{
    m_scene->m_logic->deletePart(m_part);
}

void AddPart::redo()
{
    if(!m_part)
    {
        if(m_partType == PartType::IntegratedCircuit)
            m_part = m_scene->m_logic->createIC(m_icFilename, m_pos);
        else
            m_part = m_scene->m_logic->createPart(m_partType, m_pos);
    }
    else
    {
        m_scene->startTrackingPart(m_part);
    }
}

void AddPart::undo()
{
    m_scene->stopTrackingPart(m_part);
}
