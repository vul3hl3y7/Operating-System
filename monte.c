/*
*   Use Monte-Carlo to calculate pi.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>

#define N 5000
#define maxthread 32
#define sp 1024
#define s 1

void monte();
void muli();

void *com_pi(void *);

int thits, hits[maxthread][s];
int sp_per_thread, num_threads;

int main(void)
{
	char c;
	printf("A-to use monte carlo to calculate pi.\n");
	printf("B-to use multi-thread to calculate pi.\n");
	printf("C-to end program.\n");
	c = getchar();
	getchar();

	while(1)
	{
		switch(c)
		{
			case 'A':
			{
				monte();
				break;
			}

			case 'B':
			{
				muli();
				break;
			}
			case 'C':
			{
				return 0;		
			}
			default:
			{
				printf("Try again.\n");	
				c = getchar();
				getchar();
			}
		}
	}
	return 0;
}

void monte()
{
	srand(time(NULL));

	int sum = 0;

	int i;
	for(i = 0; i < N; i++)
	{
		double x = (double) rand() / RAND_MAX;		
		double y = (double) rand() / RAND_MAX;

		if((x * x + y * y) < 1)
		{
			sum++;		
		}
	}
	printf("Monte carlo pi = %lf\n", 4.0 * (double) sum / (N - 1));
		
	return;		
}

void muli()
{
	int i;
	
	pthread_t p_threads[maxthread];
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	printf("How many threads ?(less then 32 please)\n");
	scanf("%d", &num_threads);

	thits = 0;
	sp_per_thread = sp / num_threads;

	for(i = 0; i < num_threads; i++)
	{
		hits[i][0] = i;
		pthread_create(&p_threads[i], &attr, com_pi, (void *)&hits[i]);
	}
	
	for(i = 0; i < num_threads; i++)
	{
		pthread_join(p_threads[i], NULL);
		thits += hits[i][0];
	}
	printf("Multi thread pi : %lf\n", 4.0 * (double) thits / sp);

	return ;
}

void *com_pi(void * t)
{
	unsigned int seed;
	int i;
	int *hit_p;
	double rand_x, rand_y;
	hit_p = (int *)t;
	seed = *hit_p;
	int local_hit = 0;

	for(i = 0; i < sp_per_thread; i++)
	{
		rand_x = (double)(rand_r(&seed)) / (double)(RAND_MAX);		
		rand_y = (double)(rand_r(&seed)) / (double)(RAND_MAX);		

		if(rand_x * rand_x + rand_y * rand_y < 1)
		{
			(*hit_p)++;		
		}
		seed *= i;
	}
	pthread_exit(0);
}
