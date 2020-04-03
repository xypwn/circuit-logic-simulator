#include "Scene.h"

#include <MainWindow.h>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include "Connector.h"
#include "Wire.h"
#include "Part.h"

#include "UndoCommands/AddPart.h"
#include "UndoCommands/AddWire.h"
#include "UndoCommands/MoveParts.h"
#include "UndoCommands/RemoveParts.h"
#include "UndoCommands/RemoveWire.h"

#include "FileHandler.h"

#include "Logic.h"


Scene::Scene(QGraphicsView *parentGfxView, MainWindow *parentMainWindow)
    :m_parentMainWindow(parentMainWindow), m_parentGfxView(parentGfxView), m_logic(new Logic(this)), m_undoStack(new QUndoStack)
{

}

Scene::~Scene()
{
    delete m_logic;
}

void Scene::updateGraphics()
{
    // Update all wires graphically
    for(auto wire : m_logic->m_wires)
    {
        wire->updateLine();
        wire->update();
    }
    // Update all parts graphically
    for(auto part : m_logic->m_parts)
    {
        part->update();
    }
}

void Scene::undo()
{
    m_undoStack->undo();
    m_parentMainWindow->changeMade();
}

void Scene::redo()
{
    m_undoStack->redo();
    m_parentMainWindow->changeMade();
}

void Scene::initCopy(const QList<Part*> &parts, const QList<Wire*> &wires)
{
    m_copyBuffer.clear();
    m_copyBuffer.addFromScene(parts, wires);
}

void Scene::doCopy(bool relToMousePos)
{
    // Get scene cursor pos
    QPoint gfxViewCursorPos = m_parentGfxView->mapFromGlobal(QCursor::pos());
    QPointF sceneCursorPos = m_parentGfxView->mapToScene(gfxViewCursorPos);

    // Get graphics view center scene coordinates
    QPointF gfxViewSceneCenter = m_parentGfxView->mapToScene(m_parentGfxView->viewport()->rect().center());

    CopyParts* copyPartsCommand;
    if(relToMousePos)
        // Paste in the parts with an offset of: (mouse pos) - (average part pos)
        // Because the offset is relative to the part's previous position
        copyPartsCommand  = new CopyParts(this, m_copyBuffer, sceneCursorPos - m_copyBuffer.getAvgPartPos());
    else
        copyPartsCommand  = new CopyParts(this, m_copyBuffer, gfxViewSceneCenter - m_copyBuffer.getAvgPartPos());
    m_undoStack->push(copyPartsCommand);

    // Unselect all parts
    for(auto selectedPart : selectedItems())
        selectedPart->setSelected(false);

    // Select all recently copied parts
    for(auto part : copyPartsCommand->m_copiedParts)
        part->setSelected(true);

    m_parentMainWindow->changeMade();
}

void Scene::addIC(QString filename)
{
    // Set pos to center of user's view
    QPointF gfxViewSceneCenter = m_parentGfxView->mapToScene(m_parentGfxView->viewport()->rect().center());
    m_undoStack->push(new AddPart(this, PartType::IntegratedCircuit, gfxViewSceneCenter, filename));
    m_parentMainWindow->changeMade();
}

void Scene::addPart(PartType::PartType partType)
{
    // Set pos to center of user's view
    QPointF gfxViewSceneCenter = m_parentGfxView->mapToScene(m_parentGfxView->viewport()->rect().center());
    m_undoStack->push(new AddPart(this, partType, gfxViewSceneCenter));
    m_parentMainWindow->changeMade();
}

void Scene::addWire(Connector* inputConnector, Connector* outputConnector)
{
    m_undoStack->push(new AddWire(this, inputConnector, outputConnector));

    m_parentMainWindow->changeMade();
}

void Scene::removeParts(const QList<Part*>& parts)
{
    if(parts.isEmpty())
        return;

    m_undoStack->push(new RemoveParts(this, parts));

    m_parentMainWindow->changeMade();
}

void Scene::removeWire(Wire* wire)
{
    // Push wire into undo stack
    m_undoStack->push(new RemoveWire(this, wire));

    m_parentMainWindow->changeMade();
}

