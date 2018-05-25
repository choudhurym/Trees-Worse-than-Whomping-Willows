/*******************************
*
*  Project Name: Project 5: Trees Worse than Whomping Willows
*  Description: Using binary search trees to make rosters of students with a Harry Potter theme
*  File names: main.c, Student.h
*  Date: 04/12/18
*  Authors: Muntabir(Tasin)Choudhury and Anh Nguyen
*
*******************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include "Student.h"

int main()
{
	
	char input[1024]; //Holds the user input commands
	Student* root[5]; //Holds the 5 different rosters of students
	Student* found = NULL; //Used for find() function

	char* fullName; //Holds first name + last name of students
	char firstName[1024]; //Holds first name of students
	char lastName[1024]; //Holds last name of students
	char homes[1024]; //Holds the input string for house by user
	int points = 0; //Holds number of points
	int year = 0; //Holds the year the student is
	int home = 0; //Holds the int value of the corresponding house
	char fileName[1024]; //Used to hold filenames
	int success = 0; //Flag used for error checking
	int i;
	
	//Gets User Input
	printf("Enter command: ");
	scanf("%s", input);

	//Continues to get commands until the user inputs "quit"
	while(strcmp(input, "quit") != 0)
	{
		//Resets the success flag back to zero
		success = 0; 

		//If user inputs specific command then that command is executed. 
		//If command isn't recognized, prints an error message to the screen;
		if(strcmp(input, "help") == 0)
		{
			//Displays help menu
			help();
		}
		else if(strcmp(input, "add") == 0)
		{
			scanf("%s %s %d %d %s", firstName, lastName, &points, &year, homes); 

			//Checks to make sure correct year was entered 
			if(year < 1 || year > 7)
				printf("Add failed. Invalid year: %d\n", year);
			else
			{
				//Simultaneously checks if a valid house is entered, and adds student if house found. 
				for(i = 0; i < 4; i++)
				{
					//Checks user input to actual house names to see if one matches
					if((strcmp(homes, HOUSE_NAMES[i]) == 0))
					{
						//Flag set to 1 to notify that house was typed in successfully
						success = 1;
						//Checks to see if person was already added to the specific roster
						if(find(root[i], firstName, lastName, i) == NULL)
						{	
							//Adds student to the correct roster
							root[i] = add(root[i], firstName, lastName, points, year, i);
							printf("Added %s %s to roster.\n", firstName, lastName);
						}
						else
							printf("Add Failed. Student named %s %s already present in roster.\n", firstName, lastName);
							
					}
				}
				
				//If success flag was not triggered, then the house entered was not correct
				if(success != 1)
					printf("Add failed. Invalid house: %s\n", homes);
			}

		}
		else if(strcmp(input, "preorder") == 0)
		{
			printf("Preorder print-out of roster:\n");

			//Prints out all rosters using preorder traversal
			for(i = 0; i < 4; i++)
			{
				printf("\n%s House\n\n", HOUSE_NAMES[i]);
				preorder(root[i]);
			}	
			printf("\nDeceased\n\n");
			preorder(root[DECEASED]);
			printf("\n\n");
		}
		else if(strcmp(input, "postorder") == 0)
		{
			printf("Postorder print-out of roster:\n");

			//Prints out all rosters using postorder traversal
			for(i = 0; i < 4; i++)
			{
				printf("\n%s House\n\n", HOUSE_NAMES[i]);
				postorder(root[i]);
			}	
			
			//Have this outside the loop to match Dr.Wittman's formatting
			printf("\nDeceased\n\n");
			postorder(root[DECEASED]);
			printf("\n\n");
		}
		else if(strcmp(input, "inorder") == 0)
		{
			printf("Inorder print-out of roster:\n");

			//Prints out all rosters using inorder traversal
			for(i = 0; i < 4; i++)
			{
				printf("\n%s House\n\n", HOUSE_NAMES[i]);
				inorder(root[i]);
			}	
		
			//Have this outside the loop to match Dr.Wittman's formatting
			printf("\nDeceased\n\n");
			inorder(root[DECEASED]);
			printf("\n\n");
		}
		else if(strcmp(input, "score") == 0)
		{
			//Need to reset the score total everytime function is called
			int scoreTotal=0;
			
			printf("Point totals:\n\n");

			//Prints out the total scores for each house
			for(i = 0; i < 4; i++)
			{
				printf("%s House:\t%d\n",HOUSE_NAMES[i], score(root[i],scoreTotal));
				scoreTotal=0;
			}
		}
		else if(strcmp(input, "clear") == 0)
		{
			//Clears each of the rosters
			for(i = 0; i < 5; i++)
			{
				clear(root[i]);
				root[i] = NULL;
			}
			printf("All data cleared.\n");
		}
		else if(strcmp(input, "load") == 0)
		{
			scanf("%s", fileName);
			//Loads data from a text file into the rosters
			load(root, fileName);
		}
		else if(strcmp(input, "save") == 0)
		{
			scanf("%s", fileName);
			
			//Opens the file the user wants to write to
			FILE* file = fopen(fileName, "w");
			
			//Saves all of the data from each house into the specified file
			for(i = 0; i < 4; i++)
				save(root[i], file);

			printf("Successfully saved data to file %s.\n", fileName);
			//Closes the file
			fclose(file);
		}
		else if(strcmp(input, "points") == 0)
		{
			int userPoints; //Holds the users input points			
			scanf("%s %s %s %d", firstName, lastName, homes, &userPoints);
			
			//Cycles through all the houses
			for(i = 0; i < 4; i++)
			{
				//Checks to see if input house was valid
				if((strcmp(homes, HOUSE_NAMES[i]) == 0))
				{
						//Uses find function to find the student entered by user
						found = find(root[i], firstName, lastName, i);
						//Keeps track of what house was selected
						home = i;
						//Sets success flag to one saying that house was input correctly
						success = 1;
				}
			}
			
			//If house was input correctly, then add the points, otherwise, don't
			if(success == 1)
			{
				//Checks to make sure the student actually exists
				if(found == NULL)
					printf("Point change failed. %s %s was not found in %s House.\n", firstName, lastName, HOUSE_NAMES[home]);
				else
				{
					//Adds points to specified student
					found->points+=userPoints;
					//Puts student's first and last name into one string
					fullName = strdup(found->first);
					strcat(fullName, " ");
					strcat(fullName, found->last);
					printf("Points for %s changed to %d.\n", fullName, found->points);
				}
			}
			else
				printf("Point change failed. Invalid house: %s\n", homes);
		}
		else if(strcmp(input, "find") == 0)
		{
			scanf("%s %s %s", firstName, lastName, homes);
			
			//Cycles through all the houses
			for(i = 0; i < 4; i++)
			{
				//Checks to see if input house string is valid
				if((strcmp(homes, HOUSE_NAMES[i]) == 0))
				{
						//Uses find function to find the student entered by user
						found = find(root[i], firstName, lastName, i);
						//Keeps track of what house was selected
						home = i;
						//Sets success flag to one saying that house was input correctly
						success = 1;
				}
			}
			
			//If house was input correctly, then print the found students info, otherwise, don't
			if(success == 1)
			{
				//If student wasn't found print error
				if(found == NULL)
					printf("Find failed. %s %s was not found in %s House.\n", firstName, lastName, HOUSE_NAMES[home]);
				else
				{
					//Puts found student's first and last name into one string
					fullName = strdup(found->first);
					strcat(fullName, " ");
					strcat(fullName, found->last);
					printf("%-25sPoints: %d\tYear: %d House: %s\n", fullName, found->points, found->year, HOUSE_NAMES[found->house]);
				}
			}
			else
				printf("Find failed. Invalid house: %s\n", homes);
				
		}
		else if(strcmp(input, "kill") == 0)
		{
			scanf("%s %s %s", firstName, lastName, homes);
			
			//Cycles through all the houses
			for(i = 0; i < 4; i++)
			{
				//Checks to see if input house was valid
				if((strcmp(homes, HOUSE_NAMES[i]) == 0))
				{
						//Uses find function to find the student entered by user
						found = find(root[i], firstName, lastName, i);
						//Keeps track of what house was selected
						home = i;
						//Sets success flag to one saying that house was input correctly
						success = 1;
				}
			}
			
			//If house was input correctly, kill student, otherwise, don't
			if(success == 1)
			{
				//If student wasn't found print error
				if(found == NULL)
					printf("Kill failed. %s %s was not found in %s House.\n", firstName, lastName, HOUSE_NAMES[home]);
				else
				{
					//Adds the specified student to the deceased list before killing them
					root[DECEASED] = add(root[DECEASED], firstName, lastName, points, year, home);
					//Kills the desired student
					root[home] = kill(root[home], firstName, lastName, home);
					printf("Moved %s %s to list of deceased students.\n", firstName, lastName);
				}
			}
			else
				printf("Kill failed. Invalid house: %s\n", homes);
				
		}
		else //If user does not input a valid command, an error is displayed
			printf("Unknown Command: %s\n", input);

		//Clears the garbage that was written after the invalid command (if any)
		char temp = getchar();
		while(temp != '\n')
			temp = getchar();

		//Prompts the user to input another command
		printf("Enter command: ");
		scanf("%s", input);

	}

	//User quit, clear data
	for(i = 0; i < 5; i++)
	{
		clear(root[i]);
		root[i] = NULL;
	}

	printf("All data cleared.\n");
	return 0; 
}

/* 
   Description: This function displays the help menu, detailing what each function the user can calls does
				
   Parameters: N/A
				
   Returns: N/A
*/ 
void help()
{  
	//Prints Help Menu Stuff~
	printf("help\nPrints this display\n\n");
	printf("load <filename>\nAdds the contents of a file to the current roster\n\n");
	printf("save <filename>\nSaves the current roster to a file\n\n");
	printf("clear\nClear the current roster\n\n");
	printf("inorder\nPrint out an inorder traversal of the roster for each house\n\n");
	printf("preorder\nPrint out a preorder traversal of the roster for each house\n\n");
	printf("postorder\nPrint out a postorder traversal of the roster for each house\n\n");
	printf("add <firstname> <lastname> <points> <year> <house>\nAdds a student to the roster\n\n");
	printf("kill <firstname> <lastname> <house>\nMoves a student to the deceased list\n\n");
	printf("find <firstname> <lastname> <house>\nSearches for a student in a house\n\n");
	printf("points <firstname> <lastname> <house> <points>\nChanges the points a student has earned by the specified amount\n\n");
	printf("score\nLists the point totals for all four houses\n\n");
	printf("quit\nQuits the program\n\n");	

}

