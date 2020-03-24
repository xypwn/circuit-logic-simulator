#ifndef COPYPARTS_H
#define COPYPARTS_H

#include <QUndoStack>
#include <QUndoCommand>
#include <QList>
#include <QPointF>

#include "../ePartType.h"

class Scene;
class Part;
class Wire;

class CircuitBuffer;

class CopyParts : public QUndoCommand
{
public:
    typedef QPair<PartType::PartType, QPointF> PartData;
    typedef QPair<PartData*, PartData*> WireData;

    friend class Scene;

    CopyParts(Scene* scene, const CircuitBuffer& toCopy, QPointF relPos);
    ~CopyParts();

    void redo() override;
    void undo() override;

private:
    Scene* m_scene;
    // Since m_toCopy get used immediately, there shouldn't be any segfaults due to any parts that have changed
    const CircuitBuffer& m_toCopy;
    QList<Part*> m_copiedParts;
    QPointF m_relPos;

    bool m_isFirstRedo = true;

    QUndoStack* m_wireUndoStack;
};

#endif // COPYPARTS_H
