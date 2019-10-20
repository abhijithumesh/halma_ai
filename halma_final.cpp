#include <stdio.h>
#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <tuple>
#include <climits>
#include <numeric>
#include <chrono>
#include <string>
#include <set>

#define WIN 1500
#define DRAW 0
#define LOSS -1500

using namespace std;
using namespace std::chrono;

typedef std::pair<int, int> xyCoordinates;
typedef std::vector<xyCoordinates> pawnVector;
typedef tuple<xyCoordinates, string, int> coordinate_dis;
typedef tuple<int, xyCoordinates, xyCoordinates, string> miniMaxRet;
typedef tuple<xyCoordinates, xyCoordinates, string, int> src_dest_dis;

class CompareDist
{
public:
    bool operator()(const src_dest_dis& n1, const src_dest_dis& n2) const
    {
        return get<2>(n1).length() > get<2>(n2).length();
    }

};

typedef std::set<src_dest_dis, CompareDist> resultArray;

class HALMA{

private:
    string mPlayType;
    string mMyPawn;
    double mTimeRemaining;
    char mHalmaBoard[16][16];
    pawnVector mWhitePawn;
    pawnVector mBlackPawn;
    int mVisited[16][16];
    //multimap<xyCoordinates, coordinate_dis> mResult;
    resultArray mResult;
    xyCoordinates mSource;
    pawnVector mWhiteGoal;
    pawnVector mBlackGoal;
    int board_count;

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

