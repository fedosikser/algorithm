#include <iostream>
#include <algorithm>
#include <ctime>
#include <string>
#include <cstdlib>
#include <cmath>
#include <fstream>  
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

enum suit {Hearts, Diamonds, Clubs, Spades};

struct card {
    bool InDesk;
    int value;
    string str;
    string icon;
    string suit;
    int count;
} StrCard[52];

struct player {
    card* Cards;
    int n;
    double balance;
    int rate;
    int ace{};
    string name;
} User, Diller;

map <string,double> users;

void Safe_exit() {
    users[User.name] = User.balance;
    fstream fout("users.txt");
    for (auto it = users.begin(); it != users.end(); it++) {
        fout << it->first << " " << it->second << endl;
    }
    fout.close();
}

void RequestHandler(bool& answer, int type) {
    cin.ignore(32767, '\n');
    string StrAns;
    cin >> StrAns;
    if (!(StrAns == "0" || StrAns == "1")) {
        cout << "\nInvalid input. Please enter 0 or 1.\n";
        RequestHandler(answer, type);
    } else answer = (StrAns == "1") ? 1 : 0;
}

//Polymorphism 
void RequestHandler(int& answer, int type) {
    string StrAns;
    cin >> StrAns;
    if (StrAns.size() != 1) {
        for (char& c : StrAns) {
            if (!(c >= '0' && c <= '9') || StrAns[0]=='0') {
                cout << "\nInvalid input. Please try again.\n";
                RequestHandler(answer, type);
            } else answer = stoi(StrAns);
        }
    }
    if (!(StrAns >= "0" && StrAns <= "9")) {
        cout << "\nInvalid input. Try again.\n";
        RequestHandler(answer, type);
    } else {
        int _answer = stoi(StrAns);
        if (_answer <= type)  answer = _answer;
        else {
            cout << "\nInvalid input. Try again.\n";
            RequestHandler(answer, type);
        }
    }
}
card NumberToCard(int num) {
    if (StrCard[num].InDesk == 1) return NumberToCard(rand() % 52);
    else StrCard[num].InDesk = 1;
    return StrCard[num];
}

card SetRandomCard() {
    srand(time(0));
    return NumberToCard(rand() % 52);
}

void ValueToString(card& c) {

    if (c.value >=2 && c.value <= 10)  c.str = to_string(c.value);
    else if (c.value == 11) c.str = "J";
    else if (c.value == 12) c.str = "Q";
    else if (c.value == 13) c.str = "K";
    else c.str = "A";

}
void SetDeskCard(card* c) {
    int k{};
    for (int j=0; j<4; j++) {
        for (int i = 2; i <= 14; i++) {
            c[k].value = i;
            
            //c[k].suit = (j == 1) ? "Hearts♥" : (j == 2) ? "Diamonds♦" : (j == 3) ? "Clubs♣" : "Spades♠)";
            c[k].value=i;
            switch(j) {
                case Hearts: c[k].suit = "H"; c[k].icon = "♥"; break;
                case Diamonds: c[k].suit = "D"; c[k].icon = "♦"; break;
                case Clubs: c[k].suit = "C"; c[k].icon = "♣"; break;
                case Spades: c[k].suit = "S"; c[k].icon = "♠"; break;
            }
            c[k].InDesk = 0;
            c[k].count = min(i,10);
            if (i == 14) c[k].count = 11;
            ValueToString(c[k]);
            k++;
        } 
    }
}

void PrintCard(card c,bool sign) {
    sign ? cout << "\nYour Card is: " : cout << "";
    cout << c.str;
    //cout << c.suit;
    cout << c.icon << " ";
}

void PrintUserCard(player& _User, int n, bool sign) {
    for (int i = 0; i < n; i++) {
        PrintCard(_User.Cards[i],sign);
    }
}

void PrintDeskCard(card* c) {
    cout << "\nDesk card: \n";
    for (int i = 0; i < 52; i++) {
        PrintCard(c[i],0);
    }
    cout << endl;
}
int EstimateUserCards(card* c, int n) {
    int rate = 0;
    for (int i = 0; i < n; i++) {
        rate += c[i].count;
    }
    return rate;
}

