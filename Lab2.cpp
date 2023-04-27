//Brandon St Pierre
//Lab 2

#include <stdio.h> 
#include <pthread.h> 
#include <stdlib.h>
#include <iostream> 
using namespace std; 

int nThreads;            // #threads 

int turn;                // turn points which thread should run 
pthread_mutex_t mutex;   // a lock for this critical section 
pthread_cond_t *cond;    // array of condition variable[nThreads] 


void *thread_func(void *arg)
{ 
        int id = ((int *)arg)[0];                  // this thread's identifier 
        delete (int *)arg; 

        for (int loop = 0; loop < 10; loop++) // Run 10 times
         { 
           // enter the critical section
           pthread_mutex_lock(&mutex); 

           while (turn != id)
          { 
            int prevThread = (id + nThreads - 1) % nThreads;
            // wait until the (id - 1)th thread signals me. 
            pthread_cond_wait(&cond[id], &mutex);
          } 

          cout << "thread["<< id << "] got " << loop << "th turn" << endl;
          turn = (turn + 1) % nThreads; 

         // signal the next thread 
         pthread_cond_signal(&cond[turn]);

         // leave the critical section 
         pthread_mutex_unlock(&mutex);
         } 
} 

 int main(int argc, char *argv[])
{ 
   if (argc != 2)
   { 
       cerr << "usage: lab2 #threads" << endl; 
       return -1; 
   } 

   nThreads = atoi( argv[1] ); 
   if (nThreads < 1)
   { 
      cerr << "usage: lab1 #threads" << endl; 
      cerr << "where #threads >= 1" << endl; 
      return -1;
   }

   pthread_t *tid = new pthread_t[nThreads];  // an array of thread identifiers 
   cond = new pthread_cond_t[nThreads];       // an array of condition variables 
   turn = 0;                                  // points to which thread should run 

   pthread_mutex_init(&mutex, NULL);
   for (int i = 0; i < nThreads; i++) {
       pthread_cond_init(&cond[i], NULL);
   }

   for (int i = 0; i < nThreads; i++)
   {    
      int *id = new int[1]; 
      id[0] = i; 
      pthread_create( &tid[i], NULL, thread_func, (void *)id ); 
   } 

   for (int i = 0; i < nThreads; i++) // wait for all threads
   { 
      pthread_join( tid[i], NULL );
   }

   pthread_mutex_destroy(&mutex);
    for (int i = 0; i < nThreads; i++) {
        pthread_cond_destroy(&cond[i]);
    }

    delete[] tid;
    delete[] cond;
    
   return 0;
} 