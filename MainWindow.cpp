#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsRectItem>
#include <QtMath>
#include <QScrollBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QCloseEvent>
#include <QDir>
#include <QUndoView>
#include <QPushButton>

#include "Connector.h"
#include "Connector.h"
#include "Wire.h"
#include "Part.h"
#include "Scene.h"
#include "Logic.h"

#include "UndoCommands/CopyParts.h"

MainWindow::MainWindow(QWidget *parent, QString loadFile)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Setup UI
    ui->setupUi(this);
    // Add tab widget to tool bar to make it moveable and floatable
    // TODO: Use a less hacky solution and make it work horizontally as well
    ui->toolBarBottom->addWidget(ui->tabWidget);
    // Set title name
    m_title = "Circuit Simulator";
    // Set initial window title
    setWindowTitle(m_title + " - Unsaved Circuit");
    // Initialize graphics scene
    m_scene = new Scene(ui->graphicsView, this);
    if(!loadFile.isEmpty())
    {
        m_currFilename = loadFile;
        m_fileLoaded = true;
        if(!m_scene->m_logic->loadFromFile(loadFile))
        {
            m_fileLoaded = false;
        }
        else
            setWindowTitle(m_title + " - " + QFileInfo(m_currFilename).fileName());
    }
    // Set active tool mode to select
    toolSelect();
    // Set active mode to sim started
    simStart();
    // Assign scene to graphics view and show it
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->show();
    // Setup undoView
    undoView = new QUndoView(m_scene->m_undoStack);
    undoView->setWindowTitle("Undo View");
    undoView->setAttribute(Qt::WA_QuitOnClose, false);
    // Setup graphics timer
    QTimer *gfxTimer = new QTimer(this);
    connect(gfxTimer, &QTimer::timeout, this, &MainWindow::onGfxTimerTimeout);
    gfxTimer->start(17);
    // Setup logic timer
    QTimer *logicTimer = new QTimer(this);
    connect(logicTimer, &QTimer::timeout, this, &MainWindow::onLogicTimerTimeout);
    logicTimer->start(1);
    // Refresh IC list
    reloadLocalICs();
    //CONNECT EVENTS
    connect(ui->zoomSlider, &QSlider::valueChanged, this, &MainWindow::updateMatrix);

    connect(ui->btnZoomIn, &QToolButton::clicked, this, &MainWindow::zoomIn);
    connect(ui->btnZoomOut, &QToolButton::clicked, this, &MainWindow::zoomOut);

    connect(ui->actionZoom_In, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(ui->actionZoom_Out, &QAction::triggered, this, &MainWindow::zoomOut);
    connect(ui->actionReset_Zoom, &QAction::triggered, this, &MainWindow::resetZoom);

    connect(ui->actionToggle_Undo_View, &QAction::triggered, this, &MainWindow::toggleUndoView);

    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::editUndo);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::editRedo);
    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::editCut);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::editCopy);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::editPaste);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::editDelete);

    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::fileNew);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::fileOpen);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::fileSave);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::fileSaveAs);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::fileExit);

    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::toolConnect);
    connect(ui->actionRemove_Connections, &QAction::triggered, this, &MainWindow::toolDisconnect);
    connect(ui->actionSelect, &QAction::triggered, this, &MainWindow::toolSelect);
    connect(ui->actionPan, &QAction::triggered, this, &MainWindow::toolPan);

    connect(ui->actionStep, &QAction::triggered, this, &MainWindow::simStep);
    connect(ui->actionStart, &QAction::triggered, this, &MainWindow::simStart);
    connect(ui->actionStop, &QAction::triggered, this, &MainWindow::simStop);

    connect(ui->btnGateBuffer, &QToolButton::clicked, this, &MainWindow::toolAddComponent);
    connect(ui->btnGateNot, &QToolButton::clicked, this, &MainWindow::toolAddComponent);
    connect(ui->btnGateAnd, &QToolButton::clicked, this, &MainWindow::toolAddComponent);
    connect(ui->btnGateOr, &QToolButton::clicked, this, &MainWindow::toolAddComponent);
    connect(ui->btnGateNand, &QToolButton::clicked, this, &MainWindow::toolAddComponent);
    connect(ui->btnGateNor, &QToolButton::clicked, this, &MainWindow::toolAddComponent);
    connect(ui->btnGateXor, &QToolButton::clicked, this, &MainWindow::toolAddComponent);
    connect(ui->btnGateXnor, &QToolButton::clicked, this, &MainWindow::toolAddComponent);

    connect(ui->btnIOHighConstant, &QToolButton::clicked, this, &MainWindow::toolAddComponent);
    connect(ui->btnIOLowConstant, &QToolButton::clicked, this, &MainWindow::toolAddComponent);
    connect(ui->btnIOLightBulb, &QToolButton::clicked, this, &MainWindow::toolAddComponent);
    connect(ui->btnIOToggleButton, &QToolButton::clicked, this, &MainWindow::toolAddComponent);
    //END CONNECT EVENTS
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::isFileLoaded()
{
    return m_fileLoaded;
}

