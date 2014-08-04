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
    uint* val;
    int size;
    int capacity;
public:
    BigNumber( uint _val )
    {
        val = new uint[30];
        size = 1;
        capacity = 32;
        val[0] = _val;
    }
    ~BigNumber()
    {
        delete[] val;
    }
    void Append( uint a )
    {
        if( size == capacity )
        {
            capacity <<= 1;
            uint* newArr = new uint[capacity];
            for (int i = 0; i < size ; i++)
            {
                newArr[i] = val[i];
            }
            delete[] val;
            val = newArr;
        }
        val[size++] = a;
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
            jnc l1
            cmp esi,edx
            jge l2
            mov ecx,this
            push 1
            call BigNumber::Append
            jmp l1
        l2:
            inc esi
            mov ecx, 1
            jmp addArr
        l1:
        };
    }
    void Add( const BigNumber& bn )
    {
        const int size2 = bn.size;
        uint8 carry = 0;
        for (int i = 0; i < size2 ; i++)
        {
            if( i >= size )
            {
                Append( 0 );
            }

            if( carry && val[i] == MAX_UINT )
            {
                val[i] = bn.val[i];
                // leave carry at 1
            }
            else
            {
                val[i] += carry;
                val[i] += bn.val[i];
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
        uint* newArr = new uint[capacity];
        uint* arr = val;
        uint carry = 0;
        uint s = size;
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

        delete[] val;
        val = newArr;
        if( carry )
        {
            Append( carry );
        }
    }

    void Mul( const BigNumber& bn )
    {
        int cap = size + bn.size;
        cap = 1 << (Log2( cap )+1);
        const int newCapacity = max( capacity, cap );
        uint* newArr = new uint[newCapacity];
        
        uint carry = 0;
        // init only values needed
        for (int i = 0; i < size + bn.size ; i++)
        {
            newArr[i] = 0;
        }
        for (int j = 0; j < bn.size ; j++)
        {
            const uint m1 = bn.val[j];
            for (int i = 0; i < size ; i++)
            {
                uint m2 = val[i];
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
        size = size + bn.size - 1;
        if( carry )
        {
            newArr[size++] = carry;
        }
        delete[] val;
        val = newArr;
        capacity = newCapacity;
    }

    void Print()
    {
        printf( "%X",val[size-1] );
        for (int i = size - 2; i >= 0 ; i--)
        {
            printf( "%08X",val[i] );
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
    BigNumber b1( 75 );
    for (int i = 0; i < 100000 ; i++)
    {
        b1.Mul( 75 );
    }
    b1.Print();
    
	cout << "Press enter to continue...";
	char temp;
	cin.read(&temp, 1);
	return 0;
}
