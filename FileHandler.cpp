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
#include "Label.h"

#include "Parts/IntegratedCircuit.h"

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
    // Add each and it's data into the file
    for(auto part : m_logic->m_parts)
    {
        fOut << part << " " << part->m_partType << " " << part->pos().x() << " " << part->pos().y();
        if(part->m_partType == PartType::IntegratedCircuit)
        {
            // Strip out IC file path
            QFileInfo fileInfo(((IntegratedCircuit*)part)->filename());
            QString icFile(fileInfo.fileName());
            // Write IC file name into file
            fOut << " \"" << icFile << "\"";
        }
        // Write label text into file
        fOut << " \"" << part->m_label->text() << "\"";
        fOut << "\n";
    }

    fOut << "[wires]\n";
    // Add each wire and it's data into the file
    for(auto wire : m_logic->m_wires)
    {
        auto wireInputPart = wire->m_connectorInput->parentPart();
        auto wireOutputPart = wire->m_connectorOutput->parentPart();

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
        None,
        Parts,
        Wires
    };

    Sector currentSector = None;

    // A .csim file stores all parts with IDs, this map simply keeps track of which ID corresponds to which pointer
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
            // If the parser is in between two double quotes ("")
            bool parserInString = false;
            for(QChar c : line)
            {
                if(c == QChar::Space && !parserInString)
                {
                    words.append(currWord);
                    currWord.clear();
                }
                else if(c == '\"')
                {
                    parserInString = !parserInString;
                }
                else
                    currWord.append(c);
            }
            words.append(currWord);
            // Remove the \n from last word
            words.last().remove(words.last().length() - 1, 1);

            if(words.length() < 4)
                return false;

            if(currentSector == None)
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
                    part->setLabel(words[5]);
                }
                else
                {
                    part = m_logic->createPart(type, pos);
                    part->setLabel(words[4]);
                }
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
