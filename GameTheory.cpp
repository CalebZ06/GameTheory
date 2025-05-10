#include <iostream>
#include <cstdlib>
#include <ctime>

class Player {
    public:
        int score = 0; // Initialize score to 0
        int PrvEnemyMove = 1; // Initialize PreviousMove to -1
        int Round = 0; // Initialize Round to 0

        virtual int ReturnMove() = 0; // Pure virtual function for polymorphism
        virtual ~Player() {} // Virtual destructor for proper cleanup
};

class Random : public Player {
    public:
        int ReturnMove() {
            return rand() % 2; // Randomly returns 0 or 1
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

class GameTheory {
    public:
        int Round = 0;
        int maxRounds = 5; // Maximum number of rounds
        int** score;
        Player* player1;
        Player* player2;

        GameTheory(Player* Player1, Player* Player2, int MaxRounds = 10) {
            player1 = Player1;
            player2 = Player2;
            maxRounds = MaxRounds; // Set maximum rounds

            score = new int*[2];
            for (int i = 0; i < 2; i++) {
                score[i] = new int[2];
            }
        }

        ~GameTheory() {
            for (int i = 0; i < 2; i++) {
                delete[] score[i];
            }
            delete[] score;
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
    TitForTat player2;

    GameTheory game(&player1, &player2, 40);

    game.PlayGame(); // Start the game
    std::cout << "Game Over!" << std::endl;

    return 0;
}