QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CircuitBuffer.cpp \
    Label.cpp \
    Logic.cpp \
    Parts/AndGate.cpp \
    Parts/BufferGate.cpp \
    Parts/HighConstant.cpp \
    Parts/IntegratedCircuit.cpp \
    Parts/LightBulb.cpp \
    Parts/LowConstant.cpp \
    Parts/NandGate.cpp \
    Parts/NorGate.cpp \
    Parts/NotGate.cpp \
    Parts/OrGate.cpp \
    Parts/ToggleButton.cpp \
    Parts/XnorGate.cpp \
    Parts/XorGate.cpp \
    Connector.cpp \
    FileHandler.cpp \
    MainWindow.cpp \
    Part.cpp \
    Scene.cpp \
    UndoCommands/AddPart.cpp \
    UndoCommands/AddWire.cpp \
    UndoCommands/CopyParts.cpp \
    UndoCommands/MoveParts.cpp \
    UndoCommands/RemoveParts.cpp \
    UndoCommands/RemoveWire.cpp \
    Wire.cpp \
    main.cpp

HEADERS += \
    CircuitBuffer.h \
    Label.h \
    Logic.h \
    Parts/AndGate.h \
    Parts/BufferGate.h \
    Parts/HighConstant.h \
    Parts/IntegratedCircuit.h \
    Parts/LightBulb.h \
    Parts/LowConstant.h \
    Parts/NandGate.h \
    Parts/NorGate.h \
    Parts/NotGate.h \
    Parts/OrGate.h \
    Parts/ToggleButton.h \
    Parts/XnorGate.h \
    Parts/XorGate.h \
    Connector.h \
    FileHandler.h \
    MainWindow.h \
    Part.h \
    Scene.h \
    UndoCommands/AddPart.h \
    UndoCommands/AddWire.h \
    UndoCommands/CopyParts.h \
    UndoCommands/MoveParts.h \
    UndoCommands/RemoveParts.h \
    UndoCommands/RemoveWire.h \
    Wire.h \
    eAlignMode.h \
    eConnectorType.h \
    ePartType.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
