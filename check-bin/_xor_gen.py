#!/usr/bin/env python3
"""Dev helper: print C XOR arrays for flag hex digests. Not installed on VM."""
key = b"discover101"
for name, plain in [
    ("nano", "b51eeea2920ad9f39864d129cfcacc42"),
    ("rm", "70d8bf6b18efd7949d10f5d61485264b"),
    ("livrable1", "189e15d024987af4682d4b5ff4c6fa7e"),
    ("grep_aa7566", "2dc61dfb1569612e42aafc076574ac91"),
    ("sort_grades", "25a8b459f6d72ff79bbb45f6c180cf99"),
    ("sort_on_time", "721ebc3d07eca00cef8b72443fd77b5a"),
    ("head_last5", "c84cbb04663da5b3328fbc6e94a101fe"),
    ("head_first10", "b8959e6546babad95facba150184cd40"),
    ("head_sample", "c2061874f9722c1e64435b987f80bfdb"),
    ("cut_grades", "c60b86fe7360b893f5b0ef92574a14cf"),
    ("cut_cars", "2cfba3c4bf29b8720993e860f0662670"),
    ("sed", "ac2cc53f7c5e91b83bb6c01ce82bfc62"),
    ("find_concat", "f346c3a39948bb6b4f811a853548e229"),
    ("grep_avance", "10908 92"),
]:
    enc = [ord(plain[i]) ^ key[i % len(key)] for i in range(len(plain))]
    print(f"/* {name} */")
    print("{" + ", ".join(str(b) for b in enc) + "}")
