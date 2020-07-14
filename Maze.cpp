#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;


class index
{
public:
	int row;
	int column;

	//defualt constructor
	index()
	{
		row = 0;
		column = 0;
	} 

	//istream operator used for reading elements in file
	friend istream& operator >> (istream &in,index &read)
	{
				in >> read.row;
				in >> read.column;
				return in;
	} 

	//ostream operator used to give output
	friend ostream& operator << (ostream &out, index &read)  
	{
		out << read.row;
		out << read.column;
		return out;
	}
	
	//checks for the equality between two indices
	bool operator ==(const index& otherIndex) 
	{
		return(row == otherIndex.row && column == otherIndex.column);
	}
	
	/*index operator=(const int n)
	{
	row = n;
	column = n;
	return *this;
	}
	*/
	/*const index& operator=(const int &number)
	{
		row = object.row;
		column = object.column;
		return *this;
	}
*/
	//friend bool operator ==(const index& otherIndex, int number)
	//{
	//	return(/*otherIndex.row == number &&*/ otherIndex.column == number);
	//}

};


//function prototype to find path in maze
void findPath(index indice, int**& Array2D, index start, index goal);


//index class inherited to stack class to make all data members and functions accessible
class stack : public index
{
public:

	index *visitedIndices;
	int stackTop;
	int maxSize;
	index *invalidIndices;

	//stack defualt constructor
	stack()
	{
		visitedIndices = new index[1];
		invalidIndices = new index[1];
		stackTop = 0;
	}

	//stack is empty only when stackTop is equal to 0
	bool isEmptyStack()
	{
		return (stackTop == 0);
	}

	//push function: stores newIndice in visitedIndices and inValidIndices and extends both arrays
	void push(const index& newIndice)
	{
		int size1 = stackTop;
		int size2 = stackTop;
		if (stackTop < maxSize) //if old size is greater than newSize 
			size2 = maxSize;
		else if (maxSize < stackTop)
			size2 = maxSize;
		else//if old size is equal to new size
			maxSize = stackTop;

		visitedIndices[stackTop] = newIndice;
		invalidIndices[size2] = newIndice;
		maxSize++;
		stackTop++;
		index *temporary1 = new index[size1];
		index *temporary2 = new index[size2];
		for (int i = 0; i <= size1; i++)
		{
			temporary1[i] = visitedIndices[i];
		}

		for (int i = 0; i <= size2; i++)
		{
			temporary2[i] = invalidIndices[i];
		}

		visitedIndices = nullptr;
		invalidIndices = nullptr;
		delete[]visitedIndices;
		delete[]invalidIndices;

		invalidIndices = new index[maxSize];
		visitedIndices = new index[stackTop];
		for (int i = 0; i <= size1; i++)
		{
			visitedIndices[i] = temporary1[i];
		}
		for (int i = 0; i <= size2; i++)
		{
			invalidIndices[i] = temporary2[i];
		}
	}

	//checks if top of stack is not zero, then returns value stored at top of stack
	index top() const
	{
		if (stackTop != 0)
		return visitedIndices[stackTop - 1];
	}

	//checks that if stack is not empty, then it pops(deletes) the element at top by decreasing stack size
	void pop()
	{
		if (!isEmptyStack())
		{
			stackTop--;
			index *temporary = new index[stackTop];
			for (int i = 0; i <= stackTop-1; i++)
				temporary[i] = visitedIndices[i];
			visitedIndices = nullptr;
			delete[]visitedIndices;
			visitedIndices = new index[stackTop];
			for (int i = 0; i <= stackTop-1; i++)
			{
				visitedIndices[i] = temporary[i];
				//cout << endl;
				//cout << visitedIndices[i];
			}
			maxSize = stackTop+1;//stores old size
		}
		else
			cout << "CANNOT REMOVE! STACK IS EMPTY!\n";
	}

	//destructor: deletes all dynamic memory
	~stack()
	{
		delete[]visitedIndices;
		visitedIndices = nullptr;
		delete[]invalidIndices;
		invalidIndices = nullptr;
	}
	 
	//checks for the validity of index
	bool isValidIndex(index indexRecieved, index totalIndice, int **&Array)
	{
		//if index is present in array, validIndices, then it means that the index sent is invalid and has already been used, so returns false
		for (int i = 0; i <= maxSize; i++)
		{
			if (invalidIndices[i] == indexRecieved)
				return false;
		}

		//if index's row is out of bounds, return false, means not valid
		if (indexRecieved.row<0 || indexRecieved.row>((totalIndice.row) - 1))
			return false;

		//if index's column is out of of bounds, return false, means not valid
		else if (indexRecieved.column<0 || indexRecieved.column>((totalIndice.column) - 1))
			return false;

		//if element of Array present at that index is equal to 0, return false
		else if (Array[indexRecieved.row][indexRecieved.column] == 0)
			return false;

		else
			return true;
	}

};
	
