#pragma once
#include "ChessPiece.h"
class King :
	public ChessPiece
{
public:
    King( PieceConstructorArgs );
	~King();
    virtual ChessPiece* Copy()const
    {
        return new King( mTeam, mLocation );
    }
};

