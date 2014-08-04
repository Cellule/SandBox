#include "Rook.h"
#include "ChessGrid.h"
#define max(a,b)    (((a) > (b)) ? (a) : (b))

Rook::Rook( PieceConstructorArgs ) :
PieceSuperInit
{
}


Rook::~Rook()
{
}


void Rook::FindEats( ChessGrid* grid, vector<ChessPiece*>& canEat )
{
#define CheckPiece(piece,e) \
    if( piece )\
    {\
    e = true;\
    if( piece->GetTeam() != mTeam )\
    {\
    canEat.push_back( piece ); \
}\
}
    bool eaten1 = false;
    bool eaten2 = false;
    bool eaten3 = false;
    bool eaten4 = false;

    for( int i = 1; i < max( Grid_Rows, Grid_Colomns ); ++i )
    {
        int x1 = mLocation.row + i;
        int y1 = mLocation.colomn + i;
        int x2 = mLocation.row - i;
        int y2 = mLocation.colomn - i;
        bool isDone = true;
        if( x1 < Grid_Rows )
        {
            if( !eaten1 && y1 < Grid_Colomns )
            {
                isDone = false;
                ChessPiece* piece = grid->GetPiece( x1, y1 );
                CheckPiece( piece, eaten1 );
            }
            if( !eaten2 && y2 >= 0 )
            {
                isDone = false;
                ChessPiece* piece = grid->GetPiece( x1, y2 );
                CheckPiece( piece, eaten2 );
            }
        }
        if( x2 >= 0 )
        {
            if( !eaten3 && y1 < Grid_Colomns )
            {
                isDone = false;
                ChessPiece* piece = grid->GetPiece( x2, y1 );
                CheckPiece( piece, eaten3 );
            }
            if( !eaten4 && y2 >= 0 )
            {
                isDone = false;
                ChessPiece* piece = grid->GetPiece( x2, y2 );
                CheckPiece( piece, eaten4 );
            }
        }
        if( isDone )
        {
            break;
        }
    }
}

