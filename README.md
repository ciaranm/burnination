Burnination
===========

This is some very quick and dirty code which solves a firefighter problem. You
probably don't want to use this in practice.

Compiling
---------

To build, type 'make'. You will need a C++17 compiler (we test with GCC 9.3 and
Xcode 10.2).

Running
-------

To run:

```shell session
$ ./burnination input.txt
```

The input file looks like this:

```
vertices = 11
fires = 3 8
budget = 2
1,2
2,3
3,4
4,5
5,6
6,7
7,8
8,9
9,10
10,11
```

If your input file isn't valid, you will get a very unhelpful error message. I
did warn you not to use this code.

This specifies an eleven vertex (path) graph, with fires initially in locations
3 and 8, and we are allowed to place two defenders per turn. The solution we
find is:

```
solution: 4 [ 2 4 ] [ 6 10 ]
```

where four vertices get burninated, and we defend vertices 2 and 4 on the first
turn and 6 and 10 on the second turn.

<!-- vim: set tw=100 spell spelllang=en : -->

