#ifndef REMOVEPARTS_H
#define REMOVEPARTS_H

#include <QUndoCommand>
#include <QList>

class Scene;
class Part;
class Connector;

class RemoveParts : public QUndoCommand
{
public:
    RemoveParts(Scene* scene, const QList<Part*>& parts);
    ~RemoveParts();

    void redo() override;
    void undo() override;

private:
    Scene* m_scene;
    QList<Part*> m_parts;

    QUndoStack* m_wireUndoStack;
};

#endif // REMOVEPARTS_H
