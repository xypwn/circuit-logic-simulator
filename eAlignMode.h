#ifndef EALIGNMODE_H
#define EALIGNMODE_H

#define mkAlignMode(hPolicy, vPolicy) (AlignMode::AlignMode)(hPolicy | vPolicy)

namespace AlignMode
{
    enum AlignMode : int
    {
        Left = 1,
        Right = 2,
        HCenter = 4,
        Top = 8,
        Bottom = 16,
        VCenter = 32,

        Default = Left | Top
    };
}

#endif // EALIGNMODE_H
