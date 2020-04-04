#include "RemoveParts.h"

#include "../Scene.h"
#include "../Part.h"
#include "../Connector.h"
#include "../Logic.h"

#include "RemoveWire.h"

RemoveParts::RemoveParts(Scene* scene, const QList<Part*>& parts)
    :m_scene(scene), m_parts(parts), m_wireUndoStack(new QUndoStack)
{
    setText("Remove Parts");
}

RemoveParts::~RemoveParts()
{
    delete m_wireUndoStack;
}

void RemoveParts::redo()
{
    for(auto part : m_parts)
    {
        for(auto input : part->m_inputs)
        {
            for(auto wire : input->m_wires)
                // Remove wire
                m_wireUndoStack->push(new RemoveWire(m_scene, wire));
        }
        for(auto output : part->m_outputs)
        {
            for(auto wire : output->m_wires)
                // Remove wire
                m_wireUndoStack->push(new RemoveWire(m_scene, wire));
        }
        m_scene->stopTrackingPart(part);
    }
}

void RemoveParts::undo()
{
    for(auto part : m_parts)
    {
        m_scene->startTrackingPart(part);

        // Add wires back in
        while(m_wireUndoStack->canUndo())
                m_wireUndoStack->undo();
        }
}