/* 
   Description: This function takes a text file and loads it's contents into the rosters
				
   Parameters: root -an array of all the trees that can be added to
			   fileName - the name of the file that data is to be added to
			   
   Returns: N/A
*/ 
void load(Student* root[], char* fileName)
{

	char firstName[1024]; //Holds first name of students
	char lastName[1024]; //Holds last name of students
	char homes[1024]; //Holds the input string for house by user
	int points = 0; //Holds number of points
	int year = 0; //Holds the year the student is
	int success = 0; //Flag used for error checking
	int i;
	
	//Opens the file for writing
	FILE* file = fopen(fileName, "r");
	
	//Makes sure the file exists. If it doesn't, prints an error
	if(file)
	{
		//Used to make sure there are still items left to scan in the file
		int check = fscanf(file, "%s %s %d %d %s", firstName, lastName, &points, &year, homes);
		
		//Continues until the function reaches the end of the file
		while(check == 5)
		{

			//Checks to make sure the specified year is correct
			if(year < 1 || year > 7)
				printf("Add failed. Invalid year: %d\n", year);
			else
			{
				//Cycles through all of the houses
				for(i = 0; i < 4; i++)
				{
					//Checks to see if specified house is correct
					if((strcmp(homes, HOUSE_NAMES[i]) == 0))
					{
						//Sets success flag to one saying that house was correct
						success = 1;
						//Checks to make sure the person wasn't already added, if not, then add them
						if(find(root[i], firstName, lastName, i) == NULL)
							root[i] = add(root[i], firstName, lastName, points, year, i);
						else
							printf("Add Failed. Student named %s %s already present in roster.\n", firstName, lastName);
					}
				}
				
				//If success flag is not one, then the house was invalid
				if(success != 1)
					printf("Add failed. Invalid house: %s\n", homes);
			}

			//Get the next line
			check = fscanf(file, "%s %s %d %d %s", firstName, lastName, &points, &year, homes);
		
		}
		printf("Successfully loaded data from file %s.\n", fileName);
		//Closes the file
		fclose(file);
	}
	else
	{
		printf("Load failed. Invalid file: %s\n", fileName);
	}
}

