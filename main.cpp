#include <iostream> //for inut Output
#include <cstdlib> // for rand function random number generation
#include <ctime> // for time function
#include <conio.h> // For _kbhit() and _getch()
#include <fstream> // for File handling
#include <windows.h> // For SetConsoleCursorPosition

using namespace std;

//Global Variables

const int sizeofmaze = 25;// size of the maze for 2-D Array
char maze[sizeofmaze][sizeofmaze];// 2-D Array for the maze
char player = 'v';// Pac-mans Symbol

int player_X = 19, player_Y = 12;
int ghost1_X = 4, ghost1_Y = 6;
int ghost2_X = 20, ghost2_Y = 6;
int ghost3_X = 4, ghost3_Y = 18;
int powerpellet_X = -1, powerpellet_Y = -1;  // Coordinates of the power pellet
int score = 0;  // global variable for score van be used anywhere

unsigned long long powerpellet_timer = 0;  // Timer to track 6 seconds of ghost-eating ability
unsigned long long highestScore;// for highest score

bool eat_ghosts = false;  // Whether Pac-Man can eat ghosts
bool valid_move = false; // check for a valid move
bool pellets_collected = false;// bool statement for pellets collection


//  Functions Prototype
void set_cursorposition(int x, int y);//  it is used to set the cursor to specific position for the output
void read_highestscore();//  it will read the highest score
void save_highestscore();//  it will save the highest score
void update_highestscore();//  it will update the highest score
void new_highscoremsg();//  it will display a msg for highest score 
void display_highestscore();//  it will display the highest score
void display_score(); //  it will display the runtime score
void display_powerpellet_timer();//8 it will display the power pellet timer
void placing_powerpellet();//  it will place a power pallet at a random position in the maze
void initialize_maze();//  it will initialize the design of maze for the later display
void displayMaze(); // it will display the maze after intialization
void pacman_movement(char move);//  it will be used to move the pac-man
void ghost_movement(int& ghost_X, int& ghost_Y);//  it used for the ghost continuous movement
bool check_collisions();//  it will check whether their is a collision between pac-man and ghost or wall or obstacle.
bool check_pellets_collection(); //  it will check that if all the pellets are collected to display you won the game 
void Menu(); //  it will display the menu
void gameplay_instructions();
void rungame(); //  this function will run the overall game function and then will be called in main function (because it hace a recursive condition


// Function to set the cursor position in the console

void set_cursorposition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, position);

}

//Functions for highest scores

// Function to read the highest score from a file
void read_highestscore() {
    ifstream infile("highscore.txt"); // Open file for reading highscore
    if (infile.is_open()) {
        infile >> highestScore; // Reading the highest score
        infile.close();
    }

}

// Function to save the highest score to a file
void save_highestscore() {
    ofstream outfile("highscore.txt"); // Open file for writing
    if (outfile.is_open()) {
        outfile << highestScore; // Writing the highest score to the file
        outfile.close();
    }
}

// Function to check and update the highest score
void update_highestscore() {
    if (score > highestScore) {

        highestScore = score; // Update highest score
        save_highestscore();   // Save the new highest score

    }

}
void new_highscoremsg() {
    set_cursorposition(0, sizeofmaze + 3);//for highs score msg
    if (score > highestScore) {
        cout << "Congratulation! you have created a new High Score : " << score << endl;
    }
}
void display_highestscore() {
    set_cursorposition(0, sizeofmaze + 2); // Position below the maze
    cout << "Highest Score: " << highestScore << endl;
}

// Display the score
void display_score() {
    set_cursorposition(0, sizeofmaze + 1); // Position below the maze
    cout << "Score: " << score << "    ";
}

// It will display the power pellet timer
void display_powerpellet_timer() {

    set_cursorposition(0, sizeofmaze + 3); // Positioning the cursor below the Highscore

    if (eat_ghosts) {
        unsigned long long time_elapse = time(0) - powerpellet_timer;

        int remaining_time = 6 - time_elapse; // power pellet effect will last for 6 seconds

        if (remaining_time > 0) {
            cout << "Power Pellet Timer: " << remaining_time << " seconds    ";
        }
        else {
            cout << "Power Pellet Timer: 0 seconds    ";
        }
    }
    else {
        cout << "Power Pellet Timer: 0 seconds    ";
    }
}

