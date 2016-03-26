// g++ -std=c++11 assign2.cpp -o assign2 && ./assign2
//
// What have I learned ?
//
// 1) std library: I find it a very rich set of library providing the support of many useful 
//    data structures, including map, priority_queue, vector, list. These make programming more
//    advanced algorithms like Dijkstra's Algorithm easy.
// 2) The c++11 standard is nice, and coding in cpp is easier than I thought.
// 3) cpp is fast and can surprisingly be fun.
// 4) g++ is nice, and powerful.
// 5) Dijkstra's Algorithm is simpler than I can remember.
// 6) Don't over abstract.  Choosing the right level of abstraction is critical for a nice and
//    clean solution.
// 7) Clearly, the higher the density of a randomly generated graph, the lower the expected length
//    of the shortest path, since the higher density results in a higher probability of finding a
//    shorter alternative to get from one node to another.
//
// Sample Output 1 (without all the details):
//
// Average for 20% density: 5.36735
// Average for 40% density: 3.7551
//
// Sample Output 2: (with all the Graph internals and shortest path details):
// (setting IS_PRINT_GRAPH = true and IS_PRINT_PATH = true)

/*
== Graph ==

0 -> 14 (1), 21 (2), 48 (3), 32 (9), 17 (10), 27 (8), 35 (5), 16 (8), 47 (9), 19 (2), 38 (4), 8 (9), 45 (1), 46 (6)
1 -> 47 (3), 25 (5), 38 (7), 4 (5), 14 (7), 45 (9), 46 (6), 11 (3), 31 (8), 23 (10), 6 (2), 10 (7), 16 (4), 19 (8), 17 (6), 48 (1)
2 -> 14 (3), 27 (1), 37 (8), 4 (8)
3 -> 26 (8), 46 (2), 31 (9), 39 (5), 47 (9), 8 (5), 6 (7)
4 -> 1 (5), 45 (6), 13 (6), 42 (4), 46 (6), 2 (8), 26 (4), 18 (2)
5 -> 25 (1), 26 (2), 36 (4), 10 (6), 39 (5), 43 (9)
6 -> 14 (9), 38 (6), 1 (2), 43 (10), 31 (4), 3 (7)
7 -> 41 (7), 10 (3), 32 (4), 15 (2), 26 (6), 47 (10), 25 (1), 37 (1), 21 (1), 9 (6), 17 (1), 43 (5)
8 -> 32 (8), 13 (1), 18 (7), 45 (3), 16 (7), 42 (3), 0 (9), 3 (5)
9 -> 48 (2), 37 (10), 13 (6), 44 (10), 33 (9), 7 (6)
10 -> 7 (3), 26 (8), 11 (1), 47 (9), 19 (3), 34 (7), 1 (7), 5 (6)
11 -> 36 (4), 49 (2), 44 (8), 10 (1), 1 (3), 40 (5), 26 (1), 31 (8)
12 -> 42 (7), 34 (8), 15 (7), 28 (2), 27 (4), 20 (7), 32 (6), 18 (10)
13 -> 39 (4), 8 (1), 46 (6), 4 (6), 9 (6), 16 (3)
14 -> 0 (1), 2 (3), 30 (10), 1 (7), 6 (9), 21 (1), 45 (4)
15 -> 21 (3), 30 (2), 12 (7), 7 (2), 20 (9), 49 (6), 33 (3)
16 -> 17 (3), 48 (6), 0 (8), 8 (7), 18 (10), 1 (4), 13 (3), 26 (10), 29 (10)
17 -> 16 (3), 31 (6), 29 (8), 45 (4), 0 (10), 42 (2), 41 (4), 7 (1), 37 (2), 26 (3), 1 (6), 36 (6)
18 -> 36 (6), 31 (3), 8 (7), 32 (7), 16 (10), 4 (2), 42 (2), 12 (10)
19 -> 36 (3), 29 (7), 41 (6), 0 (2), 10 (3), 23 (1), 1 (8)
20 -> 21 (6), 42 (7), 15 (9), 43 (6), 12 (7), 48 (9)
21 -> 0 (2), 15 (3), 20 (6), 22 (1), 29 (4), 14 (1), 7 (1), 45 (5)
22 -> 21 (1), 26 (10), 28 (2), 27 (7), 30 (4), 48 (4), 38 (9), 24 (5)
23 -> 29 (4), 49 (3), 35 (10), 1 (10), 26 (2), 19 (1), 27 (8)
24 -> 41 (10), 22 (5)
25 -> 30 (6), 5 (1), 1 (5), 36 (9), 41 (6), 7 (1), 45 (2)
26 -> 3 (8), 10 (8), 22 (10), 45 (3), 5 (2), 7 (6), 4 (4), 11 (1), 23 (2), 17 (3), 48 (3), 16 (10)
27 -> 22 (7), 0 (8), 2 (1), 12 (4), 40 (1), 31 (8), 43 (8), 23 (8)
28 -> 22 (2), 40 (2), 46 (4), 12 (2)
29 -> 45 (4), 49 (9), 23 (4), 19 (7), 17 (8), 21 (4), 48 (7), 33 (1), 47 (6), 32 (2), 44 (9), 31 (2), 16 (10)
30 -> 25 (6), 15 (2), 14 (10), 36 (2), 22 (4)
31 -> 45 (6), 32 (4), 3 (9), 17 (6), 18 (3), 35 (5), 1 (8), 6 (4), 27 (8), 29 (2), 11 (8)
32 -> 8 (8), 39 (4), 7 (4), 31 (4), 0 (9), 18 (7), 45 (5), 29 (2), 48 (6), 41 (2), 12 (6)
33 -> 46 (6), 48 (10), 39 (3), 29 (1), 15 (3), 9 (9), 49 (6)
34 -> 49 (1), 12 (8), 10 (7), 47 (4)
35 -> 42 (2), 49 (7), 43 (8), 23 (10), 40 (8), 47 (2), 0 (5), 31 (5), 46 (8), 37 (1), 44 (9)
36 -> 11 (4), 18 (6), 19 (3), 25 (9), 40 (8), 37 (6), 41 (10), 30 (2), 46 (7), 39 (5), 5 (4), 17 (6)
37 -> 36 (6), 9 (10), 49 (6), 2 (8), 7 (1), 35 (1), 17 (2)
38 -> 1 (7), 44 (7), 48 (8), 6 (6), 0 (4), 22 (9), 47 (7)
39 -> 13 (4), 32 (4), 33 (3), 36 (5), 3 (5), 5 (5)
40 -> 36 (8), 28 (2), 35 (8), 11 (5), 27 (1), 49 (8)
41 -> 7 (7), 24 (10), 36 (10), 19 (6), 25 (6), 17 (4), 32 (2)
42 -> 35 (2), 12 (7), 20 (7), 4 (4), 47 (10), 48 (9), 17 (2), 8 (3), 49 (5), 18 (2)
43 -> 35 (8), 20 (6), 6 (10), 27 (8), 7 (5), 47 (10), 5 (9)
44 -> 11 (8), 38 (7), 46 (6), 29 (9), 9 (10), 35 (9)
45 -> 31 (6), 29 (4), 17 (4), 1 (9), 4 (6), 26 (3), 32 (5), 8 (3), 0 (1), 25 (2), 21 (5), 14 (4)
46 -> 33 (6), 3 (2), 48 (7), 28 (4), 13 (6), 44 (6), 49 (9), 4 (6), 1 (6), 36 (7), 35 (8), 0 (6)
47 -> 1 (3), 10 (9), 35 (2), 29 (6), 42 (10), 0 (9), 7 (10), 3 (9), 34 (4), 38 (7), 43 (10)
48 -> 9 (2), 16 (6), 0 (3), 46 (7), 29 (7), 33 (10), 38 (8), 32 (6), 42 (9), 22 (4), 49 (7), 26 (3), 1 (1), 20 (9)
49 -> 34 (1), 11 (2), 29 (9), 35 (7), 23 (3), 37 (6), 46 (9), 15 (6), 40 (8), 48 (7), 42 (5), 33 (6)


== Shortest path ==

0 to 1: 4, 0-48-1
0 to 2: 4, 0-14-2
0 to 3: 8, 0-46-3
0 to 4: 7, 0-45-4
0 to 5: 4, 0-45-25-5
0 to 6: 6, 0-48-1-6
0 to 7: 3, 0-21-7
0 to 8: 4, 0-45-8
0 to 9: 5, 0-48-9
0 to 10: 5, 0-19-10
0 to 11: 5, 0-45-26-11
0 to 12: 7, 0-21-22-28-12
0 to 13: 5, 0-45-8-13
0 to 14: 1, 0-14
0 to 15: 5, 0-21-15
0 to 16: 7, 0-21-7-17-16
0 to 17: 4, 0-21-7-17
0 to 18: 8, 0-21-7-17-42-18
0 to 19: 2, 0-19
0 to 20: 8, 0-21-20
0 to 21: 2, 0-21
0 to 22: 3, 0-21-22
0 to 23: 3, 0-19-23
0 to 24: 8, 0-21-22-24
0 to 25: 3, 0-45-25
0 to 26: 4, 0-45-26
0 to 27: 5, 0-14-2-27
0 to 28: 5, 0-21-22-28
0 to 29: 5, 0-45-29
0 to 30: 7, 0-19-36-30
0 to 31: 7, 0-45-31
0 to 32: 6, 0-45-32
0 to 33: 6, 0-45-29-33
0 to 34: 7, 0-19-23-49-34
0 to 35: 5, 0-35
0 to 36: 5, 0-19-36
0 to 37: 4, 0-21-7-37
0 to 38: 4, 0-38
0 to 39: 9, 0-45-29-33-39
0 to 40: 6, 0-14-2-27-40
0 to 41: 8, 0-19-41
0 to 42: 6, 0-21-7-17-42
0 to 43: 8, 0-21-7-43
0 to 44: 11, 0-38-44
0 to 45: 1, 0-45
0 to 46: 6, 0-46
0 to 47: 7, 0-35-47
0 to 48: 3, 0-48
0 to 49: 6, 0-19-23-49
Average for 20% density: 5.34694

== Graph ==

0 -> 45 (10), 33 (7), 27 (6), 25 (5), 21 (2), 26 (1), 11 (8), 1 (2), 40 (10), 29 (4), 35 (10), 32 (10), 36 (5), 3 (9), 16 (4), 5 (5)
1 -> 25 (2), 41 (7), 39 (6), 20 (6), 46 (8), 0 (2), 18 (1), 10 (4), 43 (8), 19 (8), 33 (7), 29 (9), 48 (2), 14 (10)
2 -> 11 (3), 45 (7), 47 (7), 6 (6), 48 (8), 39 (4), 34 (1), 24 (8), 12 (3), 4 (6), 25 (1), 46 (5), 18 (5), 19 (8), 36 (2), 28 (8)
3 -> 36 (6), 14 (10), 40 (6), 44 (3), 43 (2), 15 (10), 21 (10), 19 (6), 48 (1), 37 (7), 18 (9), 28 (6), 30 (2), 0 (9), 16 (7), 46 (7), 24 (2)
4 -> 11 (10), 35 (5), 7 (5), 39 (9), 44 (2), 34 (6), 15 (1), 2 (6), 47 (2), 25 (1), 23 (4), 38 (5), 29 (3), 16 (5)
5 -> 18 (5), 42 (6), 28 (8), 9 (2), 10 (7), 35 (2), 11 (5), 12 (2), 24 (10), 16 (2), 47 (4), 32 (5), 8 (5), 30 (5), 0 (5), 31 (3)
6 -> 12 (8), 2 (6), 16 (6), 22 (9), 23 (10), 33 (9), 34 (6), 17 (6), 41 (1), 36 (8), 14 (7)
7 -> 42 (8), 19 (6), 25 (7), 11 (3), 4 (5), 45 (10), 32 (3), 16 (2), 21 (9), 33 (2), 20 (5)
8 -> 11 (7), 43 (8), 38 (7), 10 (10), 16 (2), 14 (4), 23 (4), 5 (5), 13 (6), 47 (1)
9 -> 18 (9), 10 (1), 5 (2), 45 (9), 24 (8), 46 (7), 17 (2), 14 (5), 25 (7), 33 (4), 49 (9), 44 (1), 31 (2), 37 (2)
10 -> 29 (2), 46 (3), 9 (1), 40 (6), 5 (7), 39 (1), 27 (5), 25 (4), 41 (5), 19 (2), 8 (10), 35 (9), 1 (4), 48 (10), 47 (7), 43 (3), 28 (9)
11 -> 4 (10), 16 (1), 49 (9), 2 (3), 36 (5), 26 (5), 8 (7), 7 (3), 42 (8), 31 (5), 0 (8), 5 (5), 14 (10), 41 (8), 24 (3), 17 (9), 25 (5), 23 (6), 46 (4)
12 -> 20 (9), 6 (8), 35 (3), 30 (5), 32 (9), 17 (7), 2 (3), 5 (2), 38 (4), 31 (5), 43 (9), 44 (5), 18 (7), 46 (8)
13 -> 24 (10), 41 (2), 42 (3), 21 (3), 38 (10), 36 (1), 22 (7), 31 (5), 27 (2), 16 (3), 26 (2), 30 (10), 15 (2), 8 (6), 20 (8), 48 (8), 43 (10)
14 -> 3 (10), 17 (8), 28 (5), 31 (1), 29 (1), 9 (5), 38 (1), 11 (10), 34 (8), 8 (4), 41 (6), 32 (7), 43 (9), 37 (10), 6 (7), 1 (10)
15 -> 39 (3), 18 (2), 29 (4), 3 (10), 47 (9), 38 (9), 40 (1), 42 (7), 45 (8), 16 (6), 4 (1), 28 (9), 27 (1), 37 (9), 24 (3), 20 (2), 46 (8), 13 (2), 35 (5), 33 (3)
16 -> 11 (1), 22 (5), 34 (5), 6 (6), 47 (6), 7 (2), 37 (5), 8 (2), 15 (6), 41 (3), 5 (2), 18 (6), 13 (3), 31 (2), 46 (1), 3 (7), 0 (4), 4 (5)
17 -> 14 (8), 28 (5), 30 (9), 9 (2), 12 (7), 34 (8), 36 (4), 37 (10), 40 (3), 26 (8), 6 (6), 11 (9), 43 (7), 41 (2), 18 (9), 35 (5), 32 (4), 46 (9)
18 -> 5 (5), 15 (2), 9 (9), 34 (3), 39 (5), 49 (8), 40 (7), 23 (4), 22 (10), 42 (5), 1 (1), 3 (9), 16 (6), 29 (4), 12 (7), 21 (2), 2 (5), 19 (3), 17 (9), 36 (5), 43 (6), 28 (10), 44 (6)
19 -> 7 (6), 33 (5), 47 (3), 36 (2), 27 (10), 41 (2), 10 (2), 49 (9), 3 (6), 23 (10), 28 (9), 1 (8), 25 (5), 18 (3), 2 (8)
20 -> 12 (9), 1 (6), 44 (1), 38 (4), 30 (8), 48 (2), 39 (10), 42 (6), 27 (1), 35 (10), 15 (2), 43 (6), 7 (5), 13 (8)
21 -> 27 (7), 23 (3), 13 (3), 48 (5), 0 (2), 3 (10), 34 (6), 7 (9), 35 (5), 18 (2)
22 -> 16 (5), 42 (1), 43 (8), 13 (7), 18 (10), 6 (9), 46 (3), 36 (9), 37 (8)
23 -> 33 (6), 21 (3), 28 (3), 34 (5), 44 (8), 18 (4), 6 (10), 19 (10), 4 (4), 29 (10), 8 (4), 11 (6), 37 (3), 25 (6)
24 -> 13 (10), 34 (6), 40 (2), 26 (1), 44 (4), 9 (8), 30 (5), 2 (8), 5 (10), 11 (3), 15 (3), 38 (1), 42 (4), 3 (2), 29 (5), 39 (6)
25 -> 1 (2), 35 (5), 7 (7), 46 (8), 0 (5), 32 (2), 10 (4), 41 (2), 30 (1), 4 (1), 28 (4), 9 (7), 2 (1), 47 (3), 39 (8), 11 (5), 19 (5), 31 (2), 40 (6), 23 (6)
26 -> 40 (1), 11 (5), 24 (1), 32 (10), 0 (1), 42 (8), 49 (7), 17 (8), 33 (4), 13 (2), 28 (2), 45 (4), 30 (5)
27 -> 38 (3), 32 (9), 21 (7), 0 (6), 10 (5), 19 (10), 46 (10), 15 (1), 13 (2), 36 (5), 44 (2), 34 (8), 20 (1), 37 (3)
28 -> 49 (3), 14 (5), 5 (8), 23 (3), 17 (5), 15 (9), 3 (6), 25 (4), 19 (9), 26 (2), 42 (2), 2 (8), 18 (10), 10 (9)
29 -> 10 (2), 15 (4), 42 (2), 45 (2), 14 (1), 37 (9), 0 (4), 23 (10), 31 (2), 18 (4), 48 (2), 4 (3), 1 (9), 43 (8), 24 (5), 40 (6)
30 -> 41 (1), 17 (9), 12 (5), 45 (6), 20 (8), 25 (1), 24 (5), 31 (2), 3 (2), 44 (1), 38 (9), 13 (10), 47 (8), 34 (3), 40 (6), 5 (5), 26 (5)
31 -> 38 (10), 39 (3), 14 (1), 35 (5), 42 (5), 11 (5), 13 (5), 12 (5), 30 (2), 29 (2), 16 (2), 37 (1), 9 (2), 25 (2), 5 (3)
32 -> 27 (9), 45 (10), 25 (2), 26 (10), 12 (9), 7 (3), 38 (10), 0 (10), 14 (7), 5 (5), 47 (5), 17 (4), 39 (10)
33 -> 23 (6), 38 (7), 19 (5), 0 (7), 6 (9), 9 (4), 26 (4), 7 (2), 1 (7), 40 (3), 45 (9), 49 (8), 15 (3)
34 -> 35 (1), 24 (6), 36 (7), 16 (5), 43 (1), 48 (4), 18 (3), 23 (5), 21 (6), 2 (1), 17 (8), 4 (6), 14 (8), 6 (6), 27 (8), 38 (3), 41 (8), 30 (3), 37 (4), 39 (4), 44 (8)
35 -> 34 (1), 25 (5), 4 (5), 12 (3), 40 (7), 31 (5), 5 (2), 10 (9), 43 (3), 0 (10), 39 (5), 21 (5), 20 (10), 17 (5), 15 (5)
36 -> 46 (8), 3 (6), 11 (5), 34 (7), 48 (8), 19 (2), 13 (1), 37 (6), 17 (4), 22 (9), 27 (5), 0 (5), 47 (7), 18 (5), 2 (2), 6 (8), 49 (5), 43 (7)
37 -> 42 (2), 39 (4), 46 (9), 36 (6), 45 (10), 16 (5), 29 (9), 3 (7), 17 (10), 15 (9), 47 (7), 31 (1), 9 (2), 40 (4), 34 (4), 22 (8), 23 (3), 27 (3), 14 (10), 49 (9)
38 -> 33 (7), 27 (3), 31 (10), 8 (7), 13 (10), 15 (9), 20 (4), 44 (2), 14 (1), 12 (4), 46 (4), 32 (10), 4 (5), 34 (3), 30 (9), 24 (1)
39 -> 15 (3), 1 (6), 31 (3), 49 (3), 10 (1), 37 (4), 18 (5), 2 (4), 40 (8), 4 (9), 20 (10), 35 (5), 25 (8), 42 (9), 46 (7), 34 (4), 24 (6), 32 (10)
40 -> 26 (1), 3 (6), 24 (2), 10 (6), 35 (7), 15 (1), 45 (7), 18 (7), 39 (8), 0 (10), 17 (3), 42 (2), 44 (5), 37 (4), 48 (2), 25 (6), 33 (3), 30 (6), 29 (6), 47 (5)
41 -> 30 (1), 13 (2), 1 (7), 44 (10), 19 (2), 10 (5), 25 (2), 42 (3), 16 (3), 11 (8), 14 (6), 34 (8), 48 (6), 17 (2), 6 (1)
42 -> 7 (8), 37 (2), 5 (6), 22 (1), 13 (3), 29 (2), 15 (7), 11 (8), 44 (1), 49 (2), 18 (5), 31 (5), 26 (8), 41 (3), 20 (6), 40 (2), 39 (9), 28 (2), 24 (4), 45 (10)
43 -> 34 (1), 8 (8), 22 (8), 3 (2), 48 (5), 35 (3), 12 (9), 1 (8), 17 (7), 14 (9), 20 (6), 18 (6), 29 (8), 49 (9), 36 (7), 10 (3), 13 (10)
44 -> 3 (3), 41 (10), 20 (1), 24 (4), 23 (8), 42 (1), 38 (2), 4 (2), 49 (9), 27 (2), 12 (5), 9 (1), 30 (1), 40 (5), 18 (6), 34 (8)
45 -> 0 (10), 2 (7), 48 (9), 32 (10), 9 (9), 29 (2), 40 (7), 7 (10), 30 (6), 15 (8), 37 (10), 49 (8), 42 (10), 33 (9), 26 (4)
46 -> 36 (8), 10 (3), 25 (8), 37 (9), 49 (6), 9 (7), 27 (10), 1 (8), 48 (4), 47 (6), 22 (3), 38 (4), 2 (5), 16 (1), 12 (8), 3 (7), 15 (8), 39 (7), 11 (4), 17 (9)
47 -> 19 (3), 2 (7), 15 (9), 16 (6), 46 (6), 4 (2), 37 (7), 25 (3), 36 (7), 5 (4), 30 (8), 32 (5), 10 (7), 8 (1), 40 (5)
48 -> 45 (9), 36 (8), 21 (5), 34 (4), 2 (8), 46 (4), 3 (1), 43 (5), 20 (2), 49 (2), 29 (2), 41 (6), 40 (2), 10 (10), 1 (2), 13 (8)
49 -> 11 (9), 28 (3), 39 (3), 46 (6), 18 (8), 42 (2), 19 (9), 44 (9), 45 (8), 26 (7), 9 (9), 48 (2), 43 (9), 33 (8), 36 (5), 37 (9)


== Shortest path ==

0 to 1: 2, 0-1
0 to 2: 5, 0-1-25-2
0 to 3: 4, 0-26-24-3
0 to 4: 4, 0-26-40-15-4
0 to 5: 5, 0-5
0 to 6: 6, 0-26-13-41-6
0 to 7: 6, 0-16-7
0 to 8: 6, 0-16-8
0 to 9: 6, 0-26-24-38-44-9
0 to 10: 6, 0-29-10
0 to 11: 5, 0-16-11
0 to 12: 7, 0-5-12
0 to 13: 3, 0-26-13
0 to 14: 4, 0-26-24-38-14
0 to 15: 3, 0-26-40-15
0 to 16: 4, 0-16
0 to 17: 5, 0-26-40-17
0 to 18: 3, 0-1-18
0 to 19: 6, 0-1-18-19
0 to 20: 5, 0-26-40-15-20
0 to 21: 2, 0-21
0 to 22: 5, 0-26-40-42-22
0 to 23: 5, 0-21-23
0 to 24: 2, 0-26-24
0 to 25: 4, 0-1-25
0 to 26: 1, 0-26
0 to 27: 4, 0-26-40-15-27
0 to 28: 3, 0-26-28
0 to 29: 4, 0-29
0 to 30: 5, 0-1-25-30
0 to 31: 5, 0-26-24-38-14-31
0 to 32: 6, 0-1-25-32
0 to 33: 5, 0-26-33
0 to 34: 6, 0-1-25-2-34
0 to 35: 7, 0-5-35
0 to 36: 4, 0-26-13-36
0 to 37: 6, 0-26-24-38-14-31-37
0 to 38: 3, 0-26-24-38
0 to 39: 6, 0-26-40-15-39
0 to 40: 2, 0-26-40
0 to 41: 5, 0-26-13-41
0 to 42: 4, 0-26-40-42
0 to 43: 6, 0-26-24-3-43
0 to 44: 5, 0-26-24-38-44
0 to 45: 5, 0-26-45
0 to 46: 5, 0-16-46
0 to 47: 6, 0-26-40-15-4-47
0 to 48: 4, 0-1-48
0 to 49: 6, 0-1-48-49
Average for 40% density: 4.61224
*/

