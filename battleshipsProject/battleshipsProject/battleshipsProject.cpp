#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
using namespace std;

char** P1_GRID, **P2_GRID, **P1_GRID_GRAPHIC, **P2_GRID_GRAPHIC;

// A structure that keeps data. Necessary for saving data.
struct gameData {
    char** p1; 
    char** p2;
    char** p1Graphic;
    char** p2Graphic;
    int rows; 
    int cols; 
    int currentPlayer;
    int gamemode;
};

gameData dataToSave;

// Prints the main menu screen.
void printStartingScreen() {
   cout << "------------BATTLESHIPS-----------" << endl << endl
        << "                __/___            " << endl
        << "          _____/______|           " << endl
        << "  _______/_____\\_______\\_____     " << endl
        << "  \\              < < <       |    " << endl
        << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl
        << "Welcome to Battleships! Select which gamemode you" << endl << 
       "would like to play by typing out one of the following:" << endl << endl
        << "- 1P" << endl
        << "- 2P" << endl 
        << "- R (resume, only type if there's a saved game)" << endl << endl;
}

// Clears the input buffer.
void clearInput() {
    cin.clear();
    cin.ignore(cin.rdbuf()->in_avail());
}

// Clears out the console.
void clearConsole() {
    // :D
    for (size_t i = 0; i < 40; i++)
    {
        cout << endl;
    }
}

// Checks if it's possible for a number of ships to fit into a matrix.
bool shipsDontFit(int* ships, int rows, int cols) {
    int maxDimension = max(rows, cols);
    for (unsigned i = 2, spacesTakenUp = 0; i <= 5; i++)
    {
        spacesTakenUp += i * ships[i];
        if ((ships[i] != 0 && i > maxDimension) || rows * cols < spacesTakenUp)
        {
            cout << "\nThe ships don't fit, enter them again! >:(\n\n";
            return true;
        }
    }
    return false;
}

// Prints out a matrix with indicators for row/column.
void printMatrix(char** matrix, int rows, int cols) {
    cout << "     ";
    for (size_t i = 0; i < cols; i++)
    {
        cout << i << "    ";
    }
    cout << "\n";
    cout << "     ";
    for (size_t i = 0; i < cols; i++)
    {
        cout << "-    ";
    }cout << "\n";
    for (size_t i = 0; i < rows; i++)
    {
        cout << i << "|   ";
        for (size_t j = 0; j < cols; j++)
        {
            cout << matrix[i][j] << "    ";
        }
        cout << "\n\n";
    }
}

// Sets the dimensions of the grid and the amounts of each ship.
void initializeGame(int* ships, int& rows, int& cols) {
    cout << "Enter grid dimensions:";
    cin >> rows >> cols;
    clearInput();
    while (rows < 5 || cols < 5 || rows > 15 || cols > 15)
    {
        cout << "Rows/cols must be between 5 and 15! Try again.\n";
        cin >> rows >> cols;
        clearInput();
    }
    do
    {
        cout << "Enter count of carriers (5-long ships): " << endl;
        cin >> ships[5];
        clearInput();
        while (ships[5] < 0 || ships[5] * 5 > rows * cols)
        {
            cout << "Invalid count! Try again." << endl;
            cin >> ships[5];
            clearInput();
        }

        cout << "Enter count of destroyers (4-long ships): " << endl;
        cin >> ships[4];
        clearInput();
        while (ships[4] < 0 || ships[4] * 4 > rows * cols)
        {
            cout << "Invalid count! Try again." << endl;
            cin >> ships[4];
            clearInput();
        }

        cout << "Enter count of submarines (3-long ships): " << endl;
        cin >> ships[3];
        clearInput();
        while (ships[3] < 0 || ships[3] * 3 > rows * cols)
        {
            cout << "Invalid count! Try again." << endl;
            cin >> ships[3];
            clearInput();
        }

        cout << "Enter count of boats (2-long ships): " << endl;
        cin >> ships[2];
        clearInput();
        while (ships[2] < 0 || ships[2] * 2 > rows * cols)
        {
            cout << "Invalid count! Try again." << endl;
            cin >> ships[2];
            clearInput();
        }

    } while (shipsDontFit(ships, rows, cols) || (ships[2] == 0 && ships[3] == 0 && ships[4] == 0 && ships[5] == 0));
    clearConsole();
}

