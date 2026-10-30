
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

struct Expense {
    int id;
    string date;
    string category;
    string description;
    double amount;
};

Expense expenses[500];
int expenseCount = 0;
int nextId = 1;
double monthlyBudget = 0;

string username, password;

// Save all data in a file
void saveData() {
    ofstream file("expenses.txt");

    file << monthlyBudget << endl;
    file << expenseCount << endl;
    file << nextId << endl;

    for (int i = 0; i < expenseCount; i++) {
        file << expenses[i].id << endl;
        file << expenses[i].date << endl;
        file << expenses[i].category << endl;
        file << expenses[i].description << endl;
        file << expenses[i].amount << endl;
    }

    file.close();
}

// Load previously saved data
void loadData() {
    ifstream file("expenses.txt");

    if (!file) return;

    file >> monthlyBudget;
    file >> expenseCount;
    file >> nextId;

    file.ignore();

    for (int i = 0; i < expenseCount; i++) {
        file >> expenses[i].id;
        file.ignore();

        getline(file, expenses[i].date);
        getline(file, expenses[i].category);
        getline(file, expenses[i].description);

        file >> expenses[i].amount;
        file.ignore();
    }

    file.close();
}

// User registration
void registerUser() {
    cout << "\nCreate username: ";
    cin >> username;

    cout << "Create password: ";
    cin >> password;

    ofstream file("user.txt");
    file << username << endl << password;
    file.close();

    cout << "\nRegistration successful!\n";
}

// User login
bool loginUser() {
    string user, pass;

    cout << "\nEnter username: ";
    cin >> user;

    cout << "Enter password: ";
    cin >> pass;

    ifstream file("user.txt");
    string savedUser, savedPass;

    if (file) {
        file >> savedUser >> savedPass;
        file.close();

        if (user == savedUser && pass == savedPass) {
            cout << "\nLogin successful!\n";
            return true;
        }
    }

    cout << "\nInvalid username or password!\n";
    return false;
}

// Add a new expense
void addExpense() {
    if (expenseCount >= 500) {
        cout << "\nExpense limit reached!\n";
        return;
    }

    Expense e;
    e.id = nextId++;

    cout << "\nEnter date (DD-MM-YYYY): ";
    cin >> e.date;

    cout << "Enter category (Food/Travel/Shopping/Other): ";
    cin >> e.category;

    cout << "Enter description (one word): ";
    cin >> e.description;

    cout << "Enter amount: ";
    cin >> e.amount;

    if (e.amount <= 0) {
        cout << "\nInvalid amount!\n";
        nextId--;
        return;
    }

    expenses[expenseCount++] = e;
    saveData();

    cout << "\nExpense added successfully!\n";
}

// View all expenses
void viewExpenses() {
    if (expenseCount == 0) {
        cout << "\nNo expenses found!\n";
        return;
    }

    cout << "\nID\tDate\t\tCategory\tAmount\n";

    for (int i = 0; i < expenseCount; i++) {
        cout << expenses[i].id << "\t"
             << expenses[i].date << "\t"
             << expenses[i].category << "\t\t"
             << expenses[i].amount << endl;
    }
}

// Calculate total expenses
double calculateTotal() {
    double total = 0;

    for (int i = 0; i < expenseCount; i++) {
        total += expenses[i].amount;
    }

    return total;
}

// Category-wise expense report
void categoryReport() {
    string categories[] = {"Food", "Travel", "Shopping", "Other"};

    cout << "\nCategory-wise Expenses:\n";

    for (int i = 0; i < 4; i++) {
        double total = 0;

        for (int j = 0; j < expenseCount; j++) {
            if (expenses[j].category == categories[i]) {
                total += expenses[j].amount;
            }
        }

        cout << categories[i] << ": Rs. " << total << endl;
    }
}

// Set monthly budget
void setBudget() {
    cout << "\nEnter your monthly budget: ";
    cin >> monthlyBudget;

    if (monthlyBudget <= 0) {
        monthlyBudget = 0;
        cout << "\nInvalid budget!\n";
        return;
    }

    saveData();
    cout << "\nBudget saved successfully!\n";
}