/**
* Assignment 2: A Monte Carlo simulation that calculates the average shortest
* path in a graph.
*
* Basic problem: Write a set of constructors for declaring and initializing a
* graph. An edge will have a positive cost that is its distance. Have a
* procedure that produces a randomly generated set of edges with positive
* distances. Assume the graphs are undirected. The random graph procedure
* should have edge density as a parameter and distance range as a parameter. So
* a graph whose density is 0.1 would have 10% of its edges picked at random and
* its edge distance would be selected at random from the distance range. The
* procedure should run through all possible undirected edges, say (i,j) and
* place the edge in the graph if a random probability calculation is less than
* the density. Compute for a set of randomly generated graphs an average
* shortest path.
*
* Assumptions:
* <ul>
* <li>An edge will have a positive cost function like distance (no negative edge cost).</li>
* <li>Graph edges (E) are undirected.</li>
* </ul>
*/
#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <vector>

// Sets this to true if the graph is to be printed.
const bool IS_PRINT_GRAPH = false;
// Sets this to true if edges of the shortest is to be printed.
const bool IS_PRINT_PATH = false;

using namespace std;

/**
* A undirected graph data type implemented using edge lists.
*/
class Graph {
public:
	typedef pair<unsigned, unsigned> edge;

	/**
	* Constructs a graph with the given size, initializing all node values to -1.
	*/
	Graph(unsigned size)
		: vs(size)
		, node_vals(size, -1)
	{
	}

