#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <math.h>


#include <unistd.h>
static pthread_mutex_t mt = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mt2 = PTHREAD_MUTEX_INITIALIZER;
static int finished = 0;
double similarity = 0.8;

#define MAX_INSTANCE 50
#define MAX_COST 15
struct SLink {
    int source;
    int dest;
};
#define Np 11 //9 original
#define Np_C 22 //Springer SI 20190930 for merged Carrier A and B say, the converged case. This is not used since 2024.

#define TOTAL_LINK 15
#define N_A 12
#define Ne_A 12
#define Nb_A 2
#define Nc_A 8

#define LINK_NUM_A 15

#define N_B 12
#define Ne_B 12
#define Nb_B 2
#define Nc_B 8
#define LINK_NUM_B 15

/*
#define N_C 24
#define Ne_C 24
#define Nb_C 4
#define Nc_C 16
#define LINK_NUM_C 41 //30+11
*/
// NICT 2024 Carrier C is added according to Subhadeep's request
#define N_C 12
#define Ne_C 12
#define Nb_C 2
#define Nc_C 8
#define LINK_NUM_C 15

#define C_A 1
#define W_A 4

#define C_B 1
#define W_B 4

#define C_C 1
#define W_C 4


#define MAX_NODE_PAIR_A 9
#define MAX_NODE_PAIR_B 9
#define MAX_NODE_PAIR_C 9

#define MAX_TEST_NODE_PAIR_A 70
#define MAX_TEST_NODE_PAIR_B 70
#define MAX_TEST_NODE_PAIR_C 70

int Total_Customer_Carrier;



int Total_test_nodepair_A;
int Total_test_nodepair_B;
int Total_test_nodepair_C;

int Cost_A[N_A];

int V_A[N_A] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
int Ve_A[Ne_A] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
int Vb_A[Nb_A] = {1,2};
int Vp_A[Np] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; //including border node

//int Vp_A[Np] = {3, 5, 7, 9};//subset edge node 
//int Vp_A[Np] = {3, 4, 5, 6, 7, 8, 9, 10, 11}; //original 20190107


int source_A[MAX_NODE_PAIR_A] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int dest_A[MAX_NODE_PAIR_A] =   {3, 4, 5, 6, 7, 8, 9, 10,11};
int weight_A[MAX_NODE_PAIR_A] = {10,10,10,10,10,10,10,10,10};
int traffic_A[MAX_NODE_PAIR_A] = {5, 5, 5, 5, 5, 5, 5, 5, 5};

int is_test_set_A[N_A][N_A];
int test_m_A[MAX_TEST_NODE_PAIR_A];
int test_n_A[MAX_TEST_NODE_PAIR_A];
int test_weight_A[MAX_TEST_NODE_PAIR_A];
int test_weight_cost_A[MAX_COST][N_A][N_A];


int degree_limit_A[N_A] = {2, 3, 4, 2, 2, 4, 4, 2, 4, 3, 2, 3};

int F_c_w_i_A[C_A][W_A][N_A] ={
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17
    
};

int F_i_A[N_A]={
    //0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17
};

struct SLink LINK_A[LINK_NUM_A];

int Link_A_Final[N_A][N_A];
int Link_A[N_A][N_A] = {
    //0  1  2  3  4  5  6  7  8  9 10  11 12 13 14 15 16 17
//0
//    1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//1
//    0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//2
    1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
//3
    0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 
//4
    0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 
//5
    1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 
//6
    1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 
//7
    1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 
//8
    1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 
//9
    1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 
//10
    1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 
//11
    1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 
//12
    1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 
//13
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1
//14
 //   1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0,
//15
 //   1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1,
//16
//    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1
//17
//    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1
};


int V_B[N_B] =   {0, 1, 2, 3, 4,  5,  6, 7, 8, 9, 10, 11};
int Ve_B[Ne_B] = {0, 1, 2, 3, 4,  5,  6, 7, 8, 9, 10, 11};
int Vb_B[Nb_B] = {1,2};
int Vp_B[Np] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; //including border node


//int Vp_B[Np] = {3, 5, 7, 9};
//int Vp_B[Np] = {3, 4, 5, 6, 7, 8, 9, 10, 11};


int Cost_B[N_B];

int source_B[MAX_NODE_PAIR_B] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int dest_B[MAX_NODE_PAIR_B] =   {3, 4, 5, 6, 7, 8, 9, 10,11};
int weight_B[MAX_NODE_PAIR_B] = {10,10,10,10,10,10,10,10,10};
int traffic_B[MAX_NODE_PAIR_B] = {5, 5, 5, 5, 5, 5, 5, 5, 5};

int is_test_set_B[N_B][N_B];
int test_m_B[MAX_TEST_NODE_PAIR_B];
int test_n_B[MAX_TEST_NODE_PAIR_B];
int test_weight_B[MAX_TEST_NODE_PAIR_B];

int test_weight_cost_B[MAX_COST][N_B][N_B];

int degree_limit_B[N_B] = {2, 3, 4, 2, 2, 4, 4, 2, 4, 3, 2, 3};

int F_c_w_i_B[C_B][W_B][N_B] ={
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17
};

int F_i_B[N_B]={
    //0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33
17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17
    
};

struct SLink LINK_B[LINK_NUM_B];
int Link_B_Final[N_B][N_B];
int Link_B[N_B][N_B] = {
    //0  1  2  3  4  5  6  7  8  9 10  11 12 13 14 15 16 17
//0
//    1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//1
//    0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//2
    1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
//3
    0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 
//4
    0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 
//5
    1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 
//6
    1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 
//7
    1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 
//8
    1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 
//9
    1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 
//10
    1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 
//11
    1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 
//12
    1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 
//13
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1
//14
 //   1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0,
//15
 //   1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1,
//16
//    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1
//17
//    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1
};

int V_C[N_C] =   {0, 1, 2, 3, 4,  5,  6, 7, 8, 9, 10, 11};
int Ve_C[Ne_C] = {0, 1, 2, 3, 4,  5,  6, 7, 8, 9, 10, 11};
int Vb_C[Nb_C] = {1,2};
int Vp_C[Np] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; //including border node


//int Vp_B[Np] = {3, 5, 7, 9};
//int Vp_B[Np] = {3, 4, 5, 6, 7, 8, 9, 10, 11};


int Cost_C[N_C];

int source_C[MAX_NODE_PAIR_C] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int dest_C[MAX_NODE_PAIR_C] =   {3, 4, 5, 6, 7, 8, 9, 10,11};
int weight_C[MAX_NODE_PAIR_C] = {10,10,10,10,10,10,10,10,10};
int traffic_C[MAX_NODE_PAIR_C] = {5, 5, 5, 5, 5, 5, 5, 5, 5};

int is_test_set_C[N_C][N_C];
int test_m_C[MAX_TEST_NODE_PAIR_C];
int test_n_C[MAX_TEST_NODE_PAIR_C];
int test_weight_C[MAX_TEST_NODE_PAIR_C];

int test_weight_cost_C[MAX_COST][N_C][N_C];

int degree_limit_C[N_C] = {2, 3, 4, 2, 2, 4, 4, 2, 4, 3, 2, 3};

int F_c_w_i_C[C_C][W_C][N_C] ={
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17
};

int F_i_C[N_C]={
    //0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33
17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17

};

struct SLink LINK_C[LINK_NUM_C];
int Link_C_Final[N_C][N_C];
int Link_C[N_C][N_C] = {
    //0  1  2  3  4  5  6  7  8  9 10  11 12 13 14 15 16 17
//0
//    1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//1
//    0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//2
    1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//3
    0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1,
//4
    0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
//5
    1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1,
//6
    1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1,
//7
    1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1,
//8
    1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1,
//9
    1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1,
//10
    1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0,
//11
    1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0,
//12
    1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1,
//13
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1
//14
 //   1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0,
//15
 //   1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1,
//16
//    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1
//17
//    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1
};
/*
//int V_C[N_C] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29};
int V_C[N_C] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};//

//int Ve_C[Ne_C] = {0, 1, 4, 6, 12, 13, 14, 15, 16, 20, 24, 27, 28, 29};
int Ve_C[Ne_C] = {0, 1, 2, 3, 4,  5,  6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
int Vb_C[Nb_C] = {1, 2, 13, 14};
int Vp_C[Np_C] = {1, 2, 3, 4,  5,  6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};



int Cost_C[N_C];

int source_C[MAX_NODE_PAIR_C] =    {0,  0,  0,  0,  0,  0,  0,  0,  0,  12, 12, 12, 12, 12, 12, 12, 12, 12};
int dest_C[MAX_NODE_PAIR_C] =      {3,  4,  5,  6,  7,  8,  9,  10, 11, 15, 16, 17, 18, 19, 20, 21, 22, 23};
int weight_C[MAX_NODE_PAIR_C] =    {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
int traffic_C[MAX_NODE_PAIR_C] =   {5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5};

int is_test_set_C[N_C][N_C];
int test_m_C[MAX_TEST_NODE_PAIR_C];
int test_n_C[MAX_TEST_NODE_PAIR_C];
int test_weight_C[MAX_TEST_NODE_PAIR_C];

int test_weight_cost_C[MAX_COST][N_C][N_C];



int degree_limit_C[N_C] = {2, 4, 5, 3, 3, 5, 5, 3, 5, 4, 3, 4,     2, 4, 5, 3, 3, 5, 5, 3, 5, 4, 3, 4};


int F_c_w_i_C[C_C][W_C][N_C] ={
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,   17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,   17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,   17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,   17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17
};

int F_i_C[N_C]={
    //0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,   17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17
};

struct SLink LINK_C[LINK_NUM_C];
int Link_C_Final[N_C][N_C];
int Link_C[N_C][N_C];/ * = {
    //0  1  2  3  4  5  6  7  8  9 10  11 12 13 14 15 16 17
    
    1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//3
    0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//4
    0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//5
    1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//6
    1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//7
    1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//8
    1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//9
    1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//10
    1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//11
    1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//12
    1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//13 
    1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//14
    1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//15
    1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//16
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//2
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//3
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//4
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//5
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
//6
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//7
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
//8
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1,
//9
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1,
//10
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1,
//11
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1,
//12
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,
//13
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0,
//14
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0,
//15
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1,
//16
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1

};* /

*/
int Link_E[Np][Np] = {
    //0  1  2  3  4  5  6  7  8  9 10  11 12 13 14 15 16 17
//0
//    1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//1
//    0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//2
//    1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
//3
    1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 
//4
    0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 
//5
    1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 
//6
    1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 
//7
    0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 
//8
    1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 
//9
    1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 
//10
    1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 
//11
    1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 
//12
    1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 
//13
    1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1
//14
 //   1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0,
//15
 //   1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1,
//16
//    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1
//17
//    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1
};

/*
#define N_A 18
#define Ne_A 8
#define Nc_A 9
#define LINK_NUM_A 27

#define N_B 18
#define Ne_B 8
#define Nc_B 9
#define LINK_NUM_B 27


#define N_C 36
#define Ne_C 16
#define Nc_C 18
#define LINK_NUM_C 54


#define C_A 1
#define W_A 5

#define C_B 1
#define W_B 5

#define C_C 3
#define W_C 5


#define MAX_NODE_PAIR_A 9
#define MAX_NODE_PAIR_B 9
#define MAX_NODE_PAIR_C 18

#define MAX_TEST_NODE_PAIR_A 70
#define MAX_TEST_NODE_PAIR_B 70
#define MAX_TEST_NODE_PAIR_C 70


int Total_test_nodepair_A;
int Total_test_nodepair_B;
int Total_test_nodepair_C;


int V_A[N_A] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
int Ve_A[Ne_A] = {1, 2, 3, 6, 8, 14, 15, 16};

int source_A[MAX_NODE_PAIR_A] = {0, 1, 1, 1, 2, 2, 4, 5, 5};
int dest_A[MAX_NODE_PAIR_A] = {2, 2, 4, 5, 3, 6, 6, 6, 7};
int weight_A[MAX_NODE_PAIR_A] = {10, 10, 10, 10, 10, 10, 10, 10, 10};

int is_test_set_A[N_A][N_A];
int test_m_A[MAX_TEST_NODE_PAIR_A];
int test_n_A[MAX_TEST_NODE_PAIR_A];
int test_weight_A[MAX_TEST_NODE_PAIR_A];

int degree_limit_A[N_A] = {2, 2, 3, 4, 4, 2, 2, 4, 4, 2, 4, 3, 2, 3, 4, 4, 3, 2};

int F_c_w_i_A[C_A][W_A][N_A] ={
    0, 7, 7, 7, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0,
    0, 7, 7, 7, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0,
    0, 7, 7, 7, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0,
    0, 7, 7, 7, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0,
    0, 7, 7, 7, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0

};

int F_i_A[N_A]={
    //0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33
    0, 7, 7, 7, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0
};

struct SLink LINK_A[LINK_NUM_A];

int Link_A_Final[N_A][N_A];
int Link_A[N_A][N_A] = {
    //0  1  2  3  4  5  6  7  8  9 10  11 12 13 14 15 16 17
//0
    1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//1
    0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//2
    0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//3
    1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//4
    1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//5
    1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//6
    1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//7
    1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
//8
    1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,
//9
    1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
//10
    1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1,
//11
    1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1,
//12
    1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1,
//13
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1,
//14
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1,
//15
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0,
//16
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0,
//17
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1
};


int V_B[N_B] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
int Ve_B[Ne_B] = {0, 2, 3, 7, 11, 14, 15, 16};


int source_B[MAX_NODE_PAIR_B] = {0, 1, 1, 2, 3, 4, 4, 5, 5};
int dest_B[MAX_NODE_PAIR_B] = {1, 2, 4, 3, 6, 5, 6, 6, 7};
int weight_B[MAX_NODE_PAIR_B] = {10, 10, 10, 10, 10, 10, 10, 10, 10};

int is_test_set_B[N_B][N_B];
int test_m_B[MAX_TEST_NODE_PAIR_B];
int test_n_B[MAX_TEST_NODE_PAIR_B];
int test_weight_B[MAX_TEST_NODE_PAIR_B];

int degree_limit_B[N_B] = {2, 2, 3, 4, 4, 2, 2, 4, 4, 2, 4, 3, 2, 3, 4, 4, 3, 2};

int F_c_w_i_B[C_B][W_B][N_B] ={
    7, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0,
    7, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0,
    7, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0,
    7, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0,
    7, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0
};

int F_i_B[N_B]={
    //0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33
    7, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0
};

struct SLink LINK_B[LINK_NUM_B];
int Link_B_Final[N_B][N_B];
int Link_B[N_B][N_B] = {
    //0  1  2  3  4  5  6  7  8  9 10  11 12 13 14 15 16 17
//0
    1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//1
    0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//2
    0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//3
    1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//4
    1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//5
    1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//6
    1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//7
    1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
//8
    1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,
//9
    1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
//10
    1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1,
//11
    1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1,
//12
    1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1,
//13
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1,
//14
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1,
//15
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0,
//16
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0,
//17
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1
};


int V_C[N_C] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};

int Ve_C[Ne_C] = {1, 2, 3, 6, 8, 14, 15, 16, 18, 20, 21, 25, 29, 32, 33, 34};



int source_C[MAX_NODE_PAIR_C] = {0, 1, 1, 1, 2, 2, 4, 5, 5, 8, 9, 9, 10, 11, 12, 12, 13, 13};
int dest_C[MAX_NODE_PAIR_C] = {2, 2, 4, 5, 3, 6, 6, 6, 7, 9, 10, 12, 11, 14, 13, 14, 14, 15};
int weight_C[MAX_NODE_PAIR_C] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};

int is_test_set_C[N_C][N_C];
int test_m_C[MAX_TEST_NODE_PAIR_C];
int test_n_C[MAX_TEST_NODE_PAIR_C];
int test_weight_C[MAX_TEST_NODE_PAIR_C];

int degree_limit_C[N_C] = {2, 2, 3, 4, 4, 2, 2, 4, 4, 2, 4, 3, 2, 3, 4, 4, 3, 2, 2, 2, 3, 4, 4, 2, 2, 4, 4, 2, 4, 3, 2, 3, 4, 4, 3, 2};

int F_c_w_i_C[C_C][W_C][N_C] ={
    0, 7, 7, 7, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 7, 7, 7, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 7, 7, 7, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 7, 7, 7, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 7, 7, 7, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 7, 7, 7, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0, 7, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0,
    0, 7, 7, 7, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0, 7, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0,
    0, 7, 7, 7, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0, 7, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0,
    0, 7, 7, 7, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0, 7, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0,
    0, 7, 7, 7, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0, 7, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0
};

int F_i_C[N_C]={
    //0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33
    0, 7, 7, 7, 0, 0, 7, 0, 7, 0, 0, 0, 0, 0, 7, 7, 7, 0, 7, 0, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 7, 7, 0
};
struct SLink LINK_C[LINK_NUM_C];
int Link_C_Final[N_C][N_C];
int Link_C[N_C][N_C] = {
    //0  1  2  3  4  5  6  7  8  9 10  11 12 13 14 15 16 17
//0
    1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//1
    0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//2
    0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//3
    1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//4
    1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//5
    1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//6
    1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//7
    1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//8
    1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//9
    1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//10
    1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//11
    1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//12
    1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//13
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//14
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//15
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//16
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//17
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //0
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//1
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//2
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//3
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//4
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//5
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//6
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//7
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
//8
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,
//9
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
//10
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1,
//11
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1,
//12
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1,
//13
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1,
//14
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1,
//15
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0,
//16
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0,
//17
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1
};

*/

