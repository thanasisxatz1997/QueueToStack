#include <Windows.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

HANDLE queueSem;
HANDLE stackSem;

int* Dequeue(int* queue,int* queueSize)
{
	int* newQueue;
	newQueue=malloc((*queueSize-1)*sizeof(int));
	memcpy(newQueue,queue+1,(*queueSize-1)*sizeof(int));
	*queueSize=*queueSize-1;
	free(queue);
	return newQueue;
}

int* Enqueue(int* queue,int* queueSize,int item)
{	
	queue=realloc(queue,(*queueSize+1)*sizeof(int));
	queue[*queueSize]=item;
	*queueSize=*queueSize+1;
	return queue;
}

int* StackPush(int *stack,int* stackSize,int item)
{
	*stackSize=*stackSize+1;
	stack=realloc(stack,(*stackSize)*sizeof(int));
	stack[*stackSize-1]=item;
	return stack;
}

int* StackPop(int *stack,int* stackSize)
{
	*stackSize=*stackSize-1;
	stack=realloc(stack,(*stackSize)*sizeof(int));
	return stack;
}

void PrintQueue(int* queue, int* queueSize)
{
	int i;
	for(i=0;i<*queueSize;i++)
	{
		printf("queue[%d],at %d : %d\n",i,&queue[i],queue[i]);
	}
	printf("\n");
}
void PrintStack(int* stack, int* stackSize)
{
	int i;
	for(i=0;i<*stackSize;i++)
	{
		printf("stack[%d],at %d : %d\n",i,&stack[i],stack[i]);
	}
	printf("\n");
}
int* ConvertQueueToStack(int* queue,int* queueSize,int* stackSize)
{
	WaitForSingleObject(queueSem,INFINITE);
	int* stack=malloc((*queueSize)*sizeof(int));
	*stackSize=0;
	int i;
	for(i=0;i<*queueSize;i++)
	{
		WaitForSingleObject(stackSem,INFINITE);
		stack=StackPush(stack,stackSize,queue[i]);
		ReleaseSemaphore(stackSem,1,NULL);
	}
	ReleaseSemaphore(queueSem,1,NULL);
	
	return stack;
}
int main()
{
	
	queueSem=CreateSemaphore(NULL,1,1,"queueAccess");
	stackSem=CreateSemaphore(NULL,1,1,"stackAccess");
	
	int i;
	
	int n=0;
	int* queueSize;
	queueSize=&n;
	
    int* queue= malloc(*queueSize*sizeof(int));
    
    queue=Enqueue(queue,queueSize,5);
    queue=Enqueue(queue,queueSize,3);
    queue=Enqueue(queue,queueSize,1);
    queue=Enqueue(queue,queueSize,6);
    
    PrintQueue(queue,queueSize);
	
	queue=Dequeue(queue,queueSize);
	
	PrintQueue(queue,queueSize);
	
	Enqueue(queue,queueSize,9);
    Enqueue(queue,queueSize,2);
    
    PrintQueue(queue,queueSize);
    
    queue=Dequeue(queue,queueSize);
    queue=Dequeue(queue,queueSize);
    queue=Dequeue(queue,queueSize);
    queue=Dequeue(queue,queueSize);
    
	PrintQueue(queue,queueSize);
	
	queue=Enqueue(queue,queueSize,7);
    queue=Enqueue(queue,queueSize,3);
    queue=Enqueue(queue,queueSize,2);
    queue=Enqueue(queue,queueSize,5);
    
    PrintQueue(queue,queueSize);
    
    int* stack=malloc(0);
    int m=0;
    int* stackSize;
	stackSize=&m;
	
    stack=ConvertQueueToStack(queue,queueSize,stackSize);
    PrintStack(stack,stackSize);
    
    stack=StackPop(stack,stackSize);
    stack=StackPop(stack,stackSize);
    stack=StackPop(stack,stackSize);

    PrintStack(stack,stackSize);
    
    stack=StackPush(stack,stackSize,4);
    stack=StackPush(stack,stackSize,9);
    
    PrintStack(stack,stackSize);
    
    stack=StackPop(stack,stackSize);

    PrintStack(stack,stackSize);
    
    printf( "Size of queue is %u bytes\n",_msize(queue));
    free(queue);
    
    printf( "Size of stack is %u bytes\n",_msize(stack));
    free(stack);
    
    printf("DONE!");
    return 0;
}
