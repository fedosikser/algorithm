#include <iostream>
#include <string>
#include <algorithm>
#include <set>
#include <fstream>

using namespace std;

set<string> arr[26];

int ErrorSavior() {
    string s{};
    cin >> s;
    if (s.empty()) return 0;

    for (char c : s)
    {
        if (!(c>='0' && c<='9'))
            return 0;
    }
    return stoi(s);
}

void Callback(int& action, int n) {

    action=ErrorSavior(); 
   
    while(!(action>=1 && action<=n)) {
        cout << "\nThis command doesn't exist. Repit \n";
        action=ErrorSavior(); 
        
    }
}

// part from old version of code
bool BinarySearch( vector<string>& v, string& x) {
    int l = 0;
    int r = (int)v.size() - 1;
    int m;
    while (l <= r) {
        m = (l + r)/2;

        if (v[m] == x) return true;
        if (v[m] < x)  l = m + 1;
        else           r = m - 1;
    }
    return false;
}

void DBinput() {

    fstream fin("text.txt");
    if (!fin) {
        cout << "Cannot open file\n";
        return;
    }
    
    string x;
    int ind{0};

    fin >> x;
    ind=(int)toupper(x[0])-65;

    while ( x!="-1" && (ind>=0 && ind<26) ) { 

        
        arr[ind].insert(x);
        
        fin >> x;
        ind=(int)toupper(x[0])-65;

    }

    fin.close();
}

void ChangeDB(){
    fstream fout("text.txt", ios::out);
    for (int j=0; j<26; j++) {
        for (auto it = arr[j].begin(); it != arr[j].end(); ++it) {
            fout << *it << endl;
        } 
    }
    fout << -1;
    fout.close();
}

bool UserFind(string user) {

    user[0]=toupper(user[0]);
    for (int i=1; i<user.size(); i++) {
        user[i]=tolower(user[i]);
    }

    int ind;
    ind=(int)toupper(user[0])-65;
    if ( arr[ind].find(user)!=arr[ind].end() ) return 1;
    else return 0;
    
}

void UserInsert() {
    
    string user;
    int ind{};
    cout << "\nEnter user name that u want to insert:\n";
    cin >> user;

    user[0]=toupper(user[0]);
    for (int i=1; i<user.size(); i++) {
        user[i]=tolower(user[i]);
    }
    if (UserFind(user)) cout << "\n User already exist. ";
    else {
        ind=(int)toupper(user[0])-65;
        arr[ind].insert(user);
    }

    ChangeDB();


    cout << "\n Do you want repit? (no - 1/ yes - 2) \n";
    int ans;
    Callback(ans, 2);
    ans--;
    if (ans==1) UserInsert(); 
}

void UserErase() {

    string user;
    cout << "\nEnter user name that u want to erase:\n";
    cin >> user;

    if (!UserFind(user)) cout << "\n User doesn't exist. ";
    else {
        int ind;
        ind=(int)toupper(user[0])-65;
        arr[ind].erase(user);
    }

    cout << "\n Do you want repit? (no - 1/ yes - 2) \n";
    int ans;

    Callback(ans, 2);
    ans--;
    if (ans==1) UserErase(); 
    
}

void UserShowAll() {
    cout << "\nAll users: \n";
    for (int i=0; i<26; i++) {
        cout << (char)(i+65) << " : \n";
        for (auto it = arr[i].begin(); it != arr[i].end(); ++it) {
            cout << *it << " ";
        } cout << "\n";
    } 
}

void UserRequest() {
    
    cout  << "\n Сhoose an action: \n 1.Find User \n 2.Insert User \n 3.Erase User\n 4.Show all users\n";
    int action{0};
    string user;

   
    Callback(action, 4);
    
    if (action==1) {
        cout << "\nEnter user name that u want to find:\n"; 
        cin >> user;
        
        if (UserFind(user)) cout << "\n User : " << user << " exist!) \n";
        else cout << "\n User : " << user << " doesn't exist!) \n";

    }
    else if (action==2) {
        
        UserInsert();

    }
    else if (action==3) {
        
        UserErase();
        
    } else if (action==4) {
        
        UserShowAll();
        
    }


}

void PrintMenu() {

    UserRequest();
    
    int action=1;
    while (action==1) {

    cout << "\n Do u want to continue? (no - 1/ yes - 2) \n";

   
    Callback(action, 2);
    action--;
    
    if (action==1)  UserRequest();
    else action=0;   
    

   }
   cout << "\nGoodbye!\n";
}

int main() {

    DBinput();
    
    PrintMenu();

}