/* 
   Description: Saves the contents of the rosters into a specified text file in preorder traversal order
				
   Parameters: root - root of the specific tree linked to a specific house
				
   Returns: N/A
*/ 
void save(Student* root, FILE* file)
{
	//Prints the student info of the four houses in preorder traversal order
	if(root != NULL)
	{
		fprintf(file, "%s %s %d %d %s\n", root->first, root->last, root->points, root->year, HOUSE_NAMES[root->house]);
		save(root->left, file);
		save(root->right, file);
	}
}

/* 
   Description: This function prints out a preorder traversal of the house being selected.
				
   Parameters: root - The root of the house that is being traversed. 
*/ 
void preorder(Student* root)
{
	//Check continue until traversal hits NULL
	if(root != NULL)
	{
		//print the root
		printStudent(root);

		//Recursively visits the left subtree
		preorder(root->left);

		//Recursively visits the left subtree
		preorder(root->right);
	}
}

/* 
   Description: Prints the students in the roster in postorder traversal order
				
   Parameters: root -root of the specific tree linked to a specific house
				
   Returns: N/A
*/ 
void postorder(Student* root)
{
	//Check continue until traversal hits NULL
	if(root != NULL)
	{
		//Recursively visits the left subtree
		postorder(root->left);

		//Recursively visits the right subtree
		postorder(root->right);

		//print the root
		printStudent(root);
	}
}
/* 
   Description: This function prints out an inorder traversal of the house being selected.
				
   Parameters:  root - The root of the house that is being traversed. 
*/ 
void inorder(Student* root)
{
	//Check continue until traversal hits NULL
	if(root != NULL)
	{
		//Recursively visits the left subtree		
		inorder(root->left);
		
		//print the root
		printStudent(root);

		//Recursively visits the right subtree
		inorder(root->right);
	}
}

