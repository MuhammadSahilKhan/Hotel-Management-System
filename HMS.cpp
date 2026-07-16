#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

/* =========================
   ABSTRACT CLASS (OOP)
========================= */
class Person {
protected:
    int id;
    string name;

public:
    Person(int i = 0, string n = "") {
        id = i;
        name = n;
    }

    virtual void display() = 0; // Abstraction
};

/* =========================
   CUSTOMER CLASS (Inheritance)
========================= */
class Customer : public Person {
    string phone;
    int roomNo;
    int days;

public:
    Customer(int i = 0, string n = "", string p = "", int r = 0, int d = 0)
        : Person(i, n) {
        phone = p;
        roomNo = r;
        days = d;
    }

    int getId() { return id; }
    int getRoomNo() { return roomNo; }
    int getDays() { return days; }
    string getName() { return name; }
    string getPhone() { return phone; }

    void setRoom(int r, int d) {
        roomNo = r;
        days = d;
    }

    void display() override {
        cout << "\nID: " << id
             << "\nName: " << name
             << "\nPhone: " << phone
             << "\nRoom: " << roomNo
             << "\nDays: " << days << endl;
    }
};

/* =========================
   ROOM CLASS (Inheritance + Polymorphism)
========================= */
class Room {
protected:
    int roomNo;
    double price;
    bool available;

public:
    Room(int r, double p) {
        roomNo = r;
        price = p;
        available = true;
    }

    virtual string getType() = 0;

    int getRoomNo() { return roomNo; }
    bool isAvailable() { return available; }

    void bookRoom() { available = false; }
    void checkoutRoom() { available = true; }

    virtual double bill(int days) {
        return price * days;
    }

    virtual void display() {
        cout << "Room: " << roomNo
             << " | Price: " << price
             << " | Status: " << (available ? "Available" : "Booked")
             << endl;
    }

    virtual ~Room() {}
};

/* =========================
   STANDARD ROOM
========================= */
class StandardRoom : public Room {
public:
    StandardRoom(int r) : Room(r, 3000) {}

    string getType() override {
        return "Standard";
    }
};

/* =========================
   DELUXE ROOM
========================= */
class DeluxeRoom : public Room {
public:
    DeluxeRoom(int r) : Room(r, 6000) {}

    string getType() override {
        return "Deluxe";
    }
};

/* =========================
   HOTEL MANAGEMENT SYSTEM
========================= */
class HotelManagement {
private:
    vector<Customer> customers;
    vector<Room*> rooms;

public:
    HotelManagement() {
        loadData();

        // default rooms
        if (rooms.empty()) {
            rooms.push_back(new StandardRoom(101));
            rooms.push_back(new StandardRoom(102));
            rooms.push_back(new DeluxeRoom(201));
            rooms.push_back(new DeluxeRoom(202));
        }
    }

    /* ================= FILE LOAD ================= */
    void loadData() {
        ifstream file("customers.txt");
        int id, room, days;
        string name, phone;

        while (file >> id >> name >> phone >> room >> days) {
            customers.push_back(Customer(id, name, phone, room, days));
        }
        file.close();
    }

    /* ================= SAVE FILE ================= */
    void saveData() {
        ofstream file("customers.txt");

        for (auto &c : customers) {
            file << c.getId() << " "
                 << c.getName() << " "
                 << c.getPhone() << " "
                 << c.getRoomNo() << " "
                 << c.getDays() << endl;
        }
        file.close();
    }

    /* ================= ADD ROOM ================= */
    void addRoom() {
        int type, no;

        cout << "\n1. Standard\n2. Deluxe\nEnter type: ";
        cin >> type;

        cout << "Enter Room Number: ";
        cin >> no;

        if (type == 1)
            rooms.push_back(new StandardRoom(no));
        else
            rooms.push_back(new DeluxeRoom(no));

        cout << "Room Added Successfully!\n";
    }

    /* ================= DISPLAY ROOMS ================= */
    void displayRooms() {
        cout << "\n===== ROOMS =====\n";
        for (auto r : rooms) {
            r->display();
        }
    }

