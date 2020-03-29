#include "AddWire.h"

#include "../Scene.h"
#include "../Logic.h"
#include "../Wire.h"
#include "../Connector.h"
#include "../Part.h"

AddWire::AddWire(Scene* scene, Connector* connectorInput, Connector* connectorOutput)
    :m_scene(scene), m_connectorInput(connectorInput), m_connectorOutput(connectorOutput)
{
    setText("Add Wire");
}

AddWire::~AddWire()
{
    m_scene->m_logic->deleteWire(m_wire);
}

void AddWire::redo()
{
    if(m_wire == nullptr)
        m_wire = m_scene->m_logic->createWire(m_connectorInput, m_connectorOutput);
    else
    {
        m_scene->startTrackingWire(m_wire);
    }
}

void AddWire::undo()
{
    m_scene->stopTrackingWire(m_wire);
}