/* 
   Description: This function prints out a single student node.
				
   Parameters:  root - The student node that is being printed. 
*/ 
void printStudent(Student* root)
{
	//Puts first name and last name into one string
	char* fullName = strdup(root->first);
	strcat(fullName, " ");
	strcat(fullName, root->last);

	printf("%-25sPoints: %d\tYear: %d House: %s\n", fullName, root->points, root->year, HOUSE_NAMES[root->house]);
}

/* 
   Description: This function sums up all the positive integers in an
				array of integers. 
				
   Parameters:  root - root of the specific tree linked to a specific house
		firstName - first name of student to be added
		lastName - last name of student to be added
		points - number of points to be added for that student
		year - the year of the student to be added
		home - which house the student to be added belongs to
				
   Returns: the root of the specific tree linked to a specific house
*/ 
Student* add(Student* root, char* firstName, char* lastName, int points, int year, int home)
{
	//Base case: Creates new node and adds it to the roster
	if(root == NULL)
	{
		root = malloc(sizeof(Student));
		root->first = strdup(firstName);
		root->last = strdup(lastName);
		root->points = points;
		root->year = year;
		root->house = home;
		root->left = NULL;
		root->right = NULL;
		return root;
		
	}
	//If last names are equal, first names need to be compared
	else if(strcmp(lastName, root->last) == 0) 
	{
		//Decides how to traverse the tree depending on the value of the first name
		if(strcmp(firstName, root->first) < 0)
			root->left = add(root->left, firstName, lastName, points, year, home);
		else if(strcmp(firstName, root->first) > 0)
			root->right = add(root->right, firstName, lastName, points, year, home);
		else
			return root;
	}
	//If the name to be added is less than the root name, traverse left
	else if(strcmp(lastName, root->last) < 0)
		root->left = add(root->left, firstName, lastName, points, year, home);
	//If the name to be added is greater than the croot name, traverse right
	else if(strcmp(lastName, root->last) > 0)
		root->right = add(root->right, firstName, lastName, points, year, home);

	return root;	
}

