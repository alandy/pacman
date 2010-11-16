typedef struct {
	void ** q;
	int head;
	int tail;
	int size;
	int maxSize;
	struct {
		int largestSize;
		int failedPutsFull;
		int failedReadsEmpty;
		int waitTime;
		int total;
	} statistics;
} queue; 

void * QGet(queue * q);

int QPut(queue * q, void * pt);