void DillerAction() {
    cout << "\nDiller card: ";
        PrintUserCard(Diller, Diller.n,0);
        Diller.rate = EstimateUserCards(Diller.Cards,Diller.n);
        
        int mx=INT_MIN;
        if (Diller.rate <= 21) mx=Diller.rate;
        for (int i = 1; i <= Diller.ace; i++)  {
        //cout << "/" << Diller.rate-i*10;
        if (Diller.rate-i*10 <= 21) mx=max(Diller.rate-i*10,mx);
        }
        if (mx == INT_MIN) {
            cout << "\n" << Diller.name << " rate: " << Diller.rate;
            cout << "\n" << Diller.name << " lose!";
            cout << "\n" << User.name << " win! Congratulations" << endl;
        } else {
            cout << "\n" << Diller.name << " rate: " << mx;
            if (User.rate==21 && mx<21) {
                cout << "\n\n" << Diller.name << " hit.";
                int _rand=rand() % 52;
                (_rand+1)%13==0 ? Diller.ace++ : false;
                Diller.Cards[Diller.n++] = NumberToCard(_rand);
                DillerAction();
            }
            else if (mx>User.rate) cout << "\n" << Diller.name << " win!";
            else if (mx==User.rate && mx<=12) {
                cout << "\n\n" << Diller.name << " hit.";
                int _rand=rand() % 52;
                (_rand+1)%13==0 ? Diller.ace++ : false;
                Diller.Cards[Diller.n++] = NumberToCard(_rand);
                DillerAction();
            }
            else if (mx==User.rate) cout << "\nTie! There is no winner."; 
            else if (mx<User.rate) {
                cout << "\n\n" << Diller.name << " hit.";
                int _rand=rand() % 52;
                (_rand+1)%13==0 ? Diller.ace++ : false;
                Diller.Cards[Diller.n++] = NumberToCard(_rand);
                DillerAction(); 
            }
        }
        
}
int NextDeal() {
    int& n = User.n;
    cout << "\nStand(Stop) or Hit? (0/1)";
    bool answer = 1;
    RequestHandler(answer,2);
    if (answer) {

        int _rand=rand() % 52;
        (_rand+1)%13==0 ? User.ace++ : false;
        User.Cards[n] = NumberToCard(_rand);

        n++;
        cout << "\n" << User.name << " cards: ";
        PrintUserCard(User,n,0);
        User.rate = EstimateUserCards(User.Cards,n);

        cout << "\nYour rate: "  << User.rate;
        for (int i = 1; i <= User.ace; i++) 
        cout << "/" << User.rate-i*10;
        cout << endl;

        if (User.rate > 21 && User.rate-User.ace*10 > 21) {
            cout << "\n" << User.name << " lose!" << endl;
            return 0;
        } 
        return 1;
    } 
    
    int mx=INT_MIN;
    if (User.rate <= 21) mx=User.rate;
    for (int i = 1; i <= User.ace; i++)
    if (User.rate-i*10 <= 21) mx=max(User.rate-i*10,mx);
    
    User.rate = mx;
    return 2;
}
void StartBlackJackRound() {
    cout << "\n======================\n";
    cout << "Start BlackJack Round:";
    cout << "\n======================\n";
    card UserCards[11], DillerCards[11];
    User.name="You";
    Diller.name="Diller";
    srand(time(0));
    int _rand{};
    _rand=rand() % 52;
    (_rand+1)%13==0 ? User.ace++ : false;
    UserCards[0] = NumberToCard(_rand);

    _rand=rand() % 52;
    (_rand+1)%13==0 ? User.ace++ : false;
    UserCards[1] = NumberToCard(_rand);
    PrintCard(UserCards[0],1);
    PrintCard(UserCards[1],1);

    int n=2;
    int user_rate = EstimateUserCards(UserCards,n);
    cout << "\nYour rate: " << user_rate << endl;

    User.Cards = UserCards;
    User.n = 2;
    User.rate = EstimateUserCards(UserCards,2);
    User.rate = user_rate;

    _rand=rand() % 52;
    (_rand+1)%13==0 ? Diller.ace++ : false;
    DillerCards[0] = NumberToCard(_rand);

    cout << "\nDiller card: ";
    PrintCard(DillerCards[0],0);
    int diller_rate = EstimateUserCards(DillerCards,1);
    cout << "\nDiller rate: " << diller_rate << endl;

    _rand=rand() % 52;
    (_rand+1)%13==0 ? Diller.ace++ : false;
    DillerCards[1] = NumberToCard(_rand);
    
    Diller.Cards = DillerCards;
    Diller.n = 2;
    Diller.rate = diller_rate;
    bool answer = 1;
    while (NextDeal()==1);

    if (User.rate > 21 && User.rate-User.ace*10 > 21) {
            cout << "\nGame over!" << endl;
    } else {
        DillerAction();
    }
    

}

void BlackJack() {
    cout << "\nStart BlackJack Game: \n";
    bool answer = 1;
    while (answer) {
        User={};
        Diller={};
        StartBlackJackRound();
        cout << "\n\n\nDo you want to play again? (0/1)";
        RequestHandler(answer,2);
    } cout << "\nGoodbye!\n";
}



void AddMoney() {
    cout << "\nAdd money: \n";
    int money = 0;
    RequestHandler(money,INT_MAX);
    User.balance += money;
    Safe_exit();
}
void WithdrawMoney() {
    cout << "\nWithdraw money: \n";
    int money = 0;
    RequestHandler(money,User.balance);
    User.balance -= money;
    Safe_exit();
}

