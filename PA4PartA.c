#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

//globals
#define NUM_POINTS 50000000

int pointsCircle = 0; //num of points in circle global

double random_double() //generates random double
{
	return random() / ((double)RAND_MAX + 1);
}

void* generate_points(void* arg)
{
	int i;
	for(i = 0; i < NUM_POINTS; i++){ //iterates through points and generates random doubles
		double x = random_double();
		double y = random_double();

		//check if inside
		if (sqrt(x * x + y * y) < 1.0){
			pointsCircle++;
		}
	}

	pthread_exit(NULL); //exits thread
}

int main()
{

	srand(time(NULL));

	pthread_t tid; //assign thread
	pthread_create(&tid, NULL, generate_points, NULL); //create threads

	pthread_join(tid, NULL); //waiting for threads to finish

	double pi = 4.0 * pointsCircle / NUM_POINTS; //final pi number

	printf("Pi: %f\n", pi); //displays

	return 0;
}
