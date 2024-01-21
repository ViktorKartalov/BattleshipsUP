#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
using namespace std;

char** P1_GRID, **P2_GRID, **P1_GRID_GRAPHIC, **P2_GRID_GRAPHIC;

void printStartingScreen() {
   cout << "------------BATTLESHIPS-----------" << endl << endl
        << "                __/___            " << endl
        << "          _____/______|           " << endl
        << "  _______/_____\\_______\\_____     " << endl
        << "  \\              < < <       |    " << endl
        << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl
        << "Welcome to Battleships! Select which gamemode you" << endl << "would like to play by typing out one of the following:" << endl << endl
        << "- 1P" << endl
        << "- 2P" << endl << endl;
}

void clearInput() {
    cin.clear();
    cin.ignore(cin.rdbuf()->in_avail());
}

void clearConsole() {
    // :D
    for (size_t i = 0; i < 40; i++)
    {
        cout << endl;
    }
}

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

void printMatrix(char** matrix, int rows, int cols) {

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            cout << matrix[i][j] << "    ";
        }
        cout << "\n\n";
    }
}

void initializeGame(int* ships, int& rows, int& cols) {
    cout << "Enter grid dimensions:";
    cin >> rows >> cols;
    clearInput();
    do
    {
        cout << "Enter count of carriers (5-long ships): " << endl;
        cin >> ships[5];
        clearInput();

        cout << "Enter count of destroyers (4-long ships): " << endl;
        cin >> ships[4];
        clearInput();

        cout << "Enter count of submarines (3-long ships): " << endl;
        cin >> ships[3];
        clearInput();

        cout << "Enter count of boats (2-long ships): " << endl;
        cin >> ships[2];
        clearInput();

    } while (shipsDontFit(ships, rows, cols));
    clearConsole();
}

void copyArray(int* copyFrom, int* copyTo, int length) {
    for (size_t i = 0; i < length; i++)
    {
        copyTo[i] = copyFrom[i];
    }
}

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

bool sequenceExistsAtCoords(char** matrix, int shipLength, int row, int col, char orientation, char searched) {
    if (orientation != 'h' && orientation != 'v')
    {
        return false;
    }
    if (orientation == 'h')
    {
        for (size_t i = col; i < col + shipLength - 1; i++)
        {
            if (matrix[row][i] != searched || matrix[row][i] != matrix[row][i + 1])
            {
                return false;
            }
        }
        return true;
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
            while (!sequenceExistsAtCoords(grid, i, row, col, orientation, '0'))
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


void start2PGame(char** p1, char** p2, char** p1Graphic, char** p2Graphic, int rows, int cols) {
    clearConsole();
    int currentPlayer = 1;
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
}

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
}

const char ORIENTATION_OPTIONS[] = {'h', 'v'};

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
            while (!sequenceExistsAtCoords(grid, i, chosenRow, chosenCol, orientation, '0'))
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


void start1PGame(char** p1, char** p2, char** p1Graphic, char** p2Graphic, int rows, int cols) {
    clearConsole();
    int currentPlayer = 1, hitRow, hitCol, cpuHitRow, cpuHitCol;
    int possibleCoords;
    bool cpuHitShip = false;
    string buffer;
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
                    if (count == 20)
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
}

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

}

int main()
{
    cout << rand() % 10 << " " << rand() % 100;
    printStartingScreen();
    string mode;
    cin >> mode;
    clearInput();
    while (mode != "1p" && mode != "2p" && mode != "1P" && mode != "2P")
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
}
