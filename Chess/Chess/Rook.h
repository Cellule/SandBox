#pragma once
#include "ChessPiece.h"
class Rook :
	public ChessPiece
{
public:
    Rook( PieceConstructorArgs );
	~Rook();
    virtual ChessPiece* Copy()const
    {
        return new Rook( mTeam, mLocation );
    }
    virtual void FindEats( ChessGrid* grid, vector<ChessPiece*>& canEat );
};

