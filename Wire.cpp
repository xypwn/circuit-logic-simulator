#include "Wire.h"

#include <QPainter>
#include <qdebug.h>
#include "Connector.h"
#include "eConnectorType.h"

Wire::Wire(Scene* scene, Connector *A, Connector *B)
    :m_scene(scene)
{
    if(A->connectorType() == B->connectorType())
    {
        qFatal("Instance CircuitWire cannot connect two CircuitConnectors which are on the same side of a CircuitItem");
    }
    if(A->connectorType() == ConnectorType::Output)
    {
        m_connectorInput = A;
        m_connectorOutput = B;
    }
    else
    {
        m_connectorInput = B;
        m_connectorOutput = A;
    }

    updateLine();
}

void Wire::updateLine()
{
    QPointF posI = m_connectorInput->scenePos();
    QPointF posO = m_connectorOutput->scenePos();
    setLine(posI.x() + 11.f, posI.y(), posO.x() - 11.f, posO.y());
    QPen p;
    p.setWidth(2);
    if(m_state)
        p.setColor(Qt::GlobalColor::green);
    setPen(p);
}

void Wire::feedInput()
{
    // Update output state
    //m_connectorOutput->m_state = true;
    m_connectorOutput->m_state = (m_connectorOutput->m_state || m_connectorInput->m_state);
    // Update own state
    m_state = m_connectorInput->m_state;
}