	/**
	* Returns the number of nodes in the graph.
	*/
	inline int nodes() { return vs.size(); }
	/**
	* Returns the number of edges added to the graph.
	*/
	int edges()
	{
		int n = 0;
		for (list<int> es : vs) {
			n += es.size();
		}
		assert(n % 2 == 0);
		return n / 2;
	}

	/**
	* Returns true if there is an edge from node x to node y; false otherwise.
	*/
	bool adjacent(unsigned x, unsigned y)
	{
		if (x == y)
			return true;
		for (int i : vs[x]) {
			if (i == y)
				return true;
		}
		return false;
	}
	/**
	*  Returns all nodes y such that there is an edge from x to y.
	*/
	const list<int>& neighbors(unsigned x) { return vs[x]; }

	/**
	*  Adds to G the edge from x to y and return true, if it is not there;
	*  returns false otherwise.
	*/
	bool add(unsigned x, unsigned y)
	{
		if (adjacent(x, y))
			return false;
		else {
			vs[x].push_back(y);
			vs[y].push_back(x);
			return true;
		}
	}

	/**
	* Adds an edge to the graph with a specified distance if the edge does not
	* already exist in
	* the graph.
	* @param x node x; must be less than the number of nodes in the graph
	* @param y node y; must be less than the number of nodes in the graph
	* @param dist must be positive
	* @return true if the edge is added successfully to the graph; false
	* otherwise.
	*/
	bool add_edge(unsigned x, unsigned y, unsigned dist)
	{
		assert(dist > 0);
		if (add(x, y)) {
			set_edge_value(x, y, dist);
			return true;
		}
		else {
			return false;
		}
	}

