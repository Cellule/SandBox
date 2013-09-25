using namespace std;
#define _CRT_SECURE_NO_WARNINGS 1

#include <fstream>
#include <iostream>
#include <vector>
#include <time.h>
#include <ppl.h>
#include "MatrixUtils.h"
#include "conio.h"


// Transpose, optimisation for concurrent reading of second matrix elements
#define USE_TRANSPOSE 1
#define USE_MULTITHREAD 0


const int CutOffDefault = 128;
int CutOff = CutOffDefault;

typedef int(*MultiplicationFunction)(int , MatrixElem** , MatrixElem** , MatrixElem** );

float ExecuteMultiplication( int nPower, MultiplicationFunction func );
/// Only preprocess function must be visible to the rest to avoid use the basic function unintentionally
int MultMatrixBasicPreprocess(const int n, MatrixElem** m1, MatrixElem** m2, MatrixElem** matrix);
int Strassen(int n, MatrixElem** m1, MatrixElem** m2, MatrixElem** matrix);



void Pause()
{
    cout << "Press any key to continue\n";
    _getch();
}

MatrixElem** ReadMatrixFromFile(char* filename)
{
    ifstream file(filename,ios::in);
    if(!file.fail())
    {
        int n;
        file >> n;
        
#if _DEBUG
        if(n > sizeof(int)*8-1)
        {
            cout << "N value too large\n";
            goto READ_MATRIX_ERROR;
        }
#endif

    int nElem = 1 << n;
    MatrixElem** matrix = AllocateMatrix(nElem);

        // normal reading
        for(int i=0; i<nElem; ++i)
        {
            for(int j=0; j<nElem; ++j)
            {
                file >> matrix[i][j];
#if _DEBUG
                // Not enough elements
                if(j!=nElem && i!=nElem-1 && file.eof())
                {
                    printf("Invalid Matrix in file %s\n",filename);
                    goto READ_MATRIX_ERROR;
                }
#endif
            }
        }


        return matrix;
    }
#if _DEBUG
    else
    {
        printf("Cannot find file %s\n",filename);
    }

READ_MATRIX_ERROR:
#endif
    file.close();
    return 0;
}

