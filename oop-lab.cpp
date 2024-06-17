#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class MenuItems {
    string name;
    double price;
public:
    MenuItems() : name(""), price(0.0) {}
    MenuItems(string name, double price) : name(name), price(price) {}
    void setName(string name) {
        this->name = name;
    }
    void setPrice(double price) {
        this->price = price;
    }
    string getName() const {
        return name;
    }
    double getPrice() const {
        return price;
    }
};

class Cart {
    MenuItems items[100];
    int numberOfItems;
public:
    Cart() : numberOfItems(0) {}

    void addItem(const MenuItems& item) {
        items[numberOfItems] = item;
        ++numberOfItems;
        cout << "\t" << item.getName() << " has been added to your cart.\n";
    }

    void displayCart() const {
        double totalPrice = 0.0;
        if (numberOfItems == 0) {
            cout << "\n\tYour cart is empty.\n";
            return;
        }
        cout << "\n\tYour cart contains:\n";
        for (int i = 0; i < numberOfItems; ++i) {
            cout << "\t" << items[i].getName() << " - Price per item: " << items[i].getPrice() << endl;
            totalPrice += items[i].getPrice();
        }
        cout << "\tTotal Price: " << totalPrice << endl;
    }
};

class Person {
protected:
    string name;
    string password;
public:
    Person(string name, string password) : name(name), password(password) {}
    string getName() const { return name; }
    string getPassword() const { return password; }
};

class User : public Person {
    Cart cart;
public:
    User(string name, string password) : Person(name, password) {
        cout << "\t\t************ Welcome " << name << " to KFC ************\n";
    }
    friend string Search(const MenuItems items[], int size, const string& itemName);
    string buyAProduct(MenuItems items[], int size, const string& name) {
        for (int i = 0; i < size; ++i) {
            if (items[i].getName() == name) {
                cart.addItem(items[i]);
                return "\tProduct purchased successfully!";
            }
        }
        return "\tProduct not found.";
    }
    void displayCart() const {
        cart.displayCart();
    }
};

class Manager : public Person {
    int numberOfItems;
public:
    Manager(string name, string password) : Person(name, password), numberOfItems(0) {
        cout << "\t\t************ Welcome " << name << " to KFC ************\n";
    }
    friend string Search(const MenuItems items[], int size, const string& itemName);
    int getNumberOfItems() const { return numberOfItems; }

    string addItems(MenuItems items[]) {
        string itemName;
        double price;
        cout << "\t\tEnter the Item name: ";
        cin.ignore();
        getline(cin, itemName);

        // Check if the item already exists
        for (int i = 0; i < numberOfItems; i++) {
            if (items[i].getName() == itemName) {
                return "\tYour entered item is already in the bucket.";
            }
        }

        // If the item doesn't exist, add it to the array
        cout << "\t\tEnter the item Price: ";
        cin >> price;

        items[numberOfItems] = MenuItems(itemName, price);
        ++numberOfItems;

        return "\tItem added successfully.";
    }

    string deleteItems(MenuItems items[]) {
        string itemName;
        cout << "\tEnter the Item name you want to Delete: ";
        cin.ignore();
        getline(cin, itemName);

        for (int i = 0; i < numberOfItems; i++) {
            if (items[i].getName() == itemName) {
                items[i] = MenuItems("", 0.0);
                return "\tItem Deleted Successfully!";
            }
        }
        return "\tThis item is not in the list.";
    }

    void displayItems(const MenuItems items[]) const {
        if (numberOfItems == 0) {
            cout << "\n\tNo items available.\n";
            return;
        }
        cout << "\n\tAvailable items:\n";
        for (int i = 0; i < numberOfItems; ++i) {
            cout << "\t" << items[i].getName() << " - Price: " << items[i].getPrice() << endl;
        }
    }
};

string Search(const MenuItems items[], int size, const string& S_Item) {
    for (int i = 0; i < size; i++) {
        if (items[i].getName() == S_Item) {
            return "\tYes, This item is in the list.";
        }
    }
    return "\tNo, This item is not placed in the list.";
}

bool registerPerson(const string& filename, const string& name, const string& password) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cout << "\tError opening file for registration.\n";
        return false;
    }
    file << name << "," << password << endl;
    file.close();
    return true;
}

bool loginPerson(const string& filename, const string& name, const string& password) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "\tError opening file for login.\n";
        return false;
    }
    string line, uname, pass;
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, uname, ',');
        getline(ss, pass, ',');
        if (uname == name && pass == password) {
            return true;
        }
    }
    return false;
}