FILE *fconf;




void write_conf(double link_damage_similarity, int vendor_id, int exist_link_num, char *dirname, char *filename, int total_node, int total_edge, int total_C, int total_W, int RR, int total_trial_num);

void write_conf_esen_dci(double link_damage_similarity, int vendor_id, int exist_link_num, int segment_recovery_max_cost, char *dirname, char *filename, int total_node, int total_edge, int total_C, int total_W, int RR, int total_trial_num);

void Generate_Conf(int surviving_links_number_A, int surviving_links_number_B, char * sdirname_A, char * sfilename_A,char * sdirname_B, char * sfilename_B, char * sdirname_C, char * sfilename_C);

void Generate_Individual_Carrier_Conf(int surviving_links_number_A, int surviving_links_number_B, int surviving_links_number_C, int segment_recovery_max_cost_A, int segment_recovery_max_cost_B, int segment_recovery_max_cost_C, char * sdirname_A, char * sfilename_A,char * sdirname_B, char * sfilename_B, char * sdirname_C, char * sfilename_C);


void Generate_Price_File(int surviving_links_number_A, int surviving_links_number_B, char * sdirname_A, char * sfilename_A,char * sdirname_B, char * sfilename_B, char * sdirname_C, char * sfilename_C);
void Generate_Task_File(int surviving_links_number_A, int surviving_links_number_B, char * sdirname_A, char * sfilename_A,char * sdirname_B, char * sfilename_B, char * sdirname_C, char * sfilename_C);


