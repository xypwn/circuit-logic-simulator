#include "Logic.h"

#include <QFileDialog>

#include "eConnectorType.h"
#include "Part.h"
#include "Connector.h"
#include "Wire.h"
#include "FileHandler.h"
#include "Scene.h"
#include "MainWindow.h"

#include "Parts/BufferGate.h"
#include "Parts/NotGate.h"
#include "Parts/AndGate.h"
#include "Parts/OrGate.h"
#include "Parts/NandGate.h"
#include "Parts/NorGate.h"
#include "Parts/XorGate.h"
#include "Parts/XnorGate.h"
#include "Parts/HighConstant.h"
#include "Parts/LowConstant.h"
#include "Parts/LightBulb.h"
#include "Parts/ToggleButton.h"
#include "Parts/IntegratedCircuit.h"

Logic::Logic(Scene* parentScene)
    :m_parentScene(parentScene)
{

}

Scene* Logic::parentScene()
{
    return m_parentScene;
}

void Logic::doLogicStep()
{
    // The method of first updating the items, then the connections makes everything happen completely
    // synchronously and makes sure that a signal isn't propagated or computed twice in a single step

    // Let the circuitItems compute everything
    for(auto part : m_parts)
    {
        part->updateState();
    }
    // Reset the state of any connector receiving input before each step
    for(auto connector : m_inputConnectors)
    {
        connector->m_state = false;
    }
    // Make wires propagate forward their input
    for(auto wire : m_wires)
    {
        wire->feedInput();
    }
}

bool Logic::saveToFile(QString filename)
{
    if(m_parentScene)
        m_parentScene->m_parentMainWindow->changesSaved();
    FileHandler fh(this);
    return fh.save(filename);
}

bool Logic::loadFromFile(QString filename)
{
    FileHandler fh(this);
    return fh.open(filename);
}

// PRIVATE
IntegratedCircuit* Logic::createIC(QString filename, QPointF pos)
{
    IntegratedCircuit* ic = new IntegratedCircuit(this, filename);
    if(m_parentScene)
        m_parentScene->addItem(ic);
    m_parts.append(ic);
    ic->setPos(pos);
    ic->m_oldPos = pos;
    ic->m_partType = PartType::IntegratedCircuit;
    return ic;
}

Part* Logic::createPart(PartType::PartType partType, QPointF pos)
{
    Part *part;

    if(partType == PartType::GateBuffer)
        part = new BufferGate(this);
    else if(partType == PartType::GateNot)
        part = new NotGate(this);
    else if(partType == PartType::GateAnd)
        part = new AndGate(this);
    else if(partType == PartType::GateOr)
        part = new OrGate(this);
    else if(partType == PartType::GateNand)
        part = new NandGate(this);
    else if(partType == PartType::GateNor)
        part = new NorGate(this);
    else if(partType == PartType::GateXor)
        part = new XorGate(this);
    else if(partType == PartType::GateXnor)
        part = new XnorGate(this);
    else if(partType == PartType::IOHighConstant)
        part = new HighConstant(this);
    else if(partType == PartType::IOLowConstant)
        part = new LowConstant(this);
    else if(partType == PartType::IOLightBulb)
        part = new LightBulb(this);
    else if(partType == PartType::IOToggleButton)
        part = new ToggleButton(this);

    if(m_parentScene)
        m_parentScene->addItem(part);
    m_parts.append(part);
    part->setPos(pos);
    part->m_oldPos = pos;
    part->m_partType = partType;

    return part;
}

Wire* Logic::createWire(Connector *inputConnector, Connector *outputConnector)
{
    Wire* wire = new Wire(m_parentScene, inputConnector, outputConnector);
    for(auto w : inputConnector->m_wires)
    {
        // If the connection already exists, don't add it and return nullptr
        if((inputConnector->m_connectorType == ConnectorType::Input && w->m_connectorInput == outputConnector) ||
            (inputConnector->m_connectorType == ConnectorType::Output && w->m_connectorOutput == outputConnector))
        {
            return nullptr;
        }
    }
    // Append wire to connector wire arrays
    inputConnector->m_wires.append(wire);
    outputConnector->m_wires.append(wire);
    // Add wire to scene
    if(m_parentScene)
        m_parentScene->addItem(wire);
    // Add wire to scene's tracker list
    m_wires.append(wire);

    return wire;
}

void Logic::deletePart(Part* part)
{
    // Remove all of the part's connections first
    for(auto input : part->m_inputs)
    {
        for(auto wire : input->m_wires)
            deleteWire(wire);
    }
    for(auto output : part->m_outputs)
    {
        for(auto wire : output->m_wires)
            deleteWire(wire);
    }
    // Remove part out of Logic's tracking list
    m_parts.removeOne(part);
    // Deallocate part
    delete part;
}

void Logic::deleteWire(Wire *wire)
{
    // Remove wire out of Logic's tracking list
    m_wires.removeOne(wire);
    // Remove wire out of connectors
    wire->m_connectorInput->m_wires.removeOne(wire);
    wire->m_connectorOutput->m_wires.removeOne(wire);
    // Deallocate wire
    delete wire;
}
