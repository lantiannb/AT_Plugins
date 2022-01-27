// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"



/*************************************************

Author:蓝天

Date:2022-1-27

Description:
创建线程类   

**************************************************/

UENUM(BlueprintType)
 enum class ThreadState : uint8 {
	 eRun,
	 ePause,
	 eClose
 };

class AT_ALGORITHM_API AT_AsyncBindEvent: public FRunnable
{
public:
	AT_AsyncBindEvent(const FString& ThreadNameIn, TFunction<void()> FunInstance, bool bDoOnceIn = true, float LoopDelayTimeIn = 0.1f);
	~AT_AsyncBindEvent();

public:
	ThreadState											AsyncThreadState = ThreadState::eRun;		// 线程状态
	FRunnableThread*								AsyncThreadInst;											// 线程实例
	FEvent*												ThreadEvent;													// 调用改变线程状态函数的指针
	FString												ThreadName;													// 线程名
	bool														bDoOnce;														// 只执行一次
	float														LoopDelayTime;												// 循环执行间隔

public:
	// 绑定的事件
	UPROPERTY()
		TFunction<void()>	                       FunPtr = nullptr;
public:
	static TMap<FString, TSharedPtr<AT_AsyncBindEvent>> ThreadMap;

/********************************Function****************************************/
public:
	virtual bool Init() override;			// 初始化线程				生成时自动执行		若返回true则自动调用Run
	virtual uint32 Run() override;		// 执行线程							
	virtual void Stop() override;		// 暂停线程					用处不大，这里没有实现		
	virtual void Exit() override;		// 退出 Run结束自动执行

public:
	void PauseThread();
	void WakeUpThread();
	void CloseThread();

public:
	// 创建一个线程，若创建的线程名已存在且正在执行则创建失败
	static bool CreateAsyncBindEventThread(const FString& ThreadNameIn, TFunction<void()> FunInstance, bool bOnceIn = true, float DelayTimeIn = 0.1f);
	// 设置正在执行的线程状态，若该线程名已执行结束或不存在则失败
	static bool SetThreadState(const FString& ThreadNameIn, ThreadState NewThreadState);
};
