#include "Knight.h"
#include "ChessGrid.h"


Knight::Knight( PieceConstructorArgs ) :
PieceSuperInit
{
}


Knight::~Knight()
{
}

void Knight::FindEats( ChessGrid* grid, vector<ChessPiece*>& canEat )
{
	const int row = mLocation.row, col = mLocation.colomn;
	const int pos[8][2] =
	{
		{ row + 2, col + 1 },
		{ row + 2, col - 1 },
		{ row - 2, col + 1 },
		{ row - 2, col - 1 },

		{ row + 1, col + 2 },
		{ row + 1, col - 2 },
		{ row - 1, col + 2 },
		{ row - 1, col - 2 },
	};

	for (int i = 0; i < 8 ; ++i)
	{
		if( (unsigned int)pos[i][0] < Grid_Rows &&
			(unsigned int)pos[i][1] < Grid_Colomns )
		{
			ChessPiece* piece = grid->GetPiece( pos[i][0], pos[i][1] );
			if( piece && piece->GetTeam() != GetTeam() )
			{
				canEat.push_back( piece );
			}
		}
	}

}
