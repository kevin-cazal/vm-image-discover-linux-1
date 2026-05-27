#!/usr/bin/env python3
"""Dev helper: print C XOR arrays for flag hex digests. Not installed on VM."""
key = b"discover101"
for name, plain in [
    ("micro", "b51eeea2920ad9f39864d129cfcacc42"),
    ("rm", "70d8bf6b18efd7949d10f5d61485264b"),
    ("livrable1", "189e15d024987af4682d4b5ff4c6fa7e"),
]:
    enc = [ord(plain[i]) ^ key[i % len(key)] for i in range(len(plain))]
    print(f"/* {name} */")
    print("{" + ", ".join(str(b) for b in enc) + "}")