	/**
	* Removes the edge from x to y and return true, if it is there;
	* returns false otherwise.
	*/
	bool remove(unsigned x, unsigned y)
	{
		if (!adjacent(x, y) || x == y)
			return false;
		else {
			vs[x].remove(y);
			vs[y].remove(x);
			return true;
		}
	}

	/**
	* Returns the value associated with the node x; or -1 if there is none.
	*/
	int get_node_value(unsigned x) { return node_vals[x]; }

	/**
	*  Sets the value associated with the node x to a.
	*/
	void set_node_value(unsigned x, unsigned a) { node_vals[x] = a; }

	/**
	* Returns the value associated with the edge between node x and y; or -1 if
	* there is none.
	*/
	int get_edge_value(unsigned x, unsigned y)
	{
		return edge_vals[make_pair(min(x, y), max(x, y))];
	}

	/**
	*  Sets the value associated with the edge between node x and y to a.
	*/
	void set_edge_value(unsigned x, unsigned y, unsigned a)
	{
		edge_vals[make_pair(min(x, y), max(x, y))] = a;
	}

	/**
	* Resets all node values to the uninitialized value, -1.
	*/
	void reset_node_vals() { node_vals = vector<int>(nodes(), -1); }

	/**
	* Used to dump the graph to the given output stream.
	*/
	ostream& dump(ostream& os)
	{
		os << endl
			<< "== Graph ==" << endl
			<< endl;
		for (int i = 0; i < vs.size(); i++) {
			os << i << " -> ";
			bool any = false;
			for (int j : vs[i]) {
				if (any)
					os << ", " << j;
				else {
					any = true;
					os << j;
				}
				os << " (" << get_edge_value(i, j) << ")";
			}
			os << endl;
		}
		return os;
	}

private:
	vector<list<int> > vs; // nodes
	vector<int> node_vals; // node values initialized to -1
	map<edge, unsigned> edge_vals; // edge values ie edge distance
};

