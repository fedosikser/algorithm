#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

void StartGame() {

    cout << "Let's play a game.\n";
    cout << "I'm thinking of a number.\n";
    cout << "You have 7 tries to guess what it is.\n";
    cout << "Enter your guess: \n";
    int guess=-1;
    srand(time(0));
    int number=rand()%100;
    int k=1;
    while (guess != number && k<=7) {
        k++;
        cin >> guess;
        cout << "Your guess " << guess;
        if (guess > number) cout << " is bigger than the number.\n";
        else if (guess < number) cout << " is smaller than the number.\n";
        else {
            cout << " is Correct!\n You win! Congratulations))\n";
        }
    }
    if (k==8) cout << " You lose!\n The number was " << number << endl;

}

void CallBack(int& choise, int value) {
    while ( !(choise>0 && choise<=value)) {
        cout << "Invalid choise. Repit" << endl;
        cin >> choise;
    }
}
void GameMenu() {
    cout << "Welcome to the game!" << endl;
    cout << "1. Start game" << endl;
    cout << "2. Exit" << endl;
    int choise{};
    cin >> choise;
    CallBack(choise, 2);
    switch(choise) {
        case 1:
            StartGame();
            GameMenu();
            break;
        case 2:
            cout << "Exiting game..." << endl;
            break;
    }
    
    
}



int main() {

    GameMenu();

    return 0;
}