#include <stdio.h>
#include <sys/time.h> // Interval Timer
#include <signal.h> // Signal
#include <unistd.h> // Pause, Ctime
#include <stdlib.h> // Exit
#include <pthread.h> // Pthread
#include <sys/types.h> // Time
#include <time.h> // Time

void sig_handler(int); // sig_handler
#define NUM_THREADS 7 // 생성할 thread 갯수

// Thread 정보를 저장할 TCB 구조체
struct TCB
{
	int Period; // 주기
	int Thread_id; // 스레드 id
	int Time_left_to_Invoke; // 남은 time
	pthread_cond_t pcond;
};

// Condition 변수와 Mutex 변수

pthread_mutex_t API_Mutex = PTHREAD_MUTEX_INITIALIZER;

// 스레드의 정보를 저장할 TCB구조체 10개 선언
struct TCB Thread_array[10];
time_t curtime; // 현재 time을 출력하기 위한 변수

void SetThread(int period, int thread_id)
{
	// 스레드의 정보 저장
	pthread_mutex_lock(&API_Mutex);
	Thread_array[thread_id].Period = period;
	Thread_array[thread_id].Thread_id = thread_id;
	Thread_array[thread_id].Time_left_to_Invoke = period;
	pthread_cond_init(&Thread_array[thread_id].pcond, NULL);
	pthread_mutex_unlock(&API_Mutex);
}

// tt_wait_invocation(thread_id)
int Invocation(int thread_id)
{
	// 자신의 주기가 되기를 기다리는 함수
	pthread_mutex_lock(&API_Mutex);
	pthread_cond_wait(&Thread_array[thread_id].pcond, &API_Mutex);
	pthread_mutex_unlock(&API_Mutex);
	// 1을 반환함으로써 TimeTriggered에 주기가 되었음을 알려준다.
	return 1;
}

void *TimeTriggered(void* p_thread_id)
{
	// void* 형으로 받은 thread id를 int형으로 변환
	int thread_id = *(int*)(p_thread_id);
	int period = (thread_id + 1) * 1000; // thread 주기
	//int period = 1000;
	sleep(1);
	printf("thread_id : %d / period : %d\n", thread_id, period);

	// SetThread함수를 호출하여 thread의 정보 저장
	SetThread(period, thread_id);
	while (1)
	{
		// thread의 주기가 다 되었다면
		if (Invocation(thread_id))
		{
			if (time(&curtime) < 0) {
				perror("time error\n");
				exit(1);
			}
			// ctime함수를 이용하여 time을 출력해준다.
			printf("The time is %s\n", ctime(&curtime));
		}
	}
}

int main()
{
	signal(SIGALRM, sig_handler); // Set SIGALRM
	struct itimerval delay; // Interval Timer 설정

	delay.it_value.tv_sec = 1; // first alarm
	delay.it_value.tv_usec = 0;
	delay.it_interval.tv_sec = 0;
	delay.it_interval.tv_usec = 10000;
	int ticks;
	ticks = setitimer(ITIMER_REAL, &delay, NULL);
	if (ticks)
	{
		perror("setitimer");
		return 0;
	}
	pthread_t threads[NUM_THREADS];
	int rc, t;
	int pid[10] = { 0,1,2,3,4,5,6,7,8,9 }; // thread_id
	for (t = 0; t < NUM_THREADS; t++)
	{
		printf("Creating thread %d\n", t);
		// pthread 생성
		rc = pthread_create(&threads[t], NULL, TimeTriggered, (void*)(&pid[t]));
		if (rc) // pthread 생성시 오류인 경우
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	while (1)
		pause();

}
void sig_handler(int signo)
{
	int i;
	for (i = 0; i < NUM_THREADS; i++)
	{
		pthread_mutex_lock(&API_Mutex);
		// 저장된 남은 time 정보에서 10씩 빼가면서
		Thread_array[i].Time_left_to_Invoke -= 10;
		pthread_mutex_unlock(&API_Mutex);

		// 0이 되었다면 주기가 완료 된것이므로
		if (Thread_array[i].Time_left_to_Invoke == 0)
		{
			pthread_mutex_lock(&API_Mutex);
			Thread_array[i].Time_left_to_Invoke = Thread_array[i].Period;
			pthread_mutex_unlock(&API_Mutex);
			// Cond[i] signal을 보내 알려준다.
			pthread_cond_signal(&Thread_array[i].pcond);
		}
	}
	// 시그널 처리기가 1회용이므로 시그널 처리기를 계속 유지하기 위해 다시 호출
	signal(SIGALRM, sig_handler);
}