// Place the power pellet on an empty spot in the maze
void placing_powerpellet() {
    srand(time(0));
    int x, y;
    do {
        x = rand() % sizeofmaze;
        y = rand() % sizeofmaze;
    } while (maze[x][y] != '.'); // Ensure the spot is empty
    maze[x][y] = 'P';
    powerpellet_X = x;
    powerpellet_Y = y;
}

// Initialize the maze structure with obstacles
void initialize_maze() {
    for (int i = 0; i < sizeofmaze; i++) {
        for (int j = 0; j < sizeofmaze; j++) {
            if (i == 0 || j == 0 || i == sizeofmaze - 1 || j == sizeofmaze - 1)
                maze[i][j] = '#'; // Boundary walls
            else if (((2 < i) && (i < 5) && (2 < j) && (j < 9)) ||
                ((2 < i) && (i < 5) && (15 < j) && (j < 22)) ||
                ((19 < i) && (i < 22) && (2 < j) && (j < 7)) ||
                ((19 < i) && (i < 22) && (17 < j) && (j < 22)) ||
                (i == 8 && (7 < j) && (j < 17)) ||
                ((7 < i) && (i < 14) && (j == 12)) ||
                ((7 < i) && (i < 17) && (j == 4)) ||
                (i == 12 && (3 < j) && (j < 10)) ||
                ((7 < i) && (i < 17) && (j == 20)) ||
                (i == 12 && (14 < j) && (j < 21)) ||
                (i == 16 && (7 < j) && (j < 11)) ||
                (i == 16 && (13 < j) && (j < 17)) ||
                ((16 < i) && (i < 21) && (j == 8)) ||
                ((16 < i) && (i < 21) && (j == 16)) ||
                (i == 21 && (7 < j) && (j < 17))) {
                maze[i][j] = '#'; // Obstacles
            }
            else {
                maze[i][j] = '.'; // Pellets
            }
        }
    }
    placing_powerpellet();
}

// Display the maze
void displayMaze() {
    set_cursorposition(0, 0); // Resetting the cursor to the top-left
    maze[player_X][player_Y] = player;//placing the player on maze

    //maze placing
    for (int i = 0; i < sizeofmaze; i++) {
        for (int j = 0; j < sizeofmaze; j++) {
            cout << maze[i][j] << " ";
        }
        cout << endl;
    }
}

// Update specific positions in the maze
void position_update(int x, int y, char symbol) {
    set_cursorposition(x * 2, y); // Adjust for console spacing
    cout << symbol;
}

// Handle player movement or pac-mans movement