// Monthly expense report
void monthlyReport() {
    double total = calculateTotal();

    cout << "\nTotal Expenses: Rs. " << total << endl;
    cout << "Monthly Budget: Rs. " << monthlyBudget << endl;

    if (monthlyBudget > 0) {
        cout << "Remaining Budget: Rs. "
             << monthlyBudget - total << endl;
    }
}

// Budget warning
void budgetWarning() {
    double total = calculateTotal();

    if (monthlyBudget <= 0) {
        cout << "\nPlease set your monthly budget first!\n";
    }
    else if (total >= monthlyBudget) {
        cout << "\nWARNING: Monthly budget exceeded!\n";
    }
    else if (total >= monthlyBudget * 0.8) {
        cout << "\nWarning: You have used 80% of your budget!\n";
    }
    else {
        cout << "\nYou are within your budget.\n";
    }
}

// Search expense by ID
void searchExpense() {
    int searchId;
    cout << "\nEnter Expense ID to search: ";
    cin >> searchId;

    for (int i = 0; i < expenseCount; i++) {
        if (expenses[i].id == searchId) {
            cout << "\nExpense Found!\n";
            cout << "ID: " << expenses[i].id << endl;
            cout << "Date: " << expenses[i].date << endl;
            cout << "Category: " << expenses[i].category << endl;
            cout << "Description: " << expenses[i].description << endl;
            cout << "Amount: Rs. " << expenses[i].amount << endl;
            return;
        }
    }

    cout << "\nExpense not found!\n";
}

// Delete expense by ID
void deleteExpense() {
    int deleteId;
    cout << "\nEnter Expense ID to delete: ";
    cin >> deleteId;

    for (int i = 0; i < expenseCount; i++) {
        if (expenses[i].id == deleteId) {
            for (int j = i; j < expenseCount - 1; j++) {
                expenses[j] = expenses[j + 1];
            }

            expenseCount--;
            saveData();

            cout << "\nExpense deleted successfully!\n";
            return;
        }
    }

    cout << "\nExpense not found!\n";
}

// Predict monthly expenses
void predictExpense() {
    int currentDay;

    cout << "\nEnter current day of the month (1-30): ";
    cin >> currentDay;

    if (currentDay < 1 || currentDay > 30) {
        cout << "\nInvalid day!\n";
        return;
    }

    double total = calculateTotal();
    double prediction = (total / currentDay) * 30;

    cout << "\nPredicted Monthly Expense: Rs. "
         << prediction << endl;

    if (monthlyBudget > 0 && prediction > monthlyBudget) {
        cout << "Alert: You may exceed your monthly budget!\n";
    }
    else {
        cout << "Your predicted expense is within budget.\n";
    }
}

// Main expense tracker menu
void expenseMenu() {
    int choice;

    loadData();

    do {
        cout << "\n========== EXPENSE TRACKER ==========\n";
        cout << "1. Add Daily Expense\n";
        cout << "2. View Expense History\n";
        cout << "3. Category-wise Expense\n";
        cout << "4. Monthly Expense Report\n";
        cout << "5. Set Monthly Budget\n";
        cout << "6. Budget Warning\n";
        cout << "7. Expense Prediction\n";
        cout << "8. Search Expense\n";
        cout << "9. Delete Expense\n";
        cout << "10. Logout\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addExpense(); break;
            case 2: viewExpenses(); break;
            case 3: categoryReport(); break;
            case 4: monthlyReport(); break;
            case 5: setBudget(); break;
            case 6: budgetWarning(); break;
            case 7: predictExpense(); break;
            case 8: searchExpense(); break;
            case 9: deleteExpense(); break;
            case 10:
                cout << "\nLogged out successfully!\n";
                break;
            default:
                cout << "\nInvalid choice!\n";
        }

    } while (choice != 10);
}

// Main function
int main() {
    int choice;

    do {
        cout << "\n====================================\n";
        cout << "   SMART DIGITAL EXPENSE TRACKER\n";
        cout << "====================================\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                registerUser();
                break;

            case 2:
                if (loginUser()) {
                    expenseMenu();
                }
                break;

            case 3:
                cout << "\nThank you for using Expense Tracker!\n";
                break;

            default:
                cout << "\nInvalid choice!\n";
        }

    } while (choice != 3);

    return 0;
}
