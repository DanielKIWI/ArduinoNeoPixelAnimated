// TimerQueue.h

#ifndef _TIMERQUEUE_h
#define _TIMERQUEUE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include "LinkedList.h"
#include "Color.h"
#define micsTOs 1000 * 1000
#define sTOmics 1 / micsTOs

template<typename ParameterStruct>
class Task {
public:
	virtual float execute() {

	}
};

template<typename ParameterStruct>
class TimedTask : Task<ParameterStruct> {
private:
	float(*task)(ParameterStruct) = 0;
	ParameterStruct parameter;
public:
	unsigned long dueTime;
	TimedTask() {
		task = NULL;
		parameter = ParameterStruct();
		dueTime = 0;
	}
	TimedTask(float(*t)(ParameterStruct), ParameterStruct param, float deltaTime) {
		task = t;
		parameter = param;
		dueTime = (float)micros() + (unsigned long)deltaTime * (10 ^ 6);
	}
	float execute() override {
		if (task != NULL) {
			return task(parameter);
		}
		else return -1;
	}
};
enum AnimationCurve {
	Linear,
	Squared,
	Logistic,
	GaussBlink,
	Loop,
	PingPong
};

enum Type
{
	INT,
	FLOAT,
	COLOR,
};

template<typename ParameterStruct>
class AnimationTask : Task<ParameterStruct> {
private:
	void(*task)(void*, ParameterStruct);
	ParameterStruct parameter;
	AnimationCurve cuve;
	//unsigned long lastExecuted;
	//unsigned long endTimeMicros;
	void *startValue, *endValue, *actValue;
	Type valueType;
	float duration;
	float timeDone;
public:
	AnimationTask() {
		task = NULL;
		parameter = ParameterStruct();
		AnimationCurve curve = Linear;
		startValue = endValue = actValue = NULL;
		duration = timeDone = 0.f;
	}
	AnimationTask(void(*t)(void*, ParameterStruct), ParameterStruct param, Type type, void *start, void *end, float dur, AnimationCurve c, unsigned long time) {
		valueType = type;
		task = t;
		parameter = param;
		this.curve = c;
		startValue = start;
		actValue = start;
		endValue = end;
		duration = dur;
		//this.endTimeMicros = time + (unsigned long)(duration * sTOmics);
		timeDone = 0;
		//this.lastExecuted = time;
	}
	void* getActValue() {
		float factor = timeDone / duration;
		switch (valueType) {
		case INT:
			int istart = *static_cast<int*>(startValue);
			int iend = *static_cast<int*>(startValue);
			int iact = istart + (int)((iend - istart) * factor);
			int *iresult = new int(iact);
			return iresult;
			break;
		case FLOAT:
			float fstart = *static_cast<float*>(startValue);
			float fend = *static_cast<float*>(startValue);
			float fact = fstart + ((fend - fstart) * factor);
			float *fresult = new float(fact);
			return fresult;
			break;
		case COLOR:
			Color cstart = *static_cast<Color*>(startValue);
			Color cend = *static_cast<Color*>(startValue);
			Color cact = (cstart + cend * factor) - cstart * factor;
			Color *cresult = new Color(cact);
			return cresult;
			break;
		default:
			return startValue;
			break;
		}
	}
	//returns whether animation iscompleted
	bool execute(float deltaTime) {
		timeDone += deltaTime; 
		if (timeDone > duration) {
			task(endValue, parameter);
			return true;
		}

		/*
		//float deltaTime = (float)(time - lastExecuted) * micsTOs;
		switch (this.curve)
		{
		case Linear:
			actValue = timeDone / duration * (endValue - startValue);
			break;

		default:
			actValue = timeDone / duration * (endValue - startValue);
			break;
		}
		task(actValue, parameter);*/
		return false;
	}
};
template<typename ParameterStruct>
class FloatAnimationTask : AnimationTask<ParameterStruct> {
private:
	void(*task)(float, ParameterStruct);
	ParameterStruct parameter;
	AnimationCurve cuve;
	//unsigned long lastExecuted;
	//unsigned long endTimeMicros;
	float startValue, endValue, actValue;
	float duration;
	float timeDone;
public:
	FloatAnimationTask() override {
		task = NULL;
		parameter = ParameterStruct();
		AnimationCurve curve = Linear;
		startValue = endValue = actValue = 0.f;
		duration = timeDone = 0.f;
	}