void write_conf(double link_damage_similarity, int vendor_id, int exist_link_num, char *dirname, char *filename, int total_node, int total_edge, int total_C, int total_W, int RR, int total_trial_num)
{
    
    int i, ii,jj,c,w;
    int cost;
    
    
    long u,v;
    double U,V;
    
    char filename_with_different_repaircost[1024]; //ONDM2019-SI revision 202004
    
  //  cost = 2;
//  for(cost = 1; cost <=7; cost+=2)
  for(cost = 4; cost <=10; cost+=3)
  {
    //sprintf(filename, ".\/interconnection-cost0.%d\/similarity%0.1f\/Vendor%d\/link%02d\/conf_%d_%02d_%02d.txt", cost, link_damage_similarity, vendor_id, exist_link_num, vendor_id, exist_link_num, seq_id);
//  sprintf(filename, ".\/Vendor%d\/link%02d\/conf_%d_%02d_%02d.txt",vendor_id, exist_link_num, vendor_id, exist_link_num, seq_id);

    if(cost == 11)
    { // this is for the original constant large repair cost, say, with a value 10.
        sprintf(filename_with_different_repaircost, "%s/cost_11/%s", dirname, filename); 
    }else{
        // for other cost specifications, just put them into individual directory.
        sprintf(filename_with_different_repaircost, "%s/cost_%02d/%s", dirname, cost, filename);
    }
        
    printf("%s\n", filename_with_different_repaircost);
    if ((fconf = fopen(filename_with_different_repaircost,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_with_different_repaircost);
        fclose(fconf);
        exit(1);
    }

    if(vendor_id == 0)
    {
        fprintf(fconf,"N=%d Ne=%d Np=%d BN=%d C=%d W=%d R=%d T=%d O=%d\n\n\n", total_node, total_edge, Np, Nb_A, total_C, total_W, RR, total_trial_num, RR);
    }else if(vendor_id == 1)
    {
        fprintf(fconf,"N=%d Ne=%d Np=%d BN=%d C=%d W=%d R=%d T=%d O=%d\n\n\n", total_node, total_edge, Np, Nb_B, total_C, total_W, RR, total_trial_num, RR);
    }else if(vendor_id == 2)
    {
        //fprintf(fconf,"N=%d Ne=%d Np=%d BN=%d C=%d W=%d R=%d T=%d O=%d\n\n\n", total_node, total_edge, Np_C, Nb_C, total_C, total_W, RR, total_trial_num, RR);
        fprintf(fconf,"N=%d Ne=%d Np=%d BN=%d C=%d W=%d R=%d T=%d O=%d\n\n\n", total_node, total_edge, Np, Nb_C, total_C, total_W, RR, total_trial_num, RR);
    }
    
    
   
    fprintf(fconf,"NODE\n");
    for (ii = 0; ii < total_node; ii ++)
    {
        fprintf(fconf,"%d, ", ii);
    }
    fprintf(fconf,"\n");

    fprintf(fconf,"EDGE\n");
    
    switch (vendor_id)
    {
        case 0: {
                  for (ii = 0; ii < Ne_A; ii ++)
                  {
                      fprintf(fconf,"%d, ", Ve_A[ii]);
                  }
            break;
        }
        case 1: {
                  for (ii = 0; ii < Ne_B; ii ++)
                  {
                      fprintf(fconf,"%d, ", Ve_B[ii]);
                  }
            break;
        }
        
        case 2: {

                  for (ii = 0; ii < Ne_C; ii ++)
                  {
                      fprintf(fconf,"%d, ", Ve_C[ii]);
                  }
            break;
        }            
    }
        
     fprintf(fconf,"\n");
     fprintf(fconf,"EPOC\n");

     
     switch (vendor_id)
     {
        case 0: {
            for (ii = 0; ii < Np; ii ++)
            {
                fprintf(fconf,"%d, ", Vp_A[ii]);
                
            }
            break;
        }
        case 1: {
            for (ii = 0; ii < Np; ii ++)
            {
                fprintf(fconf,"%d, ", Vp_B[ii]);
                
            }
            break;
        }
        case 2: {
            //for (ii = 0; ii < Np_C; ii ++) // Np_C is double Np_A and Np_B
            for (ii = 0; ii < Np; ii ++) // NICT 2024 Added a Carrier C, according to Subhadeep's request.
            {
                fprintf(fconf,"%d, ", Vp_C[ii]);
                
            }
            break;
        }
    }

    fprintf(fconf,"\n");
    fprintf(fconf,"BORDER_NODE\n");
    switch (vendor_id)
    {
        case 0: {
                   for (ii = 0; ii < Nb_A; ii ++)
                   {
                     fprintf(fconf,"%d, ", Vb_A[ii]);
                   }
                   break;
        }
        case 1: {
                   for (ii = 0; ii < Nb_B; ii ++)
                   {
                       fprintf(fconf,"%d, ", Vb_B[ii]);
                   }
                   break;
        }
        case 2: {
                   for (ii = 0; ii < Nb_C; ii ++)
                   {
                       fprintf(fconf,"%d, ", Vb_C[ii]);
                   }
                   break;
        }
    }
    
    fprintf(fconf,"\n\n\n\n");

    
    

    fprintf(fconf,"FEASIBLE_LIGHTPATH\n");
     switch (vendor_id)
    {
    case 0: {

        for (ii = 0; ii < N_A; ii++)
        {
            for (jj = 0; jj < N_A; jj++)
            {
                if(ii == jj)
                    fprintf(fconf,"0, ");
                else
                    fprintf(fconf,"1, ");
            }
            fprintf(fconf,"\n");
        }
        break;
    }
    case 1: {

        for (ii = 0; ii < N_B; ii++)
        {
            for (jj = 0; jj < N_B; jj++)
            {
                if(ii == jj)
                    fprintf(fconf,"0, ");
                else
                    fprintf(fconf,"1, ");
            }
            fprintf(fconf,"\n");
        }
        break;
    }
    case 2: {

        for (ii = 0; ii < N_C; ii++)
        {
            for (jj = 0; jj < N_C; jj++)
            {
                if(ii == jj)
                    fprintf(fconf,"0, ");
                else
                    fprintf(fconf,"1, ");
            }
            fprintf(fconf,"\n");
        }
        break;
    }
    }
    fprintf(fconf,"\n\n\n\n");

    
      fprintf(fconf,"COST_NODE\n");
    switch (vendor_id)
    {
    case 0: {

        for (ii = 0; ii < N_A; ii++)
        {
              fprintf(fconf,"%d, ", Cost_A[ii]);
        }
        break;
    }
    case 1: {

        for (ii = 0; ii < N_B; ii++)
        {
              fprintf(fconf,"%d, ", Cost_B[ii]);
        }
        break;
    }
    case 2: {
        for (ii = 0; ii < N_C; ii++)
        {
              fprintf(fconf,"%d, ", Cost_C[ii]);
        }
        break;
    }
    }
    fprintf(fconf,"\n");

    fprintf(fconf,"\n\n\n");
    
    
    

    fprintf(fconf,"SOURCE\n");
    for (ii = 0; ii < RR; ii ++)
    {
        switch (vendor_id)
        {
        case 0: {
            fprintf(fconf,"%d, ", source_A[ii]);
            break;
        }
        case 1: {
            fprintf(fconf,"%d, ", source_B[ii]);
            break;
        }
        case 2: {
            fprintf(fconf,"%d, ", source_C[ii]);
            break;
        }
        }
    }
    fprintf(fconf,"\n");

    fprintf(fconf,"DEST\n");
    for (ii = 0; ii < RR; ii ++)
    {
        switch (vendor_id)
        {
        case 0: {
            fprintf(fconf,"%d, ", dest_A[ii]);
            break;
        }
        case 1: {
            fprintf(fconf,"%d, ", dest_B[ii]);
            break;
        }
        case 2: {
            fprintf(fconf,"%d, ", dest_C[ii]);
            break;
        }
        }
    }
    fprintf(fconf,"\n");


    fprintf(fconf,"WEIGHT\n");
    for (ii = 0; ii < RR; ii ++)
    {
        switch (vendor_id)
        {
        case 0: {
            fprintf(fconf,"%d, ", weight_A[ii]);
            break;
        }
        case 1: {
            fprintf(fconf,"%d, ", weight_B[ii]);
            break;
        }
        case 2: {
            fprintf(fconf,"%d, ", weight_C[ii]);
            break;
        }
        }
    }
    fprintf(fconf,"\n");
    fprintf(fconf,"TRAFFIC_VOLUME\n");
    for (ii = 0; ii < RR; ii ++)
    {
        switch (vendor_id)
        {
        case 0: {
            fprintf(fconf,"%d, ", traffic_A[ii]);
            break;
        }
        case 1: {
            fprintf(fconf,"%d, ", traffic_B[ii]);
            break;
        }
        
        case 2: {
            fprintf(fconf,"%d, ", traffic_C[ii]);
            break;
        }
        }
    }
    fprintf(fconf,"\n");
    fprintf(fconf,"\n\n\n");
    
    fprintf(fconf,"OUTSIDE_SOURCE\n");
    for (ii = 0; ii < RR; ii ++)
    {
        switch (vendor_id)
        {
        case 0: {
            fprintf(fconf,"%d, ", source_A[ii]);
            break;
        }
        case 1: {
            fprintf(fconf,"%d, ", source_B[ii]);
            break;
        }
        case 2: {
            fprintf(fconf,"%d, ", source_C[ii]);
            break;
        }
        }
    }
    fprintf(fconf,"\n");

    fprintf(fconf,"INSIDE_DEST\n");
    for (ii = 0; ii < RR; ii ++)
    {
        switch (vendor_id)
        {
        case 0: {
            fprintf(fconf,"%d, ", dest_A[ii]);
            break;
        }
        case 1: {
            fprintf(fconf,"%d, ", dest_B[ii]);
            break;
        }
        case 2: {
            fprintf(fconf,"%d, ", dest_C[ii]);
            break;
        }
        }
    }
    fprintf(fconf,"\n");
    fprintf(fconf,"\n\n\n");

    fprintf(fconf,"TEST_M\n");
    switch (vendor_id)
    {
    case 0: {

        for (ii = 0; ii < total_node; ii++)
        {
            for (jj = ii + 1; jj < total_node; jj ++)
            {
                if (Link_A_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", ii);
            }
        }
        break;
    }
    case 1: {

        for (ii = 0; ii < total_node; ii++)
        {
            for (jj = ii + 1; jj < total_node; jj ++)
            {
                if (Link_B_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", ii);
            }
        }
        break;
    }
    
    case 2: {

        for (ii = 0; ii < N_A; ii++)
        {
            for (jj = ii + 1; jj < N_A; jj ++)
            {
                if (Link_A_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", ii);
            }
        }
        for (ii = 0; ii < N_B; ii++)
        {
            for (jj = ii + 1; jj < N_B; jj ++)
            {
                if (Link_B_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", ii + N_A);
            }
        }
        /*
        for (ii = 0; ii < N_A; ii++)
        {
            fprintf(fconf,"%d, ", ii);
        }
        */
        break;
    }
    
    }
    fprintf(fconf,"\n");




    fprintf(fconf,"TEST_N\n");
    switch (vendor_id)
    {
    case 0: {

        for (ii = 0; ii < total_node; ii++)
        {
            for (jj = ii + 1; jj < total_node; jj ++)
            {
                if (Link_A_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", jj);
            }
        }
        break;
    }
    case 1: {

        for (ii = 0; ii < total_node; ii++)
        {
            for (jj = ii + 1; jj < total_node; jj ++)
            {
                if (Link_B_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", jj);
            }
        }
        break;
    }
    case 2: {

        for (ii = 0; ii < N_A; ii++)
        {
            for (jj = ii + 1; jj < N_A; jj ++)
            {
                if (Link_A_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", jj);
            }
        }
        for (ii = 0; ii < N_B; ii++)
        {
            for (jj = ii + 1; jj < N_B; jj ++)
            {
                if (Link_B_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", jj + N_A);
            }
        }

        /*
        for (ii = 0; ii < N_A; ii++)
        {
            fprintf(fconf,"%d, ", ii + N_A);
        }
        */
        break;
    }
    }
    fprintf(fconf,"\n");



    fprintf(fconf,"TEST_WEIGHT\n");

    switch (vendor_id)
    {
    case 0: {

        for (ii = 0; ii < total_node; ii++)
        {
            for (jj = ii + 1; jj < total_node; jj ++)
            {
                if (Link_A_Final[ii][jj] == 0)
                {
                    if(cost == 11) //specify a constant large cost e.g., 10
                    {
                        fprintf(fconf,"11, ");
                    }else{
                        fprintf(fconf,"%d, ", test_weight_cost_A[cost][ii][jj]);
                    }
                }
            }
        }
        break;
    }
    case 1: {

        for (ii = 0; ii < total_node; ii++)
        {
            for (jj = ii + 1; jj < total_node; jj ++)
            {
                if (Link_B_Final[ii][jj] == 0)
                {
                    if(cost == 11) //specify a constant large cost e.g., 10
                    {
                        fprintf(fconf,"11, ");
                    }else{
                        fprintf(fconf,"%d, ", test_weight_cost_B[cost][ii][jj]);
                    }
                }
            }
        }
        break;
    }
    
    case 2: {

        for (ii = 0; ii < N_A; ii++)
        {
            for (jj = ii + 1; jj < N_A; jj ++)
            {
                if (Link_A_Final[ii][jj] == 0)
                {
                    if(cost == 11) //specify a constant large cost e.g., 10
                    {
                        fprintf(fconf,"11, ");
                    }else{
                        fprintf(fconf,"%d, ", test_weight_cost_A[cost][ii][jj]);
                    }
                }
            }
        }
        for (ii = 0; ii < N_B; ii++)
        {
            for (jj = ii + 1; jj < N_B; jj ++)
            {
                if (Link_B_Final[ii][jj] == 0)
                {
                    if(cost == 11) //specify a constant large cost e.g., 10
                    {
                        fprintf(fconf,"11, ");
                    }else{
                        fprintf(fconf,"%d, ", test_weight_cost_B[cost][ii][jj]);
                    }
                }
            }
        }

        /*
        for (ii = 0; ii < N_A; ii++)
        {
            fprintf(fconf,"%d, ",cost);   //Interconnection cost
        }
        */
        break;
    }
    
    }
    fprintf(fconf,"\n");

    fprintf(fconf,"\n\n\n");
    
    
    

    fprintf(fconf,"INTER_TEST_M\n");
    fprintf(fconf,"INTER_TEST_N\n");
    fprintf(fconf,"INTER_TEST_WEIGHT\n");

    fprintf(fconf,"\n\n\n");

    fprintf(fconf,"DEGREE_LIMIT\n");
    switch (vendor_id)
    {
    case 0: {

        for (ii = 0; ii < total_node; ii++)
        {
            fprintf(fconf,"%d, ", degree_limit_A[ii]);
        }
        break;
    }
    case 1: {
        for (ii = 0; ii < total_node; ii++)
        {
            fprintf(fconf,"%d, ", degree_limit_B[ii]);
        }
        break;
    }
    case 2: {
        for (ii = 0; ii < total_node; ii++)
        {
            fprintf(fconf,"%d, ", degree_limit_C[ii]);
        }
        break;
    }
    }
    fprintf(fconf,"\n");


    fprintf(fconf,"\n\n\n");

    fprintf(fconf,"F_c_w_i\n");

    for (c = 0; c < total_C; c++)
    {
        for (w = 0; w < total_W; w++)
        {
            switch (vendor_id)
            {
            case 0: {

                for (ii = 0; ii < total_node; ii++)
                {
                    fprintf(fconf,"%d, ", F_c_w_i_A[c][w][ii]);
                }
                break;
            }
            case 1: {
                for (ii = 0; ii < total_node; ii++)
                {
                    fprintf(fconf,"%d, ", F_c_w_i_B[c][w][ii]);
                }
                break;
            }
            case 2: {
                for (ii = 0; ii < total_node; ii++)
                {
                    fprintf(fconf,"%d, ", F_c_w_i_C[c][w][ii]);
                }
                break;
            }
            }
            fprintf(fconf,"\n");
        }
        fprintf(fconf,"\n");
    }


    fprintf(fconf,"\n\n\n");

    fprintf(fconf,"F_i\n");
    switch (vendor_id)
    {
    case 0: {

        for (ii = 0; ii < total_node; ii++)
        {
            fprintf(fconf,"%d, ", F_i_A[ii]);
        }
        break;
    }
    case 1: {
        for (ii = 0; ii < total_node; ii++)
        {
            fprintf(fconf,"%d, ",  F_i_B[ii]);
        }
        break;
    }
    case 2: {
        for (ii = 0; ii < total_node; ii++)
        {
            fprintf(fconf,"%d, ",  F_i_C[ii]);
        }
        break;
    }
    }

    fprintf(fconf,"\n\n\n");
    fprintf(fconf,"U_w_m_n\n");

    for (w = 0; w < total_W; w++)
    {
        switch (vendor_id)
        {
        case 0: {

            for (ii = 0; ii < N_A; ii++)
            {
                for (jj = 0; jj < N_A; jj ++)
                {
                    if (Link_A_Final[ii][jj] == 2)
                        fprintf(fconf,"0, ");
                    else
                        fprintf(fconf,"1, ");
                }
                fprintf(fconf,"\n");
            }
            break;
        }
        case 1: {
            for (ii = 0; ii < N_B; ii++)
            {
                for (jj = 0; jj < N_B; jj ++)
                {
                    if (Link_B_Final[ii][jj] == 2)
                        fprintf(fconf,"0, ");
                    else
                        fprintf(fconf,"1, ");
                }
                fprintf(fconf,"\n");
            }
            break;
        }
        case 2: {

            for (ii = 0; ii < N_A; ii++)
            {
                for (jj = 0; jj < N_A; jj ++)
                {
                    if (Link_A_Final[ii][jj] == 2)
                        fprintf(fconf,"0, ");
                    else
                        fprintf(fconf,"1, ");
                }
                for (jj = 0; jj < N_B; jj ++)
                {
                    if (ii == jj && ii != 0)
                        fprintf(fconf,"0, ");
                    else
                        fprintf(fconf,"1, ");
                }
                fprintf(fconf,"\n");
            }

            for (ii = 0; ii < N_B; ii++)
            {
                for (jj = 0; jj < N_A; jj ++)
                {
                    if (ii == jj && ii != 0)
                        fprintf(fconf,"0, ");
                    else
                        fprintf(fconf,"1, ");
                }
                for (jj = 0; jj < N_B; jj ++)
                {
                    if (Link_B_Final[ii][jj] == 2)
                        fprintf(fconf,"0, ");
                    else
                        fprintf(fconf,"1, ");
                }
                fprintf(fconf,"\n");
            }
            break;
        }
        }
        fprintf(fconf,"\n");
    }
    fprintf(fconf,"\n");

    fprintf(fconf,"\n\n\n");
    fprintf(fconf,"LINK\n");


    switch (vendor_id)
    {
    case 0: {

        for (ii = 0; ii < N_A; ii++)
        {
            for (jj = 0; jj < N_A; jj ++)
            {
                if (Link_A_Final[ii][jj] == 2)
                    fprintf(fconf,"0, ");
                else
                    fprintf(fconf,"1, ");
            }
            fprintf(fconf,"\n");
        }
        break;
    }
    case 1: {
        for (ii = 0; ii < N_B; ii++)
        {
            for (jj = 0; jj < N_B; jj ++)
            {
                if (Link_B_Final[ii][jj] == 2)
                    fprintf(fconf,"0, ");
                else
                    fprintf(fconf,"1, ");
            }
            fprintf(fconf,"\n");
        }
        break;
    }
    case 2: {

        for (ii = 0; ii < N_A; ii++)
        {
            for (jj = 0; jj < N_A; jj ++)
            {
                if (Link_A_Final[ii][jj] == 2)
                    fprintf(fconf,"0, ");
                else
                    fprintf(fconf,"1, ");
            }
            for (jj = 0; jj < N_B; jj ++)
            {
                    if (ii == jj && ii != 0)
                        fprintf(fconf,"0, ");
                    else
                        fprintf(fconf,"1, ");
            }
            fprintf(fconf,"\n");
        }

        for (ii = 0; ii < N_B; ii++)
        {
            for (jj = 0; jj < N_A; jj ++)
            {
                    if (ii == jj && ii != 0)
                        fprintf(fconf,"0, ");
                    else
                        fprintf(fconf,"1, ");
            }
            for (jj = 0; jj < N_B; jj ++)
            {
                if (Link_B_Final[ii][jj] == 2)
                    fprintf(fconf,"0, ");
                else
                    fprintf(fconf,"1, ");
            }
            fprintf(fconf,"\n");
        }
        break;
    }
    }

    fprintf(fconf,"\n");
    fclose(fconf);
    } //end of cost
//end of cost
}


void write_conf_esen_dci(double link_damage_similarity, int vendor_id, int exist_link_num, int segment_recovery_max_cost, char* dirname, char* filename, int total_node, int total_edge, int total_C, int total_W, int RR, int total_trial_num)
{
    
    int i, j, ii,jj,c,w;
    int cost;
    int link_ID;
    
    
    long u,v;
    double U,V;
    
    char filename_with_different_repaircost[1024]; //ONDM2019-SI revision 202004
    
  //  cost = 2;
//  for(cost = 1; cost <=7; cost+=2)
  //for(cost = 4; cost <=10; cost+=3)
  cost = segment_recovery_max_cost;
  {
    //sprintf(filename, ".\/interconnection-cost0.%d\/similarity%0.1f\/Vendor%d\/link%02d\/conf_%d_%02d_%02d.txt", cost, link_damage_similarity, vendor_id, exist_link_num, vendor_id, exist_link_num, seq_id);
//  sprintf(filename, ".\/Vendor%d\/link%02d\/conf_%d_%02d_%02d.txt",vendor_id, exist_link_num, vendor_id, exist_link_num, seq_id);

    if(cost == 11)
    { // this is for the original constant large repair cost, say, with a value 10.
        sprintf(filename_with_different_repaircost, "%s/%s", dirname, filename); 
    }else{
        // for other cost specifications, just put them into individual directory.
        sprintf(filename_with_different_repaircost, "%s/%s", dirname, filename);
    }

    sprintf(filename_with_different_repaircost, "%s/%s.input.nodes", dirname, filename);
    if ((fconf = fopen(filename_with_different_repaircost,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_with_different_repaircost);
        exit(1);
    }
    
    fprintf(fconf,"ID\n");
    switch (vendor_id)
    {
        case 0: {
            for (i = 1; i < N_A; i ++)
            {
                fprintf(fconf,"%d\n", i);
            }
            break;
        }
        case 1: {
            for (i = 1; i < N_B; i ++)
            {
                fprintf(fconf,"%d\n", i);
            }
            break;
        }
        case 2: {
            for (i = 1; i < N_C; i ++)
            {
                fprintf(fconf,"%d\n", i);
            }
            break;
        }
        default: break;
    }

    fclose(fconf);
    
        
    sprintf(filename_with_different_repaircost, "%s/%s.input.links", dirname, filename);
    if ((fconf = fopen(filename_with_different_repaircost,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_with_different_repaircost);
        exit(1);
    }
    
    fprintf(fconf,"ID,Src_node,Dst_node,Flag\n");
    link_ID = 1;
/*
    switch (vendor_id)
    {
        case 0: {
            
            for (i = 0; i < N_A; i ++)
            {
                for (j = i +1; j < N_A; j++)
                {
                    if (Link_A_Final[i][j] == 2) //NEED CONFIRM 20210916: 0 here means the link is OK and will be outputed, other links ==1 will not be outputed.
                    {
                        fprintf(fconf,"%d,%d,%d,1\n", link_ID, i, j);
                        link_ID ++;
                    }
                    if (Link_A_Final[i][j] == 0) //NEED CONFIRM 20210916: 0 here means the link is OK and will be outputed, other links ==1 will not be outputed.
                    {
                        fprintf(fconf,"%d,%d,%d,0\n", link_ID, i, j);
                        link_ID ++;
                    }
                    
                }
            }
            break;
        }
        case 1: {
            for (i = 0; i < N_B; i ++)
            {
                for (j = i +1; j < N_B; j++)
                {
                    if (Link_B_Final[i][j] == 2) //NEED CONFIRM 20210916: 0 here means the link is OK and will be outputed, other links ==1 will not be outputed.
                    {
                        fprintf(fconf,"%d,%d,%d,1\n", link_ID, i, j);
                        link_ID ++;
                    }
                    if (Link_B_Final[i][j] == 0) //NEED CONFIRM 20210916: 0 here means the link is OK and will be outputed, other links ==1 will not be outputed.
                    {
                        fprintf(fconf,"%d,%d,%d,0\n", link_ID, i, j);
                        link_ID ++;
                    }
                    
                }
            }
            break;
        }
        case 2: { //NICT 2024 Added a Carrier C according to Subhadeep's request for Globecom2022 extension.
            for (i = 0; i < N_C; i ++)
            {
                for (j = i +1; j < N_C; j++)
                {
                    if (Link_C_Final[i][j] == 2) //NEED CONFIRM 20210916: 0 here means the link is OK and will be outputed, other links ==1 will not be outputed.
                    {
                        fprintf(fconf,"%d,%d,%d,1\n", link_ID, i, j);
                        link_ID ++;
                    }
                    if (Link_C_Final[i][j] == 0) //NEED CONFIRM 20210916: 0 here means the link is OK and will be outputed, other links ==1 will not be outputed.
                    {
                        fprintf(fconf,"%d,%d,%d,0\n", link_ID, i, j);
                        link_ID ++;
                    }

                }
            }
            break;
        }
    }

*/




//NICT 20250328
printf("############");
    switch (vendor_id)
    {
        case 0: {
            
            for (i = 1; i < N_A; i ++)
            {
                for (j = i + 1; j < N_A; j++)
                {
                    if (Link_A_Final[i][j] == 2) //NEED CONFIRM 20210916: 0 here means the link is OK and will be outputed, other links ==1 will not be outputed.
                    {
                        printf("%d,%d,%d,1\n", link_ID, i - 1, j - 1);
                        fprintf(fconf,"%d,%d,%d,1\n", link_ID, i - 1, j - 1);
                        link_ID ++;
                    }
                    if (Link_A_Final[i][j] == 0) //NEED CONFIRM 20210916: 0 here means the link is OK and will be outputed, other links ==1 will not be outputed.
                    {
                        printf("%d,%d,%d,0\n", link_ID, i - 1, j - 1);
                        fprintf(fconf,"%d,%d,%d,0\n", link_ID, i - 1, j - 1);
                        link_ID ++;
                    }
                    
                }
            }
            break;
        }
        case 1: {
            for (i = 1; i < N_B; i ++)
            {
                for (j = i +1; j < N_B; j++)
                {
                    if (Link_B_Final[i][j] == 2) //NEED CONFIRM 20210916: 0 here means the link is OK and will be outputed, other links ==1 will not be outputed.
                    {
                        fprintf(fconf,"%d,%d,%d,1\n", link_ID, i -1 , j - 1);
                        link_ID ++;
                    }
                    if (Link_B_Final[i][j] == 0) //NEED CONFIRM 20210916: 0 here means the link is OK and will be outputed, other links ==1 will not be outputed.
                    {
                        fprintf(fconf,"%d,%d,%d,0\n", link_ID, i - 1, j -1);
                        link_ID ++;
                    }
                    
                }
            }
            break;
        }
        case 2: { //NICT 2024 Added a Carrier C according to Subhadeep's request for Globecom2022 extension.
            for (i = 1; i < N_C; i ++)
            {
                for (j = i +1; j < N_C; j++)
                {
                    if (Link_C_Final[i][j] == 2) //NEED CONFIRM 20210916: 0 here means the link is OK and will be outputed, other links ==1 will not be outputed.
                    {
                        fprintf(fconf,"%d,%d,%d,1\n", link_ID, i - 1, j - 1);
                        link_ID ++;
                    }
                    if (Link_C_Final[i][j] == 0) //NEED CONFIRM 20210916: 0 here means the link is OK and will be outputed, other links ==1 will not be outputed.
                    {
                        fprintf(fconf,"%d,%d,%d,0\n", link_ID, i - 1, j - 1);
                        link_ID ++;
                    }

                }
            }
            break;
        }
    }

    fclose(fconf);
        
    
    sprintf(filename_with_different_repaircost, "%s/%s.input.conf", dirname, filename);
    
    printf("%s\n", filename_with_different_repaircost);
    
    if ((fconf = fopen(filename_with_different_repaircost,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_with_different_repaircost);
        exit(1);
    }

    if(vendor_id == 0)
    {
        fprintf(fconf,"N=%d Ne=%d Np=%d BN=%d C=%d W=%d R=%d T=%d O=%d\n\n\n", total_node, total_edge, Np, Nb_A, total_C, total_W, RR, total_trial_num, RR);
    }else if(vendor_id == 1)
    {
        fprintf(fconf,"N=%d Ne=%d Np=%d BN=%d C=%d W=%d R=%d T=%d O=%d\n\n\n", total_node, total_edge, Np, Nb_B, total_C, total_W, RR, total_trial_num, RR);
    }else if(vendor_id == 2)
    {
        //fprintf(fconf,"N=%d Ne=%d Np=%d BN=%d C=%d W=%d R=%d T=%d O=%d\n\n\n", total_node, total_edge, Np_C, Nb_C, total_C, total_W, RR, total_trial_num, RR);
        //NICT 2024 Added a Carrier C, according to Subhadeep's request for Globecom2022 Extension.
        fprintf(fconf,"N=%d Ne=%d Np=%d BN=%d C=%d W=%d R=%d T=%d O=%d\n\n\n", total_node, total_edge, Np, Nb_C, total_C, total_W, RR, total_trial_num, RR);
    }
    
    
   
    fprintf(fconf,"NODE\n");
    for (ii = 0; ii < total_node; ii ++)
    {
        fprintf(fconf,"%d, ", ii);
    }
    fprintf(fconf,"\n");

    fprintf(fconf,"EDGE\n");
    
    switch (vendor_id)
    {
        case 0: {
                  for (ii = 0; ii < Ne_A; ii ++)
                  {
                      fprintf(fconf,"%d, ", Ve_A[ii]);
                  }
            break;
        }
        case 1: {
                  for (ii = 0; ii < Ne_B; ii ++)
                  {
                      fprintf(fconf,"%d, ", Ve_B[ii]);
                  }
            break;
        }
        
        case 2: {

                  for (ii = 0; ii < Ne_C; ii ++)
                  {
                      fprintf(fconf,"%d, ", Ve_C[ii]);
                  }
            break;
        }            
    }
        
     fprintf(fconf,"\n");
     fprintf(fconf,"EPOC\n");

     
     switch (vendor_id)
     {
        case 0: {
            for (ii = 0; ii < Np; ii ++)
            {
                fprintf(fconf,"%d, ", Vp_A[ii]);
                
            }
            break;
        }
        case 1: {
            for (ii = 0; ii < Np; ii ++)
            {
                fprintf(fconf,"%d, ", Vp_B[ii]);
                
            }
            break;
        }
        case 2: {
            //for (ii = 0; ii < Np_C; ii ++) // Np_C is double Np_A and Np_B
            for (ii = 0; ii < Np; ii ++) // NICT 2024 Added a Carrier C, according to Subhadeep's request for Globecom2022 Extension.
            {
                fprintf(fconf,"%d, ", Vp_C[ii]);
            }
            break;
        }
    }

    fprintf(fconf,"\n");
    fprintf(fconf,"BORDER_NODE\n");
    switch (vendor_id)
    {
        case 0: {
                   for (ii = 0; ii < Nb_A; ii ++)
                   {
                     fprintf(fconf,"%d, ", Vb_A[ii]);
                   }
                   break;
        }
        case 1: {
                   for (ii = 0; ii < Nb_B; ii ++)
                   {
                       fprintf(fconf,"%d, ", Vb_B[ii]);
                   }
                   break;
        }
        case 2: {
                   for (ii = 0; ii < Nb_C; ii ++)
                   {
                       fprintf(fconf,"%d, ", Vb_C[ii]);
                   }
                   break;
        }
    }
    
    fprintf(fconf,"\n\n\n\n");

    
    

    fprintf(fconf,"FEASIBLE_LIGHTPATH\n");
     switch (vendor_id)
    {
    case 0: {

        for (ii = 0; ii < N_A; ii++)
        {
            for (jj = 0; jj < N_A; jj++)
            {
                if(ii == jj)
                    fprintf(fconf,"0, ");
                else
                    fprintf(fconf,"1, ");
            }
            fprintf(fconf,"\n");
        }
        break;
    }
    case 1: {

        for (ii = 0; ii < N_B; ii++)
        {
            for (jj = 0; jj < N_B; jj++)
            {
                if(ii == jj)
                    fprintf(fconf,"0, ");
                else
                    fprintf(fconf,"1, ");
            }
            fprintf(fconf,"\n");
        }
        break;
    }
    case 2: {

        for (ii = 0; ii < N_C; ii++)
        {
            for (jj = 0; jj < N_C; jj++)
            {
                if(ii == jj)
                    fprintf(fconf,"0, ");
                else
                    fprintf(fconf,"1, ");
            }
            fprintf(fconf,"\n");
        }
        break;
    }
    }
    fprintf(fconf,"\n\n\n\n");

    
      fprintf(fconf,"COST_NODE\n");
    switch (vendor_id)
    {
    case 0: {

        for (ii = 0; ii < N_A; ii++)
        {
              fprintf(fconf,"%d, ", Cost_A[ii]);
        }
        break;
    }
    case 1: {

        for (ii = 0; ii < N_B; ii++)
        {
              fprintf(fconf,"%d, ", Cost_B[ii]);
        }
        break;
    }
    case 2: {
        for (ii = 0; ii < N_C; ii++)
        {
              fprintf(fconf,"%d, ", Cost_C[ii]);
        }
        break;
    }
    }
    fprintf(fconf,"\n");

    fprintf(fconf,"\n\n\n");
    
    
    

    fprintf(fconf,"SOURCE\n");
    for (ii = 0; ii < RR; ii ++)
    {
        switch (vendor_id)
        {
        case 0: {
            fprintf(fconf,"%d, ", source_A[ii]);
            break;
        }
        case 1: {
            fprintf(fconf,"%d, ", source_B[ii]);
            break;
        }
        case 2: {
            fprintf(fconf,"%d, ", source_C[ii]);
            break;
        }
        }
    }
    fprintf(fconf,"\n");

    fprintf(fconf,"DEST\n");
    for (ii = 0; ii < RR; ii ++)
    {
        switch (vendor_id)
        {
        case 0: {
            fprintf(fconf,"%d, ", dest_A[ii]);
            break;
        }
        case 1: {
            fprintf(fconf,"%d, ", dest_B[ii]);
            break;
        }
        case 2: {
            fprintf(fconf,"%d, ", dest_C[ii]);
            break;
        }
        }
    }
    fprintf(fconf,"\n");


    fprintf(fconf,"WEIGHT\n");
    for (ii = 0; ii < RR; ii ++)
    {
        switch (vendor_id)
        {
        case 0: {
            fprintf(fconf,"%d, ", weight_A[ii]);
            break;
        }
        case 1: {
            fprintf(fconf,"%d, ", weight_B[ii]);
            break;
        }
        case 2: {
            fprintf(fconf,"%d, ", weight_C[ii]);
            break;
        }
        }
    }
    fprintf(fconf,"\n");
    fprintf(fconf,"TRAFFIC_VOLUME\n");
    for (ii = 0; ii < RR; ii ++)
    {
        switch (vendor_id)
        {
        case 0: {
            fprintf(fconf,"%d, ", traffic_A[ii]);
            break;
        }
        case 1: {
            fprintf(fconf,"%d, ", traffic_B[ii]);
            break;
        }
        
        case 2: {
            fprintf(fconf,"%d, ", traffic_C[ii]);
            break;
        }
        }
    }
    fprintf(fconf,"\n");
    fprintf(fconf,"\n\n\n");
    
    fprintf(fconf,"OUTSIDE_SOURCE\n");
    for (ii = 0; ii < RR; ii ++)
    {
        switch (vendor_id)
        {
        case 0: {
            fprintf(fconf,"%d, ", source_A[ii]);
            break;
        }
        case 1: {
            fprintf(fconf,"%d, ", source_B[ii]);
            break;
        }
        case 2: {
            fprintf(fconf,"%d, ", source_C[ii]);
            break;
        }
        }
    }
    fprintf(fconf,"\n");

    fprintf(fconf,"INSIDE_DEST\n");
    for (ii = 0; ii < RR; ii ++)
    {
        switch (vendor_id)
        {
        case 0: {
            fprintf(fconf,"%d, ", dest_A[ii]);
            break;
        }
        case 1: {
            fprintf(fconf,"%d, ", dest_B[ii]);
            break;
        }
        case 2: {
            fprintf(fconf,"%d, ", dest_C[ii]);
            break;
        }
        }
    }
    fprintf(fconf,"\n");
    fprintf(fconf,"\n\n\n");

    fprintf(fconf,"TEST_M\n");
    switch (vendor_id)
    {
    case 0: {

        for (ii = 0; ii < total_node; ii++)
        {
            for (jj = ii + 1; jj < total_node; jj ++)
            {
                if (Link_A_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", ii);
            }
        }
        break;
    }
    case 1: {

        for (ii = 0; ii < total_node; ii++)
        {
            for (jj = ii + 1; jj < total_node; jj ++)
            {
                if (Link_B_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", ii);
            }
        }
        break;
    }
    
    case 2: { //NICT 2024 Added a Carrier C according to Subhadeep's request for Globecom2022 extension.
        for (ii = 0; ii < total_node; ii++)
        {
            for (jj = ii + 1; jj < total_node; jj ++)
            {
                if (Link_C_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", ii);
            }
        }

        /*
        for (ii = 0; ii < N_A; ii++)
        {
            for (jj = ii + 1; jj < N_A; jj ++)
            {
                if (Link_A_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", ii);
            }
        }
        for (ii = 0; ii < N_B; ii++)
        {
            for (jj = ii + 1; jj < N_B; jj ++)
            {
                if (Link_B_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", ii + N_A);
            }
        }
        */
        /*
        for (ii = 0; ii < N_A; ii++)
        {
            fprintf(fconf,"%d, ", ii);
        }
        */
        break;
    }
    
    }
    fprintf(fconf,"\n");




    fprintf(fconf,"TEST_N\n");
    switch (vendor_id)
    {
    case 0: {

        for (ii = 0; ii < total_node; ii++)
        {
            for (jj = ii + 1; jj < total_node; jj ++)
            {
                if (Link_A_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", jj);
            }
        }
        break;
    }
    case 1: {

        for (ii = 0; ii < total_node; ii++)
        {
            for (jj = ii + 1; jj < total_node; jj ++)
            {
                if (Link_B_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", jj);
            }
        }
        break;
    }
    case 2: {//NICT 2024 Added a Carrier C according to Subhadeep's request for Globecom2022 extension.

        for (ii = 0; ii < total_node; ii++)
        {
            for (jj = ii + 1; jj < total_node; jj ++)
            {
                if (Link_C_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", jj);
            }
        }

        /*
        for (ii = 0; ii < N_A; ii++)
        {
            for (jj = ii + 1; jj < N_A; jj ++)
            {
                if (Link_A_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", jj);
            }
        }
        for (ii = 0; ii < N_B; ii++)
        {
            for (jj = ii + 1; jj < N_B; jj ++)
            {
                if (Link_B_Final[ii][jj] == 0)
                    fprintf(fconf,"%d, ", jj + N_A);
            }
        }
        */

        /*
        for (ii = 0; ii < N_A; ii++)
        {
            fprintf(fconf,"%d, ", ii + N_A);
        }
        */
        break;
    }
    }
    fprintf(fconf,"\n");



    fprintf(fconf,"TEST_WEIGHT\n");

    switch (vendor_id)
    {
    case 0: {

        for (ii = 0; ii < total_node; ii++)
        {
            for (jj = ii + 1; jj < total_node; jj ++)
            {
                if (Link_A_Final[ii][jj] == 0)
                {
                    if(cost == 11) //specify a constant large cost e.g., 10
                    {
                        fprintf(fconf,"11, ");
                    }else{
                        fprintf(fconf,"%d, ", test_weight_cost_A[cost][ii][jj]);
                    }
                }
            }
        }
        break;
    }
    case 1: {

        for (ii = 0; ii < total_node; ii++)
        {
            for (jj = ii + 1; jj < total_node; jj ++)
            {
                if (Link_B_Final[ii][jj] == 0)
                {
                    if(cost == 11) //specify a constant large cost e.g., 10
                    {
                        fprintf(fconf,"11, ");
                    }else{
                        fprintf(fconf,"%d, ", test_weight_cost_B[cost][ii][jj]);
                    }
                }
            }
        }
        break;
    }
    
    case 2: {//NICT 2024 Added a Carrier C according to Subhadeep's request for Globecom2022 extension.

        for (ii = 0; ii < total_node; ii++)
        {
            for (jj = ii + 1; jj < total_node; jj ++)
            {
                if (Link_C_Final[ii][jj] == 0)
                {
                    if(cost == 11) //specify a constant large cost e.g., 10
                    {
                        fprintf(fconf,"11, ");
                    }else{
                        fprintf(fconf,"%d, ", test_weight_cost_C[cost][ii][jj]);
                    }
                }
            }
        }

        /*
        for (ii = 0; ii < N_A; ii++)
        {
            for (jj = ii + 1; jj < N_A; jj ++)
            {
                if (Link_A_Final[ii][jj] == 0)
                {
                    if(cost == 11) //specify a constant large cost e.g., 10
                    {
                        fprintf(fconf,"11, ");
                    }else{
                        fprintf(fconf,"%d, ", test_weight_cost_A[cost][ii][jj]);
                    }
                }
            }
        }
        for (ii = 0; ii < N_B; ii++)
        {
            for (jj = ii + 1; jj < N_B; jj ++)
            {
                if (Link_B_Final[ii][jj] == 0)
                {
                    if(cost == 11) //specify a constant large cost e.g., 10
                    {
                        fprintf(fconf,"11, ");
                    }else{
                        fprintf(fconf,"%d, ", test_weight_cost_B[cost][ii][jj]);
                    }
                }
            }
        }
        */

        /*
        for (ii = 0; ii < N_A; ii++)
        {
            fprintf(fconf,"%d, ",cost);   //Interconnection cost
        }
        */
        break;
    }
    
    }
    fprintf(fconf,"\n");

    fprintf(fconf,"\n\n\n");
    
    
    

    fprintf(fconf,"INTER_TEST_M\n");
    fprintf(fconf,"INTER_TEST_N\n");
    fprintf(fconf,"INTER_TEST_WEIGHT\n");

    fprintf(fconf,"\n\n\n");

    fprintf(fconf,"DEGREE_LIMIT\n");
    switch (vendor_id)
    {
    case 0: {

        for (ii = 0; ii < total_node; ii++)
        {
            fprintf(fconf,"%d, ", degree_limit_A[ii]);
        }
        break;
    }
    case 1: {
        for (ii = 0; ii < total_node; ii++)
        {
            fprintf(fconf,"%d, ", degree_limit_B[ii]);
        }
        break;
    }
    case 2: {
        for (ii = 0; ii < total_node; ii++)
        {
            fprintf(fconf,"%d, ", degree_limit_C[ii]);
        }
        break;
    }
    }
    fprintf(fconf,"\n");


    fprintf(fconf,"\n\n\n");

    fprintf(fconf,"F_c_w_i\n");

    for (c = 0; c < total_C; c++)
    {
        for (w = 0; w < total_W; w++)
        {
            switch (vendor_id)
            {
            case 0: {

                for (ii = 0; ii < total_node; ii++)
                {
                    fprintf(fconf,"%d, ", F_c_w_i_A[c][w][ii]);
                }
                break;
            }
            case 1: {
                for (ii = 0; ii < total_node; ii++)
                {
                    fprintf(fconf,"%d, ", F_c_w_i_B[c][w][ii]);
                }
                break;
            }
            case 2: {
                for (ii = 0; ii < total_node; ii++)
                {
                    fprintf(fconf,"%d, ", F_c_w_i_C[c][w][ii]);
                }
                break;
            }
            }
            fprintf(fconf,"\n");
        }
        fprintf(fconf,"\n");
    }


    fprintf(fconf,"\n\n\n");

    fprintf(fconf,"F_i\n");
    switch (vendor_id)
    {
    case 0: {

        for (ii = 0; ii < total_node; ii++)
        {
            fprintf(fconf,"%d, ", F_i_A[ii]);
        }
        break;
    }
    case 1: {
        for (ii = 0; ii < total_node; ii++)
        {
            fprintf(fconf,"%d, ",  F_i_B[ii]);
        }
        break;
    }
    case 2: {
        for (ii = 0; ii < total_node; ii++)
        {
            fprintf(fconf,"%d, ",  F_i_C[ii]);
        }
        break;
    }
    }

    fprintf(fconf,"\n\n\n");
    fprintf(fconf,"U_w_m_n\n");

    for (w = 0; w < total_W; w++)
    {
        switch (vendor_id)
        {
        case 0: {

            for (ii = 0; ii < N_A; ii++)
            {
                for (jj = 0; jj < N_A; jj ++)
                {
                    if (Link_A_Final[ii][jj] == 2)
                        fprintf(fconf,"0, ");
                    else
                        fprintf(fconf,"1, ");
                }
                fprintf(fconf,"\n");
            }
            break;
        }
        case 1: {
            for (ii = 0; ii < N_B; ii++)
            {
                for (jj = 0; jj < N_B; jj ++)
                {
                    if (Link_B_Final[ii][jj] == 2)
                        fprintf(fconf,"0, ");
                    else
                        fprintf(fconf,"1, ");
                }
                fprintf(fconf,"\n");
            }
            break;
        }
        case 2: {//NICT 2024 Added a Carrier C according to Subhadeep's request for Globecom2022 extension.

            for (ii = 0; ii < N_C; ii++)
            {
                for (jj = 0; jj < N_C; jj ++)
                {
                    if (Link_C_Final[ii][jj] == 2)
                        fprintf(fconf,"0, ");
                    else
                        fprintf(fconf,"1, ");
                }
                fprintf(fconf,"\n");
            }
            /*
            for (ii = 0; ii < N_A; ii++)
            {
                for (jj = 0; jj < N_A; jj ++)
                {
                    if (Link_A_Final[ii][jj] == 2)
                        fprintf(fconf,"0, ");
                    else
                        fprintf(fconf,"1, ");
                }
                for (jj = 0; jj < N_B; jj ++)
                {
                    if (ii == jj && ii != 0)
                        fprintf(fconf,"0, ");
                    else
                        fprintf(fconf,"1, ");
                }
                fprintf(fconf,"\n");
            }

            for (ii = 0; ii < N_B; ii++)
            {
                for (jj = 0; jj < N_A; jj ++)
                {
                    if (ii == jj && ii != 0)
                        fprintf(fconf,"0, ");
                    else
                        fprintf(fconf,"1, ");
                }
                for (jj = 0; jj < N_B; jj ++)
                {
                    if (Link_B_Final[ii][jj] == 2)
                        fprintf(fconf,"0, ");
                    else
                        fprintf(fconf,"1, ");
                }
                fprintf(fconf,"\n");
            }

            */
            break;
        }
        }
        fprintf(fconf,"\n");
    }
    fprintf(fconf,"\n");

    fprintf(fconf,"\n\n\n");
    fprintf(fconf,"LINK\n");


    switch (vendor_id)
    {
    case 0: {

        for (ii = 0; ii < N_A; ii++)
        {
            for (jj = 0; jj < N_A; jj ++)
            {
                if (Link_A_Final[ii][jj] == 2)
                    fprintf(fconf,"0, ");
                else
                    fprintf(fconf,"1, ");
            }
            fprintf(fconf,"\n");
        }
        break;
    }
    case 1: {
        for (ii = 0; ii < N_B; ii++)
        {
            for (jj = 0; jj < N_B; jj ++)
            {
                if (Link_B_Final[ii][jj] == 2)
                    fprintf(fconf,"0, ");
                else
                    fprintf(fconf,"1, ");
            }
            fprintf(fconf,"\n");
        }
        break;
    }
    case 2: {//NICT 2024 Added a Carrier C according to Subhadeep's request for Globecom2022 extension.

        for (ii = 0; ii < N_C; ii++)
        {
            for (jj = 0; jj < N_C; jj ++)
            {
                if (Link_C_Final[ii][jj] == 2)
                    fprintf(fconf,"0, ");
                else
                    fprintf(fconf,"1, ");
            }
            fprintf(fconf,"\n");
        }
        /*
        for (ii = 0; ii < N_A; ii++)
        {
            for (jj = 0; jj < N_A; jj ++)
            {
                if (Link_A_Final[ii][jj] == 2)
                    fprintf(fconf,"0, ");
                else
                    fprintf(fconf,"1, ");
            }
            for (jj = 0; jj < N_B; jj ++)
            {
                    if (ii == jj && ii != 0)
                        fprintf(fconf,"0, ");
                    else
                        fprintf(fconf,"1, ");
            }
            fprintf(fconf,"\n");
        }

        for (ii = 0; ii < N_B; ii++)
        {
            for (jj = 0; jj < N_A; jj ++)
            {
                    if (ii == jj && ii != 0)
                        fprintf(fconf,"0, ");
                    else
                        fprintf(fconf,"1, ");
            }
            for (jj = 0; jj < N_B; jj ++)
            {
                if (Link_B_Final[ii][jj] == 2)
                    fprintf(fconf,"0, ");
                else
                    fprintf(fconf,"1, ");
            }
            fprintf(fconf,"\n");
        }
        */
        break;
    }
    }

    fprintf(fconf,"\n");
    fclose(fconf);
    } //end of cost
//end of cost
}

void Generate_Conf(int surviving_links_number_A, int surviving_links_number_B,   char * sdirname_A, char * sfilename_A,char * sdirname_B, char * sfilename_B, char * sdirname_C, char * sfilename_C)
{
    int rate;

    int round;

    int link_num;
   // double similarity = 0;

    long u,v;
    double U,V;
    int q;
    
    int max_node, max_link, selected_A, selected_B;
    int surviving_links;
    
    int selected;
    int link_ID;
    int Vendor_ID;
    int T_A, T_B, T_C;


    long int * pElementID;
    long int * pcurrent_var;
    long int * pcurrent_element;
    int s,d,c,w,i,j,m,n,k,a;
    int node_pair_id;
    int ii,jj;
    int R,B;
    long int size;
    long int counter = 0;
    long int counter1, counter2, counter3, counter4;
    long int counter5, counter6, counter7, counter8;
    int case_counter;
    int instance_counter;

    FILE * fconf;
    FILE * flink;

    int damage_degree;
    
    char str[30];
    char line[256];
    char temp[10240];
    char spath[1024];
    char dir_name[1024];
    char filename[1024];
    

    double node_damaged_probability = 0;
    int damage_degree1, damage_degree2;

       
    for (link_ID = 0; link_ID < LINK_NUM_A; link_ID ++)
    {
        LINK_A[link_ID].source = -1;
        LINK_A[link_ID].dest = -1;
    }
    for (link_ID = 0; link_ID < LINK_NUM_B; link_ID ++)
    {
        LINK_B[link_ID].source = -1;
        LINK_B[link_ID].dest = -1;
    }


    for (i = 0; i < N_A; i++)
    {
        Cost_A[i] = 0;
    }

    for (i = 0; i < N_B; i++)
    {
        Cost_B[i] = 0;
    }
    

    link_ID = 0;
    for (i = 0; i < N_A; i++)
    {
        for (j = i + 1; j < N_A; j++)
        {
            if (Link_A[i][j] == 0)
            {
                LINK_A[link_ID].source = i;
                LINK_A[link_ID].dest = j;
                link_ID ++;
            }
        }
    }

    link_ID = 0;
    for (i = 0; i < N_B; i++)
    {
        for (j = i + 1; j < N_B; j++)
        {
            if (Link_B[i][j] == 0)
            {
                LINK_B[link_ID].source = i;
                LINK_B[link_ID].dest = j;
                link_ID ++;
            }
        }
    }

  //  srand((unsigned) time(NULL));
  //for (similarity = 0.4; similarity <= 0.8; similarity = similarity + 0.4)
    {
        //for (link_num = 5; link_num <=20; link_num +=5)
        {
       // for (round = 0; round < 50; round ++)  //improved setup is 50 rounds (simlar to Jianping Wang Chunming Qiao's INFOCOM2011. for the revision of ANTE-D-16-00325 2017 11 09
	        //for (round = 0; round < 10; round ++) //original setup is 10 rounds for the first submission of ANTE-D-16-00325
            {
                for (i = 0; i < N_A; i++)
                {
                    for (j = 0; j < N_A; j++)
                    {
                        Link_A_Final[i][j] = Link_A[i][j];                        
                    }
                }
                
                //Specify the link between outside abstract node 0 and candidate border node 0 1 and 2.
                Link_A_Final[0][1] =2;
                Link_A_Final[1][0] =2;
                
                Link_A_Final[0][2] =2;
                Link_A_Final[2][0] =2;
                
                
                for (i = 0; i < N_B; i++)
                {
                    for (j = 0; j < N_B; j++)
                    {
                        Link_B_Final[i][j] = Link_B[i][j];                        
                    }                    
                }
                
                //Specify the link between outside abstract node 0 and candidate border node 0 1 and 2.
                Link_B_Final[0][1] =2;
                Link_B_Final[1][0] =2;
                
                Link_B_Final[0][2] =2;
                Link_B_Final[2][0] =2;

                max_node = -1;
                max_link = -1;
                selected_A = 0;
                selected_B = 0;
                
                if(N_A > N_B)
                {
                    max_node = N_A;
                    
                }else {
                    max_node = N_B;
                    
                }
                if(LINK_NUM_A > LINK_NUM_B)
                {
                    max_link = LINK_NUM_A;
                    
                }else {
                    max_link = LINK_NUM_B;
                    
                }
                
                
                //Start to decide the surviving_links of Carrier A and B.
                int surviving_links_number;
                double surviving_probability;
                do {
                    for (i = 0; i < max_node; i ++)
                    {
                        u=rand();
                        U= double (u)/RAND_MAX; //[0,1]
                        s = max_node * U;
                        
                        for(j = 0; j < max_node; j++)
                        {
                            //link centric failure
                            u=rand();
                            U= double (u)/RAND_MAX; //[0,1]
                            
                            surviving_probability = (double)surviving_links_number_A/LINK_NUM_A;
                            if( s< N_A && j < N_A && selected_A < surviving_links_number_A )
                            {
                                if (Link_A_Final[s][j] == 0 && Link_B_Final[s][j] == 2 )  //Link_A_Final[s][j] == 0 : not processed yet, B==2: B surviving_links
                                {
                                    v = rand();
                                    V = double (v)/RAND_MAX;
                                    if( V < similarity )  // Carrier A and Carrier B have the similar surviving link.
                                    {
                                        Link_A_Final[s][j] = 2;
                                        Link_A_Final[j][s] = 2;
                                        selected_A ++;
                                        
                                    }
                                    
                                }
                                if (Link_A_Final[s][j] == 0)
                                {
                                    v = rand();
                                    V = double (v)/RAND_MAX;
                                    
                                    if( V < surviving_probability )
                                    {
                                        Link_A_Final[s][j] = 2;
                                        Link_A_Final[j][s] = 2;
                                        selected_A ++;
                                        
                                    }
                                    
                                }
                                
                            }
                            surviving_probability = (double)surviving_links_number_B/LINK_NUM_B;
                            if( s< N_B && j < N_B && selected_B < surviving_links_number_B )
                            {
                                if (Link_B_Final[s][j] == 0 && Link_A_Final[s][j] == 2)
                                {
                                    v = rand();
                                    V = double (v)/RAND_MAX;
                                    
                                    if( V < similarity )
                                    {
                                        Link_B_Final[s][j] = 2;
                                        Link_B_Final[j][s] = 2;
                                        selected_B ++;
                                        
                                    }
                                    
                                }
                                if (Link_B_Final[s][j] == 0)
                                {
                                    v = rand();
                                    V = double (v)/RAND_MAX;
                                    
                                    if( V < surviving_probability )
                                    {
                                        Link_B_Final[s][j] = 2;
                                        Link_B_Final[j][s] = 2;
                                        selected_B ++;
                                        
                                    }
                                    
                                }
                                
                            }
                            
                        }//for j
                        
                    }//for i
                    
                } while (selected_A < surviving_links_number_A || selected_B < surviving_links_number_B); //do
                
                //End of surviving_links decision.
                
                int alpha, beta;
                alpha = 500;
                beta = 520;
                for (i = 0; i < N_A; i++)
                {
                    surviving_links =0;
                    for(j =0; j< N_A; j++)
                    {
                        if(Link_A_Final[i][j] == 2)
                        {
                            surviving_links++;
                            
                        }
                        
                    }
                    
                    printf("A: i %d survi %d \n", i, surviving_links);
                    u=rand();
                    U= double (u)/RAND_MAX; //[0,1]
                    
                    node_damaged_probability = (double) (degree_limit_A[i] - surviving_links) / degree_limit_A[i]; //[0,1]
                    
                    damage_degree1 = int ((node_damaged_probability * alpha) * U);
                    
                    damage_degree2 = int (double ((double) 15 / (double) (surviving_links_number_A))* beta  * U);
                    
                  //  if(damage_degree1 > damage_degree2)
                 //       damage_degree = damage_degree2;
                 //   else 
                 //       damage_degree = damage_degree1;
                    
                    damage_degree = damage_degree1 + damage_degree2;
                    Cost_A[i] = damage_degree;
                    
                    for(j = 0; j < Ne_A; j++)
                    {
                        if(Ve_A[j] == i)
                        {
                            if(damage_degree > 999)
                                Cost_A[i] = 998;
                        }
                    }
                    if(Cost_A[i] >= 999)
                        Cost_A[i] = 999;
                    
                    if(Cost_A[i] < 100 )
                        Cost_A[i] = 0;
                    
                }
                Cost_A[0] = 0;
            
                for (i = 0; i < N_B; i++)
                {
                    surviving_links =0;
                    for(j =0; j< N_B; j++)
                    {
                        if(Link_B_Final[i][j] == 2)
                        {
                            surviving_links++;
                            
                        }
                        
                    }
                    printf("B: i %d survi %d \n", i, surviving_links);
                    u=rand();
                    U= double (u)/RAND_MAX; //[0,1]
                    
                    node_damaged_probability = (double) (degree_limit_B[i] - surviving_links) / degree_limit_B[i]; //[0,1]
                    
                    damage_degree1 = int ((node_damaged_probability * alpha) * U);
                    
                    damage_degree2 = int (double ((double) 15 / (double) (surviving_links_number_B))* beta  * U);
                    
                  //  if(damage_degree1 > damage_degree2)
                  //      damage_degree = damage_degree2;
                  //  else 
                  //      damage_degree = damage_degree1;
                    
                    damage_degree = damage_degree1 + damage_degree2;
                    
                    Cost_B[i] = damage_degree;
                    
                    for(j = 0; j < Ne_B; j++)
                    {
                        if(Ve_B[j] == i)
                        {
                            if(damage_degree > 999)
                                Cost_B[i] = 998;
                            
                        }
                        
                    }
                    
                    if(Cost_B[i] >= 999)
                        Cost_B[i] = 999;
                    
                    if(Cost_B[i] < 100 )
                        Cost_B[i] = 0;
                    
                }
                Cost_B[0] = 0;
                
                for(i = 0; i < N_A + N_B; i ++)
                {
                    if(i < N_A)
                        Cost_C[i] = Cost_A[i];
                    if(i >= N_A)
                        Cost_C[i] = Cost_B[i-N_A];
                    
                }
                
                int cost, repaircost;
                for (cost =0; cost < MAX_COST; cost ++)
                { 
                    
                    //generate repaircost for Carrier A link which should be repaired.
                    for (i = 0; i < N_A; i++)
                    { 
                        for (j = i + 1; j < N_A; j ++)
                        {
                            if (Link_A_Final[i][j] == 0)
                            {
                                if( cost == 11)  //specify a constant large cost e.g., 10
                                { 
                                    test_weight_cost_A[cost][i][j] = 11;
                                    
                                }else{
                                    u=rand();
                                    U= double (u)/RAND_MAX; //[0,1]
                                    repaircost= 1 +  (int) ((double) (cost) * U);
                                    test_weight_cost_A[cost][i][j] = repaircost;
                                }
                            }
                        }
                    }

                    //generate repaircost for Carrier B link which should be repaired.
                    for (i = 0; i < N_B; i++)
                    { 
                        for (j = i + 1; j < N_B; j ++)
                        {
                            if (Link_B_Final[i][j] == 0)
                            {
                                if( cost == 11) //specify a constant large cost e.g., 10
                                { 
                                    test_weight_cost_A[cost][i][j] = 11;
                                    
                                }else{
                                    u=rand();
                                    U= double (u)/RAND_MAX; //[0,1]
                                    repaircost= 1 +  (int) ((double) (cost) * U);
                                    test_weight_cost_B[cost][i][j] = repaircost;
                                }
                            }
                        }
                    }

                    //for Merged networks, the Tmn, say the test weight cost is that in Carrier A and B, respectively. No need to generate herein.
                }
                
                Vendor_ID = 0;
                R = MAX_NODE_PAIR_A;
                T_A = TOTAL_LINK - surviving_links_number_A;
                write_conf(similarity, Vendor_ID, surviving_links_number_A, sdirname_A, sfilename_A, N_A, Ne_A, C_A, W_A, R, T_A);
                
                Vendor_ID = 1;
                R = MAX_NODE_PAIR_B;
                T_B = TOTAL_LINK - surviving_links_number_B;
                write_conf(similarity, Vendor_ID, surviving_links_number_B, sdirname_B, sfilename_B, N_B, Ne_B, C_B, W_B, R, T_B);

            Vendor_ID = 2;
            R = MAX_NODE_PAIR_C;
            T_C = TOTAL_LINK * 2 - surviving_links_number_A - surviving_links_number_B;
            write_conf(similarity, Vendor_ID, surviving_links_number_A + surviving_links_number_B, sdirname_C, sfilename_C, N_C, Ne_C, C_C, W_C, R, T_C);

            }
        }
    }
}


void Generate_Individual_Carrier_Conf(int surviving_links_number_A, int surviving_links_number_B, int surviving_links_number_C, int segment_recovery_max_cost_A, int segment_recovery_max_cost_B, int segment_recovery_max_cost_C, char * sdirname_A, char * sfilename_A,char * sdirname_B, char * sfilename_B, char * sdirname_C, char * sfilename_C)
{
    int rate;

    int round;

    int link_num;
   // double similarity = 0;

    long u,v;
    double U,V;
    int q;
    
    int max_node, max_link, selected_A, selected_B,selected_C;
    int surviving_links;
    
    int selected;
    int link_ID;
    int Vendor_ID;
    int T_A, T_B, T_C;


    long int * pElementID;
    long int * pcurrent_var;
    long int * pcurrent_element;
    int s,d,c,w,i,j,m,n,k,a;
    int node_pair_id;
    int ii,jj;
    int R,B;
    long int size;
    long int counter = 0;
    long int counter1, counter2, counter3, counter4;
    long int counter5, counter6, counter7, counter8;
    int case_counter;
    int instance_counter;

    FILE * fconf;
    FILE * flink;

    int damage_degree;
    
    char str[30];
    char line[256];
    char temp[10240];
    char spath[1024];
    char dir_name[1024];
    char filename[1024];
    

    double node_damaged_probability = 0;
    int damage_degree1, damage_degree2;

       
    for (link_ID = 0; link_ID < LINK_NUM_A; link_ID ++)
    {
        LINK_A[link_ID].source = -1;
        LINK_A[link_ID].dest = -1;
    }
    for (link_ID = 0; link_ID < LINK_NUM_B; link_ID ++)
    {
        LINK_B[link_ID].source = -1;
        LINK_B[link_ID].dest = -1;
    }


    for (link_ID = 0; link_ID < LINK_NUM_C; link_ID ++)
    {
        LINK_C[link_ID].source = -1;
        LINK_C[link_ID].dest = -1;
    }

    for (i = 0; i < N_A; i++)
    {
        Cost_A[i] = 0;
    }

    for (i = 0; i < N_B; i++)
    {
        Cost_B[i] = 0;
    }

    for (i = 0; i < N_C; i++)
    {
        Cost_C[i] = 0;
    }
    

    link_ID = 0;
    for (i = 0; i < N_A; i++)
    {
        for (j = i + 1; j < N_A; j++)
        {
            if (Link_A[i][j] == 0)
            {
                LINK_A[link_ID].source = i;
                LINK_A[link_ID].dest = j;
                link_ID ++;
            }
        }
    }

    link_ID = 0;
    for (i = 0; i < N_B; i++)
    {
        for (j = i + 1; j < N_B; j++)
        {
            if (Link_B[i][j] == 0)
            {
                LINK_B[link_ID].source = i;
                LINK_B[link_ID].dest = j;
                link_ID ++;
            }
        }
    }


    link_ID = 0;
    for (i = 0; i < N_C; i++)
    {
        for (j = i + 1; j < N_C; j++)
        {
            if (Link_C[i][j] == 0)
            {
                LINK_C[link_ID].source = i;
                LINK_C[link_ID].dest = j;
                link_ID ++;
            }
        }
    }

  //  srand((unsigned) time(NULL));
  //for (similarity = 0.4; similarity <= 0.8; similarity = similarity + 0.4)
    {
        //for (link_num = 5; link_num <=20; link_num +=5)
        {
       // for (round = 0; round < 50; round ++)  //improved setup is 50 rounds (simlar to Jianping Wang Chunming Qiao's INFOCOM2011. for the revision of ANTE-D-16-00325 2017 11 09
	        //for (round = 0; round < 10; round ++) //original setup is 10 rounds for the first submission of ANTE-D-16-00325
            {
                for (i = 0; i < N_A; i++)
                {
                    for (j = 0; j < N_A; j++)
                    {
                        Link_A_Final[i][j] = Link_A[i][j];                        
                    }
                }
                
                //Specify the link between outside abstract node 0 and candidate border node 0 1 and 2.
                Link_A_Final[0][1] =2;
                Link_A_Final[1][0] =2;
                
                Link_A_Final[0][2] =2;
                Link_A_Final[2][0] =2;
                

                for (i = 0; i < N_B; i++)
                {
                    for (j = 0; j < N_B; j++)
                    {
                        Link_B_Final[i][j] = Link_B[i][j];
                    }
                }

                //Specify the link between outside abstract node 0 and candidate border node 0 1 and 2.
                Link_B_Final[0][1] =2;
                Link_B_Final[1][0] =2;

                Link_B_Final[0][2] =2;
                Link_B_Final[2][0] =2;

                
                for (i = 0; i < N_C; i++)
                {
                    for (j = 0; j < N_C; j++)
                    {
                        Link_C_Final[i][j] = Link_C[i][j];
                    }                    
                }
                
                //Specify the link between outside abstract node 0 and candidate border node 0 1 and 2.
                Link_C_Final[0][1] =2;
                Link_C_Final[1][0] =2;
                
                Link_C_Final[0][2] =2;
                Link_C_Final[2][0] =2;

                max_node = -1;
                max_link = -1;
                selected_A = 0;
                selected_B = 0;
                selected_C = 0;
                
                if(N_A >= N_B && N_A >= N_C)
                {
                    max_node = N_A;
                }else if (N_B >= N_A && N_B >= N_C )
                {
                    max_node = N_B;
                }else if (N_C >= N_A && N_C >= N_B)
                {
                    max_node = N_C;
                }


                if(LINK_NUM_A >= LINK_NUM_B && LINK_NUM_A >= LINK_NUM_C)
                {
                    max_link = LINK_NUM_A;
                }else if (LINK_NUM_B >= LINK_NUM_A && LINK_NUM_B >= LINK_NUM_C)
                {
                    max_link = LINK_NUM_B;
                }else if (LINK_NUM_C >= LINK_NUM_A && LINK_NUM_C >= LINK_NUM_B)
                {
                    max_link = LINK_NUM_C;
                }
                
                
                //Start to decide the surviving_links of Carrier A and B.
                int surviving_links_number;
                double surviving_probability;
                do {
                    for (i = 0; i < max_node; i ++)
                    {
                        u=rand();
                        U= double (u)/RAND_MAX; //[0,1]
                        s = max_node * U;
                        
                        for(j = 0; j < max_node; j++)
                        {
                            //link centric failure
                            u=rand();
                            U= double (u)/RAND_MAX; //[0,1]
                            
                            surviving_probability = (double)surviving_links_number_A/LINK_NUM_A;
                            if( s< N_A && j < N_A && selected_A < surviving_links_number_A )
                            {
                                if (
                                    (Link_A_Final[s][j] == 0 && Link_B_Final[s][j] == 2 )
                                    ||
                                    (Link_A_Final[s][j] == 0 && Link_C_Final[s][j] == 2 )
                                )  //Link_A_Final[s][j] == 0 : not processed yet, B==2: B surviving_links
                                {
                                    v = rand();
                                    V = double (v)/RAND_MAX;
                                    if( V < similarity )  // Carrier A and Carrier B have the similar surviving link.
                                    {
                                        Link_A_Final[s][j] = 2;
                                        Link_A_Final[j][s] = 2;
                                        selected_A ++;
                                        
                                    }
                                    
                                }
                                if (Link_A_Final[s][j] == 0)
                                {
                                    v = rand();
                                    V = double (v)/RAND_MAX;
                                    
                                    if( V < surviving_probability )
                                    {
                                        Link_A_Final[s][j] = 2;
                                        Link_A_Final[j][s] = 2;
                                        selected_A ++;
                                        
                                    }
                                    
                                }
                                
                            }
                            surviving_probability = (double)surviving_links_number_B/LINK_NUM_B;
                            if( s< N_B && j < N_B && selected_B < surviving_links_number_B )
                            {
                                if (
                                    (Link_B_Final[s][j] == 0 && Link_A_Final[s][j] == 2)
                                    ||
                                    (Link_B_Final[s][j] == 0 && Link_C_Final[s][j] == 2)
                                )
                                {
                                    v = rand();
                                    V = double (v)/RAND_MAX;
                                    
                                    if( V < similarity )
                                    {
                                        Link_B_Final[s][j] = 2;
                                        Link_B_Final[j][s] = 2;
                                        selected_B ++;
                                        
                                    }
                                    
                                }
                                if (Link_B_Final[s][j] == 0)
                                {
                                    v = rand();
                                    V = double (v)/RAND_MAX;
                                    
                                    if( V < surviving_probability )
                                    {
                                        Link_B_Final[s][j] = 2;
                                        Link_B_Final[j][s] = 2;
                                        selected_B ++;
                                        
                                    }
                                    
                                }
                                
                            }

                            surviving_probability = (double)surviving_links_number_C/LINK_NUM_C;
                            if( s< N_C && j < N_C && selected_C < surviving_links_number_C )
                            {
                                if (
                                    (Link_C_Final[s][j] == 0 && Link_A_Final[s][j] == 2)
                                    ||
                                    (Link_C_Final[s][j] == 0 && Link_B_Final[s][j] == 2)
                                )
                                {
                                    v = rand();
                                    V = double (v)/RAND_MAX;

                                    if( V < similarity )
                                    {
                                        Link_C_Final[s][j] = 2;
                                        Link_C_Final[j][s] = 2;
                                        selected_C ++;

                                    }

                                }
                                if (Link_C_Final[s][j] == 0)
                                {
                                    v = rand();
                                    V = double (v)/RAND_MAX;

                                    if( V < surviving_probability )
                                    {
                                        Link_C_Final[s][j] = 2;
                                        Link_C_Final[j][s] = 2;
                                        selected_C ++;

                                    }

                                }

                            }
                            
                        }//for j
                        
                    }//for i
                    
                } while (selected_A < surviving_links_number_A || selected_B < surviving_links_number_B || selected_C < surviving_links_number_C); //do
                
                //End of surviving_links decision.
                
                int alpha, beta;
                alpha = 500;
                beta = 520;
                for (i = 0; i < N_A; i++)
                {
                    surviving_links =0;
                    for(j =0; j< N_A; j++)
                    {
                        if(Link_A_Final[i][j] == 2)
                        {
                            surviving_links++;
                            
                        }
                        
                    }
                    
                    printf("A: i %d survi %d \n", i, surviving_links);
                    u=rand();
                    U= double (u)/RAND_MAX; //[0,1]
                    
                    node_damaged_probability = (double) (degree_limit_A[i] - surviving_links) / degree_limit_A[i]; //[0,1]
                    
                    damage_degree1 = int ((node_damaged_probability * alpha) * U);
                    
                    damage_degree2 = int (double ((double) 15 / (double) (surviving_links_number_A))* beta  * U);
                    
                  //  if(damage_degree1 > damage_degree2)
                 //       damage_degree = damage_degree2;
                 //   else 
                 //       damage_degree = damage_degree1;
                    
                    damage_degree = damage_degree1 + damage_degree2;
                    Cost_A[i] = damage_degree;
                    
                    for(j = 0; j < Ne_A; j++)
                    {
                        if(Ve_A[j] == i)
                        {
                            if(damage_degree > 999)
                                Cost_A[i] = 998;
                        }
                    }
                    if(Cost_A[i] >= 999)
                        Cost_A[i] = 999;
                    
                    if(Cost_A[i] < 100 )
                        Cost_A[i] = 0;
                    
                }
                Cost_A[0] = 0;
            
                for (i = 0; i < N_B; i++)
                {
                    surviving_links =0;
                    for(j =0; j< N_B; j++)
                    {
                        if(Link_B_Final[i][j] == 2)
                        {
                            surviving_links++;
                            
                        }
                        
                    }
                    printf("B: i %d survi %d \n", i, surviving_links);
                    u=rand();
                    U= double (u)/RAND_MAX; //[0,1]
                    
                    node_damaged_probability = (double) (degree_limit_B[i] - surviving_links) / degree_limit_B[i]; //[0,1]
                    
                    damage_degree1 = int ((node_damaged_probability * alpha) * U);
                    
                    damage_degree2 = int (double ((double) 15 / (double) (surviving_links_number_B))* beta  * U);
                    
                  //  if(damage_degree1 > damage_degree2)
                  //      damage_degree = damage_degree2;
                  //  else 
                  //      damage_degree = damage_degree1;
                    
                    damage_degree = damage_degree1 + damage_degree2;
                    
                    Cost_B[i] = damage_degree;
                    
                    for(j = 0; j < Ne_B; j++)
                    {
                        if(Ve_B[j] == i)
                        {
                            if(damage_degree > 999)
                                Cost_B[i] = 998;
                            
                        }
                        
                    }
                    
                    if(Cost_B[i] >= 999)
                        Cost_B[i] = 999;
                    
                    if(Cost_B[i] < 100 )
                        Cost_B[i] = 0;
                    
                }
                Cost_B[0] = 0;
                

                for (i = 0; i < N_C; i++)
                {
                    surviving_links =0;
                    for(j =0; j< N_C; j++)
                    {
                        if(Link_C_Final[i][j] == 2)
                        {
                            surviving_links++;

                        }

                    }
                    printf("C: i %d survi %d \n", i, surviving_links);
                    u=rand();
                    U= double (u)/RAND_MAX; //[0,1]

                    node_damaged_probability = (double) (degree_limit_C[i] - surviving_links) / degree_limit_C[i]; //[0,1]

                    damage_degree1 = int ((node_damaged_probability * alpha) * U);

                    damage_degree2 = int (double ((double) 15 / (double) (surviving_links_number_C))* beta  * U);

                  //  if(damage_degree1 > damage_degree2)
                  //      damage_degree = damage_degree2;
                  //  else
                  //      damage_degree = damage_degree1;

                    damage_degree = damage_degree1 + damage_degree2;

                    Cost_C[i] = damage_degree;

                    for(j = 0; j < Ne_C; j++)
                    {
                        if(Ve_C[j] == i)
                        {
                            if(damage_degree > 999)
                                Cost_C[i] = 998;

                        }

                    }

                    if(Cost_C[i] >= 999)
                        Cost_C[i] = 999;

                    if(Cost_C[i] < 100 )
                        Cost_C[i] = 0;

                }
                Cost_C[0] = 0;

                /*
                for(i = 0; i < N_A + N_B; i ++)
                {
                    if(i < N_A)
                        Cost_C[i] = Cost_A[i];
                    if(i >= N_A)
                        Cost_C[i] = Cost_B[i-N_A];
                    
                }
                */
                
                int cost, repaircost;
                //for (cost =0; cost < MAX_COST; cost ++) //in the new version for ESEN+DCI, the cost of carrier A/B can be different and be specied individually via function parameters segment_recovery_max_cost_A and segment_recovery_max_cost_B, respectively.
                
                { 
                    
                    //generate repaircost for Carrier A link which should be repaired.
                    for (i = 0; i < N_A; i++)
                    { 
                        for (j = i + 1; j < N_A; j ++)
                        {
                            if (Link_A_Final[i][j] == 0)
                            {
                                if( segment_recovery_max_cost_A == 11)  //specify a constant large cost e.g., 10
                                { 
                                    //test_weight_cost_A[cost][i][j] = 11;
                                    test_weight_cost_A[segment_recovery_max_cost_A][i][j] = 11;
                                    
                                }else{
                                    u=rand();
                                    U= double (u)/RAND_MAX; //[0,1]
                                    //repaircost= 1 +  (int) ((double) (cost) * U);
                                    //test_weight_cost_A[cost][i][j] = repaircost;
                                    repaircost= 1 +  (int) ((double) (segment_recovery_max_cost_A) * U);
                                    test_weight_cost_A[segment_recovery_max_cost_A][i][j] = repaircost;

                                    
                                }
                            }
                        }
                    }

                    //generate repaircost for Carrier B link which should be repaired.
                    for (i = 0; i < N_B; i++)
                    { 
                        for (j = i + 1; j < N_B; j ++)
                        {
                            if (Link_B_Final[i][j] == 0)
                            {
                                /*
                                if( cost == 11) //specify a constant large cost e.g., 10
                                { 
                                    test_weight_cost_A[cost][i][j] = 11;
                                    
                                }else{
                                    u=rand();
                                    U= double (u)/RAND_MAX; //[0,1]
                                    repaircost= 1 +  (int) ((double) (cost) * U);
                                    test_weight_cost_B[cost][i][j] = repaircost;
                                }
                                */
                                if( segment_recovery_max_cost_B == 11) //specify a constant large cost e.g., 10
                                { 
                                    test_weight_cost_B[segment_recovery_max_cost_B][i][j] = 11;
                                    
                                }else{
                                    u=rand();
                                    U= double (u)/RAND_MAX; //[0,1]
                                    repaircost= 1 +  (int) ((double) (segment_recovery_max_cost_B) * U);
                                    test_weight_cost_B[segment_recovery_max_cost_B][i][j] = repaircost;
                                }
                            }
                        }
                    }

                    //generate repaircost for Carrier C link which should be repaired.
                    for (i = 0; i < N_C; i++)
                    {
                        for (j = i + 1; j < N_C; j ++)
                        {
                            if (Link_C_Final[i][j] == 0)
                            {
                                /*
                                if( cost == 11) //specify a constant large cost e.g., 10
                                {
                                    test_weight_cost_A[cost][i][j] = 11;

                                }else{
                                    u=rand();
                                    U= double (u)/RAND_MAX; //[0,1]
                                    repaircost= 1 +  (int) ((double) (cost) * U);
                                    test_weight_cost_B[cost][i][j] = repaircost;
                                }
                                */
                                if( segment_recovery_max_cost_C == 11) //specify a constant large cost e.g., 10
                                {
                                    test_weight_cost_C[segment_recovery_max_cost_C][i][j] = 11;

                                }else{
                                    u=rand();
                                    U= double (u)/RAND_MAX; //[0,1]
                                    repaircost= 1 +  (int) ((double) (segment_recovery_max_cost_C) * U);
                                    test_weight_cost_C[segment_recovery_max_cost_C][i][j] = repaircost;
                                }
                            }
                        }
                    }

                    //for Merged networks, the Tmn, say the test weight cost is that in Carrier A and B, respectively. No need to generate herein.
                }
                
                Vendor_ID = 0;
                R = 1;//MAX_NODE_PAIR_A; //NICT Globecom 2022 to reduce the CSPT scale, 
                T_A = TOTAL_LINK - surviving_links_number_A;
                write_conf_esen_dci(similarity, Vendor_ID, surviving_links_number_A, segment_recovery_max_cost_A, sdirname_A, sfilename_A, N_A, Ne_A, C_A, W_A, R, T_A);
                
                Vendor_ID = 1;
                R = 1;//MAX_NODE_PAIR_B;//NICT Globecom 2022 to reduce the CSPT scale, 
                T_B = TOTAL_LINK - surviving_links_number_B;
                write_conf_esen_dci(similarity, Vendor_ID, surviving_links_number_B, segment_recovery_max_cost_B, sdirname_B, sfilename_B, N_B, Ne_B, C_B, W_B, R, T_B);

                //NICT 2024 Added a Carrier C according to Subhadeep's request for Globecom2022 Extension.
                Vendor_ID = 2;
                R = 1;//MAX_NODE_PAIR_B;//NICT Globecom 2022 to reduce the CSPT scale,
                T_C = TOTAL_LINK - surviving_links_number_C;
                write_conf_esen_dci(similarity, Vendor_ID, surviving_links_number_C, segment_recovery_max_cost_C, sdirname_C, sfilename_C, N_C, Ne_C, C_C, W_C, R, T_C);

           // Vendor_ID = 2;
           // R = MAX_NODE_PAIR_C;
           // T_C = TOTAL_LINK * 2 - surviving_links_number_A - surviving_links_number_B;
           // write_conf(similarity, Vendor_ID, surviving_links_number_A + surviving_links_number_B, sdirname_C, sfilename_C, N_C, Ne_C, C_C, W_C, R, T_C);

            }
        }
    }
}


void Generate_Price_File(int surviving_links_number_A, int surviving_links_number_B,   char * sdirname_A, char * sfilename_A,char * sdirname_B, char * sfilename_B, char * sdirname_C, char * sfilename_C)
{
    int cost;
    char filename_with_different_repaircost[1024]; //ONDM2019-SI revision 202004

  //for(cost = 1; cost <=7; cost+=2)
  for(cost = 4; cost <=10; cost+=3)
  {

      //Carrier A
    if(cost == 11)
    { // this is for the original constant large repair cost, say, with a value 10.
        sprintf(filename_with_different_repaircost, "%s/cost_11/%s", sdirname_A, sfilename_A); 
    }else{
        // for other cost specifications, just put them into individual directory.
        sprintf(filename_with_different_repaircost, "%s/cost_%02d/%s", sdirname_A, cost, sfilename_A); 
    }
    
    if ((fconf = fopen(filename_with_different_repaircost,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_with_different_repaircost);
        fclose(fconf);
        exit(1);
    }
    
    fprintf(fconf,"Np#=%d Path#=%d CarrierID#=%d\n", Np, 0,0);
    fclose(fconf);
    
    

    //Carrier B
    if(cost == 11)
    { // this is for the original constant large repair cost, say, with a value 10.
        sprintf(filename_with_different_repaircost, "%s/cost_11/%s", sdirname_B, sfilename_B); 
    }else{
        // for other cost specifications, just put them into individual directory.
        sprintf(filename_with_different_repaircost, "%s/cost_%02d/%s", sdirname_B, cost, sfilename_B); 
    }
    
    if ((fconf = fopen(filename_with_different_repaircost,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_with_different_repaircost);
        fclose(fconf);
        exit(1);
    }
    
    fprintf(fconf,"Np#=%d Path#=%d CarrierID#=%d\n", Np, 0,1);
    fclose(fconf);

    
    // Merged 
    if(cost == 11)
    { // this is for the original constant large repair cost, say, with a value 10.
        sprintf(filename_with_different_repaircost, "%s/cost_11/%s", sdirname_C, sfilename_C); 
    }else{
        // for other cost specifications, just put them into individual directory.
        sprintf(filename_with_different_repaircost, "%s/cost_%02d/%s", sdirname_C, cost, sfilename_C); 
    }
    
    if ((fconf = fopen(filename_with_different_repaircost,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_with_different_repaircost);
        fclose(fconf);
        exit(1);
    }
    
    //fprintf(fconf,"Np#=%d Path#=%d CarrierID#=%d\n", Np_C, 0,0);
    //NICT 2024 Added a Carrier C according to Subhadeep's request for Globecom2022 Extension.
    fprintf(fconf,"Np#=%d Path#=%d CarrierID#=%d\n", Np, 0,2);
    fclose(fconf);
  }
    

    
    /*
    
    if ((fconf = fopen(sfilename_B,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", sfilename_B);
        fclose(fconf);
        exit(1);
    }
    
    fprintf(fconf,"Np#=%d Path#=%d CarrierID#=%d\n", Np, 0,0);
    fclose(fconf);
    
    if ((fconf = fopen(sfilename_C,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", sfilename_C);
        fclose(fconf);
        exit(1);
    }
    
    fprintf(fconf,"Np#=%d Path#=%d CarrierID#=%d\n", Np_C, 0,0);
    fclose(fconf);
    */
    
        
    
}
void Generate_Task_File(int surviving_links_number_A, int surviving_links_number_B,   char * sdirname_A, char * sfilename_A,char * sdirname_B, char * sfilename_B, char * sdirname_C, char * sfilename_C)
{
    int cost;
        
    char filename_with_different_repaircost[1024]; //ONDM2019-SI revision 202004
    

//  for(cost = 1; cost <=7; cost+=2)
  for(cost = 4; cost <=10; cost+=3)
  {

      //Carrier A
    if(cost == 11)
    { // this is for the original constant large repair cost, say, with a value 10.
        sprintf(filename_with_different_repaircost, "%s/cost_11/%s", sdirname_A, sfilename_A); 
    }else{
        // for other cost specifications, just put them into individual directory.
        sprintf(filename_with_different_repaircost, "%s/cost_%02d/%s", sdirname_A, cost, sfilename_A); 
    }
    
    if ((fconf = fopen(filename_with_different_repaircost,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_with_different_repaircost);
        fclose(fconf);
        exit(1);
    }
    
    fprintf(fconf,"0");
    fclose(fconf);
    
    

    //Carrier B
    if(cost == 11)
    { // this is for the original constant large repair cost, say, with a value 10.
        sprintf(filename_with_different_repaircost, "%s/cost_11/%s", sdirname_B, sfilename_B); 
    }else{
        // for other cost specifications, just put them into individual directory.
        sprintf(filename_with_different_repaircost, "%s/cost_%02d/%s", sdirname_B, cost, sfilename_B); 
    }
    
    if ((fconf = fopen(filename_with_different_repaircost,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_with_different_repaircost);
        fclose(fconf);
        exit(1);
    }
    
    fprintf(fconf,"0");
    fclose(fconf);

    
    // Merged 
    if(cost == 11)
    { // this is for the original constant large repair cost, say, with a value 10.
        sprintf(filename_with_different_repaircost, "%s/cost_11/%s", sdirname_C, sfilename_C); 
    }else{
        // for other cost specifications, just put them into individual directory.
        sprintf(filename_with_different_repaircost, "%s/cost_%02d/%s", sdirname_C, cost, sfilename_C); 
    }
    
    if ((fconf = fopen(filename_with_different_repaircost,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_with_different_repaircost);
        fclose(fconf);
        exit(1);
    }
    
    fprintf(fconf,"0");
    fclose(fconf);
  }  
    
    
    /*  Version 2 before 202004
    
       if ((fconf = fopen(sfilename_A,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", sfilename_A);
        fclose(fconf);
        exit(1);
    }
    
    fprintf(fconf,"0");
    fclose(fconf);
    
    if ((fconf = fopen(sfilename_B,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", sfilename_B);
        fclose(fconf);
        exit(1);
    }
    
    fprintf(fconf,"0");
    fclose(fconf);
        
    if ((fconf = fopen(sfilename_C,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", sfilename_C);
        fclose(fconf);
        exit(1);
    }
    
    fprintf(fconf,"0");
    fclose(fconf);
    */
        
}

int main(int argc, char **argv)
{
    int rate;

    int round;

    int link_num;
    double similarity = 0;

    long u,v;
    double U,V;
    int q;
    
    int max_node, max_link, selected_A, selected_B;
    int surviving_links;
    
    int selected;
    int link_ID;
    int Vendor_ID;
    int T_A, T_B, T_C;


    long int * pElementID;
    long int * pcurrent_var;
    long int * pcurrent_element;
    int AA,BB,CC, s,d,c,w,i,j,m,n,k,a;
    int node_pair_id;
    int ii,jj;
    int R,B;
    long int size;
    long int counter = 0;
    long int counter1, counter2, counter3, counter4;
    long int counter5, counter6, counter7, counter8;
    int case_counter;
    int instance_counter;
    int cost;
    int phase;
    
    FILE * fconf;
    FILE * flink;

    int damage_degree;
    
    char str[30];
    char line[256];
    char temp[10240];
    char spath[1024];
    char dirname_A[1024];
    char dirname_B[1024];
    char dirname_C[1024];
    
    char filename_A[1024];
    char filename_B[1024];
    char filename_C[1024];    

    double node_damaged_probability = 0;
    int damage_degree1, damage_degree2;
    
    Total_Customer_Carrier =7;

    srand((unsigned) time(NULL));

    
    sprintf(spath, "mkdir ./scenario");
    system(spath);

        
    sprintf(spath, "mkdir ./scenario/before_disaster");
    system(spath);

    sprintf(spath, "mkdir ./scenario/after_disaster");
    system(spath);    
    
    sprintf(spath, "mkdir ./scenario/before_disaster/phase_00");
    system(spath);
     
    sprintf(spath, "mkdir ./scenario/before_disaster/phase_00/carrier_0");
    system(spath);

    sprintf(spath, "mkdir ./scenario/before_disaster/phase_00/carrier_1");
    system(spath);

    sprintf(spath, "mkdir ./scenario/before_disaster/phase_00/carrier_2"); //NICT 2024 according to Subhadeep's request, a carrier C is added.
    system(spath);

    sprintf(spath, "mkdir ./scenario/before_disaster/phase_00/dcp");
    system(spath);
    

    sprintf(spath, "mkdir ./scenario/before_disaster/phase_00/Users");
    system(spath);
    
    sprintf(spath, "mkdir ./scenario/before_disaster/phase_00/dcp_1");
    system(spath);

    sprintf(spath, "mkdir ./scenario/before_disaster/phase_00/dcp_2");
    system(spath);


    sprintf(spath, "mkdir ./scenario/before_disaster/phase_00/dcp_3");//NICT 2024 according to Subhadeep's request, dcp 3 and 4 are added.
    system(spath);

    sprintf(spath, "mkdir ./scenario/before_disaster/phase_00/dcp_4");//NICT 2024 according to Subhadeep's request, dcp 3 and 4 are added.
    system(spath);

    
    sprintf(spath, "mkdir ./scenario/before_disaster/phase_00/pne");
    system(spath);

    
    sprintf(spath, "mkdir ./scenario/before_disaster/phase_01");
    system(spath);
     
    sprintf(spath, "mkdir ./scenario/before_disaster/phase_01/carrier_0");
    system(spath);

    sprintf(spath, "mkdir ./scenario/before_disaster/phase_01/carrier_1");
    system(spath);

    sprintf(spath, "mkdir ./scenario/before_disaster/phase_01/carrier_2");//NICT 2024 according to Subhadeep's request, a carrier C is added.
    system(spath);


    sprintf(spath, "mkdir ./scenario/before_disaster/phase_01/dcp");
    system(spath);
    
    sprintf(spath, "mkdir ./scenario/before_disaster/phase_01/Users");
    system(spath);
    
    sprintf(spath, "mkdir ./scenario/before_disaster/phase_01/dcp_1");
    system(spath);

    sprintf(spath, "mkdir ./scenario/before_disaster/phase_01/dcp_2");
    system(spath);


    sprintf(spath, "mkdir ./scenario/before_disaster/phase_01/dcp_3");//NICT 2024 according to Subhadeep's request, dcp 3 and 4 are added.
    system(spath);

    sprintf(spath, "mkdir ./scenario/before_disaster/phase_01/dcp_4");//NICT 2024 according to Subhadeep's request, dcp 3 and 4 are added.
    system(spath);
    
    sprintf(spath, "mkdir ./scenario/before_disaster/phase_01/pne");
    system(spath);
    
    
    sprintf(dirname_A,  "./scenario/before_disaster/phase_00/carrier_0");
    sprintf(filename_A, "carrier_0");

    sprintf(dirname_B,  "./scenario/before_disaster/phase_00/carrier_1");
    sprintf(filename_B, "carrier_1");

    sprintf(dirname_C,  "./scenario/before_disaster/phase_00/carrier_2");
    sprintf(filename_C, "carrier_2");
    
    cost = 0;
    Generate_Individual_Carrier_Conf(LINK_NUM_A,LINK_NUM_B, LINK_NUM_C, cost, cost, cost, dirname_A, filename_A, dirname_B, filename_B, dirname_C, filename_C);//NICT 2024 according to Subhadeep's request, a carrier C is added.

    
    
    sprintf(filename_A, "./scenario/pne.conf");
    if ((fconf = fopen(filename_A,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_A);
        exit(1);
    }
    
    fprintf(fconf,"Np#=%d Carrier#=3 Carrier_Customer#=7\n", Np);
    fprintf(fconf,"LINK\n");
    for (i = 0; i < Np; i ++)
    {
       for (j = 0; j< Np; j ++) 
       {
          fprintf(fconf,"%d, ", Link_E[i][j]);
       }
       fprintf(fconf,"\n");
    }
    
    
    fclose(fconf);

    //
    sprintf(filename_A, "./scenario/before_disaster/phase_00/carrier_0/carrier_0.input.price");
    if ((fconf = fopen(filename_A,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_A);
        exit(1);
    }
    
    fprintf(fconf,"Np#=%d Path#=0\n", Np);
    fclose(fconf);

    sprintf(filename_A, "./scenario/before_disaster/phase_00/carrier_0/carrier_0.input.request");
    if ((fconf = fopen(filename_A,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_A);
        exit(1);
    }
    
    fprintf(fconf,"Path#=0 Customer_Carrier#=%d\n",Total_Customer_Carrier);
    fclose(fconf);
        
    //
    //carrier B before disaster    
    sprintf(filename_B, "./scenario/before_disaster/phase_00/carrier_1/carrier_1.input.price");
    if ((fconf = fopen(filename_B,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_B);
        exit(1);
    }
    
    fprintf(fconf,"Np#=%d Path#=0\n", Np);
    fclose(fconf);

    sprintf(filename_B, "./scenario/before_disaster/phase_00/carrier_1/carrier_1.input.request");
    if ((fconf = fopen(filename_B,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_B);
        exit(1);
    }
    
    fprintf(fconf,"Path#=0 Customer_Carrier#=%d\n",Total_Customer_Carrier);
    fclose(fconf);
    
    //carrier C before disaster //NICT 2024 according to Subhadeep's request, a carrier C is added.
    sprintf(filename_C, "./scenario/before_disaster/phase_00/carrier_2/carrier_2.input.price");
    if ((fconf = fopen(filename_C,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_C);
        exit(1);
    }

    fprintf(fconf,"Np#=%d Path#=0\n", Np);
    fclose(fconf);

    sprintf(filename_C, "./scenario/before_disaster/phase_00/carrier_2/carrier_2.input.request");
    if ((fconf = fopen(filename_C,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_C);
        exit(1);
    }

    fprintf(fconf,"Path#=0 Customer_Carrier#=%d\n",Total_Customer_Carrier);
    fclose(fconf);

    //ESEN
    sprintf(filename_B, "./scenario/before_disaster/phase_00/pne/esen_nodes.csv");
    sprintf(filename_B, "./scenario/before_disaster/phase_00/pne/pne_nodes.csv");
    if ((fconf = fopen(filename_B,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_B);
        exit(1);
    }
    
    fprintf(fconf,"ID\n");
    for (i = 0; i < Np; i ++)
    {
        fprintf(fconf,"%d\n", i);
    }
    fclose(fconf);
    
    sprintf(filename_B, "./scenario/before_disaster/phase_00/ese/esen_links.csv");
    sprintf(filename_B, "./scenario/before_disaster/phase_00/pne/pne_links.csv");
    
    if ((fconf = fopen(filename_B,"w")) == NULL)
    {
        printf("Cannot create conf file %s.\n", filename_B);
        exit(1);
    }

    
    link_ID = 1;
    fprintf(fconf,"ID,Src_node,Dst_node,Flag\n");
    for (i = 0; i < Np; i ++)
    {
        for (j = i +1; j < Np; j++)
        {
            if (Link_E[i][j] == 0) //NEED CONFIRM 20210916: 0 here means the link is OK and will be outputed, other links ==1 will not be outputed.
            {
                fprintf(fconf,"%d,%d,%d,1\n", link_ID, i, j);
                link_ID ++;
            }
        }
    }
    fclose(fconf);    
    
    
    case_counter = 0;
    
    for (AA = 5; AA < 15; AA+=5)
    {
        for (BB = AA; BB < 15; BB+=5)
        {
            sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d", case_counter, AA, BB);
            system(spath);
            
            for (cost = 4; cost <=10; cost+=3)
            //for (cost = 10; cost <=10; cost+=3)
            {   
                sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d", case_counter, AA, BB, cost);
                system(spath);
                
                for (phase = 2; phase <=16; phase ++)
                {

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d", case_counter, AA, BB, cost, phase);
                    system(spath);
                    
                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/carrier_0", case_counter, AA, BB, cost, phase);
                    system(spath);

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/carrier_1", case_counter, AA, BB, cost, phase);
                    system(spath);

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/carrier_2", case_counter, AA, BB, cost, phase);//NICT 2024 according to Subhadeep's request, a carrier C is added.
                    system(spath);


                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/carrier_0/ILP", case_counter, AA, BB, cost, phase);
                    system(spath);

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/carrier_1/ILP", case_counter, AA, BB, cost, phase);
                    system(spath);

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/carrier_2/ILP", case_counter, AA, BB, cost, phase);
                    system(spath);

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/carrier_0/Heuristic", case_counter, AA, BB, cost, phase);
                    system(spath);

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/carrier_1/Heuristic", case_counter, AA, BB, cost, phase);
                    system(spath);
                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/carrier_2/Heuristic", case_counter, AA, BB, cost, phase);
                    system(spath);


                    ////////
                    //base
                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/dcp", case_counter, AA, BB, cost, phase);
                    system(spath);
                    
                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/Users", case_counter, AA, BB, cost, phase);
                    system(spath);
                    
                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/dcp_1", case_counter, AA, BB, cost, phase);
                    system(spath);
                    

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/dcp_2", case_counter, AA, BB, cost, phase);
                    system(spath);


                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/dcp_3", case_counter, AA, BB, cost, phase);//NICT 2024 according to Subhadeep's request, dcp 3 and 4 are added.
                    system(spath);


                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/dcp_4", case_counter, AA, BB, cost, phase);//NICT 2024 according to Subhadeep's request, dcp 3 and 4 are added.
                    system(spath);


                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/pne", case_counter, AA, BB, cost, phase);
                    system(spath);

                    /////


                    ////////
                    //ILP
                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/dcp/ILP", case_counter, AA, BB, cost, phase);
                    system(spath);

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/Users/ILP", case_counter, AA, BB, cost, phase);
                    system(spath);

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/dcp_1/ILP", case_counter, AA, BB, cost, phase);
                    system(spath);


                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/dcp_2/ILP", case_counter, AA, BB, cost, phase);
                    system(spath);


                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/dcp_3/ILP", case_counter, AA, BB, cost, phase);//NICT 2024 according to Subhadeep's request, dcp 3 and 4 are added.
                    system(spath);


                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/dcp_4/ILP", case_counter, AA, BB, cost, phase);//NICT 2024 according to Subhadeep's request, dcp 3 and 4 are added.
                    system(spath);


                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/pne/ILP", case_counter, AA, BB, cost, phase);
                    system(spath);

                    /////
                    ////////
                    //Heuristic
                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/dcp/Heuristic", case_counter, AA, BB, cost, phase);
                    system(spath);

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/Users/Heuristic", case_counter, AA, BB, cost, phase);
                    system(spath);

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/dcp_1/Heuristic", case_counter, AA, BB, cost, phase);
                    system(spath);


                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/dcp_2/Heuristic", case_counter, AA, BB, cost, phase);
                    system(spath);


                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/dcp_3/Heuristic", case_counter, AA, BB, cost, phase);//NICT 2024 according to Subhadeep's request, dcp 3 and 4 are added.
                    system(spath);


                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/dcp_4/Heuristic", case_counter, AA, BB, cost, phase);//NICT 2024 according to Subhadeep's request, dcp 3 and 4 are added.
                    system(spath);


                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/pne/Heuristic", case_counter, AA, BB, cost, phase);
                    system(spath);

                    /////
                    
                    
                    
                    /*
                    if(phase == 4)
                    {

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/carrier_0/ILP", case_counter, AA, BB, cost, phase);
                    system(spath);

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/carrier_1/ILP", case_counter, AA, BB, cost, phase);
                    system(spath);

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/carrier_2/ILP", case_counter, AA, BB, cost, phase);
                    system(spath);

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/carrier_0/Heuristic", case_counter, AA, BB, cost, phase);
                    system(spath);

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/carrier_1/Heuristic", case_counter, AA, BB, cost, phase);
                    system(spath);
                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/carrier_2/Heuristic", case_counter, AA, BB, cost, phase);
                    system(spath);
                    }


                    if(phase == 5)
                    {

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/dcp/ILP", case_counter, AA, BB, cost, phase);
                    system(spath);

                    sprintf(spath, "mkdir ./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_%02d/dcp/Heuristic", case_counter, AA, BB, cost, phase);
                    system(spath);
                    }
                    */


                }
            }
            case_counter++;
        }
    }

    /*
    //Phase-0: Before disaster, the carriers A and B, ESEN topology information for DCI topology evluation use.
    //The following files are for path price calculation
    //in carrier 0: carrier_0.nodes
    //in carrier 0: carrier_0.links
    //in carrier 0: carrier_0.conf //no test segment.
    //in carrier 0: carrier_0.requests empty
    //in carrier 0: carrier_0.price empty
    
    //in carrier 1: carrier_1.nodes
    //in carrier 1: carrier_1.links
    //in carrier 1: carrier_1.conf //no test segment.
    //in carrier 1: carrier_1.requests empty
    //in carrier 1: carrier_1.price empty

    //in esen: esen_nodes.csv
    //in esen: esen_links.csv
    //
    
    
    case_counter = 0;   
    for (AA = 5; AA < 15; AA+=5)
    {
        for (BB = AA; BB < 15; BB+=5)
        {
            for (cost = 4; cost <=10; cost+=3)
            {    
                //Carrier A, B
                //Generate .conf .nodes, .links file
                sprintf(dirname_A,  "./case%d_%d_%d/cost_%02d/carrier_0/phase_00", case_counter, AA, BB, cost);
                sprintf(filename_A, "carrier_0");

                sprintf(dirname_B,  "./case%d_%d_%d/cost_%02d/carrier_1/phase_00", case_counter, AA, BB, cost);
                sprintf(filename_B, "carrier_1");
                
                Generate_Individual_Carrier_Conf(LINK_NUM_A,LINK_NUM_B, cost, cost, dirname_A, filename_A, dirname_B, filename_B);

                
                
                //
                sprintf(filename_B, "./case%d_%d_%d/cost_%02d/carrier_0/phase_00/carrier_0.price", case_counter, AA, BB, cost);
                
                if ((fconf = fopen(filename_B,"w")) == NULL)
                {
                    printf("Cannot create conf file %s.\n", filename_B);
                    exit(1);
                }
                fprintf(fconf,"Np#=%d Path#=0\n", Np);
                fclose(fconf);
                //
                sprintf(filename_B, "./case%d_%d_%d/cost_%02d/carrier_0/phase_00/carrier_0.request", case_counter, AA, BB, cost);
                
                if ((fconf = fopen(filename_B,"w")) == NULL)
                {
                    printf("Cannot create conf file %s.\n", filename_B);
                    exit(1);
                }
                fprintf(fconf,"Path#=0 Customer_Carrier#=%d\n",Total_Customer_Carrier);
                fclose(fconf);
  
              //
                sprintf(filename_B, "./case%d_%d_%d/cost_%02d/carrier_1/phase_00/carrier_1.price", case_counter, AA, BB, cost);
                
                if ((fconf = fopen(filename_B,"w")) == NULL)
                {
                    printf("Cannot create conf file %s.\n", filename_B);
                    exit(1);
                }
                fprintf(fconf,"Np#=%d Path#=0\n", Np);
                fclose(fconf);
                //
                sprintf(filename_B, "./case%d_%d_%d/cost_%02d/carrier_1/phase_00/carrier_1.request", case_counter, AA, BB, cost);
                
                if ((fconf = fopen(filename_B,"w")) == NULL)
                {
                    printf("Cannot create conf file %s.\n", filename_B);
                    exit(1);
                }
                fprintf(fconf,"Path#=0 Customer_Carrier#=%d\n",Total_Customer_Carrier);
                fclose(fconf);
                  
                
                
                //ESEN
                sprintf(filename_B, "./case%d_%d_%d/cost_%02d/ese/phase_00/esen_nodes.csv", case_counter, AA, BB, cost);
                
                if ((fconf = fopen(filename_B,"w")) == NULL)
                {
                    printf("Cannot create conf file %s.\n", filename_B);

                    exit(1);
                }
                fprintf(fconf,"ID\n");
                for (i = 0; i < Np; i ++)
                {
                    fprintf(fconf,"%d\n", i);
                }
                fclose(fconf);
                
                               
                sprintf(filename_B, "./case%d_%d_%d/cost_%02d/ese/phase_00/esen_links.csv", case_counter, AA, BB, cost);
                
                if ((fconf = fopen(filename_B,"w")) == NULL)
                {
                    printf("Cannot create conf file %s.\n", filename_B);
                    exit(1);
                }
                
                link_ID = 1;
                fprintf(fconf,"ID,Src_node,Dst_node,Flag\n");
                for (i = 0; i < Np; i ++)
                {
                    for (j = i +1; j < Np; j++)
                    {
                        if (Link_E[i][j] == 0) //NEED CONFIRM 20210916: 0 here means the link is OK and will be outputed, other links ==1 will not be outputed.
                        {
                            fprintf(fconf,"%d,%d,%d,1\n", link_ID, i, j);
                            link_ID ++;
                        }
                    }
                }
                fclose(fconf);
                
                
                
                 
            }
            case_counter++;
        }
    }
    */
    
    //Phase-2: Disaster occurs, carriers A and B, ESEN topology information for DCI topology evluation use.
    //The following files are for path price calculation
    //in carrier 0: carrier_0_xx.nodes
    //in carrier 0: carrier_0_xx.links
    //in carrier 0: carrier_0_xx.conf //no test segment.
    //in carrier 0: carrier_0_xx.requests empty
    //in carrier 0: carrier_0_xx.price empty
    
    //in carrier 1: carrier_1_xx.nodes
    //in carrier 1: carrier_1_xx.links
    //in carrier 1: carrier_1_xx.conf //no test segment.
    //in carrier 1: carrier_1_xx.requests empty
    //in carrier 1: carrier_1_xx.price empty
    

    //in esen: esen_nodes.csv
    //in esen: esen_links.csv
    //
    
    instance_counter = 0;
    case_counter = 0;   
    for (AA = 5; AA < 15; AA+=5)
    {
        for (BB = AA; BB < 15; BB+=5)
        {
            if( AA == 5 && BB == 5)
            {
                CC = 5;
            }
            if( AA == 5 && BB == 10)
            {
                CC = 8;
            }
            if( AA == 10 && BB == 10)
            {
                CC = 10;
            }


            for (cost = 4; cost <=10; cost+=3)
            //for (cost = 10; cost <=10; cost+=3)
            {    
             
                for(i = 0; i < MAX_INSTANCE; i++)
                {
                    //Carrier A, B, C
                    //Generate .conf .nodes, .links file
                    sprintf(dirname_A,  "./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_02/carrier_0", case_counter, AA, BB, cost);
                    sprintf(filename_A, "carrier_0_%02d", instance_counter);
                    
                    sprintf(dirname_B,  "./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_02/carrier_1", case_counter, AA, BB, cost);
                    sprintf(filename_B, "carrier_1_%02d", instance_counter);
                    
                    //NICT 2024 according to Subhadeep's request, a carrier C is added.
                    sprintf(dirname_C,  "./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_02/carrier_2", case_counter, AA, BB, cost);
                    sprintf(filename_C, "carrier_2_%02d", instance_counter);


                    Generate_Individual_Carrier_Conf(AA,BB,CC, cost, cost, cost, dirname_A, filename_A, dirname_B, filename_B, dirname_C, filename_C);
                    
                    //Carrier A
                    sprintf(filename_A, "./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_02/carrier_0/carrier_0_%02d.input.price", case_counter, AA, BB, cost, instance_counter);
                    
                    if ((fconf = fopen(filename_A,"w")) == NULL)
                    {
                        printf("Cannot create conf file %s.\n", filename_A);
                        exit(1);
                    }
                    fprintf(fconf,"Np#=%d Path#=0\n", Np);
                    fclose(fconf);
                    //
                    sprintf(filename_A, "./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_02/carrier_0/carrier_0_%02d.input.request", case_counter, AA, BB, cost, instance_counter);

                    
                    if ((fconf = fopen(filename_A,"w")) == NULL)
                    {
                        printf("Cannot create conf file %s.\n", filename_A);
                        exit(1);
                    }
                    fprintf(fconf,"Path#=0 Customer_Carrier#=%d\n",Total_Customer_Carrier);
                    fclose(fconf);

                    //Carrier B
                    sprintf(filename_B, "./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_02/carrier_1/carrier_1_%02d.input.price", case_counter, AA, BB, cost, instance_counter);
                    
                    if ((fconf = fopen(filename_B,"w")) == NULL)
                    {
                        printf("Cannot create conf file %s.\n", filename_B);
                        exit(1);
                    }
                    fprintf(fconf,"Np#=%d Path#=0\n", Np);
                    fclose(fconf);
                    //
                    sprintf(filename_B, "./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_02/carrier_1/carrier_1_%02d.input.request", case_counter, AA, BB, cost, instance_counter);

                    
                    if ((fconf = fopen(filename_B,"w")) == NULL)
                    {
                        printf("Cannot create conf file %s.\n", filename_B);
                        exit(1);
                    }
                    fprintf(fconf,"Path#=0 Customer_Carrier#=%d\n",Total_Customer_Carrier);
                    fclose(fconf);

                    //Carrier C
                    sprintf(filename_C, "./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_02/carrier_2/carrier_2_%02d.input.price", case_counter, AA, BB, cost, instance_counter);

                    if ((fconf = fopen(filename_C,"w")) == NULL)
                    {
                        printf("Cannot create conf file %s.\n", filename_C);
                        exit(1);
                    }
                    fprintf(fconf,"Np#=%d Path#=0\n", Np);
                    fclose(fconf);
                    //
                    sprintf(filename_C, "./scenario/after_disaster/case%d_%d_%d/cost_%02d/phase_02/carrier_2/carrier_2_%02d.input.request", case_counter, AA, BB, cost, instance_counter);


                    if ((fconf = fopen(filename_C,"w")) == NULL)
                    {
                        printf("Cannot create conf file %s.\n", filename_C);
                        exit(1);
                    }
                    fprintf(fconf,"Path#=0 Customer_Carrier#=%d\n",Total_Customer_Carrier);
                    fclose(fconf);

                    instance_counter ++;
                }
            }
            case_counter++;
        }
    }

      



    return 0;
}