QString MainWindow::currFilename()
{
    return m_currFilename;
}

//PUBLIC

void MainWindow::resetTools()
{
    // Reset button checked states
    ui->actionConnect->setChecked(false);
    ui->actionRemove_Connections->setChecked(false);
    ui->actionSelect->setChecked(false);
    ui->actionPan->setChecked(false);
    // Reset drag mode
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    // Unselect selected items
    for(auto item : m_scene->selectedItems())
        item->setSelected(false);
    // Reset graphicsView interactivity
    ui->graphicsView->setInteractive(true);
    //Reset cursor shape
    ui->graphicsView->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
}

void MainWindow::changeMade()
{
    unsavedChanges = true;
    if(m_fileLoaded)
        setWindowTitle(m_title + " - " + QFileInfo(m_currFilename).fileName() + "*");
    else
        setWindowTitle(m_title + " - Unsaved Circuit*");
}

void MainWindow::changesSaved()
{
    unsavedChanges = false;
    setWindowTitle(m_title + " - " + QFileInfo(m_currFilename).fileName());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!unsavedChanges)
    {
        event->accept();
        return;
    }

    // I just blatantly ripped off the following code from this example https://doc.qt.io/qt-5/qmessagebox.html
    QMessageBox msgBox;
    msgBox.setText("The Circuit has unsaved changes.");
    msgBox.setInformativeText("Do you really want to quit?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    switch (ret)
    {
        case QMessageBox::Save:
            fileSave();
            if(!unsavedChanges)
                event->accept();
            else
                event->ignore();
            break;
        case QMessageBox::Discard:
            event->accept();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
    }
}

void MainWindow::reloadLocalICs()
{
    // Return if no file is loaded
    if(!m_fileLoaded)
        return;

    if(tabICInfoLabelExists)
    {
        delete ui->tabICInfoLabel;
        tabICInfoLabelExists = false;
    }

    for(auto btn : m_icPushButtons)
    {
        delete btn;
    }
    m_icPushButtons.clear();

    QDir fileDir(m_currFilename);
    fileDir.cdUp();
    QStringList nameFilter("*.csim");
    QStringList icFiles = fileDir.entryList(nameFilter, QDir::Files);

    for(const auto& icFile : icFiles)
    {
        // Skip step if file name is equal to currently loaded file
        if(icFile == QFileInfo(m_currFilename).fileName())
            continue;

        QPushButton* btn = new QPushButton(ui->tabICScrollAreaWidgetContents);

        ui->tabICScrollAreaWidgetContents->layout()->addWidget(btn);
        btn->setText(icFile);
        btn->setObjectName(icFile);
        btn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        m_icPushButtons.append(btn);

        connect(btn, &QPushButton::clicked, this, &MainWindow::toolAddIC);
    }
}

//SIGNALS
void MainWindow::updateMatrix()
{
    qreal scale = qPow(qreal(2), qreal(ui->zoomSlider->value() - 100) / qreal(20));

    ui->labelZoomLevel->setText(QString::number(int(scale * 100.0)) + '%');

    QMatrix matrix;
    matrix.scale(scale, scale);
    ui->graphicsView->setMatrix(matrix);
}

void MainWindow::zoomIn()
{
    ui->zoomSlider->setValue(ui->zoomSlider->value() + ui->zoomSlider->pageStep());
}

void MainWindow::zoomOut()
{
    ui->zoomSlider->setValue(ui->zoomSlider->value() - ui->zoomSlider->pageStep());
}

void MainWindow::resetZoom()
{
    ui->zoomSlider->setValue(100);
}

void MainWindow::toggleUndoView()
{
    if(undoView->isHidden())
        undoView->show();
    else
        undoView->hide();
}

void MainWindow::editUndo()
{
    m_scene->undo();
}

void MainWindow::editRedo()
{
    m_scene->redo();
}

void MainWindow::editCut()
{
    // Put parts into copy buffer
    editCopy();
    // Remove selected parts
    QList<QGraphicsItem*> selItems = m_scene->selectedItems();
    QList<Part*> selParts;
    for(auto convItm : selItems)
        selParts.append((Part*)convItm);
    m_scene->removeParts(selParts);
}

void MainWindow::editCopy()
{
    QList<QGraphicsItem*> selItems = m_scene->selectedItems();
    QSet<Part*> selParts;
    QList<Part*> selPartsList;
    QList<Wire*> selPartsWires;

    for(auto item : selItems)
        selParts.insert((Part*)item);

    selPartsList = selParts.values();

    // Problem: find all wires of which both sides are being used by selected parts
    // Go through all selected parts
    for(auto part : selPartsList)
    {
        // Go through their outgoing connectors
        for(auto outConnector : part->m_outputs)
        {
            // Go through their wires
            for(auto outWire : outConnector->m_wires)
            {
                // See if the other end of the wire contains a part which is also selected
                if(selParts.contains((Part*)outWire->m_connectorOutput->parentItem()))
                        selPartsWires.append(outWire);
            }
        }
    }

    m_scene->initCopy(selPartsList, selPartsWires);
}

