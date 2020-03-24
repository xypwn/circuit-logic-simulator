#include "FileHandler.h"

#include <QFileInfo>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include "Scene.h"
#include "Part.h"
#include "Wire.h"
#include "Connector.h"
#include "Logic.h"
#include "MainWindow.h"

#include "Parts/IntegratedCircuit.h"

#include <QDebug>

FileHandler::FileHandler(Logic* logic)
    :m_logic(logic)
{
}

bool FileHandler::exists(QString filename)
{
    QFile file(filename);
    return file.exists();
}

bool FileHandler::save(QString filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream fOut(&file);

    fOut << "[parts]\n";
    for(auto part : m_logic->m_parts)
    {
        fOut << part << " " << part->m_partType << " " << part->pos().x() << " " << part->pos().y();
        if(part->m_partType == PartType::IntegratedCircuit)
        {
            // Strip out IC file path
            QFileInfo fileInfo(((IntegratedCircuit*)part)->filename());
            QString icFile(fileInfo.fileName());
            fOut << " " << icFile;
        }
        fOut << "\n";
    }

    fOut << "[wires]\n";
    for(auto wire : m_logic->m_wires)
    {
        auto wireInputPart = (Part*)wire->m_connectorInput->parentItem();
        auto wireOutputPart = (Part*)wire->m_connectorOutput->parentItem();

        fOut << wireInputPart << " " << wireInputPart->m_outputs.indexOf(wire->m_connectorInput) << " " << wireOutputPart << " " << wireOutputPart->m_inputs.indexOf(wire->m_connectorOutput) << "\n";
    }
    return true;
}

bool FileHandler::open(QString filename)
{
    //TODO: Make it possible to load IC filenames with spaces

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    enum Sector
    {
        Parts,
        Wires
    };

    Sector currentSector;

    // A .csim file stores all parts with IDs, this map simply keeps track of which ID was given which pointer
    QMap<QString, Part*> idPartPointerMap;

    while (!file.atEnd()) {
        QString line = file.readLine();
        if(line.contains("[parts]"))
            currentSector = Parts;
        else if(line.contains("[wires]"))
            currentSector = Wires;
        else
        {
            QVector<QString> words;
            QString currWord;
            for(QChar c : line)
            {
                if(c == QChar::Space)
                {
                    words.append(currWord);
                    currWord.clear();
                }
                else
                    currWord.append(c);
            }
            words.append(currWord);
            // Remove the \n from last word
            words.last().remove(words.last().length() - 1, 1);

            if(words.length() < 4)
                return false;

            if(currentSector == Parts)
            {
                PartType::PartType type = (PartType::PartType)words[1].toInt();
                QPointF pos = QPointF(words[2].toFloat(), words[3].toFloat());
                Part* part;
                if(type == PartType::IntegratedCircuit)
                {
                    QDir icDir(filename);
                    icDir.cdUp();
                    QString icFilename = icDir.filePath(words[4]);
                    if(!QFile(icFilename).exists())
                    {
                        qFatal("Failed to open IC file: " + icFilename.toUtf8());
                    }
                    part = m_logic->createIC(icFilename, pos);
                }
                else
                    part = m_logic->createPart(type, pos);
                idPartPointerMap.insert(words[0], part);
            }
            else
            {
                auto inputConnector = idPartPointerMap[words[0]]->m_outputs[words[1].toInt()];
                auto outputConnector = idPartPointerMap[words[2]]->m_inputs[words[3].toInt()];
                m_logic->createWire(inputConnector, outputConnector);
            }
        }
    }
    return true;
}
