#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct node {
	int data;
	struct node* next;
};

struct node* head = NULL;
struct node* tail = NULL;
static int p_size = 0;    //present size of buffer

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_has_space = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_has_data = PTHREAD_COND_INITIALIZER;

void* producer(void* a)
{
	int i;
	for (i = 1; i <= 1000; i++)
	{
		pthread_mutex_lock(&mutex);
		if (p_size == 100)	// full
		{
			pthread_cond_wait(&buffer_has_space, &mutex);
		}

		struct node* new = (struct node*)malloc(sizeof(struct node));
		new->data = i;
		new->next = NULL;
		if (head == NULL && tail == NULL)
		{
			head = tail = new;
			p_size++;
		}
		else
		{
			tail->next = new;
			tail = new;
			p_size++;
			tail->next = head;
		}
		tail->next = head;

		printf("data produce = %d, Cnt=%d\n", tail->data, p_size);
		pthread_cond_signal(&buffer_has_data);
		pthread_mutex_unlock(&mutex);
	}
}

void* consumer(void* b)
{
	int i, deldata;
	for (i = 1; i <= 1000; i++)
	{
		pthread_mutex_lock(&mutex);
		if (p_size == 0)	// no data
		{
			pthread_cond_wait(&buffer_has_data, &mutex);
		}
		deldata = head->data;
		head = head->next;
		p_size--;
		printf("data consume = %d, Cnt=%d\n", deldata, p_size);
		pthread_cond_signal(&buffer_has_space);
		pthread_mutex_unlock(&mutex);

	}
}

int main(void)
{
	int i;
	int a = 0;
	pthread_t threads[2];
	pthread_create(&threads[0], NULL, producer, (void*)& a);	//producer pthread
	pthread_create(&threads[1], NULL, consumer, (void*)& a);	//consumer pthread
	for (i = 0; i < 2; i++)
	{
		pthread_join(threads[i], NULL);
	}
	return 0;
}