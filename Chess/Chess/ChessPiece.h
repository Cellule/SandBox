#pragma once
#include "Utils.h"
#include <vector>
#define PieceConstructorArgs TeamColor team, Location location
#define PieceSuperInit ChessPiece(team,location)
class ChessGrid;
class ChessPiece
{
protected:
    TeamColor mTeam;
    Location mLocation;

public:
    ChessPiece( PieceConstructorArgs );
	virtual ~ChessPiece();
    virtual ChessPiece* Copy()const = 0;
    virtual void FindEats( ChessGrid* grid, vector<ChessPiece*>& canEat )
    {
    }

public:
	inline Location GetLocation() const{return mLocation;}
	inline void SetLocation( const Location& val ){mLocation = val;}
	inline TeamColor GetTeam() const{return mTeam;}
	inline void SetTeam( TeamColor val ){mTeam = val;}
};

