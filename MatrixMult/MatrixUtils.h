#pragma once

typedef int MatrixElem;

MatrixElem** AllocateMatrix(int n)
{
    MatrixElem** m = new MatrixElem*[n];
    for(int i=0; i<n; ++i)
    {
        m[i] = new MatrixElem[n];
    }
    return m;
}

void DeleteMatrix( int n, MatrixElem** matrix )
{
    for(int i=0; i<n; ++i)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
}


int TransposeMatrix(int n, MatrixElem** m)
{
    for(int i=0; i<n; ++i)
    {
        for(int j=i+1; j<n; ++j)
        {
            swap(m[i][j],m[j][i]);
        }
    }

    return 1;
}

int SumMatrix(int n, MatrixElem** m1, MatrixElem** m2, MatrixElem** r)
{
    for(int i=0; i<n; ++i)
    {
        for(int j=0; j<n; ++j)
        {
            r[i][j] = m1[i][j] + m2[i][j];
        }
    }
    return 1;
}

int SubstractMatrix(int n, MatrixElem** m1, MatrixElem** m2, MatrixElem** r)
{
    for(int i=0; i<n; ++i)
    {
        for(int j=0; j<n; ++j)
        {
            r[i][j] = m1[i][j] - m2[i][j];
        }
    }
    return 1;
}

/// res is an array of 4 Matrix
///  _____
///  |0|1|
///  |2|3|
///   ---
int SubDivideMatrix(int n, MatrixElem** orig,  MatrixElem*** res)
{
	const int halfn = n>>1;

	for(int i=0;i<halfn;++i)
	{
		// m0
		for(int j=0; j<halfn; ++j)
		{
			res[0][i][j] = orig[i][j];
		}
		//m1
		for(int j=halfn; j<n; ++j)
		{
			res[1][i][j-halfn] = orig[i][j];
		}
	}
	for(int i=halfn;i<n;++i)
	{
		// m2
		for(int j=0; j<halfn; ++j)
		{
			res[2][i-halfn][j] = orig[i][j];
		}
		// m3
		for(int j=halfn; j<n; ++j)
		{
			res[3][i-halfn][j-halfn] = orig[i][j];
		}
	}

	return 1;
}


int CopyMatrix(int n, MatrixElem** original, MatrixElem** copie)
{
    for(int i=0; i<n; ++i)
    {
        memcpy(copie[i],original[i],n*sizeof(MatrixElem));
//         for(int j=0; j<n; ++j)
//         {
//             copie[i][j] = original[i][j];
//         }
    }

    return 1;
}

void PrintMatrix(int n, MatrixElem** matrix)
{
    cout << "\n";
    for(int i=0; i<n; ++i)
    {
        for(int j=0; j<n; ++j)
        {
            cout << matrix[i][j] << "\t";
        }
        cout << "\n";
    }
    cout << "\n";
}


void PrintMatrixSideBySide(int n, MatrixElem** m1, MatrixElem** m2, char sep)
{
    cout << "\n";
    for(int i=0; i<n; ++i)
    {
        for(int j=0; j<n; ++j)
        {
            cout << m1[i][j] << "\t";
        }
        cout << sep << "  ";
        for(int j=0; j<n; ++j)
        {
            cout << m2[i][j] << "\t";
        }

        cout << "\n";
    }
    cout << "\n";
}

MatrixElem** GenerateMatrix(int n)
{
    MatrixElem** m = AllocateMatrix(n);

    for(int i=0; i<n; ++i)
    {
        for(int j=0; j<n; ++j)
        {
            m[i][j] = rand()&0xF;
        }
    }

    return m;
}

