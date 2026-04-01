#include <iostream>
#include <vector>
using namespace std;

// HASH FUNCTION (KEYED)
int computeHash(string message, int key) {
    int hash = key;
    for (char c : message) {
        hash = (hash * 31 + c) % 100000;
    }
    return hash;
}

// KEY MATRIX (3x3)
int keyMatrix[3][3] = {
    {6, 24, 1},
    {13, 16, 10},
    {20, 17, 15}
};

// INVERSE MATRIX
int invKey[3][3] = {
    {8, 5, 10},
    {21, 8, 21},
    {21, 12, 8}
};

// MULTIPLY FUNCTION
vector<int> multiply(int mat[3][3], vector<int> vec) {
    vector<int> result(3);

    for (int i = 0; i < 3; i++) {
        result[i] = 0;
        for (int j = 0; j < 3; j++) {
            result[i] += mat[i][j] * vec[j];
        }
        result[i] %= 26;
        if (result[i] < 0) result[i] += 26;
    }
    return result;
}

// ENCRYPT
string encrypt(string text) {
    string cipher = "";

    for (int i = 0; i < text.length(); i += 3) {
        vector<int> vec(3);

        for (int j = 0; j < 3; j++) {
            vec[j] = text[i + j] - 'A';
        }

        vector<int> res = multiply(keyMatrix, vec);

        for (int j = 0; j < 3; j++) {
            cipher += (res[j] + 'A');
        }
    }

    return cipher;
}

// DECRYPT
string decrypt(string cipher) {
    string plain = "";

    for (int i = 0; i < cipher.length(); i += 3) {
        vector<int> vec(3);

        for (int j = 0; j < 3; j++) {
            vec[j] = cipher[i + j] - 'A';
        }

        vector<int> res = multiply(invKey, vec);

        for (int j = 0; j < 3; j++) {
            plain += (res[j] + 'A');
        }
    }

    return plain;
}

// MAIN FUNCTION
int main() {
    string message = "ACT";
    int secretKey = 7;

    cout << "Original Message: " << message << endl;

    // Step 1: Compute Hash
    int hash = computeHash(message, secretKey);
    cout << "Hash: " << hash << endl;

    // Step 2: Concatenate (M || H)
    string combined = message + to_string(hash);

    // Convert to uppercase letters only
    for (char &c : combined) {
        if (isdigit(c)) {
            c = (c - '0') + 'A';
        }
    }

    // Padding to make length multiple of 3
    while (combined.length() % 3 != 0) {
        combined += 'X';
    }

    cout << "Combined (M||H): " << combined << endl;

    // Step 3: Encrypt
    string cipher = encrypt(combined);
    cout << "Ciphertext: " << cipher << endl;

    // Step 4: Decrypt
    string decrypted = decrypt(cipher);
    cout << "Decrypted: " << decrypted << endl;

    // Step 5: Separate message and hash
    string extractedMessage = decrypted.substr(0, message.length());

    string extractedHashStr = decrypted.substr(message.length());

    // Convert back from A-J → digits
    string numericHash = "";
    for (char c : extractedHashStr) {
        if (c >= 'A' && c <= 'J') {
            numericHash += (c - 'A') + '0';
        }
    }

    int extractedHash = stoi(numericHash);

    // Step 6: Recompute hash
    int newHash = computeHash(extractedMessage, secretKey);

    cout << "Extracted Hash: " << extractedHash << endl;
    cout << "Recomputed Hash: " << newHash << endl;

    // Step 7: Compare
    if (extractedHash == newHash) {
        cout << "Integrity Verified (Authentic Message)" << endl;
    } else {
        cout << "Message Tampered!" << endl;
    }

    return 0;
}