void managerMenu(Manager &manager, MenuItems items[]) {
    while (true) {
        cout << "\t\t************ What would you like to do? ************\n";
        cout << "\t1: Add new item\n\t2: Delete existing item\n\t3: Search item\n\t4: Display all items\n\t5: Switch to User Mode\n\t-1: Quit\n\tSelect a number: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << manager.addItems(items) << endl;
                break;
            }
            case 2: {
                cout << manager.deleteItems(items) << endl;
                break;
            }
            case 3: {
                string searchItem;
                cout << "\tEnter the item you want to search: ";
                cin.ignore();
                getline(cin, searchItem);
                cout << Search(items, manager.getNumberOfItems(), searchItem) << endl;
                break;
            }
            case 4: {
                manager.displayItems(items);
                break;
            }
            case 5:
                return;  // Switch to user mode
            case -1:
                exit(0); // Quit the program
            default:
                cout << "\tInvalid choice." << endl;
                break;
        }
    }
}

void userMenu(User &user, MenuItems items[], int managerNumberOfItems) {
    while (true) {
        cout << "\t\t************ What would you like to do? ************\n";
        cout << "\t1: Search an item\n\t2: Buy a product\n\t3: Display all items\n\t4: View cart\n\t5: Switch to Manager Mode\n\t-1: Quit\n\tSelect a number: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                string searchItem;
                cout << "\tEnter the item you want to search: ";
                cin.ignore();
                getline(cin, searchItem);
                cout << Search(items, managerNumberOfItems, searchItem) << endl;
                break;
            }
            case 2: {
                string productName;
                cout << "\tEnter the product name you want to buy: ";
                cin.ignore();
                getline(cin, productName);
                cout << user.buyAProduct(items, managerNumberOfItems, productName) << endl;
                break;
            }
            case 3: {
                if (managerNumberOfItems == 0) {
                    cout << "\n\tNo items available.\n";
                } else {
                    for (int i = 0; i < managerNumberOfItems; ++i) {
                        if (items[i].getName() != "") {
                            cout << "\t" << items[i].getName() << " - Price: " << items[i].getPrice() << endl;
                        }
                    }
                }
                break;
            }
            case 4: {
                user.displayCart();
                break;
            }
            case 5:
                return;  // Switch to manager mode
            case -1:
                exit(0); // Quit the program
            default:
                cout << "\tInvalid choice." << endl;
                break;
        }
    }
}

int main() {
    const int MAX_ITEMS = 100;
    MenuItems items[MAX_ITEMS];
    string managerFile = "managers.txt";
    string userFile = "users.txt";

    while (true) {
        int userPosition;
        cout << "\t\t******************** Tell Us your Position ********************\n";
        cout << "\tEnter 1 if you are a Manager!\n\tEnter 2 if you are a User!\n\tEnter -1 to Quit!\n";
        cin >> userPosition;

        if (userPosition == 1 || userPosition == 2) {
            int choice;
            string name, password;
            cout << "\tEnter 1 to Register\n\tEnter 2 to Login\n";
            cin >> choice;
            cout << "\tEnter your name: ";
            cin.ignore();
            getline(cin, name);
            cout << "\tEnter your Password: ";
            getline(cin, password);

            if (choice == 1) {  // Register
                if (userPosition == 1) {
                    if (registerPerson(managerFile, name, password)) {
                        cout << "\tManager registered successfully.\n";
                    }
                } else {
                    if (registerPerson(userFile, name, password)) {
                        cout << "\tUser registered successfully.\n";
                    }
                }
            } else if (choice == 2) {  // Login
                if (userPosition == 1) {
                    if (loginPerson(managerFile, name, password)) {
                        Manager manager(name, password);
                        managerMenu(manager, items);
                    } else {
                        cout << "\tInvalid login credentials for manager.\n";
                    }
                } else {
                    if (loginPerson(userFile, name, password)) {
                        User user(name, password);
                        userMenu(user, items, MAX_ITEMS);
                    } else {
                        cout << "\tInvalid login credentials for user.\n";
                    }
                }
            } else {
                cout << "\tInvalid choice.\n";
            }
        } else if (userPosition == -1) {
            cout << "\tExiting the program." << endl;
            break;
        } else {
            cout << "\tYou entered an invalid number." << endl;
        }
    }

    return 0;
}
