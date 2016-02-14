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
        COUNT,
        FIRST = X,
        NONE = -1
    };

    // cube face
    enum class CF : int
    {
        FRONT = 0, UP, BACK, DOWN, LEFT, RIGHT,
        COUNT,
        NONE = -1
    };

    // game key
    enum class GK : int
    {
        TurnF = 0, TurnFI, TurnB, TurnBI, TurnL, TurnLI,
        TurnR, TurnRI, TurnU, TurnUI, TurnD, TurnDI,
        TurnFM, TurnFMI, TurnUM, TurnUMI, TurnRM, TurnRMI,

        RotateUP, RotateDOWN, RotateLEFT, RotateRIGHT, RotateCW, RotateACW,

        MOUSEMOVE, MOUSEROTATE,

        GAMECOLOR, GAMEPROJ, GAMERESET, GAMEMIX, GAMEUNDO, GAMEBG,

        COUNT,
        FIRST = 0,
        LAST = GAMEUNDO,
        NONE = -1
    };

    // rotate type
    enum class RT : int
    {
        UP = 0, DOWN, LEFT, RIGHT, CW, ACW,
        COUNT,
        NONE = -1
    };

    // turn type
    // F=Front, B=Back, L=Left, R=Right, U=Up, D=Dowb, I=Inverse (anticlockwise), M=Middle
    enum class TT : int
    {
        F = 0, FI, B, BI, L, LI, R, RI, U, UI, D, DI, FM, FMI, UM, UMI, RM, RMI,

        COUNT,
        FIRST = F,
        LAST = RMI,
        NONE = -1
    };
}

#endif // RCDEFINES_H_INCLUDED
