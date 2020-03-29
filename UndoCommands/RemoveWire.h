#ifndef REMOVEWIRE_H
#define REMOVEWIRE_H

#include <QUndoCommand>

class Scene;
class Wire;
class Part;

class RemoveWire : public QUndoCommand
{
public:    
    RemoveWire(Scene* scene, Wire* wire);
    ~RemoveWire();

    void redo() override;
    void undo() override;

private:  
    Scene* m_scene;
    Wire* m_wire;
};

#endif // REMOVEWIRE_H
