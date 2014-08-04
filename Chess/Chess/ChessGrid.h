#pragma once
#include "Utils.h"
#include <list>
#include <vector>

class ChessPiece;

struct ChessMove
{
    ChessMove() :prev( 0,0 ), next( 0,0 )
    {
    }
    Location prev, next;
};

class ChessGrid
{
	std::list<ChessPiece*> mTeamPieces[MAX_TEAM];
	ChessPiece* mGrid[Grid_Rows][Grid_Colomns];
public:
    vector<ChessMove> mMoves;
	ChessGrid();
	~ChessGrid();

	void AddPiece( ChessPiece* piece );
    ChessPiece* GetPiece( int x, int y )
    {
        return mGrid[x][y];
    }
    bool HasWon()const;
    void FindMoves( vector<ChessGrid*>& possibleMoves );

	ChessGrid* Copy();
    void Move( ChessPiece* p1, ChessPiece* p2 );
};

