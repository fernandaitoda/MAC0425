#include <iostream>
#include <stdlib.h>
#include<time.h>

using namespace std;

class JogoDaVelha 
{
    private: 
        char* board;
        char winner;
        char player, computer;
        int nAvailableMoves;

        void printPositions ();
        int* availableMoves ();
        void checkWinner ();
        void computerMove ();
        int* minimax (int lcurrentPlayer);

        int flipCoin (char turn);
        int* minimaxCoin (int currentPlayer);
        void computerMoveCoin ();

    public:
        void printBoard ();
        JogoDaVelha () {
            int move;
            board = new char[9];
            for (int i = 0; i < 9; i++)
                board[i] = ' ';            
            nAvailableMoves = 9;
            winner = 'N';
            cout << "--------JOGO DA VELHA--------\n";
            printBoard ();
            printPositions ();
            while (player != 'X' && player != 'O') {
                cout << "Deseja jogar com X (inicia) ou O? ";
                cin >> player;
            }

            if (player == 'X') {
                move = 1;
                computer = 'O';
            }
            else {
                move = 2;
                computer = 'X';
            }

            while (nAvailableMoves != 0 && winner == 'N') {
                if (move == 2) {
                    cout << "\n Movimento de " << computer << ":\n";
                    computerMove ();
                    move = 1;
                }
                else {
                    int pos;
                    cout << "Digite o movimento que deseja fazer: ";
                    cin >> pos;
                    if (board[pos-1] == ' ') {
                        board[pos-1] = player;
                        nAvailableMoves--;
                        move = 2;   
                    }
                    else 
                        cout << "Movimento inválido.\n";
                
                }
                
                printBoard ();

                checkWinner ();
                if (winner != 'N') 
                    cout << "Vitória de " << winner << "!\n";
                
                else if (nAvailableMoves == 0)
                    cout << "Empate! \n";

            }
        }

        ~JogoDaVelha () {}

        void coinVersion ();
};

void JogoDaVelha::printPositions () {
    cout << "Posições no Tabuleiro:\n\n";
    cout <<  "   1 | 2 | 3 \n"
             "  ---+---+---\n"
             "   4 | 5 | 6 \n"
             "  ---+---+---\n"
             "   7 | 8 | 9 \n\n";
}

int* JogoDaVelha::availableMoves () {
    int* moves = new int[nAvailableMoves]; 
    for (int i = 0, j = 0; i < 9; i++)
        if (board[i] == ' ') 
            moves[j++] = i;
    
    return moves;
}

void JogoDaVelha::checkWinner () {

    for (int i = 0; i < 9; i +=3) 
        if (board[i] != ' ' && board[i] == board[i+1] && board[i] == board[i+2])
            winner = board[i];
    
    for (int i = 0; i < 3; i++)
        if (board[i] != ' ' && board[i] == board[i+3] && board[i] == board[i+6])
            winner = board[i];

    if (board[4] != ' ') {
        if (board[0] == board[4] && board[0] == board[8])
            winner = board[0];
        
        if (board[2] == board[4] && board[2] == board[6])
            winner = board[2];
    }
}

void JogoDaVelha::printBoard () {
    cout << "\n";
    for (int i = 0; i < 8; i++) {
        if (i == 0 || i == 3 || i == 6) 
            cout << "  ";

        if ((i+1) % 3 == 0)
            cout << " " << board[i] << " \n  ---+---+--- \n";
        
        else
            cout << " " << board[i] << " |";
    }
    cout << " " << board[8] << " \n\n";
}

void JogoDaVelha:: computerMove () {
    int* pos;
    int options[5] = {0, 2, 4, 6, 8};
    if (nAvailableMoves == 9) {
        srand (time(NULL));
        pos = new int[2];
        pos[0] = options[rand() % 5];
    }
    else 
        pos = minimax (computer);

    board[pos[0]] = computer;
    nAvailableMoves--;
}

int* JogoDaVelha::minimax (int currentPlayer) {
    int* currentPosition, * bestPosition, * available; 
    char otherPlayer;
    
    bestPosition = new int[2]; 
    available = availableMoves ();

    if (currentPlayer == computer)
        otherPlayer = player;
    else
        otherPlayer = computer;

    // Check if last move was a winner
    checkWinner ();
    if (winner == otherPlayer) {
        if (otherPlayer == computer) 
            bestPosition[1] = 1 * (nAvailableMoves + 1);
        else 
            bestPosition[1] = (-1) * (nAvailableMoves + 1);
        
        return bestPosition;
    }
    else if (nAvailableMoves == 0) {
        bestPosition[1] = 0;
        return bestPosition;
    }
    
    if (currentPlayer == computer) 
        bestPosition[1] = -1000;
    else
        bestPosition[1] = +1000;

    // game simulation (test all possibilities)
    for (int i = 0; i < nAvailableMoves; i++) {
        board[available[i]] = currentPlayer;
        nAvailableMoves--;
        currentPosition = minimax (otherPlayer);
    
        // undo move
        board[available[i]] = ' ';
        nAvailableMoves++;
        winner = 'N';
        currentPosition[0] = available[i];

        if (currentPlayer == computer) {

            if (currentPosition[1] > bestPosition[1])
                bestPosition = currentPosition;
        }
        else {

            if (currentPosition[1] < bestPosition[1])
                bestPosition = currentPosition; 
        }
    }

    return bestPosition;
}

