// TimerQueue.h

#ifndef _TIMERQUEUE_h
#define _TIMERQUEUE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Math.h"
#include "LinkedList.h"
#include "Color.h"
#define micsTOs 1000 * 1000
#define sTOmics 1 / micsTOs

class Task {
public:
	unsigned long dueTime;
	virtual bool execute() {

	}
	virtual bool execute(float deltaTime) {
		return execute();
	}
};

template<typename ParameterStruct>
class ParameterTask : public Task {
protected:
	ParameterStruct *parameter;
public:
	virtual bool execute() {

	}
	virtual bool execute(float deltaTime) {
		return execute();
	}
};

template<typename ParameterStruct>
class TimedTask : public ParameterTask<ParameterStruct> {
private:
	void(*task)(ParameterStruct*) = 0;
public:
	TimedTask() {
		task = NULL;
		this->parameter = new ParameterStruct();
		this->dueTime = 0;
	}
	TimedTask(bool(*t)(ParameterStruct*), ParameterStruct *param, float deltaTime) {
		task = t;
		this->parameter = param;
		this->dueTime = micros() + (unsigned long)(deltaTime * sTOmics);
	}
	bool execute() override {
		if (task != NULL) {
			task(this->parameter);
		}
		return false;
	}
};

template<typename ParameterStruct>
class FrequentTask : public TimedTask<ParameterStruct> {
private:
	bool(*task)(ParameterStruct*) = 0;
	unsigned long deltaTime;
public:
	FrequentTask() {
		task = NULL;
		this->parameter = ParameterStruct();
		this->dueTime = 0;
	}
	FrequentTask(bool(*t)(ParameterStruct*), ParameterStruct *param, float dt) {
		task = t;
		this->parameter = param;
		deltaTime = dt;
		this->dueTime = micros() + (unsigned long)(deltaTime * sTOmics);
	}
	bool execute() override {
		if (task != NULL) {
			this->dueTime += deltaTime;
			return task(this->parameter);
		}
		else return false;
	}
};


typedef enum AnimationCurve {
	Linear,
	Squared,
	Logistic,
	GaussBlink,
	Loop,
	PingPong
} AnimationCurve;
typedef enum Type
{
	VOID,
	INT,
	FLOAT,
	COLOR
} Type;

template<typename ParameterStruct>
class AnimationTask : public ParameterTask<ParameterStruct> {
private:
	void(*task)(void*, ParameterStruct*);
	//unsigned long lastExecuted;
	//unsigned long endTimeMicros;
	void *startValue, *endValue, *actValue;
public:
	Type valueType;
	float duration;
	float timeDone;
	AnimationCurve curve;
	void* getActValue() {
		float factor = timeDone / duration;
		if (valueType == INT) {
			int start = *static_cast<int*>(startValue);
			int end = *static_cast<int*>(startValue);
			int act = start + (int)((end - start) * factor);
			int* result = new int(act);
			//(*result) = act;
			return result;
		}
		else if (valueType == FLOAT) {
			float start = *static_cast<float*>(startValue);
			float end = *static_cast<float*>(startValue);
			float act = start + ((end - start) * factor);
			float *result = new float(act);
			//(*result) = act;
			return result;
		}
		else if (valueType == COLOR) {
			Color cstart = *static_cast<Color*>(startValue);
			Color cend = *static_cast<Color*>(startValue);
			Color cact = (cstart + cend * factor) - cstart * factor;
			Color *cresult = new Color(cact);
			return cresult;
		}
		else {
			return startValue;
		}
	}
public:
	AnimationTask() {
		task = NULL;
		this->parameter = new ParameterStruct();
		this->curve = Linear;
		startValue = endValue = actValue = NULL;
		duration = timeDone = 0.f;
		this->dueTime = micros();
	}
	AnimationTask(void(*t)(void*, ParameterStruct*), ParameterStruct *param, Type type, void *start, void *end, float dur, AnimationCurve c, unsigned long time) {
		this->valueType = type;
		this->task = t;
		this->parameter = param;
		this->curve = c;
		this->startValue = start;
		this->actValue = start;
		this->endValue = end;
		this->duration = dur;
		//this.endTimeMicros = time + (unsigned long)(duration * sTOmics);
		this->timeDone = 0;
		this->dueTime = micros() + (unsigned long)( duration * sTOmics );
		//this.lastExecuted = time;
	}
	//returns whether animation iscompleted
	bool execute(float deltaTime) override {
		timeDone += deltaTime;
		 
		if (timeDone > duration) {
			if (this->curve != Loop) {
				task(endValue, this->parameter);
				return true;
			}
			else {
				timeDone = Math::modf(this->timeDone, duration);
			}
		}
		actValue = getActValue();
		task(actValue, this->parameter);
		return false;
	}
};
template<typename ParameterStruct>
class FloatAnimationTask : public AnimationTask<ParameterStruct> {
private:
	void(*task)(float, ParameterStruct*);
	AnimationCurve cuve;
	//unsigned long lastExecuted;
	//unsigned long endTimeMicros;
	float startValue, endValue, actValue;
	//float duration;
	//float timeDone;
	float getActValue() {
		switch (this->curve)
		{
		case Linear:
			actValue = this->timeDone / this->duration * (endValue - startValue);
			break;

		default:
			actValue = this->timeDone / this->duration * (endValue - startValue);
			break;
		}
	}
public:
	FloatAnimationTask() {
		task = NULL;
		this->parameter = ParameterStruct();
		AnimationCurve curve = Linear;
		startValue = endValue = actValue = 0.f;
		this->duration = this->timeDone = 0.f;
	}

