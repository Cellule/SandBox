#pragma once
#include "ChessPiece.h"
class Queen :
	public ChessPiece
{
public:
    Queen( PieceConstructorArgs );
	~Queen();
    virtual ChessPiece* Copy()const
    {
        return new Queen( mTeam, mLocation );
    }
};

