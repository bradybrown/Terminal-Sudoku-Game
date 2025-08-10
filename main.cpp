#include <iostream>
#include <string>
#include "termcolor/termcolor.hpp"
#include <fstream>
#include <cstdio>
#include <sstream>

using namespace std;

// functions to save/load the game state locally
bool save(string filename, int board[9][9])
{
    // declaring output file stream object
    ofstream fout;

    // open a file
    fout.open(filename);

    //check for errors
    if (fout.fail())
    {
        fout.close();
        return false;
    }

    // write the board state to the file
    for (int y = 0; y < 9; ++y)
    {
        for (int x = 0; x < 9; ++x)
        {
            fout << board[x][y] << " ";
        }
        fout << endl;
    }

    // close the file and return
    fout.close();
    
    cout << "This game has been saved." << endl;
    return true;

}

bool load(string filename, int board[9][9])
{
    // declaring input file stream object
    ifstream fin;

    // open a file
    fin.open(filename);

    // check for errors
    if (fin.fail())
    {
        fin.close();
        return false;
    }
    
    // read data from file
    for (int y = 0; y < 9; ++y)
    {
        for (int x = 0; x < 9; ++x)
        {
            fin >> board[x][y];
        }
    }

    // close file and return
    fin.close();

    cout << "Game is loaded." << endl;
    return true;
}



// function to draw a suduko board
void draw(int board[9][9])
{
        // cout << termcolor::on_grey;
        cout << termcolor::white << "    1 2 3   4 5 6   7 8 9" << endl << endl;
        for (int column = 0; column < 9; ++column)
        {
            if (column == 3 || column == 6) 
                cout << termcolor::white << "    ------+-------+------" << endl;

            cout << termcolor::white << char('A' + column) << "   ";
            for (int row = 0; row < 9; ++row)
            {

                if (row == 3 || row == 6)
                    cout << termcolor::white << "| ";
                


                // print empty space instead of placeholder 0
                if (board[row][column] != 0)
                    // Starting values = cyan (negative numbers in the nested array)
                    if (board[row][column] < 0)
                        cout << termcolor::cyan << abs(board[row][column]) << " ";
                    else 
                        cout << termcolor::white << board[row][column] << " ";    
                else
                    cout << "  ";
            }
            cout << endl;
        }
        cout << termcolor::white;
}





// function to check if a move is legal or not
bool isLegal(int board[9][9], int row, int column, int value)
{
    // check to make sure we are on the board
    if (row >= 9 || row < 0|| column >= 9 || column < 0) return false;

    // check if the value we want to input is valid
    if (value < 0 || value > 9) return false;

    if (board[row][column] < 0) return false;
    
    // check rows, then columns
    for (int x = 0; x < 9; ++x)
        if (abs(board[x][column]) == value) return false;    

    for (int y = 0; y < 9; ++y)
        if (abs(board[row][y] == value)) return false;    
    
    // check subgrids
    int subgrid_x = (row/3) * 3;
    int subgrid_y = (column/3) * 3;

    for (int sub_x = subgrid_x; sub_x < subgrid_x + 3; ++sub_x)
        for (int sub_y = subgrid_y; sub_y < subgrid_y + 3; ++sub_y)
            if (abs(board[sub_x][sub_y]) == value) return false;

    // if nothing makes it invalid, the move is legal
    return true;
}


// function to alphebetize and array of strings (use QuickSort alg)
void alphabetize(string strings[], int numStrings)
{
    // Selection Sort (CHANGE LATER)
    for (int i = 0; i < numStrings; ++i)
    {
        int swapIndex = i;
        for (int j = i; j < numStrings; ++j)
        {
            if (strings[j] < strings[swapIndex])
                swapIndex = j;
        }

        // perfom swap
        string temp = strings[swapIndex];
        strings[swapIndex] = strings[i];
        strings[i] = temp;
    }
}


