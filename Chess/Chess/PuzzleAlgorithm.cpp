#include "AllPieces.h"
#include "ChessGrid.h"
#include <deque>
#include <vector>
#include <iostream>

int main()
{
    ChessGrid* grid = new ChessGrid();
    ChessPiece* const pieces[] = {
        // Black Team
        new Pawn( Black, Location( 0, 3 ) ),
        new Pawn( Black, Location( 1, 2 ) ),
        new Pawn( Black, Location( 1, 5 ) ),
        new Pawn( Black, Location( 2, 4 ) ),
        new Pawn( Black, Location( 3, 6 ) ),
        new Pawn( Black, Location( 3, 7 ) ),
        new Pawn( Black, Location( 4, 4 ) ),
        new Pawn( Black, Location( 5, 3 ) ),
        new Pawn( Black, Location( 5, 5 ) ),
        new Pawn( Black, Location( 6, 0 ) ),
        new Pawn( Black, Location( 6, 2 ) ),
		new Pawn( Black, Location( 6, 5 ) ),
		new Pawn( Black, Location( 7, 1 ) ),
		new Pawn( Black, Location( 7, 5 ) ),

        // white team
        new Rook( White, Location( 2, 2 ) ),
		new Bishop( White, Location( 2, 5 ) ),
		new Knight( White, Location( 5, 2 ) ),
    };
    const int nbPieces = sizeof( pieces ) / sizeof( ChessPiece* );
    for (int i = 0; i < nbPieces ; ++i)
    {
        grid->AddPiece( pieces[i] );
    }
    deque<ChessGrid*> queue;
    queue.push_back( grid );

    bool stop = false;
    while( !queue.empty() && !stop )
    {
        grid = queue.back();
        queue.pop_back();
        
        vector<ChessGrid*> possibleMoves;
        grid->FindMoves( possibleMoves );
        const int nb = possibleMoves.size();
        for (int i = 0; i < nb ; ++i)
        {
            if( possibleMoves[i]->HasWon() )
            {
                const auto& moves = possibleMoves[i]->mMoves;
                const int nbMoves = moves.size();
                for (int j = 0; j < nbMoves ; ++j)
                {
                    printf( "Move %c%-2d => %c%-2d\n"
							, moves[j].prev.colomn + 'A'
                            , moves[j].prev.row + 1
                            , moves[j].next.colomn + 'A'
                            , moves[j].next.row + 1
                            );
                }
                cout << "Win!\n";
                for (int j = i; j < nb ; ++j)
                {
                    delete possibleMoves[j];
                }
                stop = true;
                break;
            }
            queue.push_back( possibleMoves[i] );
        }

        delete grid;
    }

    while( !queue.empty() )
    {
        grid = queue.front();
        queue.pop_front();
        delete grid;
    }

    return 0;
}