void Scene::moveParts(const QList<Part *>& parts, QPointF relPos)
{
    m_undoStack->push(new MoveParts(this, parts, relPos));

    for(auto part : parts)
    {
        part->m_oldPos = part->pos();
    }

    m_parentMainWindow->changeMade();
}

// PRIVATE

void Scene::startTrackingPart(Part* part)
{
    // Show part
    part->show();
    // Add part back into tracker list
    m_logic->m_parts.append(part);
    for(auto input : part->m_inputs)
        // Add connector back into tracker list
        m_logic->m_inputConnectors.append(input);
    for(auto output : part->m_outputs)
        // Add connector back into tracker list
        m_logic->m_outputConnectors.append(output);
}

void Scene::stopTrackingPart(Part* part)
{
    // Hide part
    part->hide();
    // Remove part from tracker list
    m_logic->m_parts.removeOne(part);
    for(auto input : part->m_inputs)
        // Remove connector from tracker list
        m_logic->m_inputConnectors.removeOne(input);
    for(auto output : part->m_outputs)
        // Remove connector from tracker list
        m_logic->m_outputConnectors.removeOne(output);
}

void Scene::startTrackingWire(Wire* wire)
{
    // Show wire
    wire->show();
    // Add wire back into tracker list
    m_logic->m_wires.append(wire);
    // Add wire back into it's input connector's wire list
    wire->m_connectorInput->m_wires.append(wire);
    // Add wire back into it's input connector's wire list
    wire->m_connectorOutput->m_wires.append(wire);
}

void Scene::stopTrackingWire(Wire* wire)
{
    // Hide wire
    wire->hide();
    // Remove wire from tracker list
    m_logic->m_wires.removeOne(wire);
    // Remove wire from it's input connector's wire list
    wire->m_connectorInput->m_wires.removeOne(wire);
    // Remove wire from it's output connector's wire list
    wire->m_connectorOutput->m_wires.removeOne(wire);
}

void Scene::connectorClicked(Connector *connector)
{
    if(m_parentMainWindow->toolMode == MainWindow::Disconnect)
    {
        removeConnectorsConnections(connector);
        return;
    }

    if(m_parentMainWindow->toolMode == MainWindow::Connect)
    {

        if(connector->m_connectorType == ConnectorType::Input)
        {
            // Toggle off if previous was clicked again
            if(m_selectedInputConnector == connector)
            {
                m_selectedInputConnector->unselect();
                m_selectedInputConnector = nullptr;
            }
            else
            {
                // Unselect previous if previous not nullptr
                if(m_selectedInputConnector) m_selectedInputConnector->unselect();
                // Update new connection
                m_selectedInputConnector = connector;
                connector->select();
            }
        }
        else
        //Same here, but with connectorSelectedRight instead of connectorSelectedLeft
        {
            if(m_selectedOutputConnector == connector)
            {
                m_selectedOutputConnector->unselect();
                m_selectedOutputConnector = nullptr;
            }
            else
            {
                if(m_selectedOutputConnector) m_selectedOutputConnector->unselect();
                m_selectedOutputConnector = connector;
                connector->select();
            }
        }
        // If both sides now have a connector, add a wire
        if(m_selectedInputConnector && m_selectedOutputConnector)
        {
            addWire(m_selectedOutputConnector, m_selectedInputConnector);
            m_selectedInputConnector->unselect();
            m_selectedInputConnector = nullptr;
            m_selectedOutputConnector->unselect();
            m_selectedOutputConnector = nullptr;
        }
    }
}

void Scene::removeConnectorsConnections(Connector *connector)
{
    // Remove wires
    for(auto wire : connector->m_wires)
    {
        removeWire(wire);
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QList<QGraphicsItem*> movedItems = selectedItems();
        if(!movedItems.isEmpty() && !(((Part*)movedItems[0])->pos() == ((Part*)movedItems[0])->m_oldPos))
        {
            QList<Part*> movedParts;
            for(auto item : movedItems)
                movedParts.append((Part*)item);
            moveParts(movedParts, movedParts[0]->pos() - movedParts[0]->m_oldPos);
        }
    }
    QGraphicsScene::mouseReleaseEvent(event);
}
