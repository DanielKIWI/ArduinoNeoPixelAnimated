// TimerQueue.h

#ifndef _TIMERQUEUE_h
#define _TIMERQUEUE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include <LinkedList.h>


class TimerQueueClass
{
 protected:

	 struct TimedTask {
		 float (*_task)() = 0;
		 unsigned long dueTime;
		 QueueTask() {
			 _task = NULL;
			 dueTime = 0;
		 }
		 TimedTask(float (*task)(), float deltaTime) {
			 _task = task;
			 dueTime = (float)micros() + (unsigned long)deltaTime * (10 ^ 6);
		 }
		 float execute() {
			 return _task();
		 }
	 };
    
    struct AnimationTask {
       	(*task)(value)
         unsigned long lastExecuted;
      float
		//	start und end value
		//	speed/ duration
		//	wenn executed
    }

	 LinkedList<TimedTask> queue;

 public:
	void init();

	void doTaskInSec(float(*_task)(), float secondsTillExecute);

	void animate() {
		//animation queue
		//	(*task)(value)
		//	lastExecuted
		//	start und end value
		//	speed/ duration
		//	wenn executed kommt ans ende wenn noch nicht fertig
		//  
	}

	void nextTask() {
		if (queue.size() > 0) {
			QueueTask topTask = queue.pop();
			if (topTask.dueTime <= micros()) {
				float nextTime = topTask.execute();
			}
			else {
				animate();
			}

		}
	}

};

extern TimerQueueClass TimerQueue;

#endif