/// Tests to validate functions and algo
void Tests()
{
    ///////////////
    // Test Sum  Sub
    ///////////////
    {
        cout << "Test Sum ==========================\n\n";
        const int n = 2;
        MatrixElem** m1 = GenerateMatrix(n), 
            **m2 = GenerateMatrix(n),
            **r = AllocateMatrix(n);
        SumMatrix(n,m1,m2,r);
        PrintMatrixSideBySide(n,m1,m2,'+');
        PrintMatrix(n,r);
        cout << "Test Sub ==========================\n\n";
        PrintMatrixSideBySide(n,r,m2,'-');
        SubstractMatrix(n,r,m2,r);
        PrintMatrix(n,r);

        for(int i=0; i<n; ++i)
        {
            for(int j=0; j<n; ++j)
            {
                if(r[i][j] != m1[i][j])
                {
                    cout << "ERROR in sub|sum\n\n";
                    break;
                }
            }
        }
        DeleteMatrix(n,m1);
        DeleteMatrix(n,m2);
        DeleteMatrix(n,r);
    }

    ///////////////
    // Test SubMatrix
    ///////////////
    {
        cout << "Test SubMatrix ==========================\n\n";
        const int n = 4;
        const int halfn = n>>1;
        MatrixElem** orig = GenerateMatrix(n);
        MatrixElem** matrix[4];
        matrix[0] = AllocateMatrix(halfn);
        matrix[1] = AllocateMatrix(halfn);
        matrix[2] = AllocateMatrix(halfn);
        matrix[3] = AllocateMatrix(halfn);

        SubDivideMatrix(n,orig,matrix);
        PrintMatrix(n,orig);
        PrintMatrixSideBySide(halfn,matrix[0],matrix[1],'|');
        PrintMatrixSideBySide(halfn,matrix[2],matrix[3],'|');

        DeleteMatrix(n,orig);
        DeleteMatrix(halfn,matrix[0]);
        DeleteMatrix(halfn,matrix[1]);
        DeleteMatrix(halfn,matrix[2]);
        DeleteMatrix(halfn,matrix[3]);
    }

    /// Test Multiplication
    {
        cout << "Test Multiplication ==========================\n\n";
        const int n = 4;
        MatrixElem** m1 = AllocateMatrix(n);
        MatrixElem** m2 = AllocateMatrix(n);
        MatrixElem** res = AllocateMatrix(n);
        MatrixElem** expectedRes = AllocateMatrix(n);

        int m1Val[] = {1, 2, 3, 4,/*;*/ 2, 1, 2, 3,/*;*/ 3, 2, 1, 2,/*;*/ 4, 3, 2, 1};
        int m2Val[] = {4,3,2,1/*;*/,3,4,3,2/*;*/,2,3,4,3/*;*/,1,2,3,4};
        int expectedVal[] = {20,28,32,30/**/,18,22,24,22/**/,22,24,22,18/**/,30,32,28,20};

        for(int i=0; i<n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                m1[i][j] = m1Val[i*n+j];
                m2[i][j] = m2Val[i*n+j];
                expectedRes[i][j] = expectedVal[i*n+j];
            }
        }

        MultMatrixBasicPreprocess(n,m1,m2,res);
        for(int i=0; i<n; ++i)
        {
            for(int j=0; j<n; ++j)
            {
                if(res[i][j] != expectedRes[i][j])
                {
                    cout << "ERROR in Basic Multiplication";
                    PrintMatrixSideBySide(n,m1,m2,'*');
                    PrintMatrixSideBySide(n,res,expectedRes,'!');

                    goto STOPTEST;
                }
            }
        }

        Strassen(n,m1,m2,res);
        for(int i=0; i<n; ++i)
        {
            for(int j=0; j<n; ++j)
            {
                if(res[i][j] != expectedRes[i][j])
                {
                    cout << "ERROR in Strassen Multiplication";
                    PrintMatrixSideBySide(n,m1,m2,'*');
                    PrintMatrixSideBySide(n,res,expectedRes,'!');

                    goto STOPTEST;
                }
            }
        }

STOPTEST:

        DeleteMatrix(n,m1);
        DeleteMatrix(n,m2);
        DeleteMatrix(n,res);
        DeleteMatrix(n,expectedRes);
    }

    Pause();
}

void FindCutOff()
{
    const int nPower = 9;
    const int n = 1<<nPower;
    CutOff = 1<<9;

    float previousTime = 99999999.f;

    bool forward = false;

    while(CutOff > 0)
    {
        float time = ExecuteMultiplication(nPower,Strassen);
        if(time < previousTime)
        {
        }
        else
        {
            forward = !forward;
        }

        printf("CutOff = %d\ntime=%f\nprevious=%f\n\n",CutOff,time,previousTime);
        if(forward)
        {
            CutOff = CutOff<<1;

        }
        else
        {
            CutOff = CutOff>>1;
        }
        previousTime = time;

    }



    Pause();
}



int main(int argc, char** argv)
{
    int nInit = 5, nFinal = 5;
    if(argc > 1)
    {
        if(strcmp(argv[1],"tests") == 0)
        {
            Tests();
            return 0;
        }
        else if(strcmp(argv[1],"cutoff") == 0)
        {
            FindCutOff();
            return 0;
        }
        nInit = atoi(argv[1]);
        nFinal = nInit;
        if(argc>2)
        {
            // make sure nFinal is bigger than nInit
            nFinal = max(nFinal,atoi(argv[2]));
            // make sure 2^nFinal fits in a 32bits integer
            nFinal = min(nFinal,31);
        }
    }

    ofstream log("result.txt",ios::out);
    bool logging = !log.fail();


    int nPower = nInit;
    {
        char buffer[256];
        sprintf(buffer,"\t\tN\t\tBasic\t\t\tStrassen(%d)\t\tStrassen(%d)\t\tStrassen(%d)\t\tStrassen(1)\n",CutOff,CutOff>>1,CutOff<<1);
        if(logging)
        {
            log << buffer;
        }
        cout << buffer;
    }

    while(nPower <= nFinal)
    {
        CutOff = CutOffDefault;
        cout << "\t\t" << nPower;
        float meanTime = ExecuteMultiplication(nPower,MultMatrixBasicPreprocess);
        cout << "\t\t" << meanTime;
        float meanTime2 = ExecuteMultiplication(nPower,Strassen);
        cout << "\t\t" << meanTime2;
        // With half the cutoff
        CutOff = CutOff>>1;
        float meanTime3 = ExecuteMultiplication(nPower,Strassen);
        cout << "\t\t" << meanTime3;
        // With double Cutoff
        CutOff = CutOff<<2;
        float meanTime4 = ExecuteMultiplication(nPower,Strassen);
        cout << "\t\t" << meanTime4;
        // With Cutoff of 1
        CutOff = 1;
        float meanTime5 = ExecuteMultiplication(nPower,Strassen);
        cout << "\t\t" << meanTime5;
        char buffer[256];
        sprintf(buffer,"\t\t%d\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f\n",nPower,meanTime,meanTime2,meanTime3,meanTime4,meanTime5);
        if(logging)
        {
            log << buffer;
        }
        cout << endl;
        //cout << buffer;
        ++nPower;
    }
    log.close();

    Pause();
    return 0;
}

