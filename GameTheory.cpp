#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

class Player {
    public:
        int score = 0; // Initialize score to 0
        int EnemyScore = 0; // Initialize EnemyScore to 0
        int PrvEnemyMove = 1; // Initialize PreviousMove to -1
        int Round = 0; // Initialize Round to 0

        virtual int ReturnMove() = 0; // Pure virtual function for polymorphism
        virtual ~Player() {} // Virtual destructor for proper cleanup
};

class Random : public Player {
    public:
        int ReturnMove() {
            static std::random_device rd; // Seed for random number generation
            static std::mt19937 gen(rd()); // Mersenne Twister random number generator
            static std::uniform_int_distribution<> dist(0, 1); // Uniform distribution between 0 and 1
            return dist(gen); // Generate a random number (0 or 1)
        }
};

class AlwaysCooperate : public Player {
    public:
        int ReturnMove() {
            return 1; // Always returns 1 (Cooperate)
        }
};

class AlwaysDefect : public Player {
    public:
        int ReturnMove() {
            return 0; // Always returns 0 (Defect)
        }
};

class TitForTat : public Player {
    public:
        int ReturnMove() {
            return PrvEnemyMove; // Returns the previous move of the opponent
        }
};
class DefectWhenLosing : public Player {
    public :
        int ReturnMove() {
            if (EnemyScore > score) {
                return 0;
            } else {
                return 1;
            }
        }
};

class GameTheory {
    public:
        int Round = 0;
        int maxRounds = 5; // Maximum number of rounds
        int** scoreTable; // Score table for different strategies
        Player* player1;
        Player* player2;

        GameTheory(Player* Player1, Player* Player2, int MaxRounds = 10) {
            player1 = Player1;
            player2 = Player2;
            maxRounds = MaxRounds; // Set maximum rounds

            scoreTable = new int*[2];
            for (int i = 0; i < 2; i++) {
                scoreTable[i] = new int[maxRounds]; // Allocate memory for scores
                for (int j = 0; j < maxRounds; j++) {
                    scoreTable[i][j] = 0; // Initialize scores to 0
                    // Note: The size of each row is determined by the variable 'maxRounds'.
                }
            }
        }

        ~GameTheory() {
            for (int i = 0; i < 2; i++) {
                delete[] scoreTable[i];
            }
            delete[] scoreTable;
        }

        void DefectWin() {
            player1->score += 5;
            player2->score += 0;

            std::cout << typeid(*player1).name() << " Defects and wins!" << std::endl;
            PrintScores(); 
        }

        void DefectLose() {
            player1->score += 0;
            player2->score += 5;

            std::cout << typeid(*player2).name() << " Defects and wins!" << std::endl;
            PrintScores();
        }

        void Cooperate() {
            player1->score += 3;
            player2->score += 3;

            std::cout << typeid(*player1).name() << " and " << typeid(*player2).name() << " cooperate!" << std::endl;
            PrintScores();
        }

        void DefectBoth() {
            player1->score += 1;
            player2->score += 1;

            std::cout << typeid(*player1).name() << " and " << typeid(*player2).name() << " both defect!" << std::endl;
            PrintScores();
        }

        void PrintScores() {
            std::cout << " Player 1 Score: " << player1->score << std::endl;
            std::cout << " Player 2 Score: " << player2->score << std::endl;
        }

        void PrintScoreTable() {
            std::cout << "Score Table:" << std::endl;
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < maxRounds; j++) {
                    std::cout << scoreTable[i][j] << " ";
                }
                std::cout << std::endl;
            }
        }

        void PlayRound() {
            int Move1 = player1->ReturnMove();
            int Move2 = player2->ReturnMove();

            player1->PrvEnemyMove = Move2; // Update previous enemy move for player1
            player2->PrvEnemyMove = Move1; // Update previous enemy move for player2

            if (Move1 == 0 && Move2 == 0) {
                DefectBoth();
            } else if (Move1 == 1 && Move2 == 1) {
                Cooperate();
            } else if (Move1 == 0 && Move2 == 1) {
                DefectWin();
            } else if (Move1 == 1 && Move2 == 0) {
                DefectLose();
            }

            player1->EnemyScore = player2->score; // Update enemy score for player1
            player2->EnemyScore = player1->score; // Update enemy score for player2

            scoreTable[0][Round] = Move1; // Store scores in the score table
            scoreTable[1][Round] = Move2;
            Round++;

        }
        void PlayGame() {
            while (Round < maxRounds) { // Play 10 rounds
                PlayRound();
                Round++;
            }
            if (player1->score > player2->score) {
                std::cout << typeid(*player1).name() << " wins!" << std::endl;
            } else if (player1->score < player2->score) {
                std::cout << typeid(*player2).name() << " wins!" << std::endl;
            } else {
                std::cout << "It's a tie!" << std::endl;
            }
        }
};

int main() {
    srand(time(0)); // Seed for random number generation

    Random player1;
    Random player2;

    GameTheory game(&player1, &player2, 10);

    game.PlayGame(); // Start the game
    game.PrintScores(); // Print final scores
    game.PrintScoreTable(); // Print score table
    std::cout << "Game Over!" << std::endl;

    return 0;
}