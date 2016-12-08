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


//int buffer_index = 0;
int *buff;
int curr_buff_size = 0;
int buffer_size;
pthread_cond_t empty;
pthread_cond_t full;
pthread_mutex_t mtx;
//int buffer = 0;

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
    while(1)
    {
	pthread_mutex_lock(&mtx);

	while(curr_buff_size == 0)
	{
	    pthread_cond_wait(&full, &mtx);
	}
        
	int fd;
	for (i = 0; i < buffer_size; i++) 
        {
	    if (buff[i] != -1) 
	    {
	        fd = buff[i];
//	        printf("fd: %d\n", fd);
		curr_buff_size--;
	        buff[i] = -1;
		break;
	    }
	    i++;
	}     
	pthread_mutex_unlock(&mtx);
	requestHandle(fd);
	close(fd);
    }
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
    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&full, NULL);
    int listenfd, connfd, port, num_of_threads, clientlen;
    struct sockaddr_in clientaddr;

    getargs(&port, &num_of_threads, &buffer_size, argc, argv);
    if (num_of_threads < 1 || buffer_size < 0)
    {
	fprintf(stderr, "Invalid number of threads of buffers");
	exit(1);	
    }
    else  if (buffer_size == 1) {
	listenfd = Open_listenfd(port);
	while (1)
	{
	    clientlen = sizeof(clientaddr);
	    connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *)&clientlen);
	    requestHandle(connfd);	    
	}
    }     
  
//    printf("port: %d\n", port);
//    printf("num_of_threads: %d\n", num_of_threads);
//    printf("buffer size: %d\n", buffer_size);    

    // 
    // CS537: Create some threads...
    //
    
    int i;
    pthread_t thread_pool[num_of_threads];    
    
    buff = malloc(sizeof(int) * buffer_size);
    for (i = 0; i < buffer_size; i++)
    {
	buff[i] = -1;
//	printf("buff: %d\n", buff[i]);	
    }    

    int j;	
    for (i = 0; i < num_of_threads; i++)
    {
        pthread_create(&thread_pool[i], NULL, Consumer, (void *)&j);
//	printf("thread pool: %d\n", thread_pool[i]);
    } 

    listenfd = Open_listenfd(port);
   
     while (1) {
	pthread_mutex_lock(&mtx);
//	printf("curr_buff: %d\n", curr_buff_size);
//	printf("buff size: %d\n", buffer_size);
	while(curr_buff_size >= buffer_size)
	{
	    pthread_cond_wait(&empty, &mtx);
//	    printf("currbuff: %d\n", curr_buff_size);
//	    printf("buff size: %d\n", buffer_size);
        }
//	printf("got to after while\n");
	clientlen = sizeof(clientaddr);
	pthread_mutex_unlock(&mtx);
//	printf("before connfd\n");
	//clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
//	printf("connfd: %d\n", connfd);
	pthread_mutex_lock(&mtx);
	
	for (i = 0; i < buffer_size; i++)
	{
	    if (buff[i] == -1)
	    {
		buff[i] = connfd;
		curr_buff_size++;
		break;
	    }
	    i++;
	}
	
	pthread_cond_signal(&full);
	pthread_mutex_unlock(&mtx);
    }

}


    


 
