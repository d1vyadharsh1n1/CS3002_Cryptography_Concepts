# Hill Cipher with Hash-Based Integrity Verification

> **Course Assignment** — Classical Cipher Implementation with Custom Hashing
> **Student:** Divyadharshini R &nbsp;|&nbsp; **Roll No:** 23011102026

---

## Table of Contents

1. [Overview](#overview)
2. [Theory](#theory)
   - [Hill Cipher](#hill-cipher)
   - [Custom Hash Function](#custom-hash-function)
   - [Hash Encoding Strategy](#hash-encoding-strategy)
3. [Pipeline](#pipeline)
4. [How to Run](#how-to-run)
5. [Worked Examples](#worked-examples)
6. [Design Decisions & Constraints](#design-decisions--constraints)

---

## Overview

This project implements:

- **Encryption / Decryption** — Hill Cipher using a 3×3 key matrix
- **Integrity Verification** — Custom double polynomial rolling hash
- **Full Pipeline** — Encrypt → Hash → Append → Encrypt → Decrypt → Verify

The system guarantees both **confidentiality** (via Hill Cipher) and **integrity** (via hash comparison), simulating a basic authenticated encryption workflow.

---

## Theory

### Hill Cipher

The Hill Cipher is a **polygraphic substitution cipher** grounded in linear algebra. Unlike monoalphabetic ciphers, it encrypts multiple letters simultaneously, making frequency analysis significantly harder.

**Letter Mapping:**
```
A → 0,  B → 1,  C → 2,  ...  Z → 25
```

**Encryption** — A plaintext block of size *n* (here *n* = 3) is multiplied by the key matrix *K*:

```
C = K · P  (mod 26)
```

| Symbol | Meaning |
|--------|---------|
| `C` | Ciphertext vector (3×1) |
| `K` | Key matrix (3×3) |
| `P` | Plaintext vector (3×1) |

**Decryption** uses the modular inverse of the key matrix:

```
P = K⁻¹ · C  (mod 26)
```

> **Requirement:** The key matrix *K* must be invertible mod 26 — i.e., `det(K)` must be coprime with 26.

---

### Custom Hash Function

This project uses a **double polynomial rolling hash** — implemented entirely from scratch without any cryptographic libraries.

**Algorithm:**

```
hash1 = (hash1 × p1 + c) mod mod1
hash2 = (hash2 × p2 + c) mod mod2
```

**Constants:**

| Parameter | Value |
|-----------|-------|
| `p1` | 131 |
| `mod1` | 10⁹ + 7 (prime) |
| `p2` | 137 |
| `mod2` | 10⁹ + 9 (prime) |

**Why this hash?**

| Property | Explanation |
|----------|-------------|
| **Low collision probability** | Two independent hashes must both collide simultaneously — probability ≈ 1/(10⁹ × 10⁹) |
| **Linear time** | O(n) computation over the message |
| **Deterministic** | Same message + key always yields the same hash |
| **Distinct** | Double polynomial rolling hash with the specific prime pair (131, 137) is a unique combination. |

---

### Hash Encoding Strategy

The Hill Cipher operates exclusively on **alphabetic characters (A–Z)**. Raw numeric hash values cannot be encrypted directly.

**Solution — Encode each byte of the hash into A–Z:**

```
encoded_char = 'A' + (digit % 26)
```

This ensures:
- Full compatibility with the Hill Cipher input format
- Deterministic, reversible encoding
- No loss of distinguishability (same input always produces the same encoded hash)

---

## Pipeline

### Sender Side

```
Original Message
      │
      ▼
 Remove non-alpha characters (keep A–Z only)
      │
      ▼
 Compute double rolling hash(message + key)
      │
      ▼
 Encode hash → A–Z string
      │
      ▼
 Append encoded hash to cleaned message
      │
      ▼
 Pad with 'X' to make length a multiple of 3
      │
      ▼
 Encrypt with Hill Cipher (3×3 key matrix)
      │
      ▼
   Ciphertext
```

### Receiver Side

```
   Ciphertext
      │
      ▼
 Decrypt with Hill Cipher (K⁻¹ mod 26)
      │
      ▼
 Split → [Original Message] + [Extracted Hash]
      │
      ▼
 Recompute hash from extracted message + key
      │
      ▼
 Encode recomputed hash → A–Z string
      │
      ▼
 Compare: extracted hash == recomputed hash?
      │
   ┌──┴──┐
  YES    NO
   │      │
Verified  Tampered 
```

---

## How to Run

### Prerequisites

- A C++ compiler supporting C++11 or later (e.g., `g++`)

### Compile

```bash
g++ main.cpp -o hill
```

### Execute

```bash
./hill
```

### Input Format

```
Enter message : <your message, alphabets preferred>
Enter secret key : <any string — used to seed the hash>
```

> Non-alphabetic characters in the message are automatically removed before processing.

---

## Worked Examples

### Example 1

**Input:**
```
Message : HELLO
Key     : abc
```

**Step-by-step:**

| Step | Value |
|------|-------|
| Cleaned message | `HELLO` |
| Hash computed | `(h1, h2)` over `HELLO` + `abc` |
| Encoded hash | e.g., `BCFMQT` (A–Z representation) |
| Combined string | `HELLOBCFMQT` → padded to multiple of 3 |
| Encrypted | Ciphertext block(s) |

**Output:**
```
Integrity Verified ✓
```

---

### Example 2

**Input:**
```
Message : DIVYA
Key     : e
```

**Output:**
```
Combined  : DIVYA<encoded_hash>
Ciphertext: XXXXXXXXXXXXXXXXX
Decrypted : DIVYA<encoded_hash>

Integrity Verified 
```

The decrypted text matches the original combined string, and the re-computed hash matches the extracted hash — confirming both correct decryption and message integrity.

---

## Design Decisions & Constraints

| Constraint | Approach Taken |
|------------|----------------|
| No built-in crypto libraries | Double polynomial rolling hash implemented from scratch |
| Hill Cipher only handles A–Z | Hash encoded into A–Z via modular character mapping |
| Key matrix must be invertible mod 26 | Fixed 3×3 matrix chosen with valid modular inverse; K⁻¹ precomputed |
| Unique hash among peers | Double rolling hash with prime pair (131, 137) and dual large-prime moduli |


---

## File Structure

```
.
├── main.cpp       # Full implementation: Hill Cipher + hash + pipeline
└── README.md      # This file
```

