#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <algorithm>

using namespace std;

// Forward declarations
class Donor;
class BloodBank;

// Utility class for date handling
class Date {
private:    
    int day, month, year;

public:
    Date(int d = 0, int m = 0, int y = 0) : day(d), month(m), year(y) {}
    
    static Date getCurrentDate() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        return Date(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
    }

    bool operator>=(const Date& other) const {
        if (year != other.year) return year >= other.year;
        if (month != other.month) return month >= other.month;
        return day >= other.day;
    }

    friend ostream& operator<<(ostream& os, const Date& date) {
        os << date.day << "/" << date.month << "/" << date.year;
        return os;
    }
};

// Donor class to store donor information
class Donor {
private:
    string donorId;
    string name;
    string bloodGroup;
    string contact;
    Date lastDonation;

public:
    Donor(string id, string n, string bg, string cont)
        : donorId(id), name(n), bloodGroup(bg), contact(cont) {}

    string getDonorId() const { return donorId; }
    string getName() const { return name; }
    string getBloodGroup() const { return bloodGroup; }
    string getContact() const { return contact; }
    Date getLastDonation() const { return lastDonation; }

    void updateLastDonation(const Date& date) {
        lastDonation = date;
    }

    bool canDonate() const {
        Date currentDate = Date::getCurrentDate();
        // Assuming 3 months (90 days) minimum gap between donations
        // This is a simplified check
        return true; // In a real system, implement proper date comparison
    }

    void displayInfo() const {
        cout << "\nDonor ID: " << donorId
             << "\nName: " << name
             << "\nBlood Group: " << bloodGroup
             << "\nContact: " << contact
             << "\nLast Donation: " << lastDonation << endl;
    }
};

// Blood Bank class to manage the entire system
class BloodBank {
private:
    vector<Donor> donors;
    map<string, int> bloodStock; // Blood group -> units available

public:
    BloodBank() {
        // Initialize blood stock with 0 units for each blood group
        bloodStock["A+"] = 0;
        bloodStock["A-"] = 0;
        bloodStock["B+"] = 0;
        bloodStock["B-"] = 0;
        bloodStock["AB+"] = 0;
        bloodStock["AB-"] = 0;
        bloodStock["O+"] = 0;
        bloodStock["O-"] = 0;
    }

    void addDonor() {
        string id, name, bloodGroup, contact;
        
        cout << "\nEnter Donor Details:";
        cout << "\nDonor ID: ";
        cin >> id;
        
        // Check if donor ID already exists
        if (findDonor(id) != -1) {
            cout << "Donor ID already exists!" << endl;
            return;
        }

        cout << "Name: ";
        cin.ignore();
        getline(cin, name);
        
        cout << "Blood Group (A+/A-/B+/B-/AB+/AB-/O+/O-): ";
        cin >> bloodGroup;
        
        // Validate blood group
        if (bloodStock.find(bloodGroup) == bloodStock.end()) {
            cout << "Invalid blood group!" << endl;
            return;
        }

        cout << "Contact: ";
        cin >> contact;

        donors.push_back(Donor(id, name, bloodGroup, contact));
        cout << "\nDonor added successfully!" << endl;
    }

    int findDonor(const string& donorId) const {
        for (size_t i = 0; i < donors.size(); i++) {
            if (donors[i].getDonorId() == donorId) {
                return i;
            }
        }
        return -1;
    }

    void searchDonor() {
        string searchId;
        cout << "\nEnter Donor ID to search: ";
        cin >> searchId;

        int index = findDonor(searchId);
        if (index != -1) {
            donors[index].displayInfo();
        } else {
            cout << "Donor not found!" << endl;
        }
    }

    void recordDonation() {
        string donorId;
        cout << "\nEnter Donor ID: ";
        cin >> donorId;

        int index = findDonor(donorId);
        if (index == -1) {
            cout << "Donor not found!" << endl;
            return;
        }

        if (!donors[index].canDonate()) {
            cout << "Donor is not eligible to donate yet!" << endl;
            return;
        }

        donors[index].updateLastDonation(Date::getCurrentDate());
        bloodStock[donors[index].getBloodGroup()]++;
        cout << "Donation recorded successfully!" << endl;
    }

    void displayBloodStock() const {
        cout << "\nCurrent Blood Stock:" << endl;
        cout << setw(10) << "Blood Group" << setw(10) << "Units" << endl;
        cout << string(20, '-') << endl;
        
        for (const auto& stock : bloodStock) {
            cout << setw(10) << stock.first << setw(10) << stock.second << endl;
        }
    }

    void requestBlood() {
        string bloodGroup;
        int units;

        cout << "\nEnter blood group needed: ";
        cin >> bloodGroup;
        
        if (bloodStock.find(bloodGroup) == bloodStock.end()) {
            cout << "Invalid blood group!" << endl;
            return;
        }

        cout << "Enter units needed: ";
        cin >> units;

        if (bloodStock[bloodGroup] >= units) {
            bloodStock[bloodGroup] -= units;
            cout << "Blood request fulfilled successfully!" << endl;
        } else {
            cout << "Insufficient blood stock!" << endl;
            // Here you could implement the emergency donor notification system
        }
    }
};

int main() {
    BloodBank bloodBank;
    int choice;

    while (true) {
        cout << "\n=== Blood Bank Management System ===" << endl;
        cout << "1. Add New Donor" << endl;
        cout << "2. Search Donor" << endl;
        cout << "3. Record Blood Donation" << endl;
        cout << "4. Request Blood" << endl;
        cout << "5. Display Blood Stock" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                bloodBank.addDonor();
                break;
            case 2:
                bloodBank.searchDonor();
                break;
            case 3:
                bloodBank.recordDonation();
                break;
            case 4:
                bloodBank.requestBlood();
                break;
            case 5:
                bloodBank.displayBloodStock();
                break;
            case 6:
                cout << "\nThank you for using Blood Bank Management System!" << endl;
                return 0;
            default:
                cout << "\nInvalid choice! Please try again." << endl;
        }
    }

    return 0;
} 