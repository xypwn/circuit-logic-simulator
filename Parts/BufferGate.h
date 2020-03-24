#ifndef BUFFERGATE_H
#define BUFFERGATE_H

#include "../Part.h"

class BufferGate : public Part
{
public:
    BufferGate(Logic* logic);

    QVector<bool> compute(QVector<bool> inputs) override;
    QPainterPath symbolPainterPath(QRect limits) override;
};

#endif // BUFFERGATE_H
