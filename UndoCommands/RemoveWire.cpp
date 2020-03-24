#include "RemoveWire.h"

#include "../Scene.h"
#include "../Logic.h"
#include "../Wire.h"
#include "../Part.h"
#include "../Connector.h"

RemoveWire::RemoveWire(Scene* scene, Wire* wire)
    :m_scene(scene), m_wire(wire)
{
    setText("Remove Wire");

    m_wireInputPart = (Part*)wire->m_connectorInput->parentItem();
    m_wireInputConnectorIdx = m_wireInputPart->m_outputs.indexOf(wire->m_connectorInput);

    m_wireOutputPart = (Part*)wire->m_connectorOutput->parentItem();
    m_wireOutputConnectorIdx = m_wireOutputPart->m_inputs.indexOf(wire->m_connectorOutput);
}

RemoveWire::~RemoveWire()
{
}

void RemoveWire::redo()
{
    m_scene->m_logic->deleteWire(m_wire);
}


void RemoveWire::undo()
{
    Connector* inputConnector = m_wireInputPart->m_outputs[m_wireInputConnectorIdx];
    Connector* outputConnector = m_wireOutputPart->m_inputs[m_wireOutputConnectorIdx];
    m_wire = m_scene->m_logic->createWire(inputConnector, outputConnector);
}
