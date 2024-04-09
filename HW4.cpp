```cpp
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>

const int TABLE_SIZE = 5;

void philosopher(int id, sem_t* semaphores) 
{
	while (true) 
	{
        int firstFork = id == 4 ? 0 : id;
        int secondFork = id == 4 ? 4 : id + 1;

		std::cout << id << ": is pondering" << std::endl;
		sleep(rand() % 3);

		if(sem_wait(&semaphores[firstFork]) < 0)
        {
            std::cerr << "sem_wait" << std::endl;
            exit(EXIT_FAILURE);
        }
		std::cout << id << ": grabbed left fork" << std::endl;

		if(sem_trywait(&semaphores[secondFork]) == -1)
		{
			if(sem_post(&semaphores[firstFork]) < 0)
            {
                std::cerr << "sem_post" << std::endl;
                exit(EXIT_FAILURE);
            }
			std::cout << id << ": released one fork" << std::endl;
		}
		else
		{
			std::cout << id << ": grabbed right fork" << std::endl;
			std::cout << id << ": is feasting" << std::endl;
			sleep(rand() % 3);

			if(sem_post(&semaphores[firstFork]) < 0)
            {
                std::cerr << "sem_post" << std::endl;
                exit(EXIT_FAILURE);
            }
			if(sem_post(&semaphores[secondFork]) < 0)
            {
                std::cerr << "sem_post" << std::endl;
                exit(EXIT_FAILURE);
            }
			std::cout << id << ": returned forks" << std::endl;
		}	

													
	}	
}

int main() 
{
	int shm_fd = shm_open("/shared_mem", O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (shm_fd == -1) 
	{
		std::cerr << "Failed to create shared memory" << std::endl;
		exit(EXIT_FAILURE);
	}
    int trunc = ftruncate(shm_fd, TABLE_SIZE * sizeof(sem_t));
    if(trunc == -1)
    {
        std::cerr << "Failed to truncate shared memory" << std::endl;
        exit(EXIT_FAILURE);
    }
	sem_t* semaphores = (sem_t*)mmap(NULL, TABLE_SIZE * sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, shm_fd, 0);
	for (int i = 0; i < TABLE_SIZE; ++i) 
	{
        if(sem_init(&semaphores[i], 1, 1) < 0)
        {
            std::cerr << "sem_init" << std::endl;
            exit(EXIT_FAILURE);
        }
	}

	std::vector<int> pids(5);
	for(int i = 0; i < pids.size(); ++i)
	{
		pids[i] = fork();
		if(pids[i] == 0)
		{
			philosopher(i, semaphores);
			exit(0);
		}
	}	
	for(int x : pids)
	{
		if(waitpid(x,nullptr,0) == -1)
		{
			std::perror("waitpid: ");
			exit(EXIT_FAILURE);
		}
	}
					
	for(int i = 0; i < TABLE_SIZE; ++i) 
	{
		if(sem_destroy(&semaphores[i]) < 0)
        {
            std::cerr << "sem_destroy" << std::endl;
            exit(EXIT_FAILURE);
        }
	}
						
	munmap(semaphores, TABLE_SIZE * sizeof(sem_t));
	shm_unlink("/shared_mem");
    if(close(shm_fd) < 0)
    {
        std::perror("close");
        exit(errno);
    }
    return 0;
}
```