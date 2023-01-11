# ToolkitForRND
This project aims to prepare an analysis toolkit for a random sequence for accelerating one's research or interests on the random numbers.

The functionality of each code is as follows:
1. dawg.c: Constructing the dynamic acyclic graph from a given sequence and evaluating the maximum order complexity. Refer [1] for the detail.
2. dist.py: Counting up the number of appearances of bits, where each bit pattern is counted by exploring the sequence with allowing overlapping.
3. auto.py: Calculating the autocorrelation of a given sequence.
4. bm.sh + berlekamp-massey.cpp (in bm.tar): Calculating the linear compelxity of a given sequence over a field by Berlekamp-Massey's algorithm [2]. 

[1] Jansen C.J.A. (1991) The Maximum Order Complexity of Sequence Ensembles. In: Davies D.W. (eds) Advances in Cryptology — EUROCRYPT ’91. EUROCRYPT 1991. Lecture Notes in Computer Science, vol 547. Springer, Berlin, Heidelberg <br />
[2] [Berlekamp-Massey algorithm](https://encyclopediaofmath.org/index.php?title=Berlekamp-Massey_algorithm)