// Overload << for class Graph
ostream& operator<<(ostream& os, Graph& g) { return g.dump(os); }

/**
* A priority queue element of <priority, node id>.
*/
typedef pair<unsigned, unsigned> q_element;

/**
* A Priority Queue data type used for computing shortest path, with the lowest
* priority at the front of the queue.
*/
class PriorityQueue {
public:
	/**
	* Inserts a queue element to the priority queue.
	*/
	void insert(const q_element& e) { q.push(e); }

	/**
	* Returns the element with the minimum priority.
	*/
	const q_element min_prioirty()
	{
		const q_element e = q.top();
		q.pop();
		return e;
	}
	/**
	* Returns true if the queue is empty; false otherwise.
	*/
	bool empty() { return q.empty(); }

private:
	// Priority queue of <priority, node-id>
	priority_queue<q_element, vector<q_element>, greater<q_element> > q;
};

/**
* Returns a graph with a randomly generated set of edges with positive
* distances.
* This procedure runs through all possible undirected edges, and place the edge
* in the graph if a random probability calculation is less than the density.
*
* @param edge_density edge density from 0.0 to 1.0
* @param distance_range distance range
*/
Graph& generate_graph(Graph& g, float edge_density = 0.2,
	pair<int, int> distance_range = { 1, 10 })
{
	const int mod = distance_range.second - distance_range.first + 1;

	const int nodes = g.nodes();
	const int max_edges = nodes * (nodes - 1) / 2;
	const int edges = max_edges * edge_density;

	for (int i = 0; i < edges; i++) {
		const int x = rand() % nodes;
		const int y = rand() % nodes;
		const int dist = distance_range.first + rand() % mod;
		g.add(x, y);
		g.set_edge_value(x, y, dist);
	}
	return g;
}

