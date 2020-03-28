#ifndef CIRCUITCONNECTION_H
#define CIRCUITCONNECTION_H

#include <QGraphicsItem>
#include <QList>
#include "eConnectorType.h"

class Part;
class Wire;
class Scene;

class Connector : private QGraphicsItem
{
public:
    friend class Scene;
    friend class Logic;
    friend class Part;
    friend class Wire;
    friend class AddPart;
    friend class RemoveParts;
    friend class CopyParts;

    friend class MainWindow;

    Connector(Scene* scene, Part *parentPart, ConnectorType::ConnectorType side);

    Part* parentPart();

    QRectF boundingRect() const override; // For drawing
    QPainterPath shape() const override; // For selection ("Hitbox")
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    ConnectorType::ConnectorType connectorType();

private:
    Scene* m_scene;

    QList<Wire*> m_wires;
    ConnectorType::ConnectorType m_connectorType;
    bool m_state = false;
    bool m_selected = false; // Separate from regular selection, for creating connections

    void select(); // Called by MainWindow
    void unselect(); // Called by MainWindow
};

#endif // CIRCUITCONNECTION_H
