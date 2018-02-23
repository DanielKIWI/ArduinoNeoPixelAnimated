// TimerQueue.h

#ifndef _TIMERQUEUE_h
#define _TIMERQUEUE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include <LinkedList.h>
#define micsTOs 1000 * 1000
#define sTOmics 1 / msTOs

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
		void (*_task)(float)
        unsigned long lastExecuted;
        unsigned long endTimeMicros;
        float startValue, endValue, actValue;
        float duration;
        float timeDone;
        AnimationTask(void (*task)(float), float start, float end, float dur){
			_task = task;
			startValue = start;
			actValue = start;
			endValue = end;
			duration = dur;
			endTimeMicros = micros() + (unsigned long)(duration * sTOmics);
			timeDone = 0;
			lastExecuted = micros();
        }
        //returns whether animation iscompleted
        bool execute() {
			unsigned long time = micros();
			if (time > endTimeMicros){
				_task(endValue)
				return true
			}
			float deltaTime = (float)(time - lastExecuted) * micsTOs
			actValue += deltaTime / duration * (endValue - startValue)
			_task(actValue)
			lastExecuted = time;
			return false;
        }
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

