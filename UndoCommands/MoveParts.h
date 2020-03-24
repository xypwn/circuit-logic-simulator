#ifndef MOVEPARTS_H
#define MOVEPARTS_H

#include <QUndoCommand>
#include <QPointF>

class Scene;
class Part;

class MoveParts : public QUndoCommand
{
public:
    MoveParts(Scene* scene, const QList<Part*>& parts, QPointF relPos);
    ~MoveParts();

    void redo() override;
    void undo() override;

private:
    Scene* m_scene;
    QList<Part*> m_parts;
    QPointF m_relPos;

    bool m_isFirstRedo = true;
};

#endif // MOVEPARTS_H
