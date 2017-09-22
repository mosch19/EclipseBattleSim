#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

enum size { INTERCEPTOR = 4, CRUISER = 6, DREADNOUGHT = 8, STARBASE = 4 };
enum hull { STANDARD = 1, IMPROVED = 2, SHARD = 3 };
enum comp { ELECTRON = 4, POSITRON = 5, GLUON = 6, AXION = 7 };
enum shield { GAUSS = 8, PHASE = 9, FLUX = 10 };
enum drive { NUCLEAR = 11, FUSION = 12, TACHYON = 13, CONFORMAL = 14 };
enum source { NUCLEAR1 = 15, FUSION1 = 16, TACHYON1 = 17 };

class Ship {
private:
    string type;
    string components[8];
    int slots;
    int hull;
    int comp;
    int shields;
    int initiative;
    int power;
    class Weapon {
    private:
        int power;
        int damage;
        bool missile;
        int dice;
    };
public:
    Ship (int size) {
        slots = size;
        hull = comp = shields = initiative = power = 0;
    }
    void addComp(int ID, bool& stop) {
        if (slots == 0) {
            cout << "This ship has no more open slots." << endl;
            stop = false;
            return;
        }
        ifstream compTable;
        string compo1, compo2;
        int tmpH, tmpC, tmpS, tmpI, tmpP;
        compTable.open("components.txt");
        string data;
        for (int i = 1; i <= ID + 2; i++) {
            getline(compTable, data);
        }
        stringstream ss;
        ss << data;
        ss >> compo1 >> compo2 >> tmpH >> tmpC >> tmpS >> tmpI >> tmpP;
        hull += tmpH;
        comp += tmpC;
        shields += tmpS;
        initiative += tmpI;
        power += tmpP;
        components[8-slots] = compo1 + ' ' + compo2;
        cout << "You have added: " << components[8-slots] << '.' << endl;
        data.clear();
        compTable.close();
        slots--;
    }
    bool checkDesign() {
        bool drive = false;
        for (int i = 0; i < 8; i++) {
            if (components[i].find("DRIVE") != string::npos) {
                drive = true;
            }
        }
        if (power < 0) {
            cout << "This ship does not have enough power! Design cancelled." << endl;
            return false;
        } else if (!drive){
            cout << "This ship does not have a drive! Design cancelled." << endl;
            return false;
        } else {
            return true;
        }
    }
    void saveShip(string fileName) {
        ofstream destination(fileName + ".txt");
        destination << "Ship name: " << fileName << "\n";
        destination << "Components: ";
        for (int i = 0; i < 8; i++) {
            destination << components[i];
            if (!components[i].empty()) {
                destination << ", ";
            }
        }
        destination << "\n";
        destination << "HULL    COMP    SHIELDS INITIATIVE  POWER\n";
        destination << hull << "\t\t" << comp << "\t\t" << shields << "\t\t" << initiative << "\t\t\t" << power << "\t\t";
        cout << "File saved." << endl;
        destination.close();
    }
    void viewShips() {
    }
    int getSlots() {
        return slots;
    }
    void printStats() {
        cout << "Here are your ship statistics." << endl;
        cout << "******************************" << endl;
        cout << "Hull: " << hull << endl;
        cout << "Computer: " << comp << endl;
        cout << "Shields: " << shields << endl;
        cout << "Initiative: " << initiative << endl;
        cout << "Power: " << power << endl;
        cout << "Components: ";
        for (int i = 0; i < 8; i++) {
            cout << components[i];
            if (!components[i].empty()) {
                cout << ", ";
            }
        }
        cout << endl;
        cout << "******************************" << endl;
    }
};

void viewComponents() {
    string row;
    ifstream compTable;
    compTable.open("components.txt");
    while (!compTable.eof()) {
        getline(compTable, row);
        cout << row << endl;
    }
    compTable.close();
}

void clearTerminal() {
    cout << "\033[2J\033[1;1H";
}

void printTitle() {
    string row;
    ifstream title;
    title.open("title.txt");
    while (!title.eof()) {
        getline(title, row);
        cout << row << endl;
    }
    title.close();
}

int main() {
    int menu;
    bool stay = true;
    clearTerminal();
    printTitle();
    cout << "Welcome to the Eclipse Battle Simulator!" << endl;
    while(stay) {
        cout << "Press 1 to design a ship and two to simulate a battle." << endl;
        cin >> menu;
        switch(menu) {
            case 1: {
                int choice;
                int ID;
                int choice2;
                bool a = true;
                string shipName;
                clearTerminal();
                cout << "Please enter a ship size to create." << endl;
                cout << "Interceptor: 1, Cruiser: 2, Dreadnought: 3" << endl;
                cin >> choice;
                size x;
                switch (choice) {
                    case 1:
                        x = INTERCEPTOR;
                        break;
                    case 2:
                        x = CRUISER;
                        break;
                    case 3:
                        x = DREADNOUGHT;
                        break;
                    case 4:
                        x = STARBASE;
                    default:
                        exit(1);
                }
                Ship mine(x);
                cout << "Time to give it components." << endl;
                while (a) {
                    cout << "Enter component ID to add it to your ship. Enter 0 to view all components." << endl;
                    cout << "You have " << mine.getSlots() << " left." << endl;
                    cin >> ID;
                    if (ID == 0) {
                        viewComponents();
                    } else {
                        mine.addComp(ID, a);
                    }
                }
                mine.printStats();
                if (mine.checkDesign()) {
                    cout << "Your ship has its components. Give it a name." << endl;
                    cin >> shipName;
                    cout << "If you would like to save this press 1." << endl;
                    cin >> choice2;
                    if (choice2 == 1) {
                        mine.saveShip(shipName);
                    }
                } else {
                    break;
                }

                break;
            }
            case 2:
                cout << "ADD SOMETHING HERE" << endl;
                break;
            default:
                stay = false;
                break;
        }
    }

    return 0;
}