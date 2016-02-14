#ifndef RCDEFINES_H_INCLUDED
#define RCDEFINES_H_INCLUDED

namespace RC
{
    const int CUBIE_COUNT = 3;
    const float CUBE_EDGE = 1.0;

    // rotation axis
    enum class RA : int
    {
        X = 0, Y, Z,
        Count,
        First = X,
        None = -1
    };

    // cube face
    enum class CF : int
    {
        Front = 0, Up, Back, Down, Left, Right,
        Count,
        None = -1
    };

    // game key
    enum class GK : int
    {
        TurnF = 0, TurnFI, TurnB, TurnBI, TurnL, TurnLI,
        TurnR, TurnRI, TurnU, TurnUI, TurnD, TurnDI,
        TurnFM, TurnFMI, TurnUM, TurnUMI, TurnRM, TurnRMI,

        RotateUp, RotateDown, RotateLeft, RotateRight, RotateCW, RotateACW,

        MouseMove, MouseRotate,

        GameColor, GameProj, GameReset, GameMix, GameUndo, GameBG,

        Count,
        First = 0,
        Last = GameUndo,
        None = -1
    };

    // rotate type
    enum class RT : int
    {
        Up = 0, Down, Left, Right, CW, ACW,
        Count,
        None = -1
    };

    // turn type
    // F=Front, B=Back, L=Left, R=Right, U=Up, D=Dowb, I=Inverse (anticlockwise), M=Middle
    enum class TT : int
    {
        F = 0, FI, B, BI, L, LI, R, RI, U, UI, D, DI, FM, FMI, UM, UMI, RM, RMI,

        Count,
        First = F,
        Last = RMI,
        None = -1
    };
}

#endif // RCDEFINES_H_INCLUDED
