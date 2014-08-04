#include "Bishop.h"
#include "ChessGrid.h"


Bishop::Bishop( PieceConstructorArgs ) :
PieceSuperInit
{
}


Bishop::~Bishop()
{
}

void Bishop::FindEats( ChessGrid* grid, vector<ChessPiece*>& canEat )
{
#define CheckPiece(piece) \
    if( piece )\
    {\
        if( piece->GetTeam() != mTeam )\
        {\
            canEat.push_back( piece );\
        }\
        break;\
    }

    for( int i = mLocation.colomn + 1; i < Grid_Colomns; ++i )
    {
        ChessPiece* piece = grid->GetPiece( mLocation.row, i );
        CheckPiece( piece );
    }
    for( int i = mLocation.colomn - 1; i >= 0; --i )
    {
        ChessPiece* piece = grid->GetPiece( mLocation.row, i );
        CheckPiece( piece );
    }
    for( int i = mLocation.row + 1; i < Grid_Rows; ++i )
    {
        ChessPiece* piece = grid->GetPiece( i,mLocation.colomn );
        CheckPiece( piece );
    }
    for (int i = mLocation.row-1; i >= 0 ; --i)
    {
        ChessPiece* piece = grid->GetPiece( i, mLocation.colomn );
        CheckPiece( piece );
    }
}