// Copies an array into another array.
void copyArray(int* copyFrom, int* copyTo, int length) {
    for (size_t i = 0; i < length; i++)
    {
        copyTo[i] = copyFrom[i];
    }
}

// Places a ship at the specified coordinates with the specified orientation.
char** placeShipAtCoordinates(char** grid, int row, int col, char orientation, int ship) {
    if (orientation == 'h')
    {
        for (size_t i = col; i < col + ship; i++)
        {
            grid[row][i] = ship + '0';
        }
    }
    if (orientation == 'v')
    {
        for (size_t i = row; i < row + ship; i++)
        {
            grid[i][col] = ship + '0';
        }
    }
    return grid;
}

// Checks if a free space for a ship exists at given coordinates.
bool sequenceExistsAtCoords(char** matrix, int shipLength, int rows, int cols, int row, int col, char orientation, char searched) {
    if (orientation != 'h' && orientation != 'v')
    {
        return false;
    }
    if (row < 0 || col < 0)
    {
        return false;
    }
    if (orientation == 'h')
    {
        if (col + shipLength >= cols)
        {
            return false;
        }
        for (size_t i = col; i < col + shipLength - 1; i++)
        {
            if (matrix[row][i] != searched || matrix[row][i] != matrix[row][i + 1])
            {
                return false;
            }
        }
        return true;
    }
    if (row + shipLength > rows)
    {
        return false;
    }
    for (size_t i = row; i < row + shipLength - 1; i++)
    {
        if (matrix[i][col] != searched || matrix[i][col] != matrix[i + 1][col])
        {
            return false;
        }
    }
    return true;
}

// Checks if there's a free space in the matrix that could fit a ship of the given length.
bool sequenceExists(char** matrix, int shipLength, int rows, int cols, char searched) {
    bool isSequence = true;
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j <= cols - shipLength; j++)
        {
            isSequence = true;
            for (size_t l = j; l <= j + shipLength - 1; l++)
            {
                if (matrix[i][j] != matrix[i][j + 1] || matrix[i][j] != searched)
                {
                    isSequence = false;
                    break;
                }
            }
            if (isSequence)
            {
                return true;
            }
        }
    }
    for (size_t i = 0; i < cols; i++)
    {
        for (size_t j = 0; j <= rows - shipLength; j++)
        {
            isSequence = true;
            for (size_t l = j; l <= j + shipLength - 1; l++)
            {
                if (matrix[j][i] != matrix[j + 1][i] || matrix[j][i] != searched)
                {
                    isSequence = false;
                }
            }
            if (isSequence)
            {
                return true;
            }
        }
    }
    return false;
}

// Sets all the elements in a matrix to 0.
char** clearMatrix(char** matrix, int rows, int cols) {
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            matrix[i][j] = '0';
        }
    }
    return matrix;
}

