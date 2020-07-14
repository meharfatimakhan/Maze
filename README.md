# Maze
This code finds the path for a given object from the start state to the goal using Stack.


Suppose a robot has to find a path from a starting point to a goal point. Zero means it’s a forbidden cell and 1 means it's allowed to go into that cell. This code finds the correct path through allowed cells to reach the goal. For example, in the above if (0,2) is the start state and (9,3) is the goal state then one possible path is given by the blue highlighted cells.  



### FORMAT OF INPUT FILE

An example is given in grid.txt and the file contains following data:

* totalRows   totalColumns
* start_row start_col
* goal_row goal_col
* the grid



### THE CODE

The code implements stack data structure. If there exists a path, the grid is displayed clearly showing the path. Otherwise, a message is displayed that the path does not exist.
