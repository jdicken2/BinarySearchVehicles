#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudoku.h"
#include <assert.h>

#define NUM_DIGITS 9
#define NUM_ROWS   NUM_DIGITS
#define NUM_COLS   NUM_DIGITS
#define NUM_PEERS  20
#define NUM_UNITS  3
#define DIGITS     "123456789"
#define ROW_NAMES  "ABCDEFGHI"
#define COL_NAMES  DIGITS

typedef struct square {
  char vals[NUM_DIGITS+1]; // string of possible values
  unsigned char row;
  unsigned char col;
  struct square *peers[NUM_PEERS];
  struct square *units[NUM_UNITS][NUM_DIGITS];
} square_t;

typedef struct puzzle {
  square_t squares[NUM_ROWS][NUM_COLS];
} puzzle_t;

void solve(unsigned char grid[9][9]);

// following are static ("private") function declarations --- add as needed

static puzzle_t *create_puzzle(unsigned char grid[9][9]);
static void init_peers(puzzle_t *puz, int row, int col);
static puzzle_t *copy_puzzle(puzzle_t *puz);
static void free_puzzle(puzzle_t *puz);
static void print_puzzle(puzzle_t *);

static puzzle_t *search(puzzle_t *puz);

static puzzle_t *assign(puzzle_t *puz, int row, int col, char val);
static puzzle_t *eliminate(puzzle_t *puz, int row, int col, char val);
static void remove_val(puzzle_t *puz, int row, int col, char val);
static square_t *find_candidate(puzzle_t *puz); 

/*************************/
/* Public solve function */
/*************************/

void solve(unsigned char grid[9][9]) {
  puzzle_t *puz = create_puzzle(grid);
  puzzle_t *solved;
  if ((solved = search(puz)) != NULL) {
    print_puzzle(solved);
  }
  free_puzzle(puz);
}

/*******************************************/
/* Puzzle data structure related functions */
/*******************************************/
//Returns newly allocated puzzle_t structure
/*By allocating memory to pointer to a puzzle structure, and
iterating through the puzzle structure, adding possible vales to each square*/
static puzzle_t *create_puzzle(unsigned char vals[9][9])
{
	//Create a puzzle 
	int i,j;
 	puzzle_t *puz=malloc(sizeof(puzzle_t));
 		for(i=0;i<NUM_ROWS;i++){
			for(j=0;j<NUM_COLS;j++){
        			if(vals[i][j]=='0' || vals[i][j]=='.')
				{
					 strncpy(puz->squares[i][j].vals, DIGITS, NUM_DIGITS+1);
				
				}
				else
				{
					puz->squares[i][j].vals[0]=vals[i][j];
					puz->squares[i][j].vals[1]='\0';
				}	
				init_peers(puz, i, j);
				puz->squares[i][j].row=i;
				puz->squares[i][j].col=j;	
		}

}		
 return puz; 
}
static void init_peers(puzzle_t *puz, int row, int col) {
 //Called once per square to help populate its peers and units with the correct pointers 
int i,j;
int peers_pos=0;
//First unit (rows)
for(i=0;i<NUM_ROWS;i++){
	puz->squares[row][col].units[0][i]=&(puz->squares[i][col]);

//Peers
if(row!=i){
	puz->squares[row][col].peers[peers_pos++]=&(puz->squares[i][col]);
	}
}
//Second Unit (columns)
for(j=0;j<NUM_COLS;j++){
	puz->squares[row][col].units[1][j]=&(puz->squares[row][j]);
//Peers
if(col!=j){
	puz->squares[row][col].peers[peers_pos++]=&(puz->squares[row][j]);
	}
}

//Third Unit

int square_row=row-(row%3);
int square_col=col-(col%3);
int unit_pos=0;
for(i=square_row;i<square_row+3;i++){
	for(j=square_col;j<square_col+3;j++){
		puz->squares[row][col].units[2][unit_pos++]=&(puz->squares[i][j]);
	
		if((i!=row) && (j!=col))
        	{
			puz->squares[row][col].peers[peers_pos++]=&(puz->squares[i][j]);
		}
	}
	
}


  	
}
static void free_puzzle(puzzle_t *puz) {
	free(puz);
}

