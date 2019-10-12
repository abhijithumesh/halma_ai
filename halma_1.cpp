#include<stdio.h>
#include<iostream>
#include<map>
#include <fstream>

using namespace std;



class HALMA{

private:
    string mPlayType;
    string mMyPawn;
    double mTimeRemaining;
    char mHalmaBoard[16][16];

public:
    HALMA()
    {
        for(int i = 0 ; i < 16; i++)
        {
            for(int j = 0 ; j < 16; j++)
            {
                mHalmaBoard[i][j] = '.';
            }
        }
    
    }


    void readInputs()
    {
        ifstream file( "input.txt", ios::in );

        file >> mPlayType;

        file >> mMyPawn;

        file >> mTimeRemaining;

        for( int i = 0; i < 16 ; i++ )
        {
            for( int j = 0 ; j < 16; j++ )
            {
                file >> mHalmaBoard[i][j];
            }
        
        }
    }

    void showInputs()
    {
        cout << "The playType mPlayType is:" << mPlayType << endl;

        cout << "The value of mMyPawn is:" << mMyPawn << endl;

        cout << "The remaining time is:" << mTimeRemaining << endl;

        /* Print the values of the halma board */

        cout << "   ";
        for( int j = 0 ; j < 16; j++ )
        {
            //printing the column numbers
            cout << j % 10 << "  ";
        }

        cout << endl;

        for( int i = 0; i < 16; i++ )
        {
            cout << i % 10 << "  ";
            for( int j = 0; j < 16; j++ )
            {
                cout << mHalmaBoard[i][j] << "  ";
            }
            cout << endl;
        }

    }

    bool isLeftGoal()
    {
        for( int i = 0 ; i < 5; i++ )
        {
            if(mHalmaBoard[0][i] != 'W')
            {
                return false;
            }

        }

        for( int i = 1; i < 5 ; i++ )
        {
            for(int j = 0; j < 6-i; j++)
            {
                if(mHalmaBoard[i][j] != 'W')
                    return false;
            }
        }

        return true;
    }

    bool isRightGoal()
    {
        for( int i = 15 ; i >= 11; i-- )
        {
            if(mHalmaBoard[15][i] != 'B')
            {
                return false;
            }
        }

        for( int i = 14; i > 10 ; i-- )
        {
            for(int j = 15; j >= 25 - i; j--)
            {
                if(mHalmaBoard[i][j] != 'B')
                    return false;
            }
        }

        return true;
    }

};


int main()
{

    HALMA game;

    game.readInputs();

    game.showInputs();

    cout << "The right goal is: " << game.isRightGoal() << endl;

    cout << "The left goal is:" << game.isLeftGoal() << endl;

    return 0;
}
