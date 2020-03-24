#ifndef EPARTTYPE_H
#define EPARTTYPE_H

namespace PartType
{
    enum PartType
    {
        GateBuffer,
        GateNot,
        GateAnd,
        GateOr,
        GateNand,
        GateNor,
        GateXor,
        GateXnor,
        IOLowConstant,
        IOHighConstant,
        IOLightBulb,
        IOToggleButton,
        IntegratedCircuit,
    };
}

#endif // EPARTTYPE_H
