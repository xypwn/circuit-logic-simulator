#include "CopyParts.h"

#include <QMap>

#include "../Scene.h"
#include "../Logic.h"
#include "../Part.h"
#include "../Connector.h"
#include "../CircuitBuffer.h"

#include "RemoveWire.h"

CopyParts::CopyParts(Scene* scene, const CircuitBuffer& toCopy, QPointF relPos)
    :m_scene(scene), m_toCopy(toCopy), m_relPos(relPos), m_wireUndoStack(new QUndoStack)
{
    setText("Copy Parts with offset " + QString::number(relPos.x()) + ", " + QString::number(relPos.y()));
}

CopyParts::~CopyParts()
{
    for(auto part : m_copiedParts)
        m_scene->m_logic->deletePart(part);
}

void CopyParts::redo()
{
    if(m_isFirstRedo)
    {
        m_isFirstRedo = false;

        m_copiedParts = m_toCopy.addIntoScene(m_scene, m_relPos).first;
    }
    else
    {
        for(auto part : m_copiedParts)
            m_scene->startTrackingPart(part);
        // Add wires back in
        while(m_wireUndoStack->canUndo())
                m_wireUndoStack->undo();
    }
}

void CopyParts::undo()
{
    for(auto part : m_copiedParts)
    {
        m_scene->stopTrackingPart(part);
        for(auto input : part->m_inputs)
        {
            for(auto wire : input->m_wires)
                m_wireUndoStack->push(new RemoveWire(m_scene, wire));
        }
        for(auto output : part->m_outputs)
        {
            for(auto wire : output->m_wires)
                m_wireUndoStack->push(new RemoveWire(m_scene, wire));
        }
    }
}
