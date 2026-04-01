# Hill Cipher with Hash-Based Integrity Verification

## Overview

This project implements the **Hill Cipher (3×3 matrix)** along with a **custom hashing mechanism** to ensure both **confidentiality** and **integrity** of the message.

* **Encryption/Decryption:** Hill Cipher (classical symmetric cipher)
* **Hashing:** Custom double polynomial rolling hash
* **Pipeline:** Encrypt → Hash → Append → Encrypt → Decrypt → Verify

---

## Theory

### Hill Cipher

The Hill Cipher is a **polygraphic substitution cipher** based on **linear algebra**.

* Each letter is mapped:

  ```
  A → 0, B → 1, ..., Z → 25
  ```
* A block of size **n (here n = 3)** is taken.
* Encryption is done using matrix multiplication:

[
C = K \cdot P \mod 26
]

Where:

* ( C ) = Cipher vector
* ( K ) = Key matrix (3×3)
* ( P ) = Plaintext vector

Decryption uses the **inverse of the key matrix**:

[
P = K^{-1} \cdot C \mod 26
]

The key matrix must be **invertible mod 26**.

---

### Hash Function (Custom)

This project uses a **double polynomial rolling hash**:

* Two hash values are computed:

  ```
  hash1 = (hash1 * p1 + c) % mod1
  hash2 = (hash2 * p2 + c) % mod2
  ```

* Constants used:

  * ( p1 = 131 ), ( mod1 = 10^9 + 7 )
  * ( p2 = 137 ), ( mod2 = 10^9 + 9 )

### Why this hash?

* Reduces collision probability (double hashing)
* Efficient (O(n))
* Easy to implement from scratch (meets constraint)
* No external libraries used

---

### Why Encode Hash into A–Z?

Hill Cipher only supports **alphabetic input (A–Z)**.

So:

* The numeric hash is converted into an **A–Z string**
* Each character is mapped using:

  ```
  encoded_char = 'A' + (ASCII_value % 26)
  ```

This ensures:

* Compatibility with Hill Cipher
* Deterministic encoding (same input → same output)

---

## How It Works

### Sender Side

1. Clean message (keep only A–Z)
2. Compute hash of message + key
3. Encode hash into A–Z format
4. Append hash to message
5. Pad with 'X' to make length multiple of 3
6. Encrypt using Hill Cipher

---

### Receiver Side

1. Decrypt ciphertext
2. Extract original message
3. Extract encoded hash
4. Recompute hash from extracted message
5. Encode recomputed hash
6. Compare both hashes

---

### Integrity Check

```
If recomputed_hash == extracted_hash → Verified
Else → Tampered
```

---

## How to Run

### Compile

```
g++ main.cpp -o hill
```

### Execute

```
./hill
```

### Input

* Enter message (alphabets preferred)
* Enter secret key (string)

---

## Worked Examples

### Example 1

**Input:**

```
Message: HELLO
Key: abc
```

**Process:**

* Cleaned message → HELLO
* Hash computed → (h1, h2)
* Encoded hash → ABCXYZ...
* Combined → HELLOABCXYZ...
* Encrypted → Ciphertext

**Output:**

```
Integrity Verified
```

---

### Example 2

**Input:**

```
Message: DIVYA
Key: e
```

**Output:**

```
Combined: DIVYA<encoded_hash>
Ciphertext: XXXXXXXXX
Decrypted: DIVYA<encoded_hash>
Integrity Verified
```

---

## Test Script (Round-Trip)

The program itself demonstrates:

```
Encrypt → Hash → Append → Encrypt
Decrypt → Extract → Re-hash → Compare
```

This ensures:

* Correct decryption
* Integrity verification

---


---

## Assumptions

* Only uppercase letters (A–Z) are used in encryption
* Non-alphabet characters are removed
* Fixed 3×3 key matrix is used
* Inverse matrix is precomputed

---



## 🏁 Conclusion

This project successfully demonstrates:

* Classical encryption using Hill Cipher
* Custom hashing for integrity
* Secure message transmission with verification

---

## Author

* Name: *Divyadharshini R*
* Roll Number: *23011102026*

---