// main function 
int main() 
{
    // stringstream object for writing data to the console on future interations of the game loop
    stringstream console;



    // variable tracking # of saved games
    int numSaves = 0;
    string savedGames[1000];

    // load the tracking data to directory.txt file
    ifstream fin("directory.txt");
    if (!fin.fail())
    {
        fin >> numSaves;
        for (int i = 0; i < numSaves; ++i)
            fin >> savedGames[i];
    }
    fin.close();

    alphabetize(savedGames, numSaves);


    // varible for the game's state (running or NOT)
    bool running = true;

    int board[9][9] = 
    {
        // Empty board
        // {0, 0, 0, 0, 0, 0, 0, 0, 0},
        // {0, 0, 0, 0, 0, 0, 0, 0, 0},
        // {0, 0, 0, 0, 0, 0, 0, 0, 0},

        // {0, 0, 0, 0, 0, 0, 0, 0, 0},
        // {0, 0, 0, 0, 0, 0, 0, 0, 0},
        // {0, 0, 0, 0, 0, 0, 0, 0, 0},

        // {0, 0, 0, 0, 0, 0, 0, 0, 0},
        // {0, 0, 0, 0, 0, 0, 0, 0, 0},
        // {0, 0, 0, 0, 0, 0, 0, 0, 0},

        // Board from online
        {-1, 0, 0, 0, 0, 0,-5, 0, 0},
        { 0, 0,-8, 0,-5,-4, 0,-9,-7},
        { 0, 0, 0, 0,-3, 0,-2, 0,-6},
        
        {-8,-9, 0, 0, 0, 0, 0, 0, 0},
        { 0,-1,-5, 0,-4, 0,-9,-7, 0},
        { 0, 0, 0, 0, 0, 0, 0,-5,-2},

        {-6, 0,-7, 0,-1, 0, 0, 0, 0},
        {-5,-2, 0,-9,-7, 0,-3, 0, 0},
        { 0, 0,-9, 0, 0, 0, 0, 0,-5}


    };



    while (running)
    {
        // clear the screen before every move/input
#ifdef _WIN32
        system("cls");    // command for Windows systems
#else
        system("clear");  // command for Unix-based systems
#endif

        // generate output (display sudoku board)
        cout << endl;
        draw(board);

        // print contents of the stringstream to the console, then clear it
        cout << endl << console.str() << endl;
        console.str("");


        cout << endl << "Enter a command: ";
        
        // get user input
        std::string input;
        getline(cin, input);



/* List of Possible Commands                                                                
    "exit"
    "set " "(lowercase letter () number) "  "(value)"
    "load (filename)"
    "save (filename)"
    "list saves"
    "delete"

*/
        
        string command = input.substr(0, 4);

        // shows the saved games to the user
        if (input == "list saves")
        {
            console << "Saved Games" << endl;
            for (int i = 0; i < numSaves; ++i)
                console << '\t' << savedGames[i] << endl;
        }


        // delete a save file
        if (input.substr(0,6) == "delete")
        {
            if (input.size() > 6)
            {
                string filename = input.substr(6);

                while (filename.size() > 0 && filename[0] == ' ')
                    filename.erase(0,1);
                
                // remove file name from directory
                for (int i = 0; i < numSaves; ++i)
                {
                    if (savedGames[i] == filename)
                    {
                        for (int j = i + 1; j < numSaves; ++j)
                            savedGames[j - 1] = savedGames[j];
                        --numSaves;
                    }
                }
                // delete file from the hard drive
                remove(filename.c_str());
            }
        }


        // loads a save file (with given name, otherwise default = autosave.txt)
        if (command == "load")
        {
            if (input.size() > 4)
            {
                string filename = input.substr(4);
                // get rid of whitespaces
                while (filename.size() > 0 && filename[0] == ' ')
                    filename.erase(0,1);
                
                // load from the file
                load(filename, board);
            }
            // otherwaise, load the autosave text file
            else load("autosave.txt", board);
        }


        // save file into given filename
        if (command == "save")
        {
            if (input.size() > 4)
            {
                string filename = input.substr(6);
                // get rid of whitespaces
                while (filename.size() > 0 && filename[0] == ' ')
                {
                    filename.erase(0,1);
                }
                
                // save to the file
                save(filename, board);

                // check if the save is already in the directory
                bool inDirectory = false;
                for (int i = 0; i < numSaves; ++i)
                {
                    if (savedGames[i] == filename)
                        inDirectory = true;
                }

                // add filename to dir
                if (!inDirectory)
                {
                    savedGames[numSaves] = filename;
                    ++numSaves;
                } 
                else cout << "Already in directory." << endl;

                // organize the directory
                alphabetize(savedGames, numSaves);

                // save updated directory.txt file
                ofstream fout("directory.txt");
                fout << numSaves << endl;
                for (int i = 0; i < numSaves; ++i)
                    fout << savedGames[i] << endl;
                fout.close(); 
            } 
            
            // else use default file
            else 
                save("autosave.txt", board);
        }


        // stops game from running
        if (command == "exit") 
        {
            running = false;
            save("autosave.txt", board);

            ofstream fout("directory.txt");
            fout << numSaves << endl;
            for (int i = 0; i < numSaves; ++i)
                fout << savedGames[i] << endl;
            fout.close();

        }
            
        // put a value somewhere on the board
        if (command == "set ")
        {
            int y = input[4] - 'a';
            int x = input[5] -  '1';
            int val = input[7] - '0';

            if (isLegal(board, x, y, val)) {
                board[x][y] = val;
            } else {
                console << "That move is illegal. Try again." << endl;
            }
        }

    }


    return 0;
}