// Starts the part of the game in which ships get placed.
char** placeShips(char** grid, int* ships, int rows, int cols) {
    int copy[6], row, col;
    char orientation;
    copyArray(ships, copy, 6);
    for (size_t i = 2; i <= 5; i++)
    {
        while (copy[i] > 0)
        {
            if (!sequenceExists(grid, i, rows, cols, '0'))
            {
                i = 2;
                copyArray(ships, copy, 6);
                grid = clearMatrix(grid, rows, cols);
                cout << "Not all of the ships can fit. Try again! >:(\n\n";
                continue;
            }
            printMatrix(grid, rows, cols);
            cout << "Enter coordinates (row and col) to place a ship with a length of " << i << ", as well as an orientation (h/v): " << endl;
            cin >> row >> col >> orientation;
            clearInput();
            while (!sequenceExistsAtCoords(grid, i, rows, cols, row, col, orientation, '0'))
            {
                cout << "Invalid coordinates/orientation. Try again. (Make sure that the orientation is in lowercase): " << endl;
                cin >> row >> col >> orientation;
                clearInput();
            }
            grid = placeShipAtCoordinates(grid, row, col, orientation, i);
            copy[i]--;
            clearConsole();
        }
    }
    return grid;
}

// Checks if a player has found all the ships in their opponent's grid.
bool gameIsFinished(char** grid, char** gridGraphic, int rows, int cols) {
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            if (grid[i][j] != gridGraphic[i][j] && grid[i][j] != '0')
            {
                return false;
            }
        }
    }
    return true;
}

// Saves the game data into a text file.
void saveData(ofstream &stream) {
    char** p1 = dataToSave.p1;
    char** p2 = dataToSave.p2;
    char** p1Graphic = dataToSave.p1Graphic;
    char** p2Graphic = dataToSave.p2Graphic;
    int rows = dataToSave.rows;
    int cols = dataToSave.cols;
    int currentPlayer = dataToSave.currentPlayer;
    int gamemode = dataToSave.gamemode;
    if (!stream.is_open())
    {
        return;
    }
    stream << "SAVED\n";
    stream << rows << endl;
    stream << cols << endl;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            stream << p1[i][j] << endl;
        }
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            stream << p2[i][j] << endl;
        }
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            stream << p1Graphic[i][j] << endl;
        }
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            stream << p2Graphic[i][j] << endl;
        }
    }
    stream << currentPlayer << endl;
    stream << gamemode << endl;
    stream.close();
}

// Starts the part of the 2 player game where players choose coordinates to strike at.
void start2PGame(char** p1, char** p2, char** p1Graphic, char** p2Graphic, int rows, int cols, int currentPlayer = 1) {
    clearConsole();
    ofstream stream;
    stream.open("gamedata.txt", std::ofstream::out | std::ofstream::trunc);
    dataToSave.gamemode = 2;
    dataToSave.p1 = p1;
    dataToSave.p2 = p2;
    dataToSave.rows = rows;
    dataToSave.cols = cols;
    while (!gameIsFinished(p1, p1Graphic, rows, cols) && !gameIsFinished(p2, p2Graphic, rows, cols))
    {
        cout << "It's player " << currentPlayer << "'s turn!\n\n";
        //P1 turn
        if (currentPlayer == 1)
        {
            printMatrix(p2Graphic, rows, cols);
            cout << "Enter the coordinates of the cell you'd like to hit: ";
            int hitRow, hitCol;
            cin >> hitRow >> hitCol;
            clearInput();
            while (hitRow < 0 || hitCol < 0 || hitRow >= rows || hitCol >= cols || p2Graphic[hitRow][hitCol] != 'X')
            {
                cout << "Invalid coordinates, try again: ";
                cin >> hitRow >> hitCol;
                clearInput();
            }
            clearConsole();
            p2Graphic[hitRow][hitCol] = p2[hitRow][hitCol];
            dataToSave.p1Graphic = p1Graphic;
            dataToSave.p2Graphic = p2Graphic;
            dataToSave.currentPlayer = currentPlayer;
            saveData(stream);
            if (p2[hitRow][hitCol] == '0')
            {
                cout << "Player 1 didn't hit a ship.\n";
                currentPlayer = 2;
            }
            else
            {
                cout << "Player 1 hit a ship!\n";
            }
        }
        //P2 turn
        else
        {
            printMatrix(p1Graphic, rows, cols);
            cout << "Enter the coordinates of the cell you'd like to hit: ";
            int hitRow, hitCol;
            cin >> hitRow >> hitCol;
            clearInput();
            while (hitRow < 0 || hitCol < 0 || hitRow >= rows || hitCol >= cols || p1Graphic[hitRow][hitCol] != 'X')
            {
                cout << "Invalid coordinates, try again: ";
                cin >> hitRow >> hitCol;
                clearInput();
            }
            clearConsole();
            p1Graphic[hitRow][hitCol] = p1[hitRow][hitCol];
            dataToSave.p1Graphic = p1Graphic;
            dataToSave.p2Graphic = p2Graphic;
            dataToSave.currentPlayer = currentPlayer;
            saveData(stream);
            if (p1[hitRow][hitCol] == '0')
            {
                cout << "Player 2 didn't hit a ship.\n";
                currentPlayer = 1;
            }
            else
            {
                cout << "Player 2 hit a ship!\n";
            }
        }
    }
    if (gameIsFinished(p2, p2Graphic, rows, cols))
    {
        cout << "All of player 2's ships have been found!\n";
        printMatrix(p2, rows, cols);
        cout << "Player 1 wins!";
    }
    if (gameIsFinished(p1, p1Graphic, rows, cols))
    {
        cout << "All of player 1's ships have been found!\n";
        printMatrix(p1, rows, cols);
        cout << "Player 2 wins!";
    }
    stream.close();
;
}