void pacman_movement(char movement) {
    maze[player_X][player_Y] = ' '; // it will clear the pac-mans current position
    position_update(player_Y, player_X, ' ');

    switch (movement) {
    case 'w':
        player = 'v';
        if (maze[player_X - 1][player_Y] != '#') {

            player_X--;
        }
        break;
    case 'a':
        player = '>';
        if (maze[player_X][player_Y - 1] != '#') {
            player_Y--;
        }
        break;
    case 'd':
        player = '<';
        if (maze[player_X][player_Y + 1] != '#') {
            player_Y++;
        }
        break;
    case 's':
        player = '^';
        if (maze[player_X + 1][player_Y] != '#') {
            player_X++;
        }
        break;
    }

    if (maze[player_X][player_Y] == '.') {
        score += 10; // Collect pellet
    }
    else if (maze[player_X][player_Y] == 'P') {
        score += 100; // Collect power pellet
        eat_ghosts = true;
        powerpellet_timer = time(0); // Start power timer
    }

    maze[player_X][player_Y] = player; // Update position
    position_update(player_Y, player_X, player);
}
void ghost_movement(int& ghost_X, int& ghost_Y) {

    // it will tracks the tile content where the ghost was located previously
    static char ghostprevioustile[sizeofmaze][sizeofmaze];

    int previous_X = ghost_X, previous_Y = ghost_Y; // Save the ghost's previous position

    // Initialize the previous tile content for the starting position of the ghost
    if (ghostprevioustile[previous_Y][previous_X] == '\0')
    {
        ghostprevioustile[previous_Y][previous_X] = '.'; // Assume the ghost starts on a pellet
    }

    // Randomly decide the direction to move

    int direction = rand() % 5;

    // Try to move in the chosen direction, ensuring the ghost doesn't walk through walls or other ghosts
    switch (direction) {
    case 0: // Move up
        if (maze[ghost_Y - 1][ghost_X] != '#' && maze[ghost_Y - 1][ghost_X] != '8') {
            ghost_Y--;
        }
        break;
    case 1: // Move left
        if (maze[ghost_Y][ghost_X - 1] != '#' && maze[ghost_Y][ghost_X - 1] != '8') {
            ghost_X--;
        }
        break;
    case 2: // Move right
        if (maze[ghost_Y][ghost_X + 1] != '#' && maze[ghost_Y][ghost_X + 1] != '8') {
            ghost_X++;
        }
        break;
    case 3: // Move down
        if (maze[ghost_Y + 1][ghost_X] != '#' && maze[ghost_Y + 1][ghost_X] != '8') {
            ghost_Y++;
        }
        break;
    }

    // It will restore the content of Tile's previous position
    maze[previous_Y][previous_X] = ghostprevioustile[previous_Y][previous_X];
    position_update(previous_X, previous_Y, ghostprevioustile[previous_Y][previous_X]); // It will Update the maze display for restored tile

    // Save the content of the new position before placing the ghost
    ghostprevioustile[ghost_Y][ghost_X] = maze[ghost_Y][ghost_X];

    // Place the ghost in its new position
    maze[ghost_Y][ghost_X] = '8';
    position_update(ghost_X, ghost_Y, '8'); // Update the maze display for the ghost's new position
}

// Check for collisions between player and ghosts
bool check_collisions() {
    if (eat_ghosts) {
        // If Pac-Man can eat ghosts, they get eaten
        if ((player_X == ghost1_Y && player_Y == ghost1_X) ||
            (player_X == ghost2_Y && player_Y == ghost2_X) ||
            (player_X == ghost3_Y && player_Y == ghost3_X)) {
            score += 200; // Add extra points for eating a ghost

            return false; // No collision
        }
    }
    else {
        // Regular collision detection
        return (player_X == ghost1_Y && player_Y == ghost1_X) ||
            (player_X == ghost2_Y && player_Y == ghost2_X) ||
            (player_X == ghost3_Y && player_Y == ghost3_X);
    }
    return false;
}


// Check if all pellets are collected
bool check_pellets_collection() {
    for (int i = 0; i < sizeofmaze; i++) {
        for (int j = 0; j < sizeofmaze; j++) {
            if (maze[i][j] == '.') return false;
        }
    }
    return true;
}


// Display the menu


void Menu() {
    bool valid_input = false; // checking if the input is valid
    int choice;

    cout << "\n--- PAC-MAN GAME ---\n";
    cout << "1. Start Game\n";
    cout << "2. Exit\n";

    while (!valid_input) {
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            // Clear the error flag and ignore the invalid input
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a numeric value.\n";
        }
        else if (choice == 1) {
            valid_input = true;
            cout << "Starting Game...\n";
        }
        else if (choice == 2) {
            cout << "Exiting Game. Thank you for Playing!" << endl;
            valid_input = true;
            exit(0);
        }
        else {
            cout << "Invalid Choice! Enter Choice Again.\n";
        }
    }
}