    /* ================= SEARCH CUSTOMER ================= */
    void searchCustomer() {
        int id;
        cout << "Enter Customer ID: ";
        cin >> id;

        for (auto &c : customers) {
            if (c.getId() == id) {
                c.display();
                return;
            }
        }
        cout << "Customer Not Found!\n";
    }

    /* ================= BOOK ROOM ================= */
    void bookRoom() {
        int id, roomNo, days;
        string name, phone;

        cout << "Enter Customer ID: ";
        cin >> id;

        cout << "Enter Name: ";
        cin >> name;

        cout << "Enter Phone: ";
        cin >> phone;

        cout << "Enter Room No: ";
        cin >> roomNo;

        cout << "Enter Days: ";
        cin >> days;

        for (auto r : rooms) {
            if (r->getRoomNo() == roomNo && r->isAvailable()) {

                r->bookRoom();

                customers.push_back(Customer(id, name, phone, roomNo, days));

                cout << "\nRoom Booked Successfully!\n";
                saveData();
                return;
            }
        }

        cout << "Room Not Available!\n";
    }

    /* ================= GENERATE BILL ================= */
    void generateBill() {
        int id;
        cout << "Enter Customer ID: ";
        cin >> id;

        for (auto &c : customers) {
            if (c.getId() == id) {

                for (auto r : rooms) {
                    if (r->getRoomNo() == c.getRoomNo()) {

                        double total = r->bill(c.getDays());

                        cout << "\n===== BILL =====";
                        cout << "\nName: " << c.getName();
                        cout << "\nRoom: " << c.getRoomNo();
                        cout << "\nDays: " << c.getDays();
                        cout << "\nTotal Bill: " << total << endl;
                        return;
                    }
                }
            }
        }

        cout << "Customer Not Found!\n";
    }

    /* ================= CHECKOUT ================= */
    void checkout() {
        int id;
        cout << "Enter Customer ID: ";
        cin >> id;

        for (int i = 0; i < customers.size(); i++) {
            if (customers[i].getId() == id) {

                for (auto r : rooms) {
                    if (r->getRoomNo() == customers[i].getRoomNo()) {
                        r->checkoutRoom();
                    }
                }

                customers.erase(customers.begin() + i);
                cout << "Checkout Successful!\n";

                saveData();
                return;
            }
        }

        cout << "Customer Not Found!\n";
    }

    /* ================= EXPORT CSV ================= */
    void exportCSV() {
        ofstream file("HotelData.csv");

        file << "ID,Name,Phone,Room,Days\n";

        for (auto &c : customers) {
            file << c.getId() << ","
                 << c.getName() << ","
                 << c.getPhone() << ","
                 << c.getRoomNo() << ","
                 << c.getDays() << "\n";
        }

        file.close();

        cout << "CSV Exported Successfully!\n";
    }

    /* ================= MENU ================= */
    void menu() {
        int choice;

        do {
            cout << "\n\n===== HOTEL MANAGEMENT SYSTEM =====";
            cout << "\n1. Add Room";
            cout << "\n2. Display Rooms";
            cout << "\n3. Book Room";
            cout << "\n4. Search Customer";
            cout << "\n5. Generate Bill";
            cout << "\n6. Checkout Customer";
            cout << "\n7. Export CSV";
            cout << "\n8. Exit";

            cout << "\nEnter Choice: ";
            cin >> choice;

            switch (choice) {
                case 1: addRoom(); break;
                case 2: displayRooms(); break;
                case 3: bookRoom(); break;
                case 4: searchCustomer(); break;
                case 5: generateBill(); break;
                case 6: checkout(); break;
                case 7: exportCSV(); break;
                case 8: saveData(); break;
                default: cout << "Invalid Choice!\n";
            }

        } while (choice != 8);
    }

    ~HotelManagement() {
        for (auto r : rooms)
            delete r;
    }
};

/* ================= MAIN ================= */
int main() {
    HotelManagement h;
    h.menu();
    return 0;
}