// This function starts the two player mode of Battleships.
void launchTwoPlayerMode() {
    clearConsole();
    int rows, cols;
    int ships[6];
    initializeGame(ships, rows, cols);
    P1_GRID = new char* [rows];
    for (int i = 0; i < rows; ++i)
    {
        P1_GRID[i] = new char[cols + 1];
        for (size_t j = 0; j < cols; j++)
        {
            P1_GRID[i][j] = '0';
        }
    }
    P2_GRID = new char* [rows];
    for (int i = 0; i < rows; ++i)
    {
        P2_GRID[i] = new char[cols];
        for (size_t j = 0; j < cols; j++)
        {
            P2_GRID[i][j] = '0';
        }
    }
    P1_GRID_GRAPHIC = new char* [rows];
    for (int i = 0; i < rows; ++i)
    {
        P1_GRID_GRAPHIC[i] = new char[cols];
        for (size_t j = 0; j < cols; j++)
        {
            P1_GRID_GRAPHIC[i][j] = 'X';
        }
    }
    P2_GRID_GRAPHIC = new char* [rows];
    for (int i = 0; i < rows; ++i)
    {
        P2_GRID_GRAPHIC[i] = new char[cols];
        for (size_t j = 0; j < cols; j++)
        {
            P2_GRID_GRAPHIC[i][j] = 'X';
        }
    }
    cout << "Player 1, enter your ships!\n";
    P1_GRID = placeShips(P1_GRID, ships, rows, cols);
    printMatrix(P1_GRID, rows, cols);
    cout << "This is the final placement of your ships. Enter any character to confirm." << endl;
    string buffer;
    cin >> buffer;
    clearInput();
    clearConsole();
    cout << "Player 2, enter your ships!\n";
    P2_GRID = placeShips(P2_GRID, ships, rows, cols);
    printMatrix(P2_GRID, rows, cols);
    cout << "This is the final placement of your ships. Enter any character to continue." << endl;
    cin >> buffer;
    clearInput();
    clearConsole();
    start2PGame(P1_GRID, P2_GRID, P1_GRID_GRAPHIC, P2_GRID_GRAPHIC, rows, cols);
    for (int i = 0; i < rows; ++i)
    {
        delete[] P1_GRID[i]; 
        delete[] P2_GRID[i];
        delete[] P1_GRID_GRAPHIC[i];
        delete[] P2_GRID_GRAPHIC[i];
    }
    delete[] P1_GRID;
    delete[] P2_GRID;
    delete[] P1_GRID_GRAPHIC;
    delete[] P2_GRID_GRAPHIC;

}

