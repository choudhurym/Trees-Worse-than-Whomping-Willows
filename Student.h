#ifndef STUDENT_H
#define STUDENT_H

#define HOUSES 4

typedef enum { GRYFFINDOR, RAVENCLAW, HUFFLEPUFF, SLYTHERIN, DECEASED } House;
char* HOUSE_NAMES[HOUSES] = {"Gryffindor", "Ravenclaw", "Hufflepuff", "Slytherin", "Deceased"};


typedef struct _Student
{
	char* first;
	char* last;
	int points;
	int year;
	House house;	
	
	struct _Student* left;
	struct _Student* right;

} Student;

//fucntion prototypes
void help(void);
Student* add(Student* root, char* firstName, char* lastName, int points, int year, int home);
void load(Student* root[], char* fileName);
int checkInputs(int year, char* homes);
Student* find(Student* root, char* firstName, char* lastName, House home);
Student* kill(Student* root, char* firstName, char* lastName, House home);
int score(Student* root, int total);
void clear(Student* root);
void preorder(Student* root);
void postorder(Student* root);
void inorder(Student* root);
void printStudent(Student* root);
void points(char* first, char* last, House home, int points);
void save(Student* root, FILE* file);
Student* findMinValue(Student* root);

#endif
