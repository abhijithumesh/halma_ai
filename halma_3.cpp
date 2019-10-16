#include <stdio.h>
#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <tuple>

#define WIN 1000
#define DRAW 0
#define LOSS -1000

using namespace std;

typedef std::pair<int, int> xyCoordinates;
typedef std::vector<xyCoordinates> pawnVector;
typedef tuple<xyCoordinates, string, int> coordinate_dis; 

class HALMA{

private:
    string mPlayType;
    string mMyPawn;
    double mTimeRemaining;
    char mHalmaBoard[16][16];
    pawnVector mWhitePawn;
    pawnVector mBlackPawn;
    int mVisited[16][16];
    multimap<xyCoordinates, vector<coordinate_dis>> mResult;
    xyCoordinates mSource;
    pawnVector mWhiteGoal;
    pawnVector mBlackGoal;

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

        cout << "The whiteGoal states are:\n";

        for(auto itr = mWhiteGoal.begin() ; itr != mWhiteGoal.end(); itr++ )
        {
            cout << "X: " << (itr->first) << "  Y: " << (itr->second) << "\n";
        }

        cout << "The blackGoal states are:\n";
        
        for(auto itr = mBlackGoal.begin() ; itr != mBlackGoal.end(); itr++ )
        {
            cout << "X: " << (itr->first) << "  Y: " << (itr->second) << "\n";
        }

    }

    void getWhiteGoal()
    {
        for( int i = 0 ; i < 5; i++ )
        {
            mWhiteGoal.push_back(std::pair<int,int>(0,i));            
        }
        
        for( int i = 1; i < 5 ; i++ )
        {
            for(int j = 0; j < 6-i; j++)
            {
                mWhiteGoal.push_back(std::pair<int,int>(i,j));          
            }
        }
    }

    void getBlackGoal()
    {
        for( int i = 15 ; i >= 11; i-- )
        {
            mBlackGoal.push_back(std::pair<int,int>(15,i));
        }

        for( int i = 14; i > 10 ; i-- )
        {
            for(int j = 15; j >= 25 - i; j--)
            {
                mBlackGoal.push_back(std::pair<int,int>(i,j));
            }

        }
    }

    bool isLeftGoal(char halmaBoard[16][16])
    {
        int blackCount = 0, whiteCount = 0;
        for( int i = 0 ; i < 5; i++ )
        {
            if(halmaBoard[0][i] == 'W')
            {
                whiteCount++;
            }
            else if(halmaBoard[0][i] == 'B')
            {
                blackCount++;
            }
            else
            {
                return false;
            }
        }

        for( int i = 1; i < 5 ; i++ )
        {
            for(int j = 0; j < 6-i; j++)
            {
                if(halmaBoard[i][j] == 'W')
                {
                    whiteCount++;
                }
                else if(halmaBoard[i][j] == 'B')
                {
                    blackCount++;
                }
                else
                {
                    return false;
                }
            }
        }
        
        if(whiteCount + blackCount == 19 && whiteCount > 1)
            return true;

        return false;
    }

    bool isRightGoal( char halmaBoard[16][16])
    {
        int blackCount = 0, whiteCount = 0;

        for( int i = 15 ; i >= 11; i-- )
        {
            if(halmaBoard[15][i] == 'W')
            {
                whiteCount++;
            }
            else if(halmaBoard[15][i] == 'B')
            {
                blackCount++;
            }
            else
            {
                return false;
            }

        }

        for( int i = 14; i > 10 ; i-- )
        {
            for(int j = 15; j >= 25 - i; j--)
            {
                if(halmaBoard[i][j] == 'W')
                {
                    whiteCount++;
                }
                else if(halmaBoard[i][j] == 'B')
                {
                    blackCount++;
                }
                else
                {
                    return false;
                }
            }
        }

        if(whiteCount + blackCount == 19 && blackCount > 1)
            return true;

        return false;
    }

    inline char getMyPawnChar()
    {
        return (mMyPawn == "WHITE")? 'W' : 'B';
    }

    inline char getOpponentPawnChar()
    {
        return (mMyPawn == "WHITE")? 'B' : 'W';
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


    void findNeighbors(int row, int col, string path)
    {
        cout << __func__ <<"row:" << row << "col:" << col <<endl;
        moveDownNeigh(row+1, col, 1, path);
        
        moveUpNeigh(row-1, col, 1, path);
        
        moveEastNeigh(row, col+1, 1, path);

        moveWestNeigh(row, col-1, 1, path);
        
        moveNENeigh(row-1, col+1, 1, path);

        moveNWNeigh(row-1, col-1, 1, path);
        
        moveSWNeigh(row+1, col-1, 1, path);

        moveSENeigh(row+1, col+1, 1, path);
    }

    void moveDown(int row, int col, int depth, string path)
    {
        row = row + 1 ;
        if(isValidCell(row,col) && !mVisited[row][col])
        {
            cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if( mHalmaBoard[row][col] == '.' && depth == 1 )
            {
                string pathStr = path + " " + to_string(row)+ "," + to_string(col);
                vector<coordinate_dis> subOptimalXY;

                // Push the destination XY and path to the destination as well.
                subOptimalXY.push_back(make_tuple(std::make_pair(row, col), pathStr, 0));

                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, subOptimalXY));
                return;
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 1 )
            {
                moveDown(row, col, depth+1, path);
            }
            else if( mHalmaBoard[row][col] == '.' && depth == 2 )
            {
                vector<coordinate_dis> v;
                string pathStr = path + " " + to_string(row)+"," + to_string(col);
                v.push_back(make_tuple(std::make_pair(row, col), pathStr, 0));
                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, v));
                findNeighbors(row, col, pathStr);
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 2)
            {
                return;
            }
        }
    
    }

    void moveDownNeigh(int row, int col, int depth, string path)
    {
        if(isValidCell(row,col) && !mVisited[row][col])
        {
            cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if(mHalmaBoard[row][col] == '.' && depth == 1)
            {
                return;
            }
            else if(mHalmaBoard[row][col] != '.' && depth == 1)
            {
                moveDownNeigh(row+1, col, 2, path);
            }
            else if(mHalmaBoard[row][col] == '.' && depth == 2)
            {
                vector<coordinate_dis> subOptimalXY;
                string pathStr = path + " " + to_string(row) + "," + to_string(col);
                subOptimalXY.push_back(make_tuple(std::make_pair(row,col), pathStr, 0));
                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, subOptimalXY));

                findNeighbors(row, col, pathStr);
            }
            mVisited[row][col] = 0;
        }
    
    }

    void moveUp(int row, int col, int depth, string path)
    {
        row = row - 1 ;
        if(isValidCell(row,col) && !mVisited[row][col])
        {
            cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if( mHalmaBoard[row][col] == '.' && depth == 1 )
            {
                string pathStr = path + " " + to_string(row)+ "," + to_string(col);
                vector<coordinate_dis> subOptimalXY;

                // Push the destination XY and path to the destination as well.
                subOptimalXY.push_back(make_tuple(std::make_pair(row, col), pathStr, 0));

                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, subOptimalXY));
                return;
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 1 )
            {
                moveUp(row, col, depth+1, path);
            }
            else if( mHalmaBoard[row][col] == '.' && depth == 2 )
            {
                vector<coordinate_dis> v;
                string pathStr = path + " " + to_string(row)+"," + to_string(col);
                v.push_back(make_tuple(std::make_pair(row, col), pathStr, 0));
                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, v));
                findNeighbors(row, col, pathStr);
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 2)
            {
                return;
            }
        }
    }
    
    void moveUpNeigh(int row, int col, int depth, string path)
    {
        if(isValidCell(row,col) && !mVisited[row][col])
        {
            cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if(mHalmaBoard[row][col] == '.' && depth == 1)
            {
                return;
            }
            else if(mHalmaBoard[row][col] != '.' && depth == 1)
            {
                moveUpNeigh(row-1, col, 2, path);
            }
            else if(mHalmaBoard[row][col] == '.' && depth == 2)
            {
                vector<coordinate_dis> subOptimalXY;
                string pathStr = path + " " + to_string(row) + "," + to_string(col);
                subOptimalXY.push_back(make_tuple(std::make_pair(row,col), pathStr, 0));
                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, subOptimalXY));

                findNeighbors(row, col, pathStr);
            }
            mVisited[row][col] = 0;
        } 
    }

    void moveEast(int row, int col, int depth, string path)
    {
        col = col + 1;
        if(isValidCell(row,col) && !mVisited[row][col])
        {
            cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if( mHalmaBoard[row][col] == '.' && depth == 1 )
            {
                string pathStr = path + " " + to_string(row)+ "," + to_string(col);
                vector<coordinate_dis> subOptimalXY;

                // Push the destination XY and path to the destination as well.
                subOptimalXY.push_back(make_tuple(std::make_pair(row, col), pathStr, 0));

                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, subOptimalXY));
                return;
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 1 )
            {
                moveEast(row, col, depth+1, path);
            }
            else if( mHalmaBoard[row][col] == '.' && depth == 2 )
            {
                vector<coordinate_dis> v;
                string pathStr = path + " " + to_string(row)+"," + to_string(col);
                v.push_back(make_tuple(std::make_pair(row, col), pathStr, 0));
                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, v));
                findNeighbors(row, col, pathStr);
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 2)
            {
                return;
            }
        }    
    }

    void moveEastNeigh(int row, int col, int depth, string path)
    {
        if(isValidCell(row,col) && !mVisited[row][col])
        {
            cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if(mHalmaBoard[row][col] == '.' && depth == 1)
            {
                return;
            }
            else if(mHalmaBoard[row][col] != '.' && depth == 1)
            {
                moveEastNeigh(row, col+1, 2, path);
            }
            else if(mHalmaBoard[row][col] == '.' && depth == 2)
            {
                vector<coordinate_dis> subOptimalXY;
                string pathStr = path + " " + to_string(row) + "," + to_string(col);
                subOptimalXY.push_back(make_tuple(std::make_pair(row,col), pathStr, 0));
                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, subOptimalXY));

                findNeighbors(row, col, pathStr);
            }
            mVisited[row][col] = 0;
        } 
    }

    void moveWest(int row, int col, int depth, string path)
    {
        col = col - 1;
        if(isValidCell(row,col) && !mVisited[row][col])
        {
            cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if( mHalmaBoard[row][col] == '.' && depth == 1 )
            {
                string pathStr = path + " " + to_string(row)+ "," + to_string(col);
                vector<coordinate_dis> subOptimalXY;

                // Push the destination XY and path to the destination as well.
                subOptimalXY.push_back(make_tuple(std::make_pair(row, col), pathStr, 0));

                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, subOptimalXY));
                return;
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 1 )
            {
                moveWest(row, col, depth+1, path);
            }
            else if( mHalmaBoard[row][col] == '.' && depth == 2 )
            {
                vector<coordinate_dis> v;
                string pathStr = path + " " + to_string(row)+"," + to_string(col);
                v.push_back(make_tuple(std::make_pair(row, col), pathStr, 0));
                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, v));
                findNeighbors(row, col, pathStr);
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 2)
            {
                return;
            }
        }
    
    }

    void moveWestNeigh(int row, int col, int depth, string path)
    {
        if(isValidCell(row,col) && !mVisited[row][col])
        {
            cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if(mHalmaBoard[row][col] == '.' && depth == 1)
            {
                return;
            }
            else if(mHalmaBoard[row][col] != '.' && depth == 1)
            {
                moveWestNeigh(row, col-1, 2, path);
            }
            else if(mHalmaBoard[row][col] == '.' && depth == 2)
            {
                vector<coordinate_dis> subOptimalXY;
                string pathStr = path + " " + to_string(row) + "," + to_string(col);
                subOptimalXY.push_back(make_tuple(std::make_pair(row,col), pathStr, 0));
                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, subOptimalXY));

                findNeighbors(row, col, pathStr);
            }
            mVisited[row][col] = 0;
        } 
    }

    void moveNE(int row, int col, int depth, string path)
    {
        col = col + 1;
        row = row - 1;
        if(isValidCell(row,col) && !mVisited[row][col])
        {
            cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if( mHalmaBoard[row][col] == '.' && depth == 1 )
            {
                string pathStr = path + " " + to_string(row)+ "," + to_string(col);
                vector<coordinate_dis> subOptimalXY;

                // Push the destination XY and path to the destination as well.
                subOptimalXY.push_back(make_tuple(std::make_pair(row, col), pathStr, 0));

                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, subOptimalXY));
                return;
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 1 )
            {
                moveNE(row, col, depth+1, path);
            }
            else if( mHalmaBoard[row][col] == '.' && depth == 2 )
            {
                vector<coordinate_dis> v;
                string pathStr = path + " " + to_string(row)+"," + to_string(col);
                v.push_back(make_tuple(std::make_pair(row, col), pathStr, 0));
                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, v));
                findNeighbors(row, col, pathStr);
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 2)
            {
                return;
            }
        }

    }

    void moveNENeigh(int row, int col, int depth, string path)
    {
        if(isValidCell(row,col) && !mVisited[row][col])
        {
            cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if(mHalmaBoard[row][col] == '.' && depth == 1)
            {
                return;
            }
            else if(mHalmaBoard[row][col] != '.' && depth == 1)
            {
                moveNENeigh(row-1, col+1, 2, path);
            }
            else if(mHalmaBoard[row][col] == '.' && depth == 2)
            {
                vector<coordinate_dis> subOptimalXY;
                string pathStr = path + " " + to_string(row) + "," + to_string(col);
                subOptimalXY.push_back(make_tuple(std::make_pair(row,col), pathStr, 0));
                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, subOptimalXY));

                findNeighbors(row, col, pathStr);
            }
            mVisited[row][col] = 0;
        } 
    }


    void moveNW(int row, int col, int depth, string path)
    {
        col = col - 1;
        row = row - 1;
        if(isValidCell(row,col) && !mVisited[row][col])
        {
            cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if( mHalmaBoard[row][col] == '.' && depth == 1 )
            {
                string pathStr = path + " " + to_string(row)+ "," + to_string(col);
                vector<coordinate_dis> subOptimalXY;

                // Push the destination XY and path to the destination as well.
                subOptimalXY.push_back(make_tuple(std::make_pair(row, col), pathStr, 0));

                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, subOptimalXY));
                return;
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 1 )
            {
                moveNW(row, col, depth+1, path);
            }
            else if( mHalmaBoard[row][col] == '.' && depth == 2 )
            {
                vector<coordinate_dis> v;
                string pathStr = path + " " + to_string(row)+"," + to_string(col);
                v.push_back(make_tuple(std::make_pair(row, col), pathStr, 0));
                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, v));
                findNeighbors(row, col, pathStr);
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 2)
            {
                return;
            }
        }
 
    }

    void moveNWNeigh(int row, int col, int depth, string path)
    {
        if(isValidCell(row,col) && !mVisited[row][col])
        {
            cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if(mHalmaBoard[row][col] == '.' && depth == 1)
            {
                return;
            }
            else if(mHalmaBoard[row][col] != '.' && depth == 1)
            {
                moveNWNeigh(row-1, col-1, 2, path);
            }
            else if(mHalmaBoard[row][col] == '.' && depth == 2)
            {
                vector<coordinate_dis> subOptimalXY;
                string pathStr = path + " " + to_string(row) + "," + to_string(col);
                subOptimalXY.push_back(make_tuple(std::make_pair(row,col), pathStr, 0));
                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, subOptimalXY));

                findNeighbors(row, col, pathStr);
            }
            mVisited[row][col] = 0;
        }
    }

    void moveSE(int row, int col, int depth, string path)
    {
        col = col + 1;
        row = row + 1;
        if(isValidCell(row,col) && !mVisited[row][col])
        {
            cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if( mHalmaBoard[row][col] == '.' && depth == 1 )
            {
                string pathStr = path + " " + to_string(row)+ "," + to_string(col);
                vector<coordinate_dis> subOptimalXY;

                // Push the destination XY and path to the destination as well.
                subOptimalXY.push_back(make_tuple(std::make_pair(row, col), pathStr, 0));

                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, subOptimalXY));
                return;
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 1 )
            {
                moveSE(row, col, depth+1, path);
            }
            else if( mHalmaBoard[row][col] == '.' && depth == 2 )
            {
                vector<coordinate_dis> v;
                string pathStr = path + " " + to_string(row)+"," + to_string(col);
                v.push_back(make_tuple(std::make_pair(row, col), pathStr, 0));
                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, v));
                findNeighbors(row, col, pathStr);
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 2)
            {
                return;
            }
        } 
    }

    void moveSENeigh(int row, int col, int depth, string path)
    {
        if(isValidCell(row,col) && !mVisited[row][col])
        {
            cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if(mHalmaBoard[row][col] == '.' && depth == 1)
            {
                return;
            }
            else if(mHalmaBoard[row][col] != '.' && depth == 1)
            {
                moveSENeigh(row+1, col+1, 2, path);
            }
            else if(mHalmaBoard[row][col] == '.' && depth == 2)
            {
                vector<coordinate_dis> subOptimalXY;
                string pathStr = path + " " + to_string(row) + "," + to_string(col);
                subOptimalXY.push_back(make_tuple(std::make_pair(row,col), pathStr, 0));
                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, subOptimalXY));

                findNeighbors(row, col, pathStr);
            }
            mVisited[row][col] = 0;
        } 
    }

    void moveSW(int row, int col, int depth, string path)
    {
        col = col - 1;
        row = row + 1;
        if(isValidCell(row,col) && !mVisited[row][col])
        {
            cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if( mHalmaBoard[row][col] == '.' && depth == 1 )
            {
                string pathStr = path + " " + to_string(row)+ "," + to_string(col);
                vector<coordinate_dis> subOptimalXY;

                // Push the destination XY and path to the destination as well.
                subOptimalXY.push_back(make_tuple(std::make_pair(row, col), pathStr, 0));

                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, subOptimalXY));
                return;
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 1 )
            {
                moveSW(row, col, depth+1, path);
            }
            else if( mHalmaBoard[row][col] == '.' && depth == 2 )
            {
                vector<coordinate_dis> v;
                string pathStr = path + " " + to_string(row)+"," + to_string(col);
                v.push_back(make_tuple(std::make_pair(row, col), pathStr, 0));
                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, v));
                findNeighbors(row, col, pathStr);
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 2)
            {
                return;
            }
        } 
    }

    void moveSWNeigh(int row, int col, int depth, string path)
    {
        if(isValidCell(row,col) && !mVisited[row][col])
        {
            cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if(mHalmaBoard[row][col] == '.' && depth == 1)
            {
                return;
            }
            else if(mHalmaBoard[row][col] != '.' && depth == 1)
            {
                moveSWNeigh(row+1, col-1, 2, path);
            }
            else if(mHalmaBoard[row][col] == '.' && depth == 2)
            {
                vector<coordinate_dis> subOptimalXY;
                string pathStr = path + " " + to_string(row) + "," + to_string(col);
                subOptimalXY.push_back(make_tuple(std::make_pair(row,col), pathStr, 0));
                mResult.insert(std::pair<xyCoordinates, vector<coordinate_dis>>(mSource, subOptimalXY));

                findNeighbors(row, col, pathStr);
            }
            mVisited[row][col] = 0;
        }
    
    }

    void searchNeighbors(int row, int col)
    {
        // Source is added in the path
        string path = to_string(row)+","+to_string(col);

        resetVisited();
        mVisited[row][col] = 1;
        moveDown(row, col, 1, path);
        
        resetVisited();
        mVisited[row][col] = 1;
        moveUp(row, col, 1, path);
        
        resetVisited();
        mVisited[row][col] = 1;
        moveEast(row, col, 1, path);
        
        resetVisited();
        mVisited[row][col] = 1;
        moveWest(row, col, 1, path);
        
        resetVisited();
        mVisited[row][col] = 1;
        moveNE(row, col, 1, path);
        
        resetVisited();
        mVisited[row][col] = 1;
        moveNW(row, col, 1, path);
        
        resetVisited();
        mVisited[row][col] = 1;
        moveSW(row, col, 1, path);
        
        resetVisited();
        mVisited[row][col] = 1;
        moveSE(row, col, 1, path);
    }

    void setSource(xyCoordinates _source)
    {
        mSource = _source;
    }

    void findLegalMoves(char turn)
    {
        int row, col;

        if(turn == 'W')
        {
            cout << __func__ <<endl;
            populatePawns();

            for(auto itr = mWhitePawn.begin(); itr != mWhitePawn.end(); itr++)
            {
                row = itr->first;
                col = itr->second;

                cout << __func__ << "row:" << row << " col:" << col << endl;

                setSource(*itr);
                searchNeighbors(row, col);
            }
            multimap<xyCoordinates, xyCoordinates> legalMoves;

            for(auto itr = mResult.begin(); itr != mResult.end(); itr++)
            {
                xyCoordinates xyValue = itr->first;

                vector<coordinate_dis> vecList = itr->second;

                cout << "The source X: " << xyValue.first << " and Y: " << xyValue.second <<"\n";

                cout << "The destination nodes are: \n";

                for(auto vec = vecList.begin(); vec != vecList.end(); vec++)
                {
                    xyCoordinates XY = get<0>(*vec);
                    string destPath = get<1>(*vec);
                    int eucledianDis = get<2>(*vec);

                    cout<< "X:" << XY.first << "Y:" << XY.second << "\n";

                    cout << "The string to destination is:" << destPath << "\n";

                    cout << "The eucledian distance is:" << eucledianDis << "\n";
                
                    legalMoves.insert(std::pair<xyCoordinates, xyCoordinates>(xyValue, XY));
                }
            
            }

#if 0
            for(auto itr = legalMoves.begin(); itr!= legalMoves.end(); itr++)
            {
                xyCoordinates xyValue = itr->first;

                xyCoordinates xy1 = itr->second;

                cout << "X: " << xyValue.first << " Y: " << xyValue.second << "::";
                cout << "X: " << xy1.first << "Y: " << xy1.second <<"\n";
            }

#endif

        }
        else
        {
            for(auto itr = mBlackPawn.begin(); itr != mBlackPawn.end(); itr++)
            {
                row = itr->first;
                col = itr->second;
            
                searchNeighbors(row, col);
            }
        }
    }

    void evaluationFunction()
    {
    
        
    
    
    }

