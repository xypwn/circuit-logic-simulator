#ifndef CIRCUITWIRE_H
#define CIRCUITWIRE_H

#include <QGraphicsLineItem>

class Connector;
class Scene;

class Wire : public QGraphicsLineItem
{
public:
    friend class Connector;
    friend class Scene;
    friend class Logic;

    friend class RemoveWire;

    friend class MainWindow;

    friend class FileHandler;

    friend class CircuitBuffer;

    // Assumes that the CircuitConnectors A and B each have a different orientation
    Wire(Scene* scene, Connector *A, Connector *B);

    void updateLine(); // Graphical update
    void feedInput(); // Logical update (feeds input to output and updates state, also causes a graphical update)
private:
    Scene* m_scene;

    Connector* m_connectorInput; // connector inputting to this wire
    Connector* m_connectorOutput; // connector receiving output from this wire

    bool m_state = false;
};

#endif // CIRCUITWIRE_H