int JogoDaVelha::flipCoin (char turn) {
    srand(time(NULL));
    // 0 para cara e 1 para coroa
    cout << ">>> Girando a moeda: \n";
        int coin = rand () % 2;
        if (coin == 0) {
            if (turn == computer) {
                cout << "Cara! Vez de " << player << "\n";
                return player;        
            }
            else {
                cout << "Cara! Vez de " << computer << "\n";
                return computer;
            }
        }
        else{
            cout << "Coroa! Jogue novamente\n";
            return turn;
        }
}

void JogoDaVelha:: computerMoveCoin () {
    int* pos;
    int options[5] = {0, 2, 4, 6, 8};
    if (nAvailableMoves == 9) {
        srand (time(NULL));
        pos = new int[2];
        pos[0] = options[rand() % 5];
    }
    else 
        pos = minimaxCoin (computer);

    board[pos[0]] = computer;
    nAvailableMoves--;
}

int* JogoDaVelha::minimaxCoin (int currentPlayer) {
    int* currentPosition, *currentPosition2, * bestPosition, * available; 
    int sum[9];
    char otherPlayer;
    
    bestPosition = new int[3]; 
    available = availableMoves ();

    if (currentPlayer == computer)
        otherPlayer = player;
    else
        otherPlayer = computer;

    // Check if last move was a winner
    checkWinner ();
    if (winner == otherPlayer) {
        if (otherPlayer == computer) 
            bestPosition[1] = 1 * (nAvailableMoves + 1);
        else 
            bestPosition[1] = (-1) * (nAvailableMoves + 1);
        
        return bestPosition;
    }
    else if (nAvailableMoves == 0) {
        bestPosition[1] = 0;
        return bestPosition;
    }
    
    if (currentPlayer == computer) 
        bestPosition[1] = -1000;
    else
        bestPosition[1] = +1000;

    // game simulation (test all possibilities)
    for (int i = 0; i < nAvailableMoves; i++) {
        board[available[i]] = currentPlayer;
        nAvailableMoves--;
        currentPosition = minimaxCoin (currentPlayer);
        currentPosition2 = minimax (otherPlayer);
    
        sum[i] = currentPosition[1] + currentPosition2[1];
 
        // undo move
        board[available[i]] = ' ';
        nAvailableMoves++;
        winner = 'N';
        currentPosition[0] = available[i];

        if (currentPlayer == computer) {

            if (currentPosition[1] > bestPosition[1])
                bestPosition = currentPosition;
        }
        else {

            if (currentPosition[1] < bestPosition[1])
                bestPosition = currentPosition; 
        }
    }

    return bestPosition;
}

void JogoDaVelha::coinVersion () {
    int move;
    board = new char[9];
    for (int i = 0; i < 9; i++)
        board[i] = ' ';            
    nAvailableMoves = 9;
    winner = 'N';
    cout << "--------JOGO DA VELHA: Versão com Moeda --------\n";
    printBoard ();
    printPositions ();
    while (player != 'X' && player != 'O') {
        cout << "Deseja jogar com X (inicia) ou O? ";
        cin >> player;
    }

    if (player == 'X') {
        move = 1;
        computer = 'O';
    }
    else {
        move = 2;
        computer = 'X';
    }

    while (nAvailableMoves != 0 && winner == 'N') {
        if (move == 2) {
            cout << "\n Movimento de " << computer << ":\n";
            computerMove ();
            move = flipCoin (move);
        }
        else {
            int pos;
            cout << "Digite o movimento que deseja fazer: ";
            cin >> pos;
            if (board[pos-1] == ' ') {
                board[pos-1] = player;
                nAvailableMoves--;
                move = flipCoin (move);  
            }
            else 
                cout << "Movimento inválido.\n";
        
        }
        
        printBoard (); 

        checkWinner ();
        if (winner != 'N') 
            cout << "Vitória de " << winner << "!\n";
        
        else if (nAvailableMoves == 0)
            cout << "Empate! \n";

    }
}

int main () {
    JogoDaVelha ();

    return 0;
}