float ExecuteMultiplication( int nPower, MultiplicationFunction func )
{
    vector<MatrixElem**> matrix; 
    float nbMult = 0;
    float meanTime = 0;
    const int n = 1<<nPower;

    for(int x=1; x<=5; ++x)
    {
        char filename[32];
        sprintf_s(filename,"data/ex_n%d.%d",nPower,x);
        MatrixElem** m = ReadMatrixFromFile(filename);

        if(!m)
        {
            m = GenerateMatrix(n);
        }

        if(m)
        {
            for(int iMatrix=0; iMatrix<(int)matrix.size(); ++iMatrix)
            {
                MatrixElem** r = AllocateMatrix(n);
                clock_t t = clock();
                // important to have new matrix on the right because it can be transpose if optim
                func(n,matrix[iMatrix],m,r);
                t = clock() - t;

                DeleteMatrix(n,r);
                meanTime += (float)t;
                ++nbMult;
            }
            matrix.push_back(m);
        }
    }

    // Cleanup
    while(!matrix.empty())
    {
        MatrixElem** m = matrix.back();
        matrix.pop_back();
        DeleteMatrix(n, m);
    }

    return nbMult!= 0? meanTime/nbMult : 0;
}



// if USE_TRANSPOSE  m2 must be transposed from its original form => m2 = m2t
int MultMatrixBasic(const int n, MatrixElem** m1, MatrixElem** m2, MatrixElem** matrix)
{
#if !USE_MULTITHREAD
    for(int leftRow=0; leftRow<n; ++leftRow)
#else
    Concurrency::parallel_for<int>(0,n, [&] (int leftRow)
#endif
    {
        for(int rightColomn=0; rightColomn<n; ++rightColomn)
        {
            int sum = 0;
            for(int leftColomn=0,rightRow=0; leftColomn<n; ++leftColomn,++rightRow)
            {
#if USE_TRANSPOSE
                sum += m1[leftRow][leftColomn]*m2[rightColomn][rightRow];
#else
                sum += m1[leftRow][leftColomn]*m2[rightRow][rightColomn];
#endif
            }
            matrix[leftRow][rightColomn] = sum;
        }
    }
#if USE_MULTITHREAD
    );
#endif

    return 1;
}

int MultMatrixBasicPreprocess(const int n, MatrixElem** m1, MatrixElem** m2, MatrixElem** matrix)
{
#if USE_TRANSPOSE
    TransposeMatrix(n,m2);
#endif
    MultMatrixBasic(n,m1,m2,matrix);
#if USE_TRANSPOSE
    TransposeMatrix(n,m2);
#endif
    return 1;
}