void readFile (index indice, index start, index goal, int **&Array2D)
{
	ifstream read;
	read.open("grid.txt");
	
	//Reading total rows and total column
	cout << "Total rows and columns: ";
	read >> indice.row;
	cout << indice.row;
	cout << " ";
	read >> indice.column;
	cout << indice.column;
	cout << endl;

	//row and column from where the path should start
	cout << "Start row and start column: ";
	read >> start.row;
	cout << start.row;
	cout << " ";
	read >> start.column;
	cout << start.column;
	cout << endl;


	//row and column where the path should end
	cout << "Goal row and goal column: ";
	read >> goal.row;
	cout << goal.row;
	cout << " ";
	read >> goal.column;
	cout << goal.column;
	cout << endl;

	//Allocation of memory of integer array
	Array2D = new int *[indice.row];
	for (int i = 0; i < indice.row; i++)
	{
		Array2D[i] = new int[indice.column];
	}

	//Printing and reading original maze from file
	cout << "\nOriginal maze: \n\n";
	for (int indice1 = 0; indice1 < indice.row; indice1++)
	{
		for (int indice2 = 0; indice2 < indice.column; indice2++)
		{
			read >> Array2D[indice1][indice2];
			cout << Array2D[indice1][indice2];
			cout << " ";
		}
		cout << endl;
	}

	read.close();
	
	//function to find path in maze
	findPath(indice, Array2D, start, goal);
	
}


void findPath(index indice, int**& Array2D, index start, index goal)
{
	stack moveStack;

	//push the starting index of maze in stack
	moveStack.push(start);

	//flag to check whether goal index found or not
	bool goalFound = false;
	 
	//flags to check if path is stuck
	bool up = false;
	bool down = false;
	bool right = false;
	bool left = false;
	bool stuckUp = true;
	bool stuckDown = true;
	bool stuckRight = true;
	bool stuckLeft = true;

	//if stack is empty and not goalFound, then come out of loop
	while (!moveStack.isEmptyStack() && !goalFound)
	{
		index moveIndex = moveStack.top();//gives value present at the top of stack

		//if path is stuck from all sides then pop from stack
		if (up && down && right && left && stuckUp && stuckDown && stuckRight && stuckLeft)
		{
			moveStack.pop();
			moveIndex = moveStack.top();
		}
		else if (!up && down && right && left)//if path is stuck from all sides except up, then pop up
		{
			moveStack.pop();
			moveIndex = moveStack.top();
		}
		else if (up && !down && right && left)//if path is stuck from all sides except down, then pop down
		{
			moveStack.pop();
			moveIndex = moveStack.top();	
		}
		else if (up && down && !right && left)//if path is stuck from all sides except right, then pop right
		{
			moveStack.pop();
			moveIndex = moveStack.top();
		}
		else if (up && down && right && !left)//if path is stuck from all sides except left, then pop left
		{
			moveStack.pop();
			moveIndex = moveStack.top();
		}
		
		//condition for up: if valid then pushes in stack and if not then comes back to original position
		moveIndex.row--;
		if (moveStack.isValidIndex(moveIndex, indice, Array2D))//UP
		{
			moveStack.push(moveIndex);
			stuckUp = false;
		}
		else
		{
			up = true;
			moveIndex.row++;
			stuckUp = true;
		}

		//condition for down: if valid then pushes in stack and if not then comes back to original position
		moveIndex.row++;
		if (moveStack.isValidIndex(moveIndex, indice, Array2D))//DOWN
		{
			moveStack.push(moveIndex);
			stuckDown = false;
		}
		else
		{
			stuckDown = true;
			down = true;
			moveIndex.row--;
		}

		//condition for right: if valid then pushes in stack and if not then comes back to original position
		moveIndex.column++;
		if (moveStack.isValidIndex(moveIndex, indice, Array2D))//RIGHT
		{
			moveStack.push(moveIndex);
			stuckRight = false;
		}
		else
		{
			right = true;
			moveIndex.column--;
			stuckRight = true;
		}

		//condition for left: if valid then pushes in stack and if not then comes back to original position
		moveIndex.column--;
		if (moveStack.isValidIndex(moveIndex, indice, Array2D))//LEFT
		{
			moveStack.push(moveIndex);
			stuckLeft = false;
		}
		else
		{
			stuckLeft = true;
			left = true;
			moveIndex.column++;
		}
		
		//if goal is found, then come out of loop
		if (moveIndex == goal)
			goalFound = true;
	}


	if (!goalFound)
		cout << "Path does not exist.\n";

	//printing path of the maze
	cout << endl << endl;
	cout << "Maze PATH: \n\n";
	for (int indice1 = 0; indice1 < indice.row; indice1++)
	{
		for (int indice2 = 0; indice2 < indice.column; indice2++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);//sets colour to red of indices that are not a part of path
			for (int k = 0; k < moveStack.stackTop; k++)
			{
				if (moveStack.visitedIndices[k].row == indice1 && moveStack.visitedIndices[k].column == indice2)//checks if visited is equal to array indices, then part of path
				{
					Array2D[indice1][indice2] = 8; //path that follows
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);//sets colour of path to green
				}
			}
			
			cout << Array2D[indice1][indice2]; 
			cout << " ";
		}
		cout << endl;
	}
}

void main()
{
	index start;
	index goal; 
	index totalRowsColumns;
	int** Array2D = 0;
	readFile(totalRowsColumns, start, goal, Array2D);

	system("pause>NULL");
}