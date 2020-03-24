#ifndef LOGIC_H
#define LOGIC_H

// This class has everything nescessary to load, save and step through a circuit.
// It does not handle anything graphical or related to undo/redo functionality

#include <QList>
#include <QPointF>
#include "ePartType.h"

class Part;
class Wire;
class Connector;
class Scene;
class IntegratedCircuit;

class Logic
{
public:
    friend class Scene;
    friend class FileHandler;
    friend class Part;

    friend class AddPart;
    friend class RemoveParts;
    friend class AddWire;
    friend class RemoveWire;
    friend class CopyParts;

    friend class CircuitBuffer;

    friend class IntegratedCircuit;

    Logic(Scene* parentScene = nullptr);

    Scene* parentScene();

    void doLogicStep();

    bool saveToFile(QString filenname);
    bool loadFromFile(QString filenname);

    // Not undoable
    IntegratedCircuit* createIC(QString filename, QPointF pos);
    // Not undoable
    Part* createPart(PartType::PartType partType, QPointF pos);
    // Not undoable
    Wire* createWire(Connector *inputConnector, Connector *outputConnector);

    // Not undoable
    void deletePart(Part* part);
    // Not undoable
    void deleteWire(Wire *wire);

private:
    // May be nullptr if there is no parent Scene and this class is purely used for logic
    Scene* m_parentScene;

    QList<Part*> m_parts;
    QList<Wire*> m_wires;
    QList<Connector*> m_inputConnectors;
    QList<Connector*> m_outputConnectors;

};

#endif // LOGIC_H
