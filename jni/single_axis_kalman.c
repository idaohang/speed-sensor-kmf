
#include "single_axis_kalman.h"
#include <stdio.h>

// read this https://www.cl.cam.ac.uk/~rmf25/papers/Understanding%20the%20Basis%20of%20the%20Kalman%20Filter.pdf for understanding

void init_kalman_filte(kalman_filter *kf) {
	kf->state.rows = 2;
 	kf->state.cols = 1;
//	kf->state.mat; //acceleration and speed
	
	kf->F_t.rows=2;
	kf->F_t.cols=2;  
	kf->F_t.mat[0][0] = 0.8;
	kf->F_t.mat[0][1] = 0;
	kf->F_t.mat[1][0] = 0.005;
	kf->F_t.mat[1][1] = 1.0; //add the acceleration, calculate the speed


	kf->B_t.rows =4;
	kf->B_t.cols =  2;
	// {0}}; // transform speed/force of the propellers to axis speed and accleration, need mass of drone and drone geometry

	kf->u_t.rows = 4;
	kf->u_t.cols = 1;
	// {0}}; //speed/force on every propeller {front left, front right, rear left, rear right}
	
	kf->P_t_t1.cols = 2;
	kf->P_t_t1.rows = 2;
	kf->P_t_t1.mat[0][0] = 1;
	kf->P_t_t1.mat[0][1] = 0;
	kf->P_t_t1.mat[1][0] = 0;
	kf->P_t_t1.mat[1][1] = 1; //we have two sensors, the accelerometer and the prediction from the motor forces/speeds

	kf->P_t1_t1.cols = 2;
	kf->P_t1_t1.rows = 2;
	kf->P_t1_t1.mat[0][0] = 1;
	kf->P_t1_t1.mat[0][1] = 0;
	kf->P_t1_t1.mat[1][0] = 0;
	kf->P_t1_t1.mat[1][1] = 1;

	kf->P_t_t.cols = 2;
	kf->P_t_t.rows = 2;
	kf->P_t_t.mat[0][0] = 1;
	kf->P_t_t.mat[0][1] = 0;
	kf->P_t_t.mat[1][0] = 0;
	kf->P_t_t.mat[1][1] = 1;
	
	kf->Kt.rows = 2;
	kf->Kt.cols = 2;
	
	//kf->Ht = {2, 2, {{1, 0}, {0, 0}}}; //to be finished, second is for delta speed in the time interval:
	kf->Ht.cols = 2;
	kf->Ht.rows = 2;
	kf->Ht.mat[0][0] = 1;
	kf->Ht.mat[0][1] = 0;
	kf->Ht.mat[1][0] = 0;
	kf->Ht.mat[1][1] = 0;

	
	//kf->Qt = {2, 2, {{0.005, 0}, {0, 0.005}} };
	kf->Qt.cols = 2;
	kf->Qt.rows = 2;
	kf->Qt.mat[0][0] = 0.005;
	kf->Qt.mat[0][1] = 0;
	kf->Qt.mat[1][0] = 0;
	kf->Qt.mat[1][1] = 0.0004;

	//kf->Rt = { 2, 2, .mat = {{0.7, 0.0}, {0.0, 0.7}} };
	kf->Rt.cols = 2;
	kf->Rt.rows = 2;
	kf->Rt.mat[0][0] = 4.7;
	kf->Rt.mat[0][1] = 0;
	kf->Rt.mat[1][0] = 0;
	kf->Rt.mat[1][1] = 0.004;
	
	kf->zt.rows = 2;
	kf->zt.cols = 2;

	
	kf->prev_ts = 0;
	kf->prev_acc = 0;

	//matrix (*get_filtered_values)( kalman_filter *kalman_instance ,const double input_acc, const int timestamp)
}

int zero_count = 400;
matrix get_filtered_values(kalman_filter *this, const double input_acc, const int timestamp) {
	
	if (zero_count == 0) {
		this->state.mat[0][0] = 0;
		this->state.mat[1][0] = 0;

	} else if (zero_count > 0) {
		zero_count--;
	}

	if (this->prev_ts == 0) { 
		init_kalman_filte(this);
		this->prev_ts = timestamp;
		this->prev_acc = input_acc;
		return this->state;
	}

	double dt = (timestamp-this->prev_ts)/1000000000.0;
	this->prev_ts = timestamp;
	this->F_t.mat[1][0] = dt;
	this->F_t.mat[1][1] = 1;
	this->prev_acc = this->state.mat[0][0];

	this->state = add(multiply(this->F_t, this->state), multiply(this->B_t, this->u_t));

 	this->P_t_t1 = add(multiply(multiply(this->F_t, this->P_t1_t1),T(this->F_t)), this->Qt);

	this->Kt = divide( multiply(this->P_t_t1, T(this->Ht)),  add(multiply(multiply(this->Ht,this->P_t_t1),T(this->Ht)),this->Rt) );

	this->zt.mat[0][0] = input_acc;
	this->zt.mat[1][0] = 0;

	this->state = add(this->state , multiply(this->Kt, subtract(this->zt, multiply(this->Ht,this->state))) ); 

	this->P_t_t = subtract(this->P_t_t1, multiply(multiply(this->Kt, this->Ht), this->P_t_t1));

	this->P_t1_t1 = this->P_t_t;

/*	printf("...........................\n");
	printf("--------\nFt: \n");
	print_matrix(this->F_t);

	printf("--------\nstate: \n");
	print_matrix(this->state);
	printf("--------\nK: \n");
	print_matrix(this->Kt);
	printf("--------\nPtt1: \n");
	print_matrix(this->P_t_t1);
	printf("--------\nPtt: \n");
	print_matrix(this->P_t_t); 

*/
	//printf("%2.10f, %2.10f, %2.10f \n", input_acc, this->state.mat[0][0]*2.5, this->state.mat[1][0]);

	return this->state;
}

int samples_count = 1;
int samples_total = 100;
double sum = 0;

void covariate(double a, double b, double EA, double EB) {
	sum+=(EA-a)*(EA-a)/samples_total;
	samples_count++;
	if (samples_count == samples_total) {
		printf("Value: %2.10f\n",sum);
	}
}
