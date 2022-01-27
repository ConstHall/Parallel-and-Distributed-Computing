#include<stdio.h>
#include<pthread.h>

int temp;
pthread_mutex_t mutex;

void *thread1(void *p) {
	pthread_mutex_lock(&mutex);
	temp=1;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void *thread2(void *p) {
	pthread_mutex_lock(&mutex);
	temp=2;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main(){
	int i;
	pthread_t t[2];
	pthread_create(&t[0],NULL,thread1,NULL);
	pthread_create(&t[1],NULL,thread2,NULL);
	for(i=0;i<=1;i++){
		pthread_join(t[i],NULL);
	}
}