/**
* Creates a new pair of elements for use in a priority queue, where the first
* element is the
* priority, and the second element the vertex id.
*/
static inline q_element new_q_element(int pri, int id)
{
	return make_pair(pri, id);
}

/**
* Returns the value of the shortest path from vertex i to vertex j, computed
* using Dijkstra's Algorithm;
* or -1 if there is no path from i to j.
*
* @param g the graph involved
* @param i the starting node id
* @param j the target node id
* @param j the "back-pointers" that can be used to reconstruct edges of the
* shortest path
*/
int shortest_path(Graph& g, const int i, const int j, vector<int>& from_nodes)
{
	PriorityQueue pq = PriorityQueue();
	g.set_node_value(i, 0);
	pq.insert(new_q_element(0, i));

	while (!pq.empty()) {
		const q_element e = pq.min_prioirty();
		const int u = e.second;
		const int path = e.first;
		if (u == j) {
			return path;
		}
		else {
			list<int> vs = g.neighbors(u);
			for (int v : vs) {
				const int nodeval = g.get_node_value(v);
				const int dist = g.get_edge_value(u, v);
				const int new_nodeval = path + dist;

				if (nodeval == -1 || new_nodeval < nodeval) {
					g.set_node_value(v, path + dist);
					pq.insert(new_q_element(path + dist, v));
					from_nodes[v] = u;
				}
			}
		}
	}
	return -1;
}