const char ORIENTATION_OPTIONS[] = {'h', 'v'};

// Generates the CPU's grid, placing ships at random spots.
char** generateCPUGrid(char** grid, int* ships, int rows, int cols) {
    srand(time(0));
    int chosenRow = rand() % rows, chosenCol = rand() % cols;
    int copy[6];
    char orientation = ORIENTATION_OPTIONS[rand() % 2];
    copyArray(ships, copy, 6);
    for (size_t i = 2; i <= 5; i++)
    {
        while (copy[i] > 0)
        {
            if (!sequenceExists(grid, i, rows, cols, '0'))
            {
                i = 2;
                copyArray(ships, copy, 6);
                grid = clearMatrix(grid, rows, cols);
                continue;
            }
            while (!sequenceExistsAtCoords(grid, i, rows, cols, chosenRow, chosenCol, orientation, '0'))
            {                
                srand(time(0));
                chosenRow = rand() % rows, chosenCol = rand() % cols, orientation = ORIENTATION_OPTIONS[rand() % 1];
            }
            grid = placeShipAtCoordinates(grid, chosenRow, chosenCol, orientation, i);
            copy[i]--;
        }
    }
    return grid;
}

// Starts the part of the 1 player game where a player inputs coordinates to strike at, while a computer opponent
// generates them randomly. 
void start1PGame(char** p1, char** p2, char** p1Graphic, char** p2Graphic, int rows, int cols, int currentPlayer = 1) {
    clearConsole();
    int hitRow, hitCol, cpuHitRow, cpuHitCol;
    int possibleCoords;
    bool cpuHitShip = false;
    string buffer;
    ofstream stream;
    stream.open("gamedata.txt", std::ofstream::out | std::ofstream::trunc);
    dataToSave.gamemode = 1;
    dataToSave.p1 = p1;
    dataToSave.p2 = p2;
    dataToSave.rows = rows;
    dataToSave.cols = cols;
    while (!gameIsFinished(p1, p1Graphic, rows, cols) && !gameIsFinished(p2, p2Graphic, rows, cols))
    {
        //P1 turn
        if (currentPlayer == 1)
        {
            cout << "It's the player's turn!\n\n";
            printMatrix(p2Graphic, rows, cols);
            cout << "Enter the coordinates of the cell you'd like to hit: ";
            cin >> hitRow >> hitCol;
            clearInput();
            while (hitRow < 0 || hitCol < 0 || hitRow >= rows || hitCol >= cols || p2Graphic[hitRow][hitCol] != 'X')
            {
                cout << "Invalid coordinates, try again: ";
                cin >> hitRow >> hitCol;
                clearInput();
            }
            clearConsole();
            p2Graphic[hitRow][hitCol] = p2[hitRow][hitCol];
            dataToSave.p1Graphic = p1Graphic;
            dataToSave.p2Graphic = p2Graphic;
            dataToSave.currentPlayer = currentPlayer;
            saveData(stream);
            if (p2[hitRow][hitCol] == '0')
            {
                cout << "Player didn't hit a ship.\n";
                currentPlayer = 2;
            }
            else
            {
                cout << "Player hit a ship!\n";
            }
        }
        //CPU turn
        else
        {
            cout << "It's the CPU's turn! (Press enter to continue)\n";
            getline(cin, buffer);         
            if(cpuHitShip)
            {
                srand(time(0));
                bool chooseRow = rand() % 2;
                srand(time(0));
                bool usePlus = rand() % 2;
                int count = 0;
                while (hitRow < 0 || hitCol < 0 || hitRow >= rows || hitCol >= cols || p1Graphic[hitRow][hitCol] != 'X')
                {
                    if (chooseRow)
                    {
                        if (usePlus)
                        {
                            hitRow = cpuHitRow;
                            hitCol = cpuHitCol + 1;
                        }
                        else
                        {
                            hitRow = cpuHitRow;
                            hitCol = cpuHitCol - 1;
                        }
                    }
                    else
                    {
                        if (usePlus)
                        {
                            hitRow = cpuHitRow + 1;
                            hitCol = cpuHitCol;
                        }
                        else
                        {
                            hitRow = cpuHitRow + 1;
                            hitCol = cpuHitCol;
                        }
                    }
                    count++;
                    if (count == 50)
                    {
                        cpuHitShip = false;
                        break;
                    }
                }            
            }
            if (!cpuHitShip)
            {
                srand(time(0));
                hitRow = rand() % rows, hitCol = rand() % cols;
                while (hitRow < 0 || hitCol < 0 || hitRow >= rows || hitCol >= cols || p1Graphic[hitRow][hitCol] != 'X')
                {
                    srand(time(0));
                    hitRow = rand() % rows, hitCol = rand() % cols;
                }
            }
            p1Graphic[hitRow][hitCol] = p1[hitRow][hitCol];
            dataToSave.p1Graphic = p1Graphic;
            dataToSave.p2Graphic = p2Graphic;
            dataToSave.currentPlayer = currentPlayer;
            saveData(stream);
            cout << "CPU hit at the coordinates " << hitRow << ", " << hitCol << "\n";
            printMatrix(p1Graphic, rows, cols);
            if (p1[hitRow][hitCol] == '0')
            {
                cout << "CPU didn't hit a ship. (Press enter to continue)\n";
                currentPlayer = 1;
                getline(cin, buffer);
                cpuHitShip = false;
            }
            else
            {
                cout << "CPU hit a ship! (Press enter to continue)\n";
                getline(cin, buffer);
                cpuHitRow = hitRow;
                cpuHitCol = hitCol;
                cpuHitShip = true;
            }
            clearConsole();
        }
    }
    if (gameIsFinished(p2, p2Graphic, rows, cols))
    {
        cout << "All of CPU's ships have been found!\n";
        printMatrix(p2, rows, cols);
        cout << "Player wins!";
    }
    if (gameIsFinished(p1, p1Graphic, rows, cols))
    {
        cout << "All of player's ships have been found!\n";
        printMatrix(p1, rows, cols);
        cout << "CPU wins!";
    }
    stream.close();
}

