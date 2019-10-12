#include <stdio.h>
#include <iostream>
#include <map>
#include <fstream>
#include <vector>

using namespace std;

typedef std::pair<int, int> xyCoordinates;
typedef std::vector<xyCoordinates> pawnVector;
typedef tuple<xyCoordinates, int> coordinate_dis; 

class HALMA{

private:
    string mPlayType;
    string mMyPawn;
    double mTimeRemaining;
    char mHalmaBoard[16][16];
    pawnVector mWhitePawn;
    pawnVector mBlackPawn;

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

    char getPawnChar()
    {
        return (mMyPawn == "WHITE")? 'W' : 'B';
    }

    void populatePawns()
    {
        for(int i = 0 ; i < 16; i++)
        {
            for(int j = 0 ; j < 16; j++)
            {
                if(mHalmaBoard[i][j] == 'W')
                {
                    mWhitePawn.push_back(std::make_pair(i,j));
                }
                else if(mHalmaBoard[i][j] == 'B')
                {
                    mBlackPawn.push_back(std::make_pair(i,j));
                }
            }   
        }
    }

    bool isValidCell(int row, int col)
    {
        if( row < 0 || row >=16 || col < 0 || col >=16 )
            return false;

        return true;
    }

    void moveUp(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result)
    {
        int newRow = row + 1;
        
        if(isValidCell(newRow,col))
        {
        
        }
    
    }

    void moveDown(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result)
    {
    
    }

    void moveEast(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result)
    {
    
    }

    void moveWest(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result)
    {
    
    }

    void moveNE(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result)
    {
    
    }

    void moveNW(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result)
    {
    
    }

    void moveSE(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result)
    {
    
    }

    void moveSW(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result)
    {
    
    }

    void searchNeighbors(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result)
    {
        moveUp(row, col, result);
        moveDown(row, col, result);
        moveEast(row, col, result);
        moveWest(row, col, result);
        moveNE(row, col, result);
        moveNW(row, col, result);
        moveSE(row, col, result);
        moveSW(row, col, result);
    }


    void findLegalMoves(char turn)
    {
        int row, col;
        multimap<xyCoordinates, vector<coordinate_dis>> result;

        if(turn == 'W')
        {
            for(auto itr = mWhitePawn.begin(); itr != mWhitePawn.end(); itr++)
            {
                row = itr->first;
                col = itr->second;

                searchNeighbors(row, col, result);
            }
        }
        else
        {
            for(auto itr = mBlackPawn.begin(); itr != mBlackPawn.end(); itr++)
            {
                row = itr->first;
                col = itr->second;
            
                searchNeighbors(row, col, result);
            }
        }
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
