#pragma once
using namespace std;


const int Grid_Rows = 8;
const int Grid_Colomns = 8;

struct Location
{
    Location( int row_, int colomn_ ) :row( row_ ), colomn( colomn_ )
    {
    }
	int row, colomn;
};

enum TeamColor
{
	Black,
	White,
	MAX_TEAM
};