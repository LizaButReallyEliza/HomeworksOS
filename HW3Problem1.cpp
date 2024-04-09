#include <iostream>
#include <semaphore.h> 
#include <pthread.h>
#include <vector>

const int BUFFER_SIZE = 8;

pthread_cond_t empty;
pthread_cond_t full;
pthread_mutex_t lock;
std::vector<char> buffer(BUFFER_SIZE, 0);
int in = 0;
int out = 0;
int count = 0;

void* producer(void* arg)
{
	char c = *((char*)arg);
	for(int i = 0; i < 2; ++i)
	{
		pthread_mutex_lock(&lock);
		while(count == BUFFER_SIZE)
			pthread_cond_wait(&empty, &lock);
		buffer[in] = c;
		in = (in + 1) % BUFFER_SIZE;
		std::cout << "Produced: " << c << std::endl;
		++count;
		pthread_cond_signal(&full);
		pthread_mutex_unlock(&lock);
	}
	return nullptr;
}

void* consumer(void* arg)
{
	for(int i = 0; i < 2; ++i) 
	{
		pthread_mutex_lock(&lock);
		while(count == 0)
			pthread_cond_wait(&full, &lock);
		char c = buffer[out];
		out = (out + 1) % BUFFER_SIZE;
		std::cout << "Consumed: " << c << std::endl;
		--count;
		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&lock);
	}
	return nullptr;
}



int main()
{
	 pthread_mutex_init(&lock, nullptr);
	 pthread_cond_init(&empty, nullptr);
	 pthread_cond_init(&full, nullptr);

	 std::vector<pthread_t> threads(20);
	 std::vector<char> data{'a','b','c','d','e','f','g','h','i','j'};
	 for(int i = 0; i < 20; i+=2)
	 {
	 	pthread_create(&threads[i], nullptr, producer, &data[i/2]);
		pthread_create(&threads[i+1], nullptr, consumer, &data[i/2]);
	 }

	 for(int i = 0; i < 20; ++i)
	 {
	 	pthread_join(threads[i], nullptr); 
	 }


	 pthread_mutex_destroy(&lock);
	 pthread_cond_destroy(&empty);
	 pthread_cond_destroy(&full);

	 return 0;
}
