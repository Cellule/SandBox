// Interesting...
// In VC++ 5.0, if you statically link with the multi-threaded C run
// time, your executable (well, this one) gets 50K bigger if you
// include <iostream> instead of <iostream.h>. Stupid!
// I'll be stdio.h would be even smaller, but I'm not going to check.
#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>

using namespace std;

typedef unsigned __int8 digit;
typedef unsigned int uint;
typedef unsigned __int8 uint8;
#define MAX_UINT 0xFFFFFFFF
#ifndef max
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

int Log2( uint v )
{
    int r; // result of log_2(v) goes here
    union { unsigned int u[2]; double d; } t; // temp

    t.u[1] = 0x43300000;
    t.u[0] = v;
    t.d -= 4503599627370496.0;
    r = (t.u[1] >> 20) - 0x3FF;
    return r;
}

class BigNumber
{
    uint* mVal;
    int mSize;
    int mCapacity;
public:
    BigNumber( uint _val )
    {
        mVal = new uint[30];
        mSize = 1;
        mCapacity = 32;
        mVal[0] = _val;
    }
    ~BigNumber()
    {
        delete[] mVal;
    }
    void EnsureSize( int size )
    {
        if( size > mCapacity )
        {
            while( size > mCapacity )
            {
                mCapacity <<= 1;
            }
            uint* newArr = new uint[mCapacity];
            for( int i = 0; i < mSize; i++ )
            {
                newArr[i] = mVal[i];
            }
            delete[] mVal;
            mVal = newArr;
        }
    }
    void Append( uint a )
    {
        EnsureSize( mSize + 1 );
        mVal[mSize++] = a;
    }
    void Add( uint a )
    {
        __asm{
            xor esi,esi
            mov edx,this
            mov eax, [edx]
            mov edx, [edx+4]
            mov ecx,a
        addArr:
            add [eax+esi*4],ecx
            jnc endAdd
            inc esi
            cmp esi, edx
            jl l2
            mov ecx,this
            push 1
            call BigNumber::Append
            jmp endAdd
        l2:
            mov ecx, 1
            jmp addArr
        endAdd:
        };
    }
    void Add( const BigNumber& bn )
    {
        const int size2 = bn.mSize;
        uint8 carry = 0;
        for (int i = 0; i < size2 ; i++)
        {
            if( i >= mSize )
            {
                Append( 0 );
            }

            if( carry && mVal[i] == MAX_UINT )
            {
                mVal[i] = bn.mVal[i];
                // leave carry at 1
            }
            else
            {
                mVal[i] += carry;
                mVal[i] += bn.mVal[i];
                __asm{
                    setc carry
                };
            }
        }
        if( carry )
        {
            Append( 1 );
        }
    }
    void Mul( uint m )
    {
        uint* newArr = new uint[mCapacity];
        uint* arr = mVal;
        uint carry = 0;
        uint s = mSize;
        __asm
        {
            xor esi,esi
            mov edi, arr
            mov ebx, newArr
loop1:
            mov eax, [edi+esi*4]
            mul m
            add eax, carry
            jnc l1
            inc edx
l1:
            mov carry, edx
            mov [ebx+esi*4], eax

            inc esi
            cmp esi, s
            jl loop1
        };

        delete[] mVal;
        mVal = newArr;
        if( carry )
        {
            Append( carry );
        }
    }

    void Mul( const BigNumber& bn )
    {
        int cap = mSize + bn.mSize;
        cap = 1 << (Log2( cap )+1);
        const int newCapacity = max( mCapacity, cap );
        uint* newArr = new uint[newCapacity];
        
        uint carry = 0;
        // init only values needed
        for (int i = 0; i < mSize + bn.mSize ; i++)
        {
            newArr[i] = 0;
        }
        for (int j = 0; j < bn.mSize ; j++)
        {
            const uint m1 = bn.mVal[j];
            for (int i = 0; i < mSize ; i++)
            {
                uint m2 = mVal[i];
                __asm{
                    mov eax,m1
                    mul m2
                    add eax,carry
                    jnc l1
                    inc edx
l1:
                    mov carry,edx
                    mov m2, eax

                    mov ecx,i
                    add ecx,j
                    mov edx, newArr
                    add [edx+ecx*4],eax
                    jnc l2
                    inc carry
                l2:
                };
            }
        }
        mSize = mSize + bn.mSize - 1;
        if( carry )
        {
            newArr[mSize++] = carry;
        }
        delete[] mVal;
        mVal = newArr;
        mCapacity = newCapacity;
    }

