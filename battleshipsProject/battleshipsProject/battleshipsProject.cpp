#include <iostream>
#include <cstring>
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
    P1_GRID = new char* [rows];
    do
    {
        cout << "Enter count of carriers (5-long ships): " << endl;
        cin >> ships[5];

        cout << "Enter count of destroyers (4-long ships): " << endl;
        cin >> ships[4];

        cout << "Enter count of submarines (3-long ships): " << endl;
        cin >> ships[3];

        cout << "Enter count of boats (2-long ships): " << endl;
        cin >> ships[2];

    } while (shipsDontFit(ships, rows, cols));
    clearConsole();
}

void copyArray(int* copyFrom, int* copyTo, int length) {
    for (size_t i = 0; i < length; i++)
    {
        copyTo[i] = copyFrom[i];
    }
}

char** placeShipAtCoordinates(char** grid, int x, int y, char orientation) {
    return grid;
}

bool sequenceExists(char** matrix, int shipLength, int rows, int cols, char searched) {

}

char** placeShips(char** grid, int* ships, int rows, int cols) {
    int copy[6], x, y;
    char orientation;
    copyArray(ships, copy, 6);
    printMatrix(grid, rows, cols);
    for (size_t i = 2; i <= 5; i++)
    {
        while (copy[i] > 0)
        {
            cout << "Enter coordinates to place a ship with a length of " << i << ", as well as an orientation (h/v): " << endl;
            cin >> x >> y >> orientation;
            while (placeShipAtCoordinates(grid, x, y, orientation))
            {
                cout << "Invalid coordinates/orientation. Try again. (Make sure that the orientation is in lowercase): " << endl;
                cin >> x >> y >> orientation;
            }
            copy[i]--;
        }
    }
    return grid;
}

void launchTwoPlayerMode() {
    clearConsole();
    int rows, cols;
    int ships[6];
    initializeGame(ships, rows, cols);
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
            P1_GRID_GRAPHIC[i][j] = '0';
        }
    }
    P2_GRID_GRAPHIC = new char* [rows];
    for (int i = 0; i < rows; ++i)
    {
        P2_GRID_GRAPHIC[i] = new char[cols];
        for (size_t j = 0; j < cols; j++)
        {
            P2_GRID_GRAPHIC[i][j] = '0';
        }
    }
    placeShips(P1_GRID, ships, rows, cols);
}

void launchOnePlayerMode() {
    clearConsole();
    // TBI
}

int main()
{
    printStartingScreen();
    string mode;
    cin >> mode;
    while (mode != "1p" && mode != "2p" && mode != "1P" && mode != "2P")
    {
        cout << "Unsupported gamemode. Try again:" << endl;
        cin >> mode;
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
