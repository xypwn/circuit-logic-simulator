#include "MoveParts.h"

#include "../Part.h"

MoveParts::MoveParts(Scene* scene, const QList<Part*>& parts, QPointF relPos)
    :m_scene(scene), m_parts(parts), m_relPos(relPos)
{
    setText("Move parts by " + QString::number(relPos.x()) + ", " + QString::number(relPos.y()));
}

MoveParts::~MoveParts()
{
}

void MoveParts::redo()
{
    if(m_isFirstRedo)
        m_isFirstRedo = false;
    else
    {
        for(auto part : m_parts)
            part->moveBy(m_relPos.x(), m_relPos.y());
    }
}


void MoveParts::undo()
{
    for(auto part : m_parts)
        part->moveBy(-m_relPos.x(), -m_relPos.y());
}
