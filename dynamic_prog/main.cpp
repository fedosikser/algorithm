#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <fstream>
#include <set>

using namespace std;   

double MinWeight=1;
double MaxWeight=4/MinWeight;

class Item{
    private:
        static int IdGenerator;
        int id;
        int weight;
        int value;
        string name;
    public:
        int GetId() const { return id; }
        int GetWeight() const { return weight; }
        int GetValue() const { return value; }
        string GetName() const { return name; }

        void SetWeight(int weight) { this->weight = weight/MinWeight; }
        void SetValue(int value) { this->value = value; }
        void SetName(string name) { this->name = name; }

        bool operator<(const Item& other) const {
            return id < other.id;
        }
        
        bool operator==(const Item& other) const {
            return id == other.id;
        }

        Item(string name, int weight, int value) {
            this->weight = weight/MinWeight;
            this->value = value;
            this->name = name;
            this->id = IdGenerator++;
        }
};
int Item::IdGenerator = 0;
class Inventory{ 
    private:
        static int InvIdGenerator;
    public:
        vector<Item> items;
        void InsertItem(Item item) { items.push_back(item); }
        void EraseItem(int id) { items.erase(items.begin() + id); }
        void PrintInventory() {
            for(int i = 0; i < items.size(); i++) {
                cout << items[i].GetName() << " " << items[i].GetWeight() << " " << items[i].GetValue() << " id : " << items[i].GetId() << endl;
            }
        }
        void SetInventory(string filename="") {

            if(filename.empty()) {
                filename = "inventory";
                filename += to_string(InvIdGenerator++);
                filename+=".txt";
            } else {
                filename += ".txt";
            }
            cout << "\nReading from file: " << filename << endl;
        
            fstream fin(filename);
        
            if(!fin.is_open()) {
                cout << "\nFile not found\n";
                return;
            }

            while(!fin.eof()) {
                string name;
                int weight, value;
                fin >> name >> weight >> value;
                this->items.push_back(Item(name, weight, value));
            }
            fin.close();
        }
        Inventory() {}
};
int Inventory::InvIdGenerator = 0;
class Cell {
    private:
        set<Item> items;
        int value;
    public:
        void SetItems(set<Item>& items) { this->items = items; 
            this->value = 0;
            for (auto item : items) {
                value += item.GetValue();
            }
        }
        auto GetValue() { return value; }
        auto& GetItems() { return this->items; }
        void InsertItem(Item& item) { items.insert(item);
            value += item.GetValue(); }
        void EraseItem(Item& item) { items.erase(item);
            value -= item.GetValue(); }

        Cell(set<Item>& items) { SetItems(items); }
        Cell() { items = set<Item>(); value = 0; }
        Cell(const Cell& other) { items = other.items; value = other.value; }
        
};
auto ChooseInventory() {
    string filename{};
    cin.ignore();
    cout << "\nChoose inventory:\n";
    getline(cin, filename);
    return filename;
}
void Solution(Inventory& inv) {
    inv.SetInventory(ChooseInventory());
    inv.PrintInventory();

    int capacity = static_cast<int>(MaxWeight); 
    vector<vector<Cell>> dp(inv.items.size() + 1, vector<Cell>(capacity + 1));
    
    // Initialize base cases
    for (int i = 0; i <= inv.items.size(); i++) {
        for (int w = 0; w <= capacity; w++) {
            if (i == 0 || w == 0) {
                dp[i][w] = Cell();
            }
        }
    }
    
    // Fill the dp table
    for (int i = 1; i <= inv.items.size(); i++) {
        for (int w = 0; w <= capacity; w++) {
            if (inv.items[i-1].GetWeight() <= w) {
                set<Item> itemsWithNewItem = dp[i-1][w - inv.items[i-1].GetWeight()].GetItems();
                itemsWithNewItem.insert(inv.items[i-1]);
                
                set<Item> itemsWithoutNewItem = dp[i-1][w].GetItems();
                
                if (dp[i-1][w - inv.items[i-1].GetWeight()].GetValue() + inv.items[i-1].GetValue() > dp[i-1][w].GetValue()) {
                    dp[i][w].SetItems(itemsWithNewItem);
                } else {
                    dp[i][w].SetItems(itemsWithoutNewItem);
                }
            } else {
                dp[i][w].SetItems(dp[i-1][w].GetItems());
            }
        }
    }
    
   
    cout << "\nOptimal solution:\n" << endl;
    for (auto& item : dp[inv.items.size()][capacity].GetItems()) {
        cout << item.GetName() << " " << item.GetWeight() << " " << item.GetValue() << endl;
    }
    cout << "\nTotal value: " << dp[inv.items.size()][capacity].GetValue() << endl;
}
int main() {

    Inventory inv;

    cout << "\nSet capacity: ";
    cin >> MaxWeight;

    Solution(inv);
    


    return 0;
}