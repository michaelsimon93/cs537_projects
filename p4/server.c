#include "cs537.h"
#include "request.h"
#include <pthread.h>
#include <stdio.h>
#include <pthread.h>

// 
// server.c: A very, very simple web server
//
// To run:
//  server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//


int buffer_index = 0;
int *buff;
int curr_buff_size = 0;
int max_buff_size = 0;

pthread_cond_t cond_consumer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_producer = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
int buffer = 0;

/*
void *Producer(void *ptr) 
{
    int i;
    
    for (i = 0; i < num_of_threads; i++)
    {
        pthread_mutex_lock(&mtx);
	while (buffer != 0)
        {
	    pthread_cond_wait(&cond_producer, &mtx);
        }
	buffer = i;
	curr_buff_size++;
	pthread_cond_signal(&cond_consumer);
	pthread_mutex_unlock(&mtx);
    } 
    pthread_exit(0);       
}
*/

void *Consumer(void *ptr)
{
    int i;

    for (i = 0; i < num_of_threads; i++) 
    {
	pthread_mutex_lock(&mtx);
        while (buff == 0)
        {
	    pthread_cond_wait(&cond_consumer, &mtx);
	}     
        *buff = -1;
	curr_buff_size--;
        pthread_cond_signal(&cond_producer);
        pthread_mutex_unlock(&mtx);
    }
    pthread_exit(0);
}

// CS537: Parse the new arguments too
void getargs(int *port, int *num_of_threads, int *buffer_size, int argc, char *argv[])
{
    if (argc != 4) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
    *num_of_threads = atoi(argv[2]);
    *buffer_size = atoi(argv[3]);
}


int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;

    getargs(&port, &num_of_threads, &buffer_size, argc, argv);

    // 
    // CS537: Create some threads...
    //
    
    int i;
    pthread_t thread_pool[num_of_threads];    
    pthread_t pro;
    pthread_t con;
    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&cond_consumer, NULL);
    pthread_cond_init(&cond_producer, NULL);        

    buff = (int *) malloc(sizeof(int) * buffer_size);

    for (i = 0; i < num_of_threads; i++)
    {
        pthread_create(&thread_pool[i], NULL, Consumer, (void *)i);
    } 
   
    listenfd = Open_listenfd(port);
   
     while (1) {
	pthread_mutex_lock(&mtx);
	while( curr_buff_size >= max_buff_size)
	{
	    pthread_cond_wait(&empty, &mtx);
        }
	pthread_cond_unlock(&mtx);

	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);

	// 
	// CS537: In general, don't handle the request in the main thread.
	// Save the relevant info in a buffer and have one of the worker threads 
	// do the work. However, for SFF, you may have to do a little work
	// here (e.g., a stat() on the filename) ...
	// 

		
	
	requestHandle(connfd);

	Close(connfd);
    }

}


    


 