	FloatAnimationTask(void(*t)(float, ParameterStruct), ParameterStruct param, void *start, void *end, float dur, AnimationCurve c, unsigned long time) override {
		task = t;
		parameter = param;
		this.curve = c;
		startValue = *static_cast<float*>(start);
		actValue = startValue;
		endValue = *static_cast<float*>(end);
		duration = dur;
		//this.endTimeMicros = time + (unsigned long)(duration * sTOmics);
		timeDone = 0;
		//this.lastExecuted = time;
	}
	FloatAnimationTask(void(*t)(float, ParameterStruct), ParameterStruct param, float start, float end, float dur, AnimationCurve c, unsigned long time) {
		task = t;
		parameter = param;
		this.curve = c;
		startValue = start;
		actValue = start;
		endValue = end;
		duration = dur;
		//this.endTimeMicros = time + (unsigned long)(duration * sTOmics);
		timeDone = 0;
		//this.lastExecuted = time;
	}
	float getActValue() override {
		switch (this.curve)
		{
		case Linear:
			actValue = timeDone / duration * (endValue - startValue);
			break;

		default:
			actValue = timeDone / duration * (endValue - startValue);
			break;
		}
	}
	//returns whether animation iscompleted
	bool execute(float deltaTime) {
		timeDone += deltaTime;
		if (timeDone > duration) {
			task(endValue, parameter);
			return true;
		}
		//float deltaTime = (float)(time - lastExecuted) * micsTOs;
		actValue = getActValue();
		task(actValue, parameter);
		return false;
	}
};

template<typename TaskParam, typename AnimParam>
class TimerQueueClass
{
 protected:

	LinkedList<TimedTask<TaskParam>> TaskQueue;
	LinkedList<AnimationTask<AnimParam>> AnimationQueue;
	ListNode<AnimationTask<AnimParam>> *firstDoneThisFrame = NULL;

	unsigned long lastTimeAnimated = 0;

 public:
	
	 TimerQueueClass<TaskParam, AnimParam>();/* : TaskQueue(), AnimationQueue() {

	}*/

	//void init();
	bool sortforDueTime(TimedTask<TaskParam> left, TimedTask<TaskParam> right);
	void doTaskInSec(float(*task)(TaskParam), TaskParam param, float secondsTillExecute);
	
	void addTask(float(*task)(TaskParam), TaskParam param);

	void addAnimationTask(float(*task)(float, AnimParam), float minValue, float maxValue, float duration, AnimationCurve c = Linear);

	void animate();

	void nextTask();

};

//extern TimerQueueClass TimerQueue;

#endif


template<typename TaskParam, typename AnimParam>
TimerQueueClass<TaskParam, AnimParam>::TimerQueueClass() {
	TaskQueue = LinkedList<TimedTask<TaskParam>>();
	AnimationQueue = LinkedList<AnimationTask<AnimParam>>();
}
/*
template<typename TaskParam, typename AnimParam>
void TimerQueueClass<TaskParam, AnimParam>::init()
{
	TaskQueue = LinkedList<TimedTask<TaskParam>>();
	AnimationQueue = LinkedList<AnimationTask<AnimParam>>();

}*/

template<typename TaskParam, typename AnimParam>
void TimerQueueClass<TaskParam, AnimParam>::addTask(float(*task)(TaskParam), TaskParam param)
{
	TimedTask<TaskParam> newTimedTask = TimedTask<TaskParam>(task, param, micros());
	AnimationQueue.addBack(newTimedTask);

}
template<typename TaskParam, typename AnimParam>
static bool TimerQueueClass<TaskParam, AnimParam>::sortforDueTime(TimedTask<TaskParam> left, TimedTask<TaskParam> right) {
	return left.dueTime < right.dueTime;
}
template<typename TaskParam, typename AnimParam>
void TimerQueueClass<TaskParam, AnimParam>::doTaskInSec(float(*task)(TaskParam), TaskParam param, float secondsTillExecute)
{
	TimedTask<TaskParam> newTimedTask = TimedTask<TaskParam>(task, param, secondsTillExecute);

	AnimationQueue.sortIn(newTimedTask, &sortforDueTime);
}

template<typename TaskParam, typename AnimParam>
void TimerQueueClass<TaskParam, AnimParam>::addAnimationTask(float(*task)(float, AnimParam), float minValue, float maxValue, float duration, AnimationCurve c = Linear)
{
	AnimationTask<AnimParam> newAnimationTask = AnimationTask<AnimParam>(task, minValue, maxValue, duration, c, micros());
	AnimationQueue.addBack(newAnimationTask);
}

template<typename TaskParam, typename AnimParam>
void TimerQueueClass<TaskParam, AnimParam>::animate()
{
	float deltaTime = (float)(micros() - lastTimeAnimated) * micsTOs;
	if (AnimationQueue.size() > 0) {
		AnimationTask<AnimParam> task = AnimationQueue.front();
		while (task.task != NULL) {
			task.execute(deltaTime);
		}
	}

}

template<typename TaskParam, typename AnimParam>
void TimerQueueClass<TaskParam, AnimParam>::nextTask() {
	if (TaskQueue.size() > 0) {
		TimedTask<TaskParam> topTask = TaskQueue.top();
		if (topTask.dueTime <= micros()) {
			float nextTime = topTask.execute();
		}
		else {
			animate();
		}

	}
}