/* 
   Description: This function removes a student from the roster they are in
				
   Parameters:  root -root of the specific tree linked to a specific house the student is in
		firstName - the first name of the student being killed
		lastName - the last name of the student being killed
		home - which house the student to be killed belongs to
				
   Returns:	    Returns a pointer to the root of the new roster
*/ 
Student* kill(Student* root, char* firstName, char* lastName, House home)
{
	
	//Base case
	if(root == NULL)
		return root;
	
	//Searches for the student to be killed traversing left and right based on student's last name
	if(strcmp(lastName, root->last) < 0)
		root->left = kill(root->left, firstName, lastName, home);
	else if(strcmp(lastName, root->last) > 0)
		root->right = kill(root->right,  firstName, lastName, home);
	//If the students last name matches, then need to check if first name matches
	else if(strcmp(lastName, root->last) == 0)
	{
		//Searches for the student to be killed traversing left and right based on student's first name
		if(strcmp(firstName, root->first) < 0)
			root->left = kill(root->left, firstName, lastName, home);
		else if(strcmp(firstName, root->first) > 0)
			root->right = kill(root->right, firstName, lastName, home);
		//Handles the case when the student is found
		else
		{
			//Handles the case when the node to be removed has either one child or no children
			if(root->left == NULL)
			{
				//Sets a temp varible to the node after the node to be deleted
				Student* temp = root->right;
				//Frees the node we want to delete
				free(root);
				return temp;
			}
			else if (root->right == NULL)
			{
				//Sets a temp varible to the node after the node to be deleted
				Student* temp = root->left;
				//Frees the node we want to delete
				free(root);
				return temp;
			}

			//Handles the case when the node to be removed has two children
			//Finds the node that will be replacing the one to be deleted
			Student* temp = findMinValue(root->right);
			
			//Copies the data from the replacment node to the node that is being replaced
			root->first = temp->first;
			root->last = temp->last;
			root->points = temp->points;
			root->year = temp->year;
			root->house = temp->house;
			
			//Kills the replacement node instead of the node that we're replacing
			//We know this works becuase we know that our replacment node is always going to have no children
			root->right = kill(root->right, temp->first, temp->last, temp->house);
		}
	}

	return root;
}

/* 
   Description: Helper function to the kill() function that finds the value that is the smallest in the tree
				
   Parameters:  root -root of the specific tree linked to a specific house the student is in
				
   Returns:	    Returns a pointer to the smallest value in the tree
*/ 
Student* findMinValue(Student* root)
{
	//A pointer to the node we are currently on
	Student* current = root;
	
	//Goes through the tree until we hit NULL searching for smallest value in the tree
	while(current->left != NULL)
		current = current->left;

	return current;
}

/* 
   Description: This function finds a student in the specified roster
				
   Parameters:  root -root of the specific tree linked to a specific house the student is in
		firstName -the first name of the student being searched for
		lastName -the last name of the student being searched for
		home -which house the student to be added belongs to
				
   Returns:	    Returns a pointer to the found student
*/ 
Student* find(Student* root, char* firstName, char* lastName, House home)
{
	//Student not found, then return NULL
	if(root == NULL)
		return NULL;
	//If the last names are equal, then checks to see if first names are the same
	else if(strcmp(lastName, root->last) == 0)
	{
		//If the first name is smaller than root, then traverse left
		if(strcmp(firstName, root->first) < 0)
			return find(root->left, firstName, lastName, home);
		//If the first name is larger than root, then traverse right
		else if(strcmp(firstName, root->first) > 0)
			return find(root->right, firstName, lastName, home);
		//First and last name are the same so return the found student
		else
			return root;
	}
	//If last name is smaller than root, then traverse left
	else if(strcmp(lastName, root->last) < 0)
		return find(root->left, firstName, lastName, home);
	//If last name is larger than root, then traverse right
	else
		return find(root->right,  firstName, lastName, home);
}

/* 
   Description: Gets the total score of the specified house
				
   Parameters:  root -root of the specific tree linked to a specific house
		total -holds the total score of the specified house
				
   Returns: N/A
*/ 
int score(Student* root, int total)
{
	//Continues until the traversals hit NULL(meaning the end of each branch)
	if(root != NULL)
	{
		//Adds the current students points to the total
		total=score(root->left, total);
		total=score(root->right, total);
		total+=root->points;
	}

	return total;
}

/* 
   Description: Clears the rosters of all the students
				
   Parameters: root -root of the specific tree linked to a specific house
				
   Returns: N/A
*/ 
void clear(Student* root)
{
	//Continues until the traversals hit NULL(meaning the end of each branch)
	if(root != NULL)
	{	
		//Frees all the nodes starting from the bottom and working your way up
		clear(root->left);
		clear(root->right);

		free(root->first);
		free(root->last);
		free(root);
	}
}
