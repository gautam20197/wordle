# Solver for wordle

The solution uses a prefix tree with the root node having children of all the characters with which a word can start.
We get the allowed_words (about 13k) and preprocess them into the prefix tree.

The prefix tree with the root and the children looks like:

<pre>Node ($, 12972)
--Node (z, 105)
--Node (y, 181)
--Node (x, 16)
--Node (w, 413)
--Node (v, 242)
--Node (u, 189)
--Node (t, 815)
--Node (s, 1565)
--Node (r, 628)
--Node (q, 78)
--Node (p, 859)
--Node (o, 262)
--Node (n, 325)
--Node (a, 737)
--Node (b, 909)
--Node (c, 922)
--Node (d, 685)
--Node (e, 303)
--Node (f, 598)
--Node (g, 638)
--Node (h, 489)
--Node (i, 165)
--Node (j, 202)
--Node (k, 376)
--Node (l, 577)
--Node (m, 693)
</pre>

Using basic frequency distributions and a greedy algorithm, the best first word comes to be STONY.
