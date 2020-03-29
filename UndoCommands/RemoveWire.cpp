#include "RemoveWire.h"

#include "../Scene.h"
#include "../Logic.h"
#include "../Wire.h"
#include "../Part.h"
#include "../Connector.h"\

RemoveWire::RemoveWire(Scene* scene, Wire* wire)
    :m_scene(scene), m_wire(wire)
{
    setText("Remove Wire");
}

RemoveWire::~RemoveWire()
{
}

void RemoveWire::redo()
{
    m_scene->stopTrackingWire(m_wire);
}


void RemoveWire::undo()
{
    m_scene->startTrackingWire(m_wire);
}
