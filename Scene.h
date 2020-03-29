#ifndef SCENE_H
#define SCENE_H

// The Scene class holds all important information to make a circuit work graphically.
// This includes things like undo, redo, etc...

#include <QGraphicsScene>
#include <QUndoStack>

#include "ePartType.h"
#include "CircuitBuffer.h"

#include "UndoCommands/CopyParts.h"

class MainWindow;
class Part;
class Wire;
class Connector;

class FileHandler;
class Logic;

class CopyParts;

class Scene : public QGraphicsScene
{
public: 
    friend class Part;
    friend class Wire;
    friend class Connector;
    friend class MainWindow;

    friend class AddPart;
    friend class RemoveParts;
    friend class AddWire;
    friend class RemoveWire;
    friend class CopyParts;
    friend class IntegratedCircuit;

    friend class Logic;

    friend class CircuitBuffer;

    Scene(QGraphicsView *parentGfxView, MainWindow *parentMainWindow);
    ~Scene();

    void updateGraphics();

    void undo();
    void redo();

    // Initialized a Copy (only puts the parts into copy buffer)
    void initCopy(const QList<Part*> &parts, const QList<Wire*> &wires);
    // Does the copy using previously populated copy buffer (pastes parts from copy buffer)
    void doCopy(bool relToMousePos = false);

    // Adds an IC undoably
    void addIC(QString filename);
    // Adds part undoably
    void addPart(PartType::PartType partType);
    // Adds wire undoably
    void addWire(Connector* inputConnector, Connector* outputConnector);

    // Removes part undoably
    void removeParts(const QList<Part*>& parts);
    // Removes wire undoably
    void removeWire(Wire* wire);
    // Undoably moves parts
    void moveParts(const QList<Part*>& parts, QPointF relPos);

private:
    MainWindow *m_parentMainWindow;
    QGraphicsView *m_parentGfxView;

    Logic *m_logic;

    QUndoStack *m_undoStack;

    Connector* m_selectedInputConnector = nullptr;
    Connector* m_selectedOutputConnector = nullptr;

    CircuitBuffer m_copyBuffer;

    void startTrackingPart(Part* part);
    void stopTrackingPart(Part* part);

    void startTrackingWire(Wire* wire);
    void stopTrackingWire(Wire* wire);

    // Called by Connector when it is clicked, for example when creating or deleting wires
    void connectorClicked(Connector *connector);
    // Removal is done undoably
    void removeConnectorsConnections(Connector *connector);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCENE_H
