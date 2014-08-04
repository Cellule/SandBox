#pragma once
#include "ChessPiece.h"
class Knight :
	public ChessPiece
{
public:
    Knight( PieceConstructorArgs );
	~Knight();

    virtual ChessPiece* Copy()const
    {
        return new Knight( mTeam, mLocation );
    }
};