	FloatAnimationTask(void(*t)(float, ParameterStruct*), ParameterStruct *param, void *start, void *end, float dur, AnimationCurve c, unsigned long time) {
		task = t;
		this->parameter = param;
		this.curve = c;
		startValue = *static_cast<float*>(start);
		actValue = startValue;
		endValue = *static_cast<float*>(end);
		this->duration = dur;
		//this.endTimeMicros = time + (unsigned long)(duration * sTOmics);
		this->timeDone = 0;
		//this.lastExecuted = time;
	}
	FloatAnimationTask(void(*t)(float, ParameterStruct*), ParameterStruct *param, float start, float end, float dur, AnimationCurve c, unsigned long time) {
		task = t;
		this->parameter = param;
		this->curve = c;
		startValue = start;
		actValue = start;
		endValue = end;
		this->duration = dur;
		//this.endTimeMicros = time + (unsigned long)(duration * sTOmics);
		this->timeDone = 0;
		//this.lastExecuted = time;
	}
	//returns whether animation iscompleted
	bool execute(float deltaTime) override {
		this->timeDone += deltaTime;
		if (this->timeDone > this->duration) {
			task(endValue, this->parameter);
			return false;
		}
		//float deltaTime = (float)(time - lastExecuted) * micsTOs;
		actValue = getActValue();
		task(actValue, this->parameter);
		return true;
	}
};

template<typename TaskParam, typename AnimParam>
class DispatchQueueClass
{
 protected:

	LinkedList<ParameterTask<TaskParam>> TaskQueue;
	LinkedList<ParameterTask<AnimParam>> AnimationQueue;
	ListNode<ParameterTask<AnimParam>> *firstDoneThisFrame = NULL;
	unsigned long nexTaskDueTime = 0;
	unsigned long nextAnimationDueTime = 0;
	unsigned long lastTimeAnimated = 0;
	unsigned long animationDeltaTime;
	//void(*finishAnimationTask)();
 public:
	
	 DispatchQueueClass();

	 DispatchQueueClass(float fps/*, void (*finishAnimTask)()*/);/* : TaskQueue(), AnimationQueue() {

	}*/

	//void init();
	//bool sortforDueTime(ParameterTask<TaskParam> left, ParameterTask<TaskParam> right);
	void doTaskInSec(bool(*task)(TaskParam*), TaskParam *param, float secondsTillExecute);
	
	void addTask(bool(*task)(TaskParam*), TaskParam *param);

	void addRepeatingTask(bool(*task)(TaskParam*), TaskParam *param, unsigned long deltaTime);

	void addAnimationTask(void(*task)(float, AnimParam*), AnimParam *param, float minValue, float maxValue, float duration, AnimationCurve c = Linear);

	void animate();

	void animate(float deltaTime);

	unsigned long tryNextTask(unsigned long time);

	void Loop();
};

//extern DispatchQueueClass TimerQueue;


template<typename TaskParam, typename AnimParam>
DispatchQueueClass<TaskParam, AnimParam>::DispatchQueueClass()
{
	TaskQueue = LinkedList<ParameterTask<TaskParam>>();
	AnimationQueue = LinkedList<ParameterTask<AnimParam>>();
	animationDeltaTime = (unsigned long)((1 / 60) * sTOmics);
	//DispatchQueueClass<TaskParam, AnimParam>(60);
}

