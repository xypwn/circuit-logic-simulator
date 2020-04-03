#ifndef CIRCUITITEM_H
#define CIRCUITITEM_H

#include <QGraphicsItem>
#include <QList>
#include <QLineEdit>

#include "ePartType.h"

class Connector;
class Logic;
class Label;

class Part : protected QGraphicsItem
{
public:
    friend class Connector;
    friend class Scene;
    friend class Logic;
    friend class AddPart;
    friend class RemoveParts;
    friend class AddWire;
    friend class RemoveWire;
    friend class CopyParts;

    friend class FileHandler;

    friend class CircuitBuffer;

    friend class MainWindow;

    enum CircuitItemColorType
    {
        PenColorNormal,
        PenColorSelected,
        PenColorSymbol,
        BrushColorNormal,
        BrushColorSelected
    };

    enum CircuitItemBaseShape
    {
        RoundedRect,
        Circle // Only supports up to 1 input and 1 output
    };

    Part(Logic* logic, CircuitItemBaseShape baseShape = RoundedRect);

    PartType::PartType partType();

    QPointF getPos() const;
    void setPos(QPointF pos);

    void setLabel(QString text);
    QString label() const;

    void addInputs(int amount);
    void addOutputs(int amount);

    void setWidth(int factor);

    void addText(); //TODO
    void addTextEdit(); //TODO

    void recalculateLayout();

    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    // Symbol to be drawn inside the circuitItem
    virtual QPainterPath symbolPainterPath(QRect limits) = 0;
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value) override;
    // Take the inputs and calculate the outputs based on the part's logic
    virtual QVector<bool> compute(QVector<bool> inputs) = 0;

protected:
    Logic* m_logic;

    QVector<Connector*> m_inputs;
    QVector<Connector*> m_outputs;

    QColor m_penColorNormal;
    QColor m_penColorSelected;
    QColor m_penColorSymbol;
    QColor m_brushColorNormal;
    QColor m_brushColorSelected;

    CircuitItemBaseShape m_baseShape;

    QPointF m_oldPos;

    PartType::PartType m_partType;

    int m_widthFactor;

private:
    // Updates all of the outputs using the inputs, called by Logic
    void updateState();

    Label* m_label;
};
#endif // CIRCUITITEM_H
