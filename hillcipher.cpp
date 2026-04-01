#include <iostream>
#include <vector>
#include <string>
using namespace std;

// HASH FUNCTION 
// Double polynomial hash
pair<long long, long long> computeHash(const string &message, const string &key) {
    const long long p1 = 131, mod1 = 1e9 + 7;
    const long long p2 = 137, mod2 = 1e9 + 9;

    long long hash1 = 0, hash2 = 0;
    string combined = key + message + key;

    for (char c : combined) {
        hash1 = (hash1 * p1 + c) % mod1;
        hash2 = (hash2 * p2 + c) % mod2;
    }

    return {hash1, hash2};
}

// HASH ENCODING
// Convert hash → A-Z only string (safe for Hill Cipher)
string encodeHash(pair<long long, long long> h) {
    string s = to_string(h.first) + "#" + to_string(h.second);
    string encoded = "";

    for (char c : s) {
        encoded += char('A' + (c % 26)); // force into A-Z
    }

    return encoded;
}

// HILL CIPHER

// Key matrix (3x3)
int keyMatrix[3][3] = {
    {6, 24, 1},
    {13, 16, 10},
    {20, 17, 15}
};

// Precomputed inverse matrix mod 26
int invKey[3][3] = {
    {8, 5, 10},
    {21, 8, 21},
    {21, 12, 8}
};

// Char → number
int charToNum(char c) {
    return c - 'A';
}

// Number → char
char numToChar(int n) {
    return (n % 26 + 26) % 26 + 'A';
}

// Matrix multiply mod 26
vector<int> multiply(int mat[3][3], const vector<int> &vec) {
    vector<int> result(3, 0);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i] += mat[i][j] * vec[j];
        }
        result[i] = (result[i] % 26 + 26) % 26; //handles negative numbers also
    }

    return result;
}

// Encrypt
string hillEncrypt(const string &text) {
    string cipher = "";

    for (int i = 0; i < text.length(); i += 3) {
        vector<int> vec(3);

        for (int j = 0; j < 3; j++) {
            vec[j] = charToNum(text[i + j]);
        }

        vector<int> res = multiply(keyMatrix, vec);

        for (int val : res) {
            cipher += numToChar(val);
        }
    }

    return cipher;
}

// Decrypt
string hillDecrypt(const string &cipher) {
    string plain = "";

    for (int i = 0; i < cipher.length(); i += 3) {
        vector<int> vec(3);

        for (int j = 0; j < 3; j++) {
            vec[j] = charToNum(cipher[i + j]);
        }

        vector<int> res = multiply(invKey, vec);

        for (int val : res) {
            plain += numToChar(val);
        }
    }

    return plain;
}

// MAIN 
int main() {
    string message, key;

    cout << "Enter message: ";
    cin >> message;

    cout << "Enter secret key: ";
    cin >> key;

    // Step 1: Clean message (A-Z only)
    string cleanMessage = "";
    for (char c : message) {
        if (isalpha(c)) cleanMessage += toupper(c);
    }

    // Step 2: Compute hash
    auto hashVal = computeHash(cleanMessage, key);
    string hashStr = encodeHash(hashVal);

    int hashLen = hashStr.length();

    // Step 3: Combine message + hash
    string combined = cleanMessage + hashStr;

    // Step 4: Padding
    while (combined.length() % 3 != 0) {
        combined += 'X';
    }

    cout << "\nCombined: " << combined << endl;

    // Step 5: Encrypt
    string cipher = hillEncrypt(combined);
    cout << "Ciphertext: " << cipher << endl;

    // ================= RECEIVER =================

    // Step 6: Decrypt
    string decrypted = hillDecrypt(cipher);
    cout << "Decrypted: " << decrypted << endl;

    // Step 7: Extract original message
    string extractedMessage = decrypted.substr(0, cleanMessage.length());

    // Step 8: Extract hash
    string extractedHash = decrypted.substr(cleanMessage.length(), hashLen);

    // Step 9: Recompute hash
    string recomputedHash = encodeHash(computeHash(extractedMessage, key));

    // Step 10: Verify
    if (recomputedHash == extractedHash) {
        cout << "Integrity Verified\n";
    } else {
        cout << "Message Tampered\n";
    }

    return 0;
}