void gameplay_instructions() {
    set_cursorposition(50, 0);
    cout << "   Game-Play Instructions" << endl;
    set_cursorposition(50, 1);
    cout << " --------------------------" << endl;
    set_cursorposition(50, 2);
    cout << "| Pac-Man Movement:        |" << endl;
    set_cursorposition(50, 3);
    cout << "| (w,a,s,d)                |" << endl;
    set_cursorposition(50, 4);
    cout << "| (up, left, down, right)  |" << endl;
    set_cursorposition(50, 5);
    cout << "| Eating Pellet ( . ):     |" << endl;
    set_cursorposition(50, 6);
    cout << "| = 10+ points             |" << endl;
    set_cursorposition(50, 7);
    cout << "| Eating PowerPellets (P): |" << endl;
    set_cursorposition(50, 8);
    cout << "| = 100+ points            |\n" << endl;
    set_cursorposition(50, 9);
    cout << "| Eating Ghost (8):        |\n" << endl;
    set_cursorposition(50, 10);
    cout << "| = 200+ points            |\n" << endl;
    set_cursorposition(50, 11);
    cout << " --------------------------" << endl;

}

// Main game loop
void rungame() {
    bool exit_game = false;

    while (!exit_game) {
        // Display menu
        Menu();

        // Reset the game state
        score = 0;
        player_Y = 12;         // Reset pac-man position
        player_X = 19;
        ghost1_X = 4;          // Reset ghosts positions
        ghost1_Y = 6;
        ghost2_X = 20;
        ghost2_Y = 6;
        ghost3_X = 4;
        ghost3_Y = 18;
        eat_ghosts = false;    // Reset power pellet effect
        powerpellet_timer = 0; // Reset power pellet timer

        // Reset maze and game over msg
        initialize_maze();  // Reset maze
        bool game_over = false;  // this ensures that game over is reset before starting the a new game

        gameplay_instructions();
        displayMaze(); // Initial draw of the maze
        display_highestscore();
        unsigned long long powerpellet_start_time = 0; // Start time for the power pellet
        bool power_activation = false; // for power pellet power effect

        while (!game_over) {
            display_score(); // Update the score display
            display_powerpellet_timer(); // Display the power pellet timer

            // it will handle power pellet timer and ghost-eating ability
            if (eat_ghosts && (time(0) - powerpellet_timer >= 6)) {
                eat_ghosts = false; // Disable ghost-eating ability

            }


            // Check for collision with ghosts
            if (check_collisions()) {
                set_cursorposition(0, sizeofmaze);
                cout << "You were caught by a ghost! Game Over!\n";
                game_over = true; // End the game
                break;
            }

            // Check if all pellets are collected
            if (check_pellets_collection()) {
                cout << "All pellets collected! You win!\n";
                game_over = true; // End the game
                break;
            }

            // Handle player input
            if (_kbhit()) {
                char player_move = _getch();
                pacman_movement(player_move); // Handle player movement
            }

            // Move ghosts
            ghost_movement(ghost1_X, ghost1_Y);
            ghost_movement(ghost2_X, ghost2_Y);
            ghost_movement(ghost3_X, ghost3_Y);






            Sleep(100);  // Add delay for smoother gameplay
        }

        new_highscoremsg();
        update_highestscore();  // Check and update the highest score

        // Ask if the player wants to return to the main menu or exit
        char replayChoice;
        bool validInput = false; // Flag to track valid input
        set_cursorposition(0, sizeofmaze + 4);
        while (!validInput) { // Loop until valid input is given

            cout << "\nDo you want to return to the main menu? (y/n): ";
            cin >> replayChoice;

            // Clear invalid input if needed
            if (cin.fail() || cin.peek() != '\n') {
                cin.clear(); // Clear the error flag
                cin.ignore(10000, '\n'); // it will remove invalid input
                cout << "Invalid input! Please enter 'y' for Yes or 'n' for No." << endl;
                continue; // Restart the loop
            }

            if (replayChoice == 'y' || replayChoice == 'Y') {
                system("cls");
                cout << "Restarting the Game......" << endl;
                rungame(); // It will restart the game
                validInput = true; // Break the loop
            }
            else if (replayChoice == 'n' || replayChoice == 'N') {
                cout << "Exiting game..." << endl;
                exit_game = true; // Exit the game loop
                validInput = true; // Break the loop
            }
            else {
                cout << "Invalid choice! Please enter 'y' for Yes or 'n' for No." << endl;
            }
        }


    }



}


//int main for main game loop
int main() {
    read_highestscore();

    rungame();
    return 0;
}
