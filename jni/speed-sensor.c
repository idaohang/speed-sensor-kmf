/*
 * Try to make a software based sensor for speed. 
 * 
 *
 */
#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <android/sensor.h>
#include <android/looper.h>
#include <pthread.h>

#include "speed-calculator.h"

#define LOOPER_ID 1
#define SAMP_PER_SEC 2000

pthread_t speed_calculating_thread;
int log_output = 0;

void* main_thread(void* );

long double abs_(long double number ) {
	return (number >= 0 )?(number):(-number);
}

long double max(long double a, long double b){
	return (abs_(a) > abs_(b))?a:b;
}

int startSpeedSensor() {
	log_output = 1;
	main_thread(NULL);
	return 0;
}

void* main_thread(void* attr) {

	ASensorManager* sensorManager = ASensorManager_getInstance();

	ALooper* looper = ALooper_forThread();
	if(looper == NULL)
		looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);

	ASensorRef accelerometerSensor = ASensorManager_getDefaultSensor(sensorManager,10);
	ASensorRef orientationSensor = ASensorManager_getDefaultSensor(sensorManager, 11);

	ASensorEventQueue* queue = ASensorManager_createEventQueue(sensorManager, looper, LOOPER_ID, NULL, NULL);

	ASensorEventQueue_enableSensor(queue, accelerometerSensor);
	ASensorEventQueue_enableSensor(queue, orientationSensor);

	ASensorEventQueue_setEventRate(queue, accelerometerSensor, (1000L/SAMP_PER_SEC)*1000); 

	int ident; 
	int events;

	ASensorEvent event;
	while (1) {
		while ((ident=ALooper_pollAll(-1, NULL, &events, NULL) >= 0)) {
			// If a sensor has data, process it now.
			if (ident == LOOPER_ID) {
				while (ASensorEventQueue_getEvents(queue, &event, 1) > 0) {
					if (event.type == 10) {
						handle_acceleration_event(event);
					} else 
					if (event.type == 11) {
						handle_rotation_event(event);
					}
				}
			}
		}
	}
	return NULL;
}

/// JAVA interfaces
jdouble
Java_co_flyver_flyvercore_MainControllers_MainController_getSpeedX() {
	return speed[0];
}
jdouble
Java_co_flyver_flyvercore_MainControllers_MainController_getSpeedY() {
	return speed[1];
}
jdouble
Java_co_flyver_flyvercore_MainControllers_MainController_getSpeedZ() {
	return speed[2];
}

jint
Java_co_flyver_flyvercore_MainControllers_MainController_startSpeedSensor() {

	return pthread_create(&speed_calculating_thread, NULL, main_thread, NULL);
}