// This function starts the one player mode of Battleships.
void launchOnePlayerMode() {
    clearConsole();
    int rows, cols;
    int ships[6];
    initializeGame(ships, rows, cols);
    P1_GRID = new char* [rows];
    for (int i = 0; i < rows; ++i)
    {
        P1_GRID[i] = new char[cols + 1];
        for (size_t j = 0; j < cols; j++)
        {
            P1_GRID[i][j] = '0';
        }
    }
    P2_GRID = new char* [rows];
    for (int i = 0; i < rows; ++i)
    {
        P2_GRID[i] = new char[cols];
        for (size_t j = 0; j < cols; j++)
        {
            P2_GRID[i][j] = '0';
        }
    }
    P1_GRID_GRAPHIC = new char* [rows];
    for (int i = 0; i < rows; ++i)
    {
        P1_GRID_GRAPHIC[i] = new char[cols];
        for (size_t j = 0; j < cols; j++)
        {
            P1_GRID_GRAPHIC[i][j] = 'X';
        }
    }
    P2_GRID_GRAPHIC = new char* [rows];
    for (int i = 0; i < rows; ++i)
    {
        P2_GRID_GRAPHIC[i] = new char[cols];
        for (size_t j = 0; j < cols; j++)
        {
            P2_GRID_GRAPHIC[i][j] = 'X';
        }
    }
    cout << "Player 1, enter your ships!\n";
    P1_GRID = placeShips(P1_GRID, ships, rows, cols);
    printMatrix(P1_GRID, rows, cols);
    cout << "This is the final placement of your ships. Enter any character to confirm." << endl;
    string buffer;
    cin >> buffer;
    clearInput();
    clearConsole();
    cout << "Generating the CPU's grid, please wait...";
    P2_GRID = generateCPUGrid(P2_GRID, ships, rows, cols);
    //printMatrix(P2_GRID, rows, cols);
    clearConsole();
    start1PGame(P1_GRID, P2_GRID, P1_GRID_GRAPHIC, P2_GRID_GRAPHIC, rows, cols);
    for (int i = 0; i < rows; ++i)
    {
        delete[] P1_GRID[i];
        delete[] P2_GRID[i];
        delete[] P1_GRID_GRAPHIC[i];
        delete[] P2_GRID_GRAPHIC[i];
    }
    delete[] P1_GRID;
    delete[] P2_GRID;
    delete[] P1_GRID_GRAPHIC;
    delete[] P2_GRID_GRAPHIC;
}