static puzzle_t *copy_puzzle(puzzle_t *puz) {
  //Copies a new puzzle and initializes the puzzle using init peers
  int i,j;
  puzzle_t *newPuz=malloc(sizeof(puzzle_t));
  memcpy(newPuz, puz, sizeof(puzzle_t));
  for(i=0;i<NUM_ROWS;i++)	
	for(j=0;j<NUM_COLS;j++){
	{
		init_peers(newPuz, i, j);
	}  
	}
	 return newPuz;

}


void print_puzzle(puzzle_t *p) {
  int i, j;
  for (i=0; i<NUM_ROWS; i++) {
    for (j=0; j<NUM_COLS; j++) {
      printf(" %9s", p->squares[i][j].vals); // may be useful while debugging
      //printf(" %2s", p->squares[i][j].vals);
    }
    printf("\n");
  }
}

/**********/
/* Search */
/**********/

static puzzle_t *search(puzzle_t *puz) {
int i,j,l;  
int k;
//square_t success;
for(k=0;k<20;k++)
{
	for(i=0;i<NUM_ROWS;i++){
		for(j=0;j<NUM_COLS;j++){
			
			if(assign(puz, i,j, puz->squares[i][j].vals[0])==NULL) {
				return NULL;
			}	
		}	

	}	

}	
square_t *we_da_best=find_candidate(puz);
if(we_da_best==NULL)
{
	return puz;
}
for(l=0;l<strlen(we_da_best->vals);l++){
	puzzle_t *shadow_clone=copy_puzzle(puz);
	shadow_clone->squares[we_da_best->row][we_da_best->col].vals[0]=we_da_best->vals[l];
	shadow_clone->squares[we_da_best->row][we_da_best->col].vals[1]='\0';
	puzzle_t *value;
	if((value=search(shadow_clone))!=NULL)
	{
		return value;	
	}
	
	free_puzzle(shadow_clone);
	
	

}

return NULL;
}

/**************************/
/* Constraint propagation */
/**************************/

static puzzle_t *assign(puzzle_t *puz, int row, int col, char val) {
	//Finds a square with one digit, and then eliminates the digit from the squares' peers
	//int i,j;
	//square_t *square=&(puz->squares[row][col]);
	if((strlen(puz->squares[row][col].vals))==1)
	{
		if(eliminate(puz, row, col, val)==NULL)
		{
			return NULL;
		}
	}				

	return puz;
}

static puzzle_t *eliminate(puzzle_t *puz, int row, int col, char val) {
  	/*//Digits Array
  	int digits[];
	for(l=0;l<=9;l++){
		digits[l++]=l++;
	}

	char indicator=0;*/

	//First Part of Eliminate
	int i;
	assert(row<NUM_ROWS);
	assert(col<NUM_COLS);
	square_t *square=&(puz->squares[row][col]);
	for(i=0;i<NUM_PEERS;i++){
		remove_val(puz, square->peers[i]->row, square->peers[i]->col, val);
		if(strlen(square->peers[i]->vals) < 1)
        	{
                	return NULL;
        	}
	

	}

	return puz;
}	

/*****************************************/
/* Misc (e.g., utility) functions follow */
/*****************************************/
static void remove_val(puzzle_t *puz, int row, int col, char val)
{	
	//Remove the specific value from one square
	int i;
	char *value=puz->squares[row][col].vals;
	int length_value=strlen(value);
	int found_val=0;//boolean showing if it found the value or not
	if(length_value<=1 && value[0] == val)
	{
		value[0]='\0';
	}
	for(i=0;i<length_value;i++)
	{
		if((!found_val) && (value[i]==val)){
			found_val=1;
			//i=i-1;					
		}
		if(found_val){
			value[i]=value[i+1];
		}
	}
}

//List of possibilites greater 1, but less than anything else in the puz
static square_t *find_candidate(puzzle_t *puz)
{
	int i,j;
	square_t *candidate=NULL;
	int least_size=NUM_DIGITS+1;
	//int size=strlen(puz->squares[row][col].vals)
	for (i=0;i<NUM_ROWS;i++){
		for(j=0;j<NUM_COLS;j++){	
		int size=strlen(puz->squares[i][j].vals);		
			if(size==2)			
			{
				candidate=&(puz->squares[i][j]);
				return candidate;

			}
			else if(size>2 && size<least_size)
			{
				least_size=size;
				candidate=&(puz->squares[i][j]);	
			}
		
				
		}	 
	}
	return candidate;
}

	
	




