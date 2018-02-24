// 
// 
// 

#include "TimerQueue.h"

void TimerQueueClass::init()
{


}

void TimerQueueClass::doTaskInSec(float(*_task)(), float secondsTillExecute) 
{

}

void TimerQueueClass::AddAnimationTask(float(*task)(float value), float minValue, float maxValue, float duration) 
{
	AnimationTask newAnimationTask = AnimationTask(task, minValue, maxValue, duration);
	if (firstDoneThisFrame == NULL)
	{
		AnimationQueue.addBack(newAnimationTask);
	}
}

void TimerQueueClass::animate()
{
	AnimationTask task = AnimationQueue.startIterate();
	while (task._task != NULL) {
		task.execute();
	}

}

void TimerQueueClass::nextTask() {
	if (TaskQueue.size() > 0) {
		TimedTask topTask = TaskQueue.pop();
		if (topTask.dueTime <= micros()) {
			float nextTime = topTask.execute();
		}
		else {
			animate();
		}

	}
}

TimerQueueClass TimerQueue;

