#ifndef ADDWIRE_H
#define ADDWIRE_H

#include <QUndoCommand>

class Scene;
class Wire;
class Connector;

class AddWire : public QUndoCommand
{
public:
    AddWire(Scene* scene, Connector* connectorInput, Connector* connectorOutput);
    ~AddWire();

    void redo() override;
    void undo() override;

private:
    Scene* m_scene;
    Wire* m_wire;
    Connector* m_connectorInput;
    Connector* m_connectorOutput;
};

#endif // ADDWIRE_H
