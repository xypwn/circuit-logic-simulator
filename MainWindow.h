#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QToolButton>
#include <QUndoStack>
#include <QUndoView>
#include <QPushButton>
#include "eConnectorType.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Connector;
class Part;
class Wire;
class Scene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    friend class Part;
    friend class Logic;
    friend class Scene;

    enum ToolMode
    {
        Connect,
        Disconnect,
        Select,
        Pan
    };

    MainWindow(QWidget *parent = nullptr, QString loadFile = "");
    ~MainWindow();

    QString currFilename();
    bool isFileLoaded();

private:
    Ui::MainWindow *ui;

    Scene* m_scene;
    ToolMode toolMode;
    QToolButton *selTool;

    bool simRunning = false;

    QUndoView *undoView;

    bool m_fileLoaded = false;
    QString m_title;
    QString m_currFilename;

    QVector<QPushButton*> m_icPushButtons;

    // True if unsaved changes have been made to the file
    bool unsavedChanges = false;

    // Label seen in the "Integrated Circuits" tab
    bool tabICInfoLabelExists = true;

    void resetTools();

    void changeMade();
    void changesSaved();

    void closeEvent(QCloseEvent *event) override;

    void reloadLocalICs();

private slots:
    void updateMatrix();

    void zoomIn();
    void zoomOut();
    void resetZoom();
    void toggleUndoView();

    void editUndo();
    void editRedo();
    void editCut();
    void editCopy();
    void editPaste();
    void editDelete();

    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void fileExit();

    void toolConnect();
    void toolDisconnect();
    void toolSelect();
    void toolPan();

    void toolAddComponent();
    void toolAddIC();


    void simStep();
    void simStart();
    void simStop();

    void onLogicTimerTimeout();
    void onGfxTimerTimeout();
};
#endif // MAINWINDOW_H
