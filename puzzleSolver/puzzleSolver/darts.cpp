
#include <iostream>

using namespace std;
#if 0
const int green = 1, blue = 2, red = 4;
const char* colorName[5] = { "","Green", "Blue", "","Red" };

struct slot
{
    int val;
    int color;
    int f;
    int getVal()const
    {
        return f*val;
    }
    bool operator<( const slot& s )const
    {
        return val < s.val;
    }
};


int main()
{
    const int val[] = { 20, 9, 11, 6, 8, 13, 17, 7 };
    const int colorSequence[] = {blue,red,green,blue,green,red,blue,green};
    const int valSize = sizeof( val ) / 4;
    const int allValSize = valSize * 3;
    slot allVal[allValSize];

    int index = 0;
    for( int f = 1; f <= 3; ++f )
    {
        for( int i = 0; i < valSize;++i )
        {
            allVal[index].val = val[i];
            allVal[index].f =  f;
            int colorIndex = ( i - ( f - 1 ) );
            if( colorIndex < 0 )
            {
                colorIndex = valSize + colorIndex;
            }
            allVal[index++].color = colorSequence[colorIndex];
        }
    }

    for( int i = 0; i < allValSize; ++i )
    {
        for( int j = i+1; j < allValSize; ++j )
        {
            for( int k = j+1; k < allValSize; ++k )
            {
                slot v1 = allVal[i];
                slot v2 = allVal[j];
                slot v3 = allVal[k];
                if( v1.getVal() + v2.getVal() + v3.getVal() == 91 )
                {
                    if( ( v1.color | v2.color | v3.color ) == ( blue | green | red ) )
                    {

                        slot temp;
                        //sort them
                        if( v1.getVal()>v3.getVal() )
                        {
                            temp = v1;
                            v1 = v3;
                            v3 = temp;
                        }
                        if( v2.getVal() > v3.getVal() )
                        {
                            temp = v2;
                            v2 = v3;
                            v3 = temp;
                        }
                        if( v1.getVal() > v2.getVal() )
                        {
                            temp = v1;
                            v1 = v2;
                            v2 = temp;
                        }

                        // check condition
                        if( v1.color == red && v3.color == green )
                        {
                            printf( "%d(x%d)(%s) < %d(x%d)(%s) < %d(x%d)(%s)\n", 
                                    v1.val,
                                    v1.f,
                                    colorName[v1.color], 
                                    v2.val, 
                                    v2.f,
                                    colorName[v2.color],
                                    v3.val, 
                                    v3.f,
                                    colorName[v3.color]
                                    );
                        }

                        if( v1.color == green && v3.color == red )
                        {
                            printf( "%d(x%d)(%s) > %d(x%d)(%s) > %d(x%d)(%s)\n",
                                    v1.val,
                                    v1.f,
                                    colorName[v1.color],
                                    v2.val,
                                    v2.f,
                                    colorName[v2.color],
                                    v3.val,
                                    v3.f,
                                    colorName[v3.color]
                                    );
                        }
                    }
                }
            }
        }
    }




    return 0;
}








#endif

