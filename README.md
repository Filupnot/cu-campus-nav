# CU Campus Navigation

This program is designed to help people get around the University of Colorado Boulder's college campus by finding the most shortest route between any two points. It uses an adjacency list to create a graph with the vertices and edges representing walkable paths around campus, and implements Dijkstra's algorithm to find the shortest path from one selected vertex to another. It also shows information including distance and walk time.

## Use
![Visualized graph with vertex names & edges along paths](https://github.com/Filupnot/cu-campus-nav/edit/master/map.jpg)

Upon starting the program, the user will be greeted with this menu:
~~~
====MENU====
1. Print all vertices
2. Find best path
3. Quit
~~~

The first option will print all vertices and relevant information individually:
~~~
Vertex: bakrNE [425x450]
Adjacency: bakrNW, bakrSE, libNE, duanSE,

Vertex: bakrNW [494x450]
Adjacency: duanSW, bakrSW, bakrNE,

Vertex: bakrSW [491x403]
Adjacency: imigN, bakrSE, bakrNW, ...
~~~

The second option will prompt the user to enter the name of start and end vertices. The program will then output the optimal path in terms of other vertices.
~~~
Start location: c4cSW
End location: umcSW

Best route:
c4cSW => willSE => willSW => cheyNE => cheyNW => imigSW => imigW => umcSE => umcSW
776.199 meters (0.482411 miles)
7 minutes 38 seconds.
~~~

(Note: the vertex names are arbitrary, and their names and respective locations can be found on map.jpg)

This was my final project for CSCI 2275: Programming and Data Structures at the University of Colorado Boulder in December, 2019. 