int Strassen(int n, MatrixElem** m1, MatrixElem** m2, MatrixElem** matrix)
{
    if(n <= CutOff)
    {
        return MultMatrixBasicPreprocess(n,m1,m2,matrix);
    }

    int halfn = n>>1;

    // 0 = A1,1   1 = A1,2    2 = A2,1    3 = A2,2
    // if USE_TRANSPOSE  swap 1 & 2
    MatrixElem** a[4];
    a[0] = AllocateMatrix(halfn);
    a[1] = AllocateMatrix(halfn);
    a[2] = AllocateMatrix(halfn);
    a[3] = AllocateMatrix(halfn);

    // 0 = B1,1   1 = B1,2    2 = B2,1    3 = B2,2
    // if USE_TRANSPOSE  swap 1 & 2
    MatrixElem** b[4];
    b[0] = AllocateMatrix(halfn);
    b[1] = AllocateMatrix(halfn);
    b[2] = AllocateMatrix(halfn);
    b[3] = AllocateMatrix(halfn);

    SubDivideMatrix(n,m1,a);
    SubDivideMatrix(n,m2,b);

    MatrixElem** m[7];
    for (int i = 0; i < 7 ; ++i)
    {
        m[i] = AllocateMatrix(halfn);
    }

    MatrixElem** aTemp = AllocateMatrix(halfn), **bTemp = AllocateMatrix(halfn);

    ////  M1 - M7  /////////////////////////////
    // M1 = ( A1,1 + A2,2 ) *  ( B1,1 + B2,2 )
    SumMatrix(halfn,a[0],a[3],aTemp);
    SumMatrix(halfn,b[0],b[3],bTemp);
    Strassen(halfn,aTemp,bTemp,m[0]);

    // M2 = (A2,1 + A2,2)* B1,1
    SumMatrix(halfn,a[2],a[3],aTemp);
    Strassen(halfn,aTemp,b[0],m[1]);

    // M3 = A1,1 * ( B1,2 - B2,2 )
    SubstractMatrix(halfn,b[1],b[3],bTemp);
    Strassen(halfn,a[0],bTemp,m[2]);

    // M4 = A2,2 * (B2,1 - B1,1)
    SubstractMatrix(halfn,b[2],b[0],bTemp);
    Strassen(halfn,a[3],bTemp,m[3]);

    // M5 = (A1,1 + A1,2)* B2,2
    SumMatrix(halfn,a[0],a[1],aTemp);
    Strassen(halfn,aTemp,b[3],m[4]);

    // M6 = (A2,1 - A1,1)*(B1,1 + B1,2)
    SubstractMatrix(halfn,a[2],a[0],aTemp);
    SumMatrix(halfn,b[0],b[1],bTemp);
    Strassen(halfn,aTemp,bTemp,m[5]);

    // M7 = (A1,2 - A2,2)*(B2,1 + B2,2)
    SubstractMatrix(halfn,a[1],a[3],aTemp);
    SumMatrix(halfn,b[2],b[3],bTemp);
    Strassen(halfn,aTemp,bTemp,m[6]);


    // C1,1 = M1 + M4 - M5 + M7
    SumMatrix(halfn,m[0],m[3],matrix);
    SubstractMatrix(halfn,matrix,m[4],matrix);
    SumMatrix(halfn,matrix,m[6],matrix);

    // C1,2 = M3 + M5
    SumMatrix(halfn,m[2],m[4],aTemp);
    for(int i=0;i<halfn;++i)
    {
        for(int j=halfn; j<n; ++j)
        {
            matrix[i][j] = aTemp[i][j-halfn];
        }
    }
    // C2,1 = M2 + M4
    SumMatrix(halfn,m[1],m[3],bTemp);

    // C2,2 = M1 - M2 + M3 + M6
    SubstractMatrix(halfn,m[0],m[1],aTemp);
    SumMatrix(halfn,aTemp,m[2],aTemp);
    SumMatrix(halfn,aTemp,m[5],aTemp);
    for(int i=halfn;i<n;++i)
    {
        // C3
        for(int j=0; j<halfn; ++j)
        {
            matrix[i][j] = bTemp[i-halfn][j];
        }
        // C4
        for(int j=halfn; j<n; ++j)
        {
            matrix[i][j] = aTemp[i-halfn][j-halfn];
        }
    }


    /// Cleanup
    for (int i = 0; i < 7 ; ++i)
    {
        DeleteMatrix(halfn,m[i]);
    }
    for (int i = 0; i < 4 ; ++i)
    {
        DeleteMatrix(halfn,a[i]);
        DeleteMatrix(halfn,b[i]);
    }
    DeleteMatrix(halfn,aTemp);
    DeleteMatrix(halfn,bTemp);

    return 1;
}
