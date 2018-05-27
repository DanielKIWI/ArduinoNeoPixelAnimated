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
#define sTOmics 1e6
#define micsTOs 1e-6

class Task {
public:
	unsigned long dueTime;
	virtual bool execute() { Serial.println("Wrong function"); return true; }
	virtual bool execute(float deltaTime) {
		return execute();
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

class AnimationTask {
public:
	bool isDone;
	virtual bool execute(float deltaTime) {
		return true;
	}
};

class DispatchQueueClass
{
private:
	bool ledOn = false;
 protected:

	LinkedList<Task*> TaskQueue;
	LinkedList<AnimationTask*> AnimationQueue;
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
	//void doTaskInSec(Task *task);
	
	void addTask(Task *task);

	//void addRepeatingTask(Task *task);

	void addAnimationTask(AnimationTask *animation);

	void animate();

	void animate(float deltaTime);

	unsigned long tryNextTask(unsigned long time);

	void Loop();
};

//extern DispatchQueueClass TimerQueue;


DispatchQueueClass::DispatchQueueClass()
{
	TaskQueue = LinkedList<Task*>();
	AnimationQueue = LinkedList<AnimationTask*>();
	animationDeltaTime = (unsigned long)((1 / (float)60) * sTOmics);
	//DispatchQueueClass(60);
}


DispatchQueueClass::DispatchQueueClass(float fps/*, void(*finishAnimTask)()*/) {
	//finishAnimationTask = finishAnimTask;
	TaskQueue = LinkedList<Task*>();
	AnimationQueue = LinkedList<AnimationTask*>();
	animationDeltaTime = (unsigned long)( (1 / fps) * sTOmics );
}
/*

void DispatchQueueClass::init()
{
	TaskQueue = LinkedList<TimedTask<TaskParam>>();
	AnimationQueue = LinkedList<AnimationTask<AnimParam>>();

}*/

void DispatchQueueClass::addTask(Task *task)
{
	TaskQueue.addBack(task);

}

static bool sortforDueTime(Task *left, Task *right) {
	return left->dueTime < right->dueTime;
}

void DispatchQueueClass::addAnimationTask(AnimationTask *animation) {
	AnimationQueue.addBack(animation);
}

void DispatchQueueClass::animate()
{
	float deltaTime = (float)(micros() - lastTimeAnimated) * micsTOs;
	this->animate(deltaTime);
}

void DispatchQueueClass::animate(float deltaTime)
{
	if (AnimationQueue.size() > 0) {
		Serial.print("Animate; deltaTime: ");
		Serial.print(deltaTime);
		Serial.print('\n');
		bool isfirstTask = true;
		for (int i = 0; i < AnimationQueue.size(); i++) {
			isfirstTask = false;
			AnimationTask *animation = AnimationQueue.front();
			bool doAgain = animation->execute(deltaTime);
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


unsigned long DispatchQueueClass::tryNextTask(unsigned long time/* = micros()*/) {
	//Serial.println("tryNextTask");
	if (TaskQueue.size() > 0) {
		Task *topTask = TaskQueue.front();
		/*Serial.print("dueTime = ");
		Serial.print((float)topTask->dueTime * micsTOs);
		Serial.print('\n');*/
		if (topTask->dueTime <= time) {
			bool doAgain = topTask->execute();
			if (doAgain) {
				TaskQueue.sortFrontIn(&sortforDueTime);
			}
			else {
				TaskQueue.removeFront();
			}
		}
		return topTask->dueTime;
	}
	else return 0.f;
}

void DispatchQueueClass::Loop() {
	digitalWrite(LED_BUILTIN, ledOn ? HIGH : LOW);
	ledOn = !ledOn;
	unsigned long time;
	while (true) {
		time = micros();
		/*Serial.print("Loop; time: ");
		Serial.print((float)time * micsTOs, 3);
		Serial.print(", nextTaskDueTime: ");
		Serial.print((float)nexTaskDueTime * micsTOs, 4);
		Serial.print('\n');*/
		
		if (time > nextAnimationDueTime) {
			float actualDeltaTime = (float)(micros() - lastTimeAnimated) * micsTOs;
			lastTimeAnimated = time;
			nextAnimationDueTime += animationDeltaTime;
			//animate(actualDeltaTime);
			//Serial.println("");
		}
		if (time > nexTaskDueTime) {
			nexTaskDueTime = tryNextTask(time);
		}
	}
}


#endif