template<typename TaskParam, typename AnimParam>
DispatchQueueClass<TaskParam, AnimParam>::DispatchQueueClass(float fps/*, void(*finishAnimTask)()*/) {
	//finishAnimationTask = finishAnimTask;
	TaskQueue		= LinkedList<ParameterTask<TaskParam>>();
	AnimationQueue	= LinkedList<ParameterTask<AnimParam>>();
	animationDeltaTime = (unsigned long)( (1 / fps) * sTOmics );
}
/*
template<typename TaskParam, typename AnimParam>
void DispatchQueueClass<TaskParam, AnimParam>::init()
{
	TaskQueue = LinkedList<TimedTask<TaskParam>>();
	AnimationQueue = LinkedList<AnimationTask<AnimParam>>();

}*/

template<typename TaskParam, typename AnimParam>
void DispatchQueueClass<TaskParam, AnimParam>::addTask(bool(*task)(TaskParam*), TaskParam *param)
{
	TimedTask<TaskParam> newTask = TimedTask<TaskParam>(task, param, micros());
	AnimationQueue.addBack(newTask);

}
template<typename TaskParam, typename AnimParam>
void DispatchQueueClass<TaskParam, AnimParam>::addRepeatingTask(bool(*task)(TaskParam*), TaskParam *param, unsigned long deltaTime)
{
	FrequentTask<TaskParam> newTask = FrequentTask<TaskParam>(task, param, deltaTime);
	AnimationQueue.addBack(newTask);
}

static bool sortforDueTime(Task left, Task right) {
	return left.dueTime < right.dueTime;
}
template<typename TaskParam, typename AnimParam>
void DispatchQueueClass<TaskParam, AnimParam>::doTaskInSec(bool(*task)(TaskParam*), TaskParam *param, float secondsTillExecute)
{
	TimedTask<TaskParam> newTimedTask = TimedTask<TaskParam>(task, param, secondsTillExecute);

	AnimationQueue.sortIn(newTimedTask, &sortforDueTime);
}

template<typename TaskParam, typename AnimParam>
void DispatchQueueClass<TaskParam, AnimParam>::addAnimationTask(void(*task)(float, AnimParam*), AnimParam *param, float minValue, float maxValue, float duration, AnimationCurve c = Linear)
{
	FloatAnimationTask<AnimParam> newAnimationTask = FloatAnimationTask<AnimParam>(task, param, minValue, maxValue, duration, c, micros());
	AnimationQueue.addBack(newAnimationTask);
}

template<typename TaskParam, typename AnimParam>
void DispatchQueueClass<TaskParam, AnimParam>::animate()
{
	float deltaTime = (float)(micros() - lastTimeAnimated) * micsTOs;
	this->animate(deltaTime);
}
template<typename TaskParam, typename AnimParam>
void DispatchQueueClass<TaskParam, AnimParam>::animate(float deltaTime)
{
	if (AnimationQueue.size() > 0) {
		
		while (AnimationQueue.getRoot() != firstDoneThisFrame) {
			ParameterTask<AnimParam> task = AnimationQueue.front();
			bool doAgain = task.execute(deltaTime);
			if (!doAgain) {
				AnimationQueue.cycleThrough();
			}
			else {
				AnimationQueue.removeFront();
			}
		}
		//finishAnimationTask();
	}
}

template<typename TaskParam, typename AnimParam>
unsigned long DispatchQueueClass<TaskParam, AnimParam>::tryNextTask(unsigned long time/* = micros()*/) {
	if (TaskQueue.size() > 0) {
		ParameterTask<TaskParam> topTask = TaskQueue.front();
		if (topTask.dueTime <= time) {
			bool doAgain = topTask.execute();
			if (doAgain) {
				TaskQueue.sortFrontIn(&sortforDueTime);
			}
			else {
				TaskQueue.removeFront();
			}
		}
		return topTask.dueTime;
	}
	else return 0.f;
}

template<typename TaskParam, typename AnimParam>
void DispatchQueueClass<TaskParam, AnimParam>::Loop() {
	unsigned long time;
	while (true) {
		time = micros();
		if (time > nextAnimationDueTime) {
			float actualDeltaTime = (float)(micros() - lastTimeAnimated) * micsTOs;
			lastTimeAnimated = time;
			nextAnimationDueTime += animationDeltaTime;
			animate(actualDeltaTime);
		}
		else if (time > nexTaskDueTime) {
			nexTaskDueTime = tryNextTask(time);
		}
	}
}


#endif