#include <string>
#include <iostream>
#include <fstream>
#include "conio.h"
#include <ctime>
#include <regex>
#include <Windows.h>

#include "pi.h"
using namespace std;


class skippingException : public exception{};


#define PrintCurrentState() system("cls");printf("%s\n\nSuccess:%d  Errors:%d",rememberedDigits.c_str(),currentScore.digits,currentScore.mistakes);

string logFileName = "log.txt";

struct Score
{
    Score()
    {
        digits = 0;
        mistakes = 0;
        time = 0;
        digitPerSec = 0;
    }
    int digits,mistakes;
    int time; // in ms

    float digitPerSec;
};

Score bestScore;
HANDLE scoreThread = NULL;

string getScoreStringFormat(const Score& s)
{
    char buffer[128];
    sprintf(buffer,"%d number with %d mistakes in %dh %dm %d.%ds",s.digits, s.mistakes,s.time/3600000, ((int)s.time/60000)%60, (s.time/1000)%60, s.time%1000 );
    return buffer;
}

// Checks whether given thread is alive.
bool IsThreadAlive(const HANDLE hThread , DWORD& dwExitCode)
{
    // Read thread's exit code.
    dwExitCode = 0;
    GetExitCodeThread(hThread, &dwExitCode);
    return dwExitCode == STILL_ACTIVE;
}

// time in ms
int HighScoreUpdate(Score& s)
{
    // 5sec per mistakes
    float timeCorrected = s.time/1000.f + s.mistakes*5;
    s.digitPerSec = (float)s.digits/timeCorrected;
    if(s.digitPerSec > bestScore.digitPerSec || s.digits > bestScore.digits+10 /*Petite marge d'erreur*/)
    {
        if(s.digits > bestScore.digits-10)
        {
            bestScore = s;
            return 1;
        }
    }
    return 0;
}

bool safetoprint = false;

DWORD WINAPI GetBestScore( LPVOID arg )
{
    ifstream log(logFileName.c_str(),ios::in);
    if(log.fail())
    {
        log.close();
        return -1;
    }

    string line;
    getline(log,line);
    regex reg("(\\d+) number.*(\\d+).*(\\d+)h\\s*(\\d+)m\\s*(\\d+\\.\\d+)s");
    std::smatch match;

    while(!log.eof())
    {
        while (std::regex_search (line,match,reg)) 
        {
            Score score;
            int h,m;
            float sec;
            int i=0; 
            for (auto x:match)
            {
                switch (i)
                {
                case 0:
                    break;
                case 1:
                    score.digits = atoi(x.str().c_str());
                    break;
                case 2:
                    score.mistakes = atoi(x.str().c_str());
                    break;
                case 3:
                    h = atoi(x.str().c_str());
                    score.time += h*3600000;
                    break;
                case 4:
                    m = atoi(x.str().c_str());
                    score.time += m*60000;
                    break;
                case 5:
                    sec = atof(x.str().c_str());
                    score.time += (int)(sec*1000.f);
                    break;
                default:
                    break;
                }
                ++i;
            }
            line = match.suffix().str();

            HighScoreUpdate(score);
        }
        getline(log,line);
    }

    log.close();
    if(safetoprint)
        cout << "\n\nCurrent HighScore " << getScoreStringFormat(bestScore) << endl << endl;
    return 0;
}



int main(int argc, char** argv)
{
    DWORD thId1;
    scoreThread = CreateThread(NULL, 0, GetBestScore, NULL,NULL,&thId1);
    if(scoreThread == NULL)
    {
        // backup in serial
        GetBestScore(0);
    }

    do{
        system("cls");
        int currentDigit = 0;
        Score currentScore;
        
        ifstream pi("pi.txt", ios::in);
        if(pi.fail())
        {
            cout << "missing file pi.txt";
            system("pause");
            return -1;
        }
        else
        {
            cout << "Enter the digits of PI. No cheating and do your best :)\nPress Enter or Escape to finish\n\n" ;
            {
                DWORD exitCode;
                if(!IsThreadAlive(scoreThread,exitCode))
                {
                    // if thread finished early
                    cout << "Current HighScore " << getScoreStringFormat(bestScore) << endl << endl;
                }
                safetoprint = true;
            }
            system("pause");
            system("cls");
            printf("3.\n\nSuccess:0  Errors:0");

            int time = clock();

            string rememberedDigits = "3.";
            char c;
            pi >> c;

            int SkippingDigit = 0;

            try
            {
                while(!pi.eof())
                {
                    unsigned char n = c-'0';
                    if(n < 10)
                    {
                        if( SkippingDigit <= currentDigit)
                        {
                            try
                            {
                                unsigned int p = 0;
                                do
                                {
                                    do
                                    {

                                        p = _getch();
                                        if(p == 13 || p == 27)
                                        {
                                            throw exception("Execution Stopped by user");
                                        }
                                        else if(p == 's')
                                        {
                                            system("cls");
                                            cout << "Skipping to digit :";
                                            int s;
                                            string line;
                                            getline(cin, line, '\n');
                                            s = atoi(line.c_str());
                                            if(s> currentDigit)
                                            {
                                                SkippingDigit = s;
                                                throw skippingException();
                                            }
                                            PrintCurrentState();
                                        }
                                        p -= '0';


                                    }while( !( p<10 ) );

                                    if( p != n )
                                    {
                                        ++currentScore.mistakes;
                                        cout << (char)7;
                                        PrintCurrentState();
                                    }
                                }while( p != n );

                                ++currentScore.digits;
                            }
                            catch(skippingException&)
                            {

                            }
                        }

                        rememberedDigits.append(1,c);
                        ++currentDigit;
                        PrintCurrentState();
                    }

                    pi >> c;
                    if(pi.eof())
                    {
                        cout << "No more digits available in the file\n\n";
                    }
                }
            }
            catch(exception&)
            {
                // just let go through
            }


            pi.close();
            currentScore.time = clock() - time;

            // open log file at the end
            ofstream log(logFileName.c_str(),ios::out|ios::app);

            char logEntry[512];

            sprintf(logEntry,"\n\nYou've successfully entered %s\n",getScoreStringFormat(currentScore).c_str());
            cout << logEntry;
            DWORD exitCode;
            while(IsThreadAlive(scoreThread,exitCode))
            { Sleep(500); }

            if(bestScore.digits != 0)
            {
                printf("Previous High Score %s\n\n",getScoreStringFormat(bestScore).c_str());
            }
            else
            {
                cout << endl;
            }

            if(!log.fail())
            {
                log << logEntry;
                log.close();
            }

            if(HighScoreUpdate(currentScore))
            {
                cout << "New High Score !!!!!\n\n";
            }
        }
        system("pause");
    }while(1);

    system("pause");
    return 0;
}