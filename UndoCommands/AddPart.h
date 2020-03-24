#ifndef ADDPART_H
#define ADDPART_H

#include <QUndoCommand>
#include <QPointF>

#include "../ePartType.h"

class Scene;
class Part;
class RemovePart;

class AddPart : public QUndoCommand
{
public:
    AddPart(Scene* scene, PartType::PartType partType, QPointF pos, QString icFilename = "");
    ~AddPart();

    void redo() override;
    void undo() override;

private:
    Scene* m_scene;
    Part* m_part = nullptr;
    PartType::PartType m_partType;
    QPointF m_pos;
    QString m_icFilename;
};

#endif // ADDPART_H
