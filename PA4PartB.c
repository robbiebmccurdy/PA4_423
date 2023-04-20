#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define MIN_THREADS 1
#define MIN_POINTS 50000000

double pointsCircle = 0.0;
pthread_mutex_t lock;

double random_double()
{
	return random() / ((double)RAND_MAX + 1);
}

void *generate_points(void *arg)
{
	int points = *(int *)arg;
	int pointsIn = 0;
	int i;

	for(i = 0; i < points; i++)
	{
		double x = random_double();
		double y = random_double();
	
		if (sqrt(x * x + y * y) < 1.0)
		{
			pointsIn++;
		}
	}

	pthread_mutex_lock(&lock);
	pointsCircle += pointsIn;

	pthread_mutex_unlock(&lock);

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s <num_threads>\n", argv[0]);
		return -1;
	}

	int num_threads = atoi(argv[1]);
	if (num_threads < MIN_THREADS)
	{
		printf("Error: Number of threads must be a positive integer\n");
		return -1;
	}

	int pointsPerThread = ceil(MIN_POINTS / num_threads);
	int totalPoints = num_threads * pointsPerThread;

	if (totalPoints < MIN_POINTS)
	{
		printf("Error: Total number of points must be no less than %d\n", MIN_POINTS);
		return -1;
	}

	pthread_t threads[num_threads];

	pthread_mutex_init(&lock, NULL);

	int i;

	for (i = 0; i < num_threads; i++)
	{
		pthread_create(&threads[i], NULL, generate_points, &pointsPerThread);
	}

	for (i = 0; i < num_threads; i++)
	{
		pthread_join(threads[i], NULL);
	}

	double pi = 4.0 * pointsCircle / totalPoints;
	printf("Pi: %1f\n", pi);

	pthread_mutex_destroy(&lock);

	return 0;
}
