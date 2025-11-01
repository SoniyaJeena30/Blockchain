#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
using namespace std;

// dummy sha 256
string sha256(const string &inputData)
{
    unsigned long hash = 5381;
    for (char ch : inputData)
        hash = ((hash << 5) + hash) + ch; // hash * 33 + c

    ostringstream os;
    os << hex << hash;
    return os.str();
}

struct Block
{
    int index;
    long timestamp;
    string hash;
    string data;
    string prevHash;
    unsigned long nonce;
    Block *next;

    Block(int index, const string &data, const string &prevHash)
    {
        this->index = index;
        this->data = data;
        this->prevHash = prevHash;
        this->timestamp = time(NULL);
        this->nonce = 0;
        this->hash = calculateHash();
        this->next = NULL;
    }

    string calculateHash() const
    {
        ostringstream ss;
        ss << index << timestamp << data << prevHash << nonce;
        return sha256(ss.str());
    }
    void mineBlock(int difficulty)
    {
        hash.insert(0, difficulty, '0');
        string numOfZeros(difficulty, '0');
        long long maxAttempts = 1000; // safety limit
        long long attempts = 0;
        while (attempts < maxAttempts)
        {
            // hash = calculateHash();
            if (hash.substr(0, difficulty) == numOfZeros)
            {
                cout << "Block " << index << " mined with hash: " << hash << endl;
                return;
            }
            nonce++;
            attempts++;
        }
        cout << "Mining aborted: too many attempts, no valid hash found.\n";
        return;
    }
};

class Blockchain
{
private:
    Block *head;
    Block *tail;
    int difficulty;

public:
    Blockchain(int dif = 4)
    {
        this->difficulty = dif;
        head = new Block(0, "Genesis Block", "0");
        head->mineBlock(difficulty);
        tail = head;
        cout << "Genesis block created!\n";
    }

    ~Blockchain()
    {
        Block *current = head;
        while (current)
        {
            Block *next = current->next;
            delete current;
            current = next;
        }
    }

    void addBlock(const string &data)
    {
        Block *newBlock = new Block(tail->index + 1, data, tail->hash);
        newBlock->mineBlock(difficulty);
        tail->next = newBlock;
        tail = newBlock;
        cout << "Block " << newBlock->index << " added successfully!\n";
        return;
    }

    void displayChain() const
    {
        cout << "\nBLOCKCHAIN DETAILS";
        const Block *current = head;
        while (current)
        {
            cout << "\n\nBlock " << current->index << endl;
            cout << "-----------------------------\n";
            cout << "Timestamp: " << current->timestamp;
            cout << "\nData: " << current->data;
            cout << "\nPrevious Hash: " << current->prevHash;
            cout << "\nHash: " << current->hash;
            cout << "\nNonce: " << current->nonce;
            current = current->next;
        }
        cout << "\n\n\n";
    }

    void verifyChain() const
    {
        const Block *current = head;
        while (current && current->next)
        {
            if (current->hash != current->calculateHash())
            {
                cout << "Blockchain Vaidity: Invalid\n";
                cout << "Block " << current->index << " has been tampered with!\n";
                return;
            }
            else if (current->next->prevHash != current->hash)
            {
                cout << "Blockchain Vaidity: Invalid\n";
                cout << "Block " << current->index + 1 << " has been tampered with!\n";
                return;
            }
            current = current->next;
        }
        // last block left
        if (current && current->hash != current->calculateHash())
        {
            cout << "Blockchain Vaidity: Invalid\n";
            cout << "Block " << current->index << " has been tampered with!\n";
            return;
        }
        cout << "Blockchain Vaidity: valid\n";
        return;
    }

    void editBlock(int index, const string &newData)
    {
        Block *current = head;
        while (current)
        {
            if (current->index == index)
            {
                current->data = newData;
                current->hash = current->calculateHash();

                // Updating subsequent blocks
                Block *prev = current;
                Block *next = current->next;
                while (next)
                {
                    next->prevHash = prev->hash;
                    next->hash = next->calculateHash();
                    prev = next;
                    next = next->next;
                }
                cout << "Block " << index << " updated.\n";
                current->mineBlock(difficulty);
                return;
            }
            current = current->next;
        }
        cout << "Block " << index << " not found.\n";
        return;
    }
};

int main()
{
    Blockchain bc(3);
    int choice;
    string data;
    int index;
    do
    {
        cout << "\nMenu for Blockchain Operations\n";
        cout << "1.Add block 2.Display blockchain 3.Verify blockchain 4.Edit block data 5.Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            int n;
            cout << "enter no. of block to add: ";
            cin >> n;
            cin.ignore();
            for (int i = 0; i < n; i++)
            {
                cout << "Enter data for block" << i + 1 << " : ";
                getline(cin, data);
                bc.addBlock(data);
            }
            break;
        case 2:
            bc.displayChain();
            break;
        case 3:
            bc.verifyChain();
            break;
        case 4:
            cout << "Enter block index to edit: ";
            cin >> index;
            cin.ignore();
            cout << "Enter new data: ";
            getline(cin, data);
            bc.editBlock(index, data);
            break;
        case 5:
            cout << "exiting from progrom!\n";
            return 0;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 5);
    return 0;
}
