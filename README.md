# Algorithm: Authentication Type A using Hill Cipher and Keyed Hash

---

## Hill Cipher

### Key Matrix Used

```
[ 6  24  1 ]
[13 16 10 ]
[20 17 15 ]
```

### Reason for Choosing This Matrix

* The determinant of this matrix is **non-zero**
* It satisfies the condition:

```
gcd(det, 26) = 1
```

* Hence, the matrix is **invertible modulo 26**
* This ensures that **decryption is possible**

---

### Encryption Formula

```
C = (K × P) mod 26
```

### Decryption Formula

```
P = (K⁻¹ × C) mod 26
```

---

## Overview

This algorithm implements a secure communication system using:

* **Authentication Type A (Authenticate-then-Encrypt)**
* **Custom Keyed Hash Function**
* **Hill Cipher (3×3 matrix)**

---

## Authentication Type A

### Flow

```id="j8r1y9"
M → H(M) → (M || H(M)) → Encrypt → Ciphertext
```

At receiver:

```id="3mqm0x"
Ciphertext → Decrypt → (M || H(M)) → Split → Verify
```

---

## ⚙️ Algorithm Steps

---

## 🔹 Sender Side

### Step 1: Input Message

* Take plaintext message `M`

---

### Step 2: Compute Keyed Hash

Use:

```id="fxf0a9"
hash = (hash * 31 + ASCII(c)) mod 100000
```

* Initialize `hash = secret_key`
* Process each character of message

---

### Step 3: Concatenate Message and Hash

```id="0b7nzb"
Combined = M || H(M)
```

---

### Step 4: Convert Hash to Alphabet (A–J Mapping)

Since Hill Cipher only supports letters:

| Digit | Letter |
| ----- | ------ |
| 0–9   | A–J    |

---

### Step 5: Padding

Ensure length is multiple of 3:

```id="7v7q5r"
If length % 3 ≠ 0 → add 'X'
```

---

### Step 6: Hill Cipher Encryption

* Split into blocks of size 3
* Convert letters to numbers (A=0 to Z=25)
* Apply:

```id="p4rj9y"
C = (K × P) mod 26
```

* Convert result back to letters

---

## 🔹 Receiver Side

---

### Step 7: Decryption

Apply inverse matrix:

```id="9f8x6k"
P = (K⁻¹ × C) mod 26
```

---

### Step 8: Separate Message and Hash

```id="zv0x4m"
Extract:
Message = first part
Hash = remaining part
```

---

### Step 9: Convert A–J back to Digits

```id="k9u2r1"
A–J → 0–9
```

---

### Step 10: Recompute Hash

```id="c2v5w7"
NewHash = H(M)
```

---

### Step 11: Verification

```id="r8t6p3"
If ExtractedHash == NewHash:
    Message is Authentic
Else:
    Message is Tampered
```

---

## Key Components

| Component     | Role               |
| ------------- | ------------------ |
| Hash Function | Integrity          |
| Secret Key    | Authentication     |
| Hill Cipher   | Confidentiality    |
| A–J Mapping   | Data compatibility |

---

## Conclusion

This algorithm ensures:

* Secure encryption of data
* Verification of message integrity
* Detection of any unauthorized modifications

---
