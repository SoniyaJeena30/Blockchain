/*#include <iostream>
#include <unordered_map>
#include <string>
#include <cctype> // for isupper, islower, isdigit, ispunct
#include <sstream>*/

#include <bits/stdc++.h>
using namespace std;

// Converting plain Password into djb2 hash
string passwordHash(const string &password)
{
    unsigned long hash = 5381;
    for (char c : password)
        // djb2 hash function (Daniel J. Bernstein)
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    stringstream ss;
    ss << hex << hash;
    return ss.str();
}

// Check password strength and return true if strong
bool isPasswordStrong(const string &password)
{
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;

    for (char ch : password)
    {
        if (isupper(ch))
            hasUpper = true;
        else if (islower(ch))
            hasLower = true;
        else if (isdigit(ch))
            hasDigit = true;
        else
            hasSpecial = true;
    }
    int total = hasUpper + hasLower + hasDigit + hasSpecial;

    if (total < 4)
    {
        cout << "Password Strength: Weak (Suggestion: use all lowecase,uppercase,digits and special characters)\n";
        return false;
    }
    else
    {
        cout << "Password Strength: Strong\n";
        return true;
    }
}

int main()
{
    unordered_map<string, string> users;
    int choice;
    string username, password;

    while (true)
    {
        cout << "\n1. Register\n2. Login\n3. Exit\nEnter choice: ";
        cin >> choice;
        cin.ignore(); // clear newline from input buffer

        if (choice == 1)
        {
            // UserName
            while (true)
            {
                cout << "Enter username: ";
                getline(cin, username);

                if (users.find(username) != users.end()) // users.end() = iterator one past the last element.
                    cout << "Username already exists! Please choose another.\n";
                else
                    break; // unique username
            }
            // Password
            do
            {
                cout << "Enter password: ";
                getline(cin, password);
            } while (!isPasswordStrong(password));

            // Storing password as hashes
            users[username] = passwordHash(password);
            cout << "Register successfully, " << username << "\n";
        }
        else if (choice == 2)
        {
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter password: ";
            getline(cin, password);

            string hashedLoginPass = passwordHash(password);

            if (users.find(username) != users.end() && users[username] == hashedLoginPass)
                cout << "Login successful, " << username << endl;
            else
                cout << "Invalid username or password.\n";
        }
        else if (choice == 3)
        {
            cout << "Exiting...\n";
            break;
        }
        else
            cout << "Invalid choice.\n";
    }
    return 0;
}