    void Sub( uint val )
    {
        if( mVal[0] >= val )
        {
            mVal[0] -= val;
            return;
        }
        int i = 1;
        while( mVal[i] == 0 )
        {
            mVal[i++] = MAX_UINT;
        }
        if( i >= mSize )
        {
            throw exception( "Error for x-y ===> y<=x" );
        }
        mVal[i] -= 1;
        mVal[0] = (MAX_UINT - val) + 1 + mVal[0];
        if( !mVal[i] )
        {
            --mSize;
        }
    }

    void Sub( const BigNumber& bn )
    {
        for (int i = 0; i < mSize ; ++i)
        {
            if( mVal[i] >= bn.mVal[i] )
            {
                mVal[i] -= bn.mVal[i];
            }
            else
            {
                int j = i+1;
                while( mVal[j] == 0 )
                {
                    mVal[j++] = MAX_UINT;
                }
                if( j >= mSize )
                {
                    throw exception( "Error for x-y ===> y<=x" );
                }
                mVal[j] -= 1;
                mVal[i] = ( MAX_UINT - bn.mVal[i] ) + 1 + mVal[i];
            }
        }
        if( !mVal[mSize-1] )
        {
            --mSize;
        }
    }

    void Shl( uint a )
    {
        if( a == 0 ) return;
        if( (a & 31) == 0 )
        {
            const int newSlots = ( a >> 5 );
            const int newSize = mSize + newSlots;
            EnsureSize( newSize );
            for( int i = newSize - 1; i >= newSlots; --i )
            {
                mVal[i] =  mVal[i - newSlots];
            }
            for( int i = newSlots - 1; i >= 0; --i )
            {
                mVal[i] = 0;
            }
            mSize = newSize;
            return;
        }
        const int newSlots = (a >> 5) + 1;
        const int newSize = mSize + newSlots;
        EnsureSize( newSize );
        const int decalLeft = a & 31;
        const int decalRight = 32 - decalLeft;

        for (int i = mSize; i < newSize ; ++i)
        {
            mVal[i] = 0;
        }

        for (int i = newSize - 1; i >= newSlots ; --i)
        {
            mVal[i] = ( mVal[i - newSlots + 1] << decalLeft ) | ( mVal[i - newSlots] >> decalRight );
        }
        mVal[newSlots - 1] = mVal[0] << decalLeft;
        for (int i = newSlots - 2; i >= 0 ; --i)
        {
            mVal[i] = 0;
        }
        if( mVal[newSize - 1] )
        {
            mSize = newSize;
        }
        else
        {
            mSize = newSize - 1;
        }
    }

    void Shr( uint a )
    {
        if( a == 0 ) return;
        if( ( a & 31 ) == 0 )
        {
            const int delSlots = ( a >> 5 );
            const int newSize = mSize - delSlots;
            EnsureSize( newSize );
            for( int i = 0; i < newSize; ++i )
            {
                mVal[i] = mVal[i + delSlots];
            }
            mSize = newSize;
            return;
        }
        const int delSlots = ( a >> 5 );
        const int newSize = mSize - delSlots;
        const int decalRight = a & 31;
        const int decalLeft = 32 - decalRight;

        for (int i = 0; i < newSize-1 ; ++i)
        {
            mVal[i] = ( mVal[i + delSlots + 1] << decalLeft ) | ( mVal[i + delSlots] >> decalRight );
        }
        mVal[newSize-1] = mVal[mSize-1] >> decalRight;
        if( mVal[newSize - 1] || newSize == 1 )
        {
            mSize = newSize;
        }
        else
        {
            mSize = newSize - 1;
        }
    }

    void Print()
    {
        printf( "%X",mVal[mSize-1] );
        for (int i = mSize - 2; i >= 0 ; i--)
        {
            printf( "%08X",mVal[i] );
        }
        printf( "\n" );
    }
};


class Series5
{
    int n;
    int val;
    Series5(int _n) : n(_n) { 
        val = 1 << ( n + 4 );

    }

};


int main(int argc, char *argv[])
{
    BigNumber b1( 0x80000000 );
    b1.Print();
    for( int i = 0; i < 100; i++ )
    {
        b1.Add( 1<<31 );
        b1.Print();
    }
    for( int i = 0; i < 102; i++ )
    {
        b1.Sub( 1<<31 );
        b1.Print();
    }
    
	cout << "Press enter to continue...";
	char temp;
	cin.read(&temp, 1);
	return 0;
}