// Resumes a game that has been saved.
void resumeGame() {
    ifstream stream("gamedata.txt");
    if (stream.is_open())
    {
        string buffer;
        stream >> buffer;
        if (buffer != "SAVED")
        {
            cout << "There is no saved data.";
            stream.close();
            return;
        }
        int rows, cols;
        stream >> rows;
        stream >> cols;
        char** p1 = new char*[rows];
        for (size_t i = 0; i < rows; i++)
        {
            p1[i] = new char[cols];
            for (size_t j = 0; j < cols; j++)
            {
                stream >> p1[i][j];
            }
        }
        char** p2 = new char* [rows];
        for (size_t i = 0; i < cols; i++)
        {
            p2[i] = new char[cols];
            for (size_t j = 0; j < cols; j++)
            {
                stream >> p2[i][j];
            }
        }
        char** p1Graphic = new char* [rows];
        for (size_t i = 0; i < cols; i++)
        {
            p1Graphic[i] = new char[cols];
            for (size_t j = 0; j < cols; j++)
            {
                stream >> p1Graphic[i][j];
            }
        }
        char** p2Graphic = new char* [rows];
        for (size_t i = 0; i < cols; i++)
        {
            p2Graphic[i] = new char[cols];
            for (size_t j = 0; j < cols; j++)
            {
                stream >> p2Graphic[i][j];
            }
        }
        int currentPlayer;
        stream >> currentPlayer;
        int gamemode;
        stream >> gamemode;
        if (gamemode == 1)
        {
            start1PGame(p1, p2, p1Graphic, p2Graphic, rows, cols, currentPlayer);
        }
        else if (gamemode == 2)
        {
            start2PGame(p1, p2, p1Graphic, p2Graphic, rows, cols, currentPlayer);
        }
        stream.close();
        for (int i = 0; i < cols; i++)
        {
            delete[] p1[i];
        }
        delete[] p1;
        for (int i = 0; i < cols; i++)    
        {
            delete[] p2[i];
        }
        delete[] p2;
        for (int i = 0; i < cols; i++)
        {
            delete[] p1Graphic[i];
        }
        delete[] p1Graphic;
        for (int i = 0; i < cols; i++)
        {
            delete[] p2Graphic[i];
        }
        delete[] p2Graphic;
    }
    else
    {
        cout << "File reading error.";
        return;
    }
}


int main()
{
    printStartingScreen();
    string mode;
    cin >> mode;
    clearInput();
    while (mode != "1p" && mode != "2p" && mode != "1P" && mode != "2P" && mode != "r" && mode != "R")
    {
        cout << "Unsupported gamemode. Try again:" << endl;
        cin >> mode;
        clearInput();
    }
    if (mode == "2p" || mode == "2P")
    {
        launchTwoPlayerMode();
    }
    else if (mode == "1p" || mode == "1P")
    {
        launchOnePlayerMode();
    }
    if (mode == "r" || mode == "R")
    {
        resumeGame();
    }
}
