#include "queue.h"

void * QGet(queue * q) {
	void * pt;
	
	if (q->size <= 0) {
		q->statistics.failedReadsEmpty++;
		return 0;
	} else {
		pt = q->q[q->head];
		if (q->head++ > q->maxSize) {
			q->head = 0;
		}
		q->size--;
		return pt;
	}
	
}

int QPut(queue * q, void * pt) {

	if (q->size >= q->maxSize) {
		q->statistics.failedPutsFull++;
		return -1;
	} else {
		q->q[q->tail] = pt;
		if (q->tail++ > q->maxSize) {
			q->tail = 0;
		}
		q->size++;
		if (q->size > q->statistics.largestSize) {
			q->statistics.largestSize = q->size;
		}
		q->statistics.total++;
		return q->size;
	}

}

