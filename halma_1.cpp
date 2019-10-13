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
    int mVisited[16][16];

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

    void resetVisited()
    {
        for(int i = 0; i < 16; i++)
        {
            for(int j = 0; j < 16; j++)
            {
                mVisited[i][j] = 0;
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


    void findNeighbors(int row, int col, vector<coordinate_dis> vec)
    {
        moveUpNeigh(row+1, col, vec, 1);
        moveDownNeigh(row-1, col, vec, 1);
        moveEastNeigh(row, col+1, vec, 1);
        moveWestNeigh(row, col-1, vec, 1);
        moveNWNeigh(row-1, col-1, vec, 1);
        moveNENeigh(row-1, col+1, vec, 1);
        moveSENeigh(row+1, col+1, vec, 1);
        moveSWNeigh(row+1, col-1, vec, 1);
    }

    void moveUp(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result, int depth)
    {
        int newRow = row + 1;
        
        if(isValidCell(newRow,col) && !mVisited[newRow][col])
        {
            mVisited[newRow][col] = 1;
            if( mHalmaBoard[newRow][col] == '.' && depth == 1 )
            {
                xyCoordinates XY = std::make_pair(row,col);
                vector<coordinate_dis> v;

                v.push_back(make_tuple(std::make_pair(newRow, col), 0));

                result.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(XY, v));
                return;
            }
            else if( mHalmaBoard[newRow][col] != '.' && depth == 1 )
            {
                moveUp(newRow, col, result, depth+1);
            }
            else if( mHalmaBoard[newRow][col] == '.' && depth == 2 )
            {
                vector<coordinate_dis> v;
                v.push_back(make_tuple(std::make_pair(newRow, col), 0));
                findNeighbors(newRow, col, v);
            }
            else if( mHalmaBoard[newRow][col] != '.' && depth == 2)
            {
                return;
            }
        }
    
    }

    void moveUpNeigh(int row, int col, vector<coordinate_dis> res, int depth)
    {
        if(isValidCell(row,col) && !mVisited[row][col])
        {
            if(mHalmaBoard[row][col] == '.' && depth == 1)
            {
                return;
            }
            else if(mHalmaBoard[row][col] != '.' && depth == 1)
            {
                moveUpNeigh(row, col, res, 2);
            }
            else if(mHalmaBoard[row][col] == '.' && depth == 2)
            {
                res.push_back(make_tuple(std::make_pair(row,col),0));
                findNeighbors(row, col, res);
            }
        }
    
    }

    void moveDown(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result, int depth)
    {
    
    }
    
    void moveDownNeigh(int row, int col, vector<coordinate_dis> res, int depth)
    {
    
    }

    void moveEast(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result, int depth)
    {
    
    }

    void moveEastNeigh(int row, int col, vector<coordinate_dis> res, int depth)
    {
    
    }

    void moveWest(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result, int depth)
    {
    
    }

    void moveWestNeigh(int row, int col, vector<coordinate_dis> res, int depth)
    {
    
    }

    void moveNE(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result, int depth)
    {
    
    }

    void moveNENeigh(int row, int col, vector<coordinate_dis> res, int depth)
    {
    
    }


    void moveNW(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result, int depth)
    {
    
    }

    void moveNWNeigh(int row, int col, vector<coordinate_dis> res, int depth)
    {
    
    }

    void moveSE(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result, int depth)
    {
    
    }

    void moveSENeigh(int row, int col, vector<coordinate_dis> res, int depth)
    {
    
    }

    void moveSW(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result, int depth)
    {
    
    }

    void moveSWNeigh(int row, int col, vector<coordinate_dis> res, int depth)
    {
    
    }

    void searchNeighbors(int row, int col, multimap<xyCoordinates, vector<coordinate_dis>> result)
    {
        moveUp(row, col, result, 1);
        moveDown(row, col, result, 1);
        moveEast(row, col, result, 1);
        moveWest(row, col, result, 1);
        moveNE(row, col, result, 1);
        moveNW(row, col, result, 1);
        moveSE(row, col, result, 1);
        moveSW(row, col, result, 1);
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