void MainWindow::editPaste()
{
    // Only do copy relative to mouse if graphicsView under mouse
    // Else, copy into center of graphics view
    m_scene->doCopy(ui->graphicsView->underMouse());
}

void MainWindow::editDelete()
{
    QList<QGraphicsItem*> selItems = m_scene->selectedItems();
    QList<Part*> selParts;
    for(auto convItm : selItems)
        selParts.append((Part*)convItm);
    m_scene->removeParts(selParts);
}

void MainWindow::fileNew()
{
    MainWindow* newWindow = new MainWindow();
    newWindow->show();
}

void MainWindow::fileOpen()
{
    QString filter = tr("CSIM Files(*.csim)");
    auto filename = QFileDialog::getOpenFileName(this, tr("Open File"), "", filter, &filter);
    if(!filename.isEmpty())
    {
        // If there is already a file loaded or there are unsaved changes, create a new window
        if(m_fileLoaded || unsavedChanges)
        {
            MainWindow* newWindow = new MainWindow(nullptr, filename);
            newWindow->show();
        }
        // Else, load file into current window
        else
        {
            if(m_scene->m_logic->loadFromFile(filename))
            {
                m_currFilename = filename;
                m_fileLoaded = true;
                setWindowTitle(m_title + " - " + QFileInfo(m_currFilename).fileName());
            }
        }
    }
    reloadLocalICs();
}

void MainWindow::fileSave()
{
    if(m_fileLoaded)
        m_scene->m_logic->saveToFile(m_currFilename);
    else
        fileSaveAs();
    reloadLocalICs();
}

void MainWindow::fileSaveAs()
{
    QString filter = tr("CSIM Files(*.csim)");
    auto filename = QFileDialog::getSaveFileName(this, tr("Save As"), "", filter, &filter);
    if(!filename.isEmpty())
    {
        if(m_scene->m_logic->saveToFile(filename))
        {
            m_fileLoaded = true;
            m_currFilename = filename;
            setWindowTitle("Circuit Simulator - " + QFileInfo(m_currFilename).fileName());
        }
    }
    reloadLocalICs();
}

void MainWindow::fileExit()
{
    this->close();
}

void MainWindow::toolConnect()
{
    resetTools();
    toolMode = Connect;
    ui->actionConnect->setChecked(true);
    ui->graphicsView->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
}

void MainWindow::toolDisconnect()
{
    resetTools();
    toolMode = Disconnect;
    ui->actionRemove_Connections->setChecked(true);
    ui->graphicsView->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
}

void MainWindow::toolSelect()
{
    resetTools();
    toolMode = Select;
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    ui->actionSelect->setChecked(true);
}

void MainWindow::toolPan()
{
    resetTools();
    toolMode = Pan;
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->setInteractive(false);
    ui->actionPan->setChecked(true);
}

void MainWindow::toolAddComponent()
{
    QToolButton *button = (QToolButton*)sender();

    if(button->objectName() == "btnGateBuffer")
        m_scene->addPart(PartType::GateBuffer);
    else if(button->objectName() == "btnGateNot")
        m_scene->addPart(PartType::GateNot);
    else if(button->objectName() == "btnGateAnd")
        m_scene->addPart(PartType::GateAnd);
    else if(button->objectName() == "btnGateOr")
        m_scene->addPart(PartType::GateOr);
    else if(button->objectName() == "btnGateNand")
        m_scene->addPart(PartType::GateNand);
    else if(button->objectName() == "btnGateNor")
        m_scene->addPart(PartType::GateNor);
    else if(button->objectName() == "btnGateXor")
        m_scene->addPart(PartType::GateXor);
    else if(button->objectName() == "btnGateXnor")
        m_scene->addPart(PartType::GateXnor);
    else if(button->objectName() == "btnIOHighConstant")
        m_scene->addPart(PartType::IOHighConstant);
    else if(button->objectName() == "btnIOLowConstant")
        m_scene->addPart(PartType::IOLowConstant);
    else if(button->objectName() == "btnIOLightBulb")
        m_scene->addPart(PartType::IOLightBulb);
    else if(button->objectName() == "btnIOToggleButton")
        m_scene->addPart(PartType::IOToggleButton);
}

void MainWindow::simStep()
{
    m_scene->m_logic->doLogicStep();
}

void MainWindow::simStart()
{
    ui->actionStart->setEnabled(false);
    ui->actionStep->setEnabled(false);
    ui->actionStop->setEnabled(true);
    simRunning = true;
}

void MainWindow::simStop()
{
    ui->actionStart->setEnabled(true);
    ui->actionStep->setEnabled(true);
    ui->actionStop->setEnabled(false);
    simRunning = false;
}

void MainWindow::onLogicTimerTimeout()
{
    if(simRunning)
        m_scene->m_logic->doLogicStep();
}

void MainWindow::onGfxTimerTimeout()
{
    m_scene->updateGraphics();
}

void MainWindow::toolAddIC()
{
    QDir fileDir(m_currFilename);
    fileDir.cdUp();
    QString path = fileDir.absoluteFilePath(sender()->objectName());
    m_scene->addIC(path);
}
//END SIGNALS
