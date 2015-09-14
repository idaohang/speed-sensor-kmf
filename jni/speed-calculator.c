#include <android/sensor.h>

#include "speed-calculator.h"
#include "single_axis_kalman.h"
//#include "matrix.h"

matrix  x_state = {0}; //acceleration x
matrix  y_state = {0}; //acceleration x

kalman_filter kx, ky;

void handle_acceleration_event(const ASensorEvent event) {

	x_state = get_filtered_values(&kx, event.acceleration.x, event.timestamp);
	y_state = get_filtered_values(&ky, event.acceleration.y, event.timestamp);


	printf("%2.10f, %2.10f, ", x_state.mat[0][0], x_state.mat[1][0]);
	printf("%2.10f, %2.10f\n", y_state.mat[0][0], y_state.mat[1][0] );

	//printf("%2.10f, %2.10f\n", event.acceleration.x, event.acceleration.y);

	//printf("%2.10f\n", x_state[0] );//event.acceleration.x);
	//printf("%2.10f %2.10f %2.10f \n", event.acceleration.x, event.acceleration.y, event.acceleration.z);
	//printf("%2.10f \n", event.acceleration.x, event.acceleration.y, event.acceleration.z);

	//printf("x=%2.10f acc=%2.10f K=%2.10f P_k=%2.10f y_k = %2.10f\n",event.acceleration.x, acc_x, K, P_k, y_k );//event.acceleration.x);

	speed[0] = x_state.mat[1][0]; 
	speed[1] = y_state.mat[1][0]; 

}

void handle_rotation_event(ASensorEvent event) {

}