        board_count = 0;

    }

    inline void resetVisited()
    {
        memset(mVisited, 0, sizeof(mVisited));
    }

    void readInputs()
    {
        ifstream file( "input_raghu2.txt", ios::in );

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

        cout << "The board count is:" << board_count << "\n";

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
        mWhitePawn.clear();
        mBlackPawn.clear();

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

    inline bool isValidCell(int row, int col)
    {
        if( row < 0 || row >=16 || col < 0 || col >=16 )
            return false;

        return true;
    }


    void findNeighbors(int row, int col, string path)
    {
        //cout << __func__ <<"row:" << row << "col:" << col <<endl;
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
            //cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if( mHalmaBoard[row][col] == '.' && depth == 1 )
            {
                string pathStr = path + " " + to_string(row)+ "," + to_string(col);

                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));
                return;
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 1 )
            {
                mVisited[row][col] = 0;
                moveDown(row, col, depth+1, path);
            }
            else if( mHalmaBoard[row][col] == '.' && depth == 2 )
            {
                string pathStr = path + " " + to_string(row)+"," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));
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
            //cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if(mHalmaBoard[row][col] == '.' && depth == 1)
            {
                return;
            }
            else if(mHalmaBoard[row][col] != '.' && depth == 1)
            {
                mVisited[row][col] = 0;
                moveDownNeigh(row+1, col, 2, path);
            }
            else if(mHalmaBoard[row][col] == '.' && depth == 2)
            {
                string pathStr = path + " " + to_string(row) + "," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));
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
            //cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if( mHalmaBoard[row][col] == '.' && depth == 1 )
            {
                string pathStr = path + " " + to_string(row)+ "," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));

                return;
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 1 )
            {
                mVisited[row][col] = 0;
                moveUp(row, col, depth+1, path);
            }
            else if( mHalmaBoard[row][col] == '.' && depth == 2 )
            {
                string pathStr = path + " " + to_string(row)+"," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));
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
            //cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if(mHalmaBoard[row][col] == '.' && depth == 1)
            {
                return;
            }
            else if(mHalmaBoard[row][col] != '.' && depth == 1)
            {
                mVisited[row][col] = 0;
                moveUpNeigh(row-1, col, 2, path);
            }
            else if(mHalmaBoard[row][col] == '.' && depth == 2)
            {
                string pathStr = path + " " + to_string(row) + "," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));

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
            //cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if( mHalmaBoard[row][col] == '.' && depth == 1 )
            {
                string pathStr = path + " " + to_string(row)+ "," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));

                return;
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 1 )
            {
                mVisited[row][col] = 0;
                moveEast(row, col, depth+1, path);
            }
            else if( mHalmaBoard[row][col] == '.' && depth == 2 )
            {
                string pathStr = path + " " + to_string(row)+"," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));
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
            //cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if(mHalmaBoard[row][col] == '.' && depth == 1)
            {
                return;
            }
            else if(mHalmaBoard[row][col] != '.' && depth == 1)
            {
                mVisited[row][col] = 0;
                moveEastNeigh(row, col+1, 2, path);
            }
            else if(mHalmaBoard[row][col] == '.' && depth == 2)
            {
                string pathStr = path + " " + to_string(row) + "," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));

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
            //cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if( mHalmaBoard[row][col] == '.' && depth == 1 )
            {
                string pathStr = path + " " + to_string(row)+ "," + to_string(col);

                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));
                return;
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 1 )
            {
                mVisited[row][col] = 0;
                moveWest(row, col, depth+1, path);
            }
            else if( mHalmaBoard[row][col] == '.' && depth == 2 )
            {
                string pathStr = path + " " + to_string(row)+"," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));
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
            //cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if(mHalmaBoard[row][col] == '.' && depth == 1)
            {
                return;
            }
            else if(mHalmaBoard[row][col] != '.' && depth == 1)
            {
                mVisited[row][col] = 0;
                moveWestNeigh(row, col-1, 2, path);
            }
            else if(mHalmaBoard[row][col] == '.' && depth == 2)
            {
                string pathStr = path + " " + to_string(row) + "," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));

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
            //cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if( mHalmaBoard[row][col] == '.' && depth == 1 )
            {
                string pathStr = path + " " + to_string(row)+ "," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));
                return;
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 1 )
            {
                mVisited[row][col] = 0;
                moveNE(row, col, depth+1, path);
            }
            else if( mHalmaBoard[row][col] == '.' && depth == 2 )
            {
                string pathStr = path + " " + to_string(row)+"," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));
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
            //cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if(mHalmaBoard[row][col] == '.' && depth == 1)
            {
                return;
            }
            else if(mHalmaBoard[row][col] != '.' && depth == 1)
            {
                mVisited[row][col] = 0;
                moveNENeigh(row-1, col+1, 2, path);
            }
            else if(mHalmaBoard[row][col] == '.' && depth == 2)
            {
                string pathStr = path + " " + to_string(row) + "," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));

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
            //cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if( mHalmaBoard[row][col] == '.' && depth == 1 )
            {
                string pathStr = path + " " + to_string(row)+ "," + to_string(col);

                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));
                return;
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 1 )
            {
                mVisited[row][col] = 0;
                moveNW(row, col, depth+1, path);
            }
            else if( mHalmaBoard[row][col] == '.' && depth == 2 )
            {
                string pathStr = path + " " + to_string(row)+"," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));
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
            //cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if(mHalmaBoard[row][col] == '.' && depth == 1)
            {
                return;
            }
            else if(mHalmaBoard[row][col] != '.' && depth == 1)
            {
                mVisited[row][col] = 0;
                moveNWNeigh(row-1, col-1, 2, path);
            }
            else if(mHalmaBoard[row][col] == '.' && depth == 2)
            {
                string pathStr = path + " " + to_string(row) + "," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));

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
            //cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if( mHalmaBoard[row][col] == '.' && depth == 1 )
            {
                string pathStr = path + " " + to_string(row)+ "," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));

                return;
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 1 )
            {
                mVisited[row][col] = 0;
                moveSE(row, col, depth+1, path);
            }
            else if( mHalmaBoard[row][col] == '.' && depth == 2 )
            {
                string pathStr = path + " " + to_string(row)+"," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));
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
            //cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if(mHalmaBoard[row][col] == '.' && depth == 1)
            {
                return;
            }
            else if(mHalmaBoard[row][col] != '.' && depth == 1)
            {
                mVisited[row][col] = 0;
                moveSENeigh(row+1, col+1, 2, path);
            }
            else if(mHalmaBoard[row][col] == '.' && depth == 2)
            {
                string pathStr = path + " " + to_string(row) + "," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));

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
            //cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if( mHalmaBoard[row][col] == '.' && depth == 1 )
            {
                string pathStr = path + " " + to_string(row)+ "," + to_string(col);

                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));
                return;
            }
            else if( mHalmaBoard[row][col] != '.' && depth == 1 )
            {
                mVisited[row][col] = 0;
                moveSW(row, col, depth+1, path);
            }
            else if( mHalmaBoard[row][col] == '.' && depth == 2 )
            {
                string pathStr = path + " " + to_string(row)+"," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));
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
            //cout << __func__ <<"row:" << row << "col:" << col <<endl;
            mVisited[row][col] = 1;
            if(mHalmaBoard[row][col] == '.' && depth == 1)
            {
                return;
            }
            else if(mHalmaBoard[row][col] != '.' && depth == 1)
            {
                mVisited[row][col] = 0;
                moveSWNeigh(row+1, col-1, 2, path);
            }
            else if(mHalmaBoard[row][col] == '.' && depth == 2)
            {
                string pathStr = path + " " + to_string(row) + "," + to_string(col);
                mResult.insert(make_tuple(mSource, std::make_pair(row, col), pathStr, 0));

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
        //cout << __func__ <<endl;

        //Get the state of white pawns and black pawns
        populatePawns();

        //Show the state of the board.
        //showInputs();

        mResult.clear();

        // Legal Moves source and destination
        multimap<xyCoordinates, xyCoordinates> legalMoves;

        if(turn == 'W')
        {
            for(auto itr = mWhitePawn.begin(); itr != mWhitePawn.end(); itr++)
            {
                row = itr->first;
                col = itr->second;

                //cout << __func__ << "row:" << row << " col:" << col << endl;

                setSource(*itr);
                searchNeighbors(row, col);
            }

        }
        else
        {
            for(auto itr = mBlackPawn.begin(); itr != mBlackPawn.end(); itr++)
            {
                row = itr->first;
                col = itr->second;

                //cout << __func__ << "row:" << row << " col:" << col << endl;

                setSource(*itr);
                searchNeighbors(row, col);
            }
                        
        }

        //cout << __func__ << "   mResult.size()" << mResult.size() <<endl;

#if 0
        sort( mResult.begin( ), mResult.end( ), [ ]( const src_dest_dis& n1, const src_dest_dis& n2 )
        {
            if(get<2>(n1).length() == get<2>(n2).length())
            {
                return get<3>(n1) > get<3>(n2);
            }
            return get<2>(n1).length() > get<2>(n2).length(); 
        });
        for(auto itr = mResult.begin(); itr != mResult.end(); itr++)
        {
            xyCoordinates xyValue = get<0>(*itr);

            xyCoordinates xyValue1 = get<1>(*itr);

            string path = get<2>(*itr);

            cout << "The source X: " << xyValue.first << " and Y: " << xyValue.second <<"\n";

            cout << "The destination X: " << xyValue1.first << " and Y: " << xyValue1.second <<"\n";
       

            cout << "The path is:" << path <<endl;
        }
#endif
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

    miniMaxRet evaluationFunction(char marker, xyCoordinates XY)
    {  
/*
        int row = 0, col = 0; 
        if( marker == 'B') 
        {    
            row = 15;
            col = 15;
        }    

        return std::make_tuple(-1 * diagonalDistance( XY.first, XY.second, row, col), make_pair(-1, -1), make_pair(-1, -1), "" );

*/
        int value = 0;

        for(int i = 0; i < 16; i++ )
        {
            for(int j = 0; j < 16; j++)
            {
                char ch = mHalmaBoard[j][i];

                if( ch == 'W' )
                {
                    vector<int> distances;
                    for( auto itr = mWhiteGoal.begin(); itr != mWhiteGoal.end(); itr++)
                    {
                        int X = itr -> first;
                        int Y = itr -> second;

                        if(mHalmaBoard[X][Y] != 'W')
                            continue;

                        distances.push_back(diagonalDistance(j, i, X, Y));
                    
                    }

                    int max_value = -50;
                    if (distances.size() > 0)
                        max_value = *max_element(distances.begin(), distances.end());
                    value += max_value;
                
                }
                else if( ch == 'B' )
                {
                    vector<int> distances;
                    for( auto itr = mBlackGoal.begin(); itr != mBlackGoal.end(); itr++)
                    {

                        int X = itr -> first;
                        int Y = itr -> second;

                        if(mHalmaBoard[X][Y] != '.' || mHalmaBoard[X][Y] == 'B')
                            continue;

                        distances.push_back(diagonalDistance(j, i, X, Y));
                    
                    }

                    int max_value = -50;
                    if (distances.size() > 0)
                        max_value = *max_element(distances.begin(), distances.end());
                    value -= max_value;
                
                }
            
            }

        }

        if(marker == 'B')
        {
            value  *= -1;
        }

        //cout << "Value:" << value << endl;
        return std::make_tuple( value, make_pair(-1, -1), make_pair(-1, -1), "" );

    }


#if 0
    miniMaxRet evaluationFunction(char marker)
    {
    //    cout << "Evaluating" << "\n" ;
        findLegalMoves(marker);

        multimapResult _result = mResult;

        mResult.clear();

        // set of all legal moves for 19 pawns.
        multimap<xyCoordinates, xyCoordinates> legalMoves;

        for(auto itr = _result.begin(); itr != _result.end(); itr++)
        {
            // getting the source node where the pawn in currently
            xyCoordinates xyValue = itr->first;

            vector<coordinate_dis> vecList = itr->second;

            for(auto vec = vecList.begin(); vec != vecList.end(); vec++)
            {
                //set of all XY coordinates where the pawn can move.
                xyCoordinates XY = get<0>(*vec);

                legalMoves.insert(std::pair<xyCoordinates, xyCoordinates>(xyValue, XY));
            }

        }

        for(auto itr = legalMoves.begin(); itr != legalMoves.end(); itr++)
        {
            // get the source
            xyCoordinates currPos = itr->first;

            // get the destination
            xyCoordinates nextMove = itr->second;

            // Move the pawn from current state to next state.
            mHalmaBoard[currPos.first][currPos.second] = '.';
            mHalmaBoard[nextMove.first][nextMove.second] = marker;

            if( isRightGoal(mHalmaBoard) )
            {
                mHalmaBoard[currPos.first][currPos.second] = marker;
                mHalmaBoard[nextMove.first][nextMove.second] = '.';

                if(getMyPawnChar() == 'B')
                    return std::make_tuple( INT_MAX, currPos, nextMove, "" );
                else
                    return std::make_tuple( INT_MIN, currPos, nextMove, "" );
            }

            if( isLeftGoal(mHalmaBoard) )
            {
                mHalmaBoard[currPos.first][currPos.second] = marker;
                mHalmaBoard[nextMove.first][nextMove.second] = '.';

                if(getMyPawnChar() == 'W')
                    return std::make_tuple( INT_MAX, currPos, nextMove, "" );
                else
                    return std::make_tuple( INT_MIN, currPos, nextMove, "" );

            }

            int value = call_utility();
            mHalmaBoard[currPos.first][currPos.second] = marker;
            mHalmaBoard[nextMove.first][nextMove.second] = '.';

            

        }
        int value = 0;

        for(int i = 0; i < 16; i++ )
        {
            for(int j = 0; j < 16; j++)
            {
                char ch = mHalmaBoard[j][i];

                if( ch == 'W' )
                {
                    vector<int> distances;
                    for( auto itr = mWhiteGoal.begin(); itr != mWhiteGoal.end(); itr++)
                    {

                        int X = itr -> first;
                        int Y = itr -> second;

                        if(mHalmaBoard[X][Y] != '.' ||  mHalmaBoard[X][Y] =='W')
                            continue;

                        distances.push_back(diagonalDistance(j, i, X, Y));
                    
                    }

                    int max_value = *max_element(distances.begin(), distances.end());
                    value -= max_value;
                
                }
                else if( ch == 'B' )
                {
                    vector<int> distances;
                    for( auto itr = mBlackGoal.begin(); itr != mBlackGoal.end(); itr++)
                    {

                        int X = itr -> first;
                        int Y = itr -> second;

                        if(mHalmaBoard[X][Y] != '.' || mHalmaBoard[X][Y] == 'B')
                            continue;

                        distances.push_back(diagonalDistance(j, i, X, Y));
                    
                    }

                    int max_value = *max_element(distances.begin(), distances.end());
                    value += max_value;
                
                }
            
            }

        }

        if(marker == 'W')
        {
            value  = value * -1;
        }


        //cout << "Value:" << value << endl;
        return std::make_tuple( value, make_pair(-1, -1), make_pair(-1, -1), "" );
        int value = 0;

        vector<int> distances;

        for(int i = 0; i < 16; i++ )
        {
            for(int j = 0; j < 16; j++)
            {
                char ch = mHalmaBoard[j][i];

                if( ch == marker && marker == 'W' )
                {
                    for( auto itr = mWhiteGoal.begin(); itr != mWhiteGoal.end(); itr++)
                    {

                        int X = itr -> first;
                        int Y = itr -> second;

                        if(mHalmaBoard[X][Y] != '.')
                            continue;

                        distances.push_back(diagonalDistance(j, i, X, Y));

                    }
                
                }
                
                if( ch == marker && marker == 'B' )
                {
                    for( auto itr = mBlackGoal.begin(); itr != mBlackGoal.end(); itr++)
                    {

                        int X = itr -> first;
                        int Y = itr -> second;

                        if(mHalmaBoard[X][Y] != '.')
                            continue;

                        distances.push_back(diagonalDistance(j, i, X, Y));

                    }
                
                }

            }
        }

        value = accumulate(distances.begin(), distances.end(), 0);
    
    /*
        if( marker == 'B')
        {
            value = value * 1;
        }
        else
            value = value * -1;
    */

        cout << "Value:" << value << endl;
        return std::make_tuple( value, make_pair(-1, -1), make_pair(-1, -1), "" );
    }
#endif


    // Apply the minimax game optimization algorithm
    miniMaxRet minimax_optimization(char marker, int depth, int alpha, int beta, xyCoordinates XY = make_pair(-1, -1))
    {
        if( depth == 3 )
        {
            return evaluationFunction(marker, XY);
        }
        std::pair<int, int> best_move = std::make_pair(-1, -1);
        std::pair<int, int> curr_move = std::make_pair(-1, -1);
        int best_score = (marker == getMyPawnChar()) ? INT_MIN : INT_MAX; 
        if( isRightGoal(mHalmaBoard) )
        {
            if(getMyPawnChar() == 'B')
                return std::make_tuple( INT_MAX, make_pair(-1, -1), make_pair(-1, -1), "" );
            else
                return std::make_tuple( INT_MIN, make_pair(-1, -1), make_pair(-1, -1), "" );
        }


        if( isLeftGoal(mHalmaBoard) )
        {
            if(getMyPawnChar() == 'W')
                return std::make_tuple( INT_MAX, make_pair(-1, -1), make_pair(-1, -1), "" );
            else
                return std::make_tuple( INT_MIN, make_pair(-1, -1), make_pair(-1, -1), "" );
        }

        // stores the result in mResult.
        findLegalMoves(marker);

        // save the state of the result before going to change it.
        resultArray _result = mResult;

        mResult.clear();

        for(auto itr = _result.begin(); itr != _result.end(); itr++)
        {
            // get the source
            xyCoordinates currPos = get<0>(*itr);

            // get the destination
            xyCoordinates nextMove = get<1>(*itr);

            board_count ++;
            // Move the pawn from current state to next state.
            mHalmaBoard[currPos.first][currPos.second] = '.';
            mHalmaBoard[nextMove.first][nextMove.second] = marker;

            // Maximizing agent
            if( marker == getMyPawnChar() )
            {

                //cout << "Maximising" << "\n" ;
                char opponent = (marker == 'W') ? 'B' : 'W';
                miniMaxRet ret = minimax_optimization( opponent, depth + 1, alpha, beta, nextMove);
                int score = get<0>(ret);

               // cout << __func__ << " Maximizing Score:" <<score << "\n";

                if( score > best_score )
                {
                    best_score = score;
                    best_move = nextMove;
                    curr_move = currPos;

                    alpha = std::max(alpha, best_score);

                    mHalmaBoard[currPos.first][currPos.second] = marker;
                    mHalmaBoard[nextMove.first][nextMove.second] = '.';

                    if( beta <= alpha )
                    {
                        //cout << "Hitting here 1." << "\n";
                        break;
                    }
                }
            }
            else // Minimizing opponent's turn
            {
                //cout << "Minimizing" << "\n" ;
                
                char opponent = (marker == 'W') ? 'B' : 'W';
                miniMaxRet ret = minimax_optimization( opponent, depth + 1, alpha, beta, nextMove);
                int score = get<0>(ret);

                //cout << __func__ << " Minimizing Score:" <<score ;
                
                if( score < best_score )
                {
                    best_score = score;
                    best_move = nextMove;
                    curr_move = currPos;

                    beta = std::min(beta, best_score);

                    mHalmaBoard[currPos.first][currPos.second] = marker;
                    mHalmaBoard[nextMove.first][nextMove.second] = '.';


                    if( beta <= alpha )
                    {
                        //cout << "Hitting here 2." << "\n";
                        break;
                    }
                }

            }

            mHalmaBoard[currPos.first][currPos.second] = marker;
            mHalmaBoard[nextMove.first][nextMove.second] = '.';

        }
        
        return std::make_tuple( best_score, curr_move, best_move, "" );
    }

    int diagonalDistance(int x1, int y1, int x2, int y2)
    {
       int diff1 = x1 - x2;
       int diff2 = y1 - y2;

       diff1 = abs(diff1);
       diff2 = abs(diff2);

       return (diff1 > diff2) ? diff1 : diff2;
    
    }

};

int main()
{
    HALMA game;

    game.readInputs();
    
    game.getBlackGoal();

    game.getWhiteGoal();

    game.showInputs();

#if 0
    game.findLegalMoves('W');

#endif
    auto start = high_resolution_clock::now();
    miniMaxRet ai_move = game.minimax_optimization('W', 0, INT_MIN, INT_MAX);
    auto stop = high_resolution_clock::now();

    cout << "Abhi!!!" << "\n\n\n";

    xyCoordinates src = get<1>(ai_move);
    xyCoordinates dest = get<2>(ai_move);

    cout << "Source X: " << src.first  << " Y: " << src.second << "\n";
    cout << "Destination X: " << dest.first  << " Y: " << dest.second << "\n";

    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Duration:" << duration.count() << endl;
    game.showInputs();
    return 0;
}
