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
        new Pawn( Black, Location( 0, 1 ) ),
        new Pawn( Black, Location( 0, 2 ) ),
        new Pawn( Black, Location( 0, 5 ) ),
        new Pawn( Black, Location( 2, 5 ) ),
        new Pawn( Black, Location( 2, 7 ) ),
        new Pawn( Black, Location( 3, 2 ) ),
        new Pawn( Black, Location( 3, 4 ) ),
        new Pawn( Black, Location( 3, 5 ) ),
        new Pawn( Black, Location( 3, 7 ) ),
        new Pawn( Black, Location( 4, 7 ) ),
        new Pawn( Black, Location( 6, 3 ) ),
        new Pawn( Black, Location( 6, 5 ) ),

        // white team
        new Bishop( White, Location( 0, 3 ) ),
        new Rook( White, Location( 7, 4 ) ),
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
                            , moves[j].prev.row + 'A'
                            , moves[j].prev.colomn + 1
                            , moves[j].next.row + 'A'
                            , moves[j].next.colomn + 1
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