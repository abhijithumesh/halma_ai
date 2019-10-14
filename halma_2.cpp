#include <stdio.h>
#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <tuple>

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
                
                }
            
            }

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

};

int main()
{

    HALMA game;

    game.readInputs();

    game.showInputs();

    cout << "The right goal is: " << game.isRightGoal() << endl;

    cout << "The left goal is:" << game.isLeftGoal() << endl;

    game.findLegalMoves('W');

    return 0;
}