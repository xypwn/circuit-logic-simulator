#include "CircuitBuffer.h"

#include "Part.h"
#include "Wire.h"
#include "Connector.h"
#include "Scene.h"
#include "Logic.h"

#include "Parts/IntegratedCircuit.h"

CircuitBuffer::CircuitBuffer()
{

}

void CircuitBuffer::addFromScene(const QList<Part*> &parts, const QList<Wire*>& wires)
{
    // Maps the pointer of every given part to the corresponding pointer to an element in m_parts
    QMap<Part*, PartData*> partPtrToDataPtr;

    for(auto part : parts)
    {
        // Create partData
        PartData partData;
        partData.type = part->m_partType;
        partData.pos = part->pos();
        if(part->partType() == PartType::IntegratedCircuit)
            partData.icFilename = ((IntegratedCircuit*)part)->filename();
        else
            partData.icFilename = QString();
        // Add part to parts list
        m_parts.append(partData);
        // Make it possible to find partData pointer with Part pointer
        partPtrToDataPtr.insert(part, &m_parts.last());
    }
    for(auto wire : wires)
    {
        auto wireInputPart = (Part*)wire->m_connectorInput->parentItem();
        auto wireOutputPart = (Part*)wire->m_connectorOutput->parentItem();

        // Create wireData
        WireData wireData;
        wireData.inputPart = partPtrToDataPtr[wireInputPart];
        wireData.inputPartConnectorIdx = wireInputPart->m_outputs.indexOf(wire->m_connectorInput);
        wireData.outputPart = partPtrToDataPtr[wireOutputPart];
        wireData.outputPartConnectorIdx = wireOutputPart->m_inputs.indexOf(wire->m_connectorOutput);
        m_wires.append(wireData);
    }
}

QPair<QList<Part*>, QList<Wire*>> CircuitBuffer::addIntoScene(Scene* scene, QPointF posOffset) const
{
    QList<Part*> allocatedParts;
    QList<Wire*> allocatedWires;

    QMap<const PartData*, Part*> dataToAllocatedPart;

    for(auto& partData : m_parts)
    {
        Part* part;
        if(partData.type == PartType::IntegratedCircuit)
            part = scene->m_logic->createIC(partData.icFilename, partData.pos + posOffset);
        else
            part = scene->m_logic->createPart(partData.type, partData.pos + posOffset);
        dataToAllocatedPart.insert(&partData, part);

        allocatedParts.append(part);
    }

    for(auto& wireData : m_wires)
    {
        // input and output are relative to the wire
        Part* inputPart = dataToAllocatedPart[wireData.inputPart];
        Connector* inputConnector = inputPart->m_outputs[wireData.inputPartConnectorIdx];
        Part* outputPart = dataToAllocatedPart[wireData.outputPart];
        Connector* outputConnector = outputPart->m_inputs[wireData.outputPartConnectorIdx];
        Wire* wire = scene->m_logic->createWire(inputConnector, outputConnector);

        allocatedWires.append(wire);
    }
    return QPair<QList<Part*>, QList<Wire*>>(allocatedParts, allocatedWires);
}

QPointF CircuitBuffer::getAvgPartPos()
{
    QPointF sum(0.f, 0.f);
    for(auto part : m_parts)
        sum += part.pos;
    return sum / m_parts.length();
}

void CircuitBuffer::clear()
{
    m_parts.clear();
    m_wires.clear();
}