#if 0
    int getBoardState(char board[16][16], char marker)
    {
        char opponentMarker = (marker == 'W') ? 'B' : 'W';

        
    
    }

    // Apply the minimax game optimization algorithm
    std::pair<int, std::pair<int, int>> minimax_optimization(char marker, int depth, int alpha, int beta)
    {
        //Initialize the best move
        std::pair<int, int> best_move = std::make_pair(-1, -1);
        int best_score = (marker == getMyPawnChar()) ? LOSS : WIN;

        if(depth == 3)
        {
            //TODO write evaluation function.
        
        }

        if( isRightGoal(board) )
        {
            if(marker == 'B')
                return std::make_pair(WIN, best_move);
            else
                return std::make_pair(LOSS, best_move);
        }

        if( isLeftGoal(board) )
        {
            if(marker == 'W')
                return std::make_pair(WIN, best_move);
            else
                return std::make_pair(LOSS, best_move);
        }

        findLegalMoves(marker);

        multimap<xyCoordinates, vector<coordinate_dis>> _result = mResult;

        multimap<xyCoordinates, xyCoordinates> legalMoves;

        for(auto itr = _result.begin(); itr != _result.end(); itr++)
        {
            xyCoordinates xyValue = itr->first;

            vector<coordinate_dis> vecList = itr->second;

            for(auto vec = vecList.begin(); vec != vecList.end(); vec++)
            {
                xyCoordinates XY = get<0>(*vec);

                legalMoves.push_back(std::pair<xyCoordinates, xyCoordinates>(xyValue, XY));

            }

        }

        for(auto itr = legalMoves.begin(); itr != legalMoves.end(); itr++)
        {
            xyCoordinates currMove = itr->first;
            xyCoordinates nextMove = itr->second;

            mHalmaBoard[curr_move.first][currMove.second] = '.';

            mHalmaBoard[nextMove.first][nextMove.second] = marker;


            if( marker == getMyPawnChar())
            {
                char opponent = (marker == 'W') ? 'B' : 'W'; 
                
                int score = minimax_optimization( opponent, depth + 1, alpha, beta).first;

                if( best_score < score)
                {
                    // TODO change.
                    best_score = score - depth * 10;
                    best_move = currMove;

                    alpha = std::max(alpha, best_score);

                    mHalmaBoard[curr_move.first][currMove.second] = marker;
                    mHalmaBoard[nextMove.first][nextMove.second] = '.';

                    if(beta <= alpha)
                    {
                        break;
                    }
                
                }
            
            
            
            
            }








        }

    }

#endif

};

int main()
{

    HALMA game;

    game.readInputs();
    
    game.getBlackGoal();

    game.getWhiteGoal();

    game.showInputs();

//    cout << "The right goal is: " << game.isRightGoal() << endl;

//    cout << "The left goal is:" << game.isLeftGoal() << endl;

    game.findLegalMoves('W');

    return 0;
}