void MoneyMenu() {
    cout << fixed << setprecision(2);
    cout << "\nYour balance: " << User.balance << "$";
    cout << "\nMoney menu: \n";
    cout << "0. Exit from money menu \n";
    cout << "1. Add money \n";
    cout << "2. Withdraw money \n";
    int answer = 1;
    RequestHandler(answer,2);
    switch(answer) {
        case 1: AddMoney(); MoneyMenu(); break;
        case 2: WithdrawMoney(); MoneyMenu(); break;
        default: break;
    }
}

void ShowWin(int bet, double k, double commission) {
    cout << "\nYou win " << bet * k * commission << "$!";
    User.balance += bet * k *commission;
}
void SlotsGame() {
    srand(time(0));
    // Reset all cards to not in desk for slots game
    for (int i = 0; i < 52; i++) {
        StrCard[i].InDesk = 0;
    }
    double commission=0.9;
    int bet{};
    cout << "\nEnter your bet: ";
    RequestHandler(bet, User.balance);
    User.balance -= bet;
    Diller.balance += bet;

    card first = SetRandomCard();

    card second = SetRandomCard();
    card third = SetRandomCard();

    cout << "\nYour cards: ";
    

    PrintCard(first,0);
    PrintCard(second,0);
    PrintCard(third,0);

    // sort values for straight check
    int& a = first.value;
    int& b = second.value;
    int& c = third.value;

    if (a > b) swap(a, b);
    if (b > c) swap(b, c);
    if (a > b) swap(a, b);

    // THREE OF A KIND
    if (first.value == second.value && second.value == third.value) {
        double k = 425.0;
        ShowWin(bet, k, commission);
        cout << "\n" << User.name
            << " won because all three cards have the same rank.\n";
    }

    // STRAIGHT FLUSH
    else if (
        first.suit == second.suit &&
        second.suit == third.suit &&
        b == a + 1 &&
        c == b + 1
    ) {
        double k = 455.0;
        ShowWin(bet, k, commission);
        cout << "\n" << User.name
            << " won because the cards form a straight flush (same suit and consecutive values).\n";
    }

    // FLUSH
    else if (
        first.suit == second.suit &&
        second.suit == third.suit
    ) {
        double k = 19.3;
        ShowWin(bet, k, commission);
        cout << "\n" << User.name
            << " won because all three cards are of the same suit.\n";
    }

    // STRAIGHT
    else if (
        b == a + 1 &&
        c == b + 1
    ) {
        double k = 30.7;
        ShowWin(bet, k, commission);
        cout << "\n" << User.name
            << " won because the cards form a straight (consecutive values).\n";
    }

    // ONE PAIR
    else if (
        first.value == second.value ||
        second.value == third.value ||
        first.value == third.value
    ) {
        double k = 1.67;
        ShowWin(bet, k, commission);
        cout << "\n" << User.name
            << " won because two cards share the same rank (one pair).\n";
    }

    // TWO CARDS OF THE SAME SUIT
    else if (
        first.suit == second.suit ||
        second.suit == third.suit ||
        first.suit == third.suit
    ) {
        double k = 4.16;
        ShowWin(bet, k, commission);
        cout << "\n" << User.name
            << " won because two cards share the same suit.\n";
    }
    // NO COMBINATION
    else {
        cout << "\n" << User.name
            << " lost because no winning combination was formed.\n";
    }

    Safe_exit();
    cout << "\nDo you want to play again?(0/1)";
    bool answer;
    RequestHandler(answer, 1);
    if (answer) SlotsGame();
}

void Autorising() {
    
    cout << "\nAutorising: \n";
    cout << "Enter your name: ";

    cin >> User.name;
    cout << "\nHello, " << User.name << "!";
    cin.ignore(32767, '\n');
    fstream fin("users.txt");

    string name;
    double balance = 0;
    while (fin >> name >> balance) {
        users[name] = balance;
    }
    fin.close();
    
    if (users.find(User.name) == users.end()) {
        cout << "\nUser not found!";
        cout << "\nCreating new user...";
        users[User.name] = 0;
        User.balance = 0;
    } else {
        User.balance = users[User.name];
    }
    Safe_exit();
    MoneyMenu();
}

bool SelectGame() {
    cout << "\nSelect aciton: \n";
    cout << "0. Exit \n";
    cout << "1. BlackJack \n";
    cout << "2. Show Card Desk\n";
    cout << "3. Slots\n";
    cout << "4. Autorising\n";
    cout << "5. Show Balance\n";
    int answer = 1;
    RequestHandler(answer,5);
    switch(answer) {
        case 1: BlackJack(); break;
        case 2: PrintDeskCard(StrCard); break;
        case 3: SlotsGame(); break;
        case 4: Autorising(); break;
        case 5: MoneyMenu(); break;
        default: return 0;
    }
    return 1;

}
int main() {
    
    SetDeskCard(StrCard);

    //PrintCard(SetRandomCard(),1);

    Autorising();

    while(SelectGame());     
    cout << "\nGoodbye" << User.name << "!\n"; 

    return 0;
}
