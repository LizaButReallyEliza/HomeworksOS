```cpp
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t tobacco;
sem_t paper;
sem_t match;
sem_t bartender;

void* smoker_tobacco(void* arg)
{
	while(true)
	{
		sem_wait(&tobacco);
		std::cout << 'T' << std::endl;
		sem_post(&bartender);
	}
	return nullptr;
}

void* smoker_paper(void* arg)
{
	while(true)
	{
		sem_wait(&paper);
		std::cout << 'P' << std::endl;
		sem_post(&bartender);
	}
	return nullptr;
}


void* smoker_match(void* arg)
{
	while(true)
	{
		sem_wait(&match);
		std::cout << 'M' << std::endl;
		sem_post(&bartender);
	}
	return nullptr;
}

void* bartender_func(void* arg)
{
	while(true)
	{
		sem_wait(&bartender);
		
		char item;

		std::cin >> item;

		if(item == 't')
		{
			sem_post(&tobacco);
		}
		else if(item == 'p')
		{
			sem_post(&paper);
		}
		else if(item == 'm')
		{
			sem_post(&match);
		}
	}
	return nullptr;
}

int main()
{
	sem_init(&tobacco, 0, 0);
	sem_init(&paper, 0, 0);
	sem_init(&match, 0, 0);
	sem_init(&bartender, 0, 1); 
	   
	pthread_t t_thread, p_thread, m_thread, b_thread;
	           	
	pthread_create(&t_thread, nullptr, smoker_tobacco, nullptr);
	pthread_create(&p_thread, nullptr, smoker_paper, nullptr);
	pthread_create(&m_thread, nullptr, smoker_match, nullptr);
	pthread_create(&b_thread, nullptr, bartender_func, nullptr);
	
	pthread_join(t_thread, nullptr);
	pthread_join(p_thread, nullptr);
	pthread_join(m_thread, nullptr);	
	pthread_join(b_thread, nullptr);
	
	sem_destroy(&tobacco);
	sem_destroy(&paper);
	sem_destroy(&match);
	sem_destroy(&bartender);
	
	return 0;
}
```