/**
* Prints the edges of the shortest path from S to T.
*/
static void print_shortest_path(int S, int T, vector<int>& from_nodes)
{
	int n = S;
	do {
		cout << n << "-";
		n = from_nodes[n];
	} while (n != T);
	cout << n << endl;
}

/**
* Runs the the average path length calculation using the specified density on a
* graph of 50 nodes with a distance range of 1.0 to 10.0.
* Each run involves computing the 49 paths: 0 to 1, 0 to 2, ... 0 to 49.
*/
static void run(double density)
{
	Graph g = Graph(50);
	generate_graph(g, density, { 1, 10 });
	if (IS_PRINT_GRAPH)
		cout << g << endl;
	vector<int> from_nodes(g.nodes(), -1);
	if (IS_PRINT_PATH)
		cout << endl
		<< "== Shortest path ==" << endl
		<< endl;
	int total = 0;
	double n = 49.0;
	for (int i = 1; i < 50; i++) {
		const int x = shortest_path(g, i, 0, from_nodes);
		if (x > -1)
			total += x;
		else {
			// there is no path between 0 and i; so omit that value from the average
			n -= 1;
		}
		if (IS_PRINT_PATH) {
			cout << 0 << " to " << i << ": " << x << ", ";
			print_shortest_path(0, i, from_nodes);
		}
		g.reset_node_vals();
	}
	cout << "Average for " << 100 * density << "% density: " << total / n << endl;
}

// g++ -std=c++11 assign2.cpp -o assign2 && ./assign2
int main()
{
	srand(time(0));
	run(0.2); // 20% density
	run(0.4); // 40% density
	return 0;
}
