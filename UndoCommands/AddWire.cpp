#include "AddWire.h"

#include "../Scene.h"
#include "../Logic.h"
#include "../Wire.h"

AddWire::AddWire(Scene* scene, Connector* connectorInput, Connector* connectorOutput)
    :m_scene(scene), m_connectorInput(connectorInput), m_connectorOutput(connectorOutput)
{
    setText("Add Wire");
}

AddWire::~AddWire()
{
}


void AddWire::redo()
{
    m_wire = m_scene->m_logic->createWire(m_connectorInput, m_connectorOutput);
}

void AddWire::undo()
{
    m_scene->m_logic->deleteWire(m_wire);
}
