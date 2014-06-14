#pragma once
#include "ChessPiece.h"
class Bishop :
	public ChessPiece
{
public:
    Bishop( PieceConstructorArgs );
	~Bishop();
    virtual ChessPiece* Copy()const
    {
        return new Bishop( mTeam, mLocation );
    }

    virtual void FindEats( ChessGrid* grid, vector<ChessPiece*>& canEat );

};

