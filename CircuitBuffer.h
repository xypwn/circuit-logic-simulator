#ifndef CIRCUITBUFFER_H
#define CIRCUITBUFFER_H

class Part;
class Wire;
class Scene;

#include <QList>
#include <QPointF>

#include "ePartType.h"

class CircuitBuffer
{
public:
    struct PartData
    {
        PartType::PartType type;
        QPointF pos;
        QString icFilename;
    };

    struct WireData
    {
        PartData* inputPart;
        int inputPartConnectorIdx;
        PartData* outputPart;
        int outputPartConnectorIdx;
    };

    CircuitBuffer();

    void addFromScene(const QList<Part*>& parts, const QList<Wire*>& wires);
    QPair<QList<Part*>, QList<Wire*>> addIntoScene(Scene* scene, QPointF posOffset) const;
    QPointF getAvgPartPos();

    void clear();

private:
    QList<PartData> m_parts;
    QList<WireData> m_wires;
};

#endif // CIRCUITBUFFER_H
