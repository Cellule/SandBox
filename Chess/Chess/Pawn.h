#pragma once
#include "ChessPiece.h"
class Pawn :
	public ChessPiece
{
public:
    Pawn( PieceConstructorArgs );
	~Pawn();
    virtual ChessPiece* Copy()const
    {
        return new Pawn( mTeam, mLocation );
    }
};

