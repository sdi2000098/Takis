#define ERROR -1

#define YES_WHILE 5

#define NO_WHILE 6

#define YES_IF 7

#define NO_IF 8

#define YES_ELSE 10

#define MAXWORDS 23

#define SIZE_ARGUMENT 2

#define YES_BREAK 11

#define YES_CONTINUE 12

#define ITS_ARRAY -13

#define ARGUMENT 3

#define YES_FOR -12
struct valid
{
	int ok;
	double x;
	int its_double ;
};

struct var{
	int x;
	char name[100];
};

struct ipl_arrays{
        char name[100];
        int dim;
        int  *array;
};

struct double_num{
	char name[100];
	double x;
};
