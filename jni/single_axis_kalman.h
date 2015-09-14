#include "matrix.h"

struct kalman_filter {
	matrix state ; 
	
	matrix  F_t ;
	
	matrix B_t ;
	matrix u_t ;
	
	matrix P_t_t1;
	matrix P_t1_t1; 
	matrix P_t_t ; 
	
	matrix Kt;
	
	matrix Ht;
	
	matrix Qt ;
	
	matrix Rt ;
	
	matrix zt ;
	
	int prev_ts;
	double prev_acc;
};

typedef struct kalman_filter kalman_filter;

matrix get_filtered_values(kalman_filter *this, const double input_acc, const int timestamp);
void init_kalman_filte(kalman_filter *kf);
