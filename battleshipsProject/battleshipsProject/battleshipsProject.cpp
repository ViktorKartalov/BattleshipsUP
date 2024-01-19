#include <iostream>
#include <cstring>
using namespace std;

char** P1_GRID, P2_GRID;

void printStartingScreen() {
    cout << "       ----BATTLESHIPS----" << endl << endl
        << "                __/___            " << endl
        << "          _____/______|           " << endl
        << "  _______/_____\\_______\\_____     " << endl
        << "  \\              < < <       |    " << endl
        << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl
        << "Welcome to Battleships! Select which gamemode you would like to play by typing out one of the following:" << endl << endl
        << "- 1P" << endl
        << "- 2P" << endl << endl;
}

void clearConsole() {
    // :D
    for (size_t i = 0; i < 20; i++)
    {
        cout << endl;
    }
}

string toLower(string &str) {
    string result = "";
    for (char c : str)
    {
        if (c >= 'A' && c <= 'Z') {
            // Convert to lowercase by adding the ASCII offset
            result += c + ('a' - 'A');
        }
        else
        {
            result += c;
        }
    }
    return result;
}


void launchTwoPlayerMode() {
    clearConsole();
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
    mode = toLower(mode);
    while (mode != "1p" && mode != "2p")
    {
        cout << "Unsupported gamemode. Try again:" << endl;
        cin >> mode;
    }
    if (mode == "2p")
    {
        launchTwoPlayerMode();
    }
    else if (mode == "1p")
    {
        launchOnePlayerMode();
    }
}
