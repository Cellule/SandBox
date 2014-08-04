#include "ChessGrid.h"
#include "ChessPiece.h"
#include <exception>


ChessGrid::ChessGrid()
{
	for (int i = 0; i < Grid_Rows ; ++i)
	{
	    for (int j = 0; j < Grid_Colomns ; ++j)
	    {
			mGrid[i][j] = nullptr;
	    }
	}
}


ChessGrid::~ChessGrid()
{
    for( int i = 0; i < MAX_TEAM; ++i )
    {
        for( auto it = mTeamPieces[i].begin(); it != mTeamPieces[i].end(); ++it )
        {
            delete *it;
        }
    }
}

void ChessGrid::AddPiece( ChessPiece* piece )
{
	const Location& loc = piece->GetLocation();
	if( loc.row >= Grid_Rows || loc.colomn >= Grid_Colomns )
	{
		throw exception( "Invalid piece location" );
	}
	mGrid[loc.row][loc.colomn] = piece;
	mTeamPieces[piece->GetTeam()].push_back( piece );
}

ChessGrid* ChessGrid::Copy()
{
	ChessGrid* grid = new ChessGrid();
	for (int i = 0; i < MAX_TEAM ; ++i)
	{
        for( auto it = mTeamPieces[i].begin(); it != mTeamPieces[i].end(); ++it )
        {
            grid->AddPiece( ( *it )->Copy() );
        }
	}
    if( !mMoves.empty() )
    {
        grid->mMoves.resize( mMoves.size() );
        memcpy( &grid->mMoves[0], &mMoves[0], mMoves.size()*sizeof( ChessMove ) );
    }
    return grid;
}

bool ChessGrid::HasWon() const
{
    return mTeamPieces[Black].empty();
}

void ChessGrid::FindMoves( vector<ChessGrid*>& possibleMoves )
{
    for( auto it = mTeamPieces[White].begin(); it != mTeamPieces[White].end(); ++it )
    {
        vector<ChessPiece*> canEat;
        ( *it )->FindEats( this, canEat );
        if( !canEat.empty() )
        {
            int nb = canEat.size();
            for (int i = 0; i < nb ; ++i)
            {
                ChessGrid* newGrid = Copy();
                newGrid->Move( *it, canEat[i] );
                possibleMoves.push_back( newGrid );
            }
        }
    }
}

void ChessGrid::Move( ChessPiece* p1, ChessPiece* p2 )
{
    if( !p1 || !p2 )
    {
        throw exception( "Invalid chess pieces" );
    }
    p1 = mGrid[p1->GetLocation().row][p1->GetLocation().colomn];
    p2 = mGrid[p2->GetLocation().row][p2->GetLocation().colomn];
    if( !p1 || !p2 )
    {
        throw exception( "Invalid chess pieces" );
    }
    ChessMove move;
    move.prev = p1->GetLocation();
    move.next = p2->GetLocation();
    mMoves.push_back( move );

    mTeamPieces[p2->GetTeam()].remove( p2 );
    mGrid[p1->GetLocation().row][p1->GetLocation().colomn] = nullptr;
    mGrid[p2->GetLocation().row][p2->GetLocation().colomn] = p1;
    p1->SetLocation( p2->GetLocation() );
    delete p2;
}
