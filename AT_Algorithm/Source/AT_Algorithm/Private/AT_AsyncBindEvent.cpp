// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_AsyncBindEvent.h"
#include "HAL/RunnableThread.h"

TMap<FString, TSharedPtr<AT_AsyncBindEvent>> AT_AsyncBindEvent::ThreadMap = TMap<FString, TSharedPtr<AT_AsyncBindEvent>>();

AT_AsyncBindEvent::AT_AsyncBindEvent(const FString &ThreadNameIn, TFunction<void()> FunInstance, bool bDoOnceIn, float LoopDelayTimeIn):
	ThreadName(ThreadNameIn), FunPtr(FunInstance), bDoOnce(bDoOnceIn), LoopDelayTime(LoopDelayTimeIn){
	this->ThreadEvent = FPlatformProcess::GetSynchEventFromPool();
	this->AsyncThreadInst = FRunnableThread::Create(this, *ThreadName, 0, TPri_BelowNormal);
	UE_LOG(LogTemp, Warning, TEXT("Thread ID:%d"), this->AsyncThreadInst->GetThreadID());
}

AT_AsyncBindEvent::~AT_AsyncBindEvent(){
	if (ThreadEvent)	// 清空 FEvent*
	{
		FPlatformProcess::ReturnSynchEventToPool(ThreadEvent); // delete ThreadEvent;
		ThreadEvent = nullptr;
	}
	if (AsyncThreadInst)		// 清空 FRunnableThread*
	{
		delete AsyncThreadInst;
		AsyncThreadInst = nullptr;
	}
}

bool AT_AsyncBindEvent::Init(){ return true; }

uint32 AT_AsyncBindEvent::Run(){
	FPlatformProcess::Sleep(0.03f);
	if (this->bDoOnce) {
		if (this->FunPtr)
			this->FunPtr();
	}
	else {
		int count = 0;
		while (this->AsyncThreadState == ThreadState::eRun) {
			if (this->FunPtr)
				this->FunPtr();
			UE_LOG(LogTemp, Warning, TEXT("ThreadID: %d, Count: %d"), AsyncThreadInst->GetThreadID(), ++count);

			FPlatformProcess::Sleep(this->LoopDelayTime);
			if (this->AsyncThreadState == ThreadState::ePause)
				this->ThreadEvent->Wait();

		}
	}
	//this->AsyncThreadState = ThreadState::eClose;
	UE_LOG(LogTemp, Warning, TEXT("RunEnd %s!"), *this->ThreadName);

	return 0;
}

void AT_AsyncBindEvent::Stop(){}

void AT_AsyncBindEvent::Exit(){
	UE_LOG(LogTemp, Warning, TEXT("Thread:%s is Exit"), *ThreadName);	
}

void AT_AsyncBindEvent::PauseThread(){
	if (this->AsyncThreadState != ThreadState::eClose)
		this->AsyncThreadState = ThreadState::ePause;
}

void AT_AsyncBindEvent::WakeUpThread(){
	if(this->AsyncThreadState != ThreadState::eClose)
		this->AsyncThreadState = ThreadState::eRun;
	ThreadEvent->Trigger();		// 唤醒

}

void AT_AsyncBindEvent::CloseThread(){
	if (this->AsyncThreadState == ThreadState::ePause) {
		this->AsyncThreadState = ThreadState::eClose;
		this->WakeUpThread();
	}
	else {
		this->AsyncThreadState = ThreadState::eClose;
	}

	this->AsyncThreadInst->WaitForCompletion();
}


bool AT_AsyncBindEvent::CreateAsyncBindEventThread(const FString& ThreadNameIn, TFunction<void()> FunInstance, bool bOnceIn, float DelayTimeIn) {
	//判断Runnable是否存在以及是否支持多线程
	if (FPlatformProcess::SupportsMultithreading() && (!AT_AsyncBindEvent::ThreadMap.Find(ThreadNameIn))){
		TSharedPtr<AT_AsyncBindEvent> AT_AsyncPtr = MakeShareable(new AT_AsyncBindEvent(ThreadNameIn, FunInstance, bOnceIn, DelayTimeIn));
		//将FRunnable实例存入ThreadMap中
		AT_AsyncBindEvent::ThreadMap.Add(ThreadNameIn, AT_AsyncPtr);
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("异步线程名字冲突，原\"%s\"线程正在等待或运行!"), *ThreadNameIn);
	return false;
}



bool AT_AsyncBindEvent::SetThreadState(const FString& ThreadNameIn, ThreadState NewThreadState){
	
	
	if (AT_AsyncBindEvent::ThreadMap.Contains(ThreadNameIn)) {
		auto TempAsyncThread = AT_AsyncBindEvent::ThreadMap[ThreadNameIn];
		if (TempAsyncThread && TempAsyncThread.Get()) {
			
			/*TempAsyncThread->Get()->AsyncThreadState = NewThreadState;*/
			if (NewThreadState != TempAsyncThread->AsyncThreadState) {
				if (NewThreadState == ThreadState::eClose) {
					TempAsyncThread->CloseThread();
					TempAsyncThread->CloseThread();
					AT_AsyncBindEvent::ThreadMap.Remove(ThreadNameIn);
				}
				else if (NewThreadState == ThreadState::ePause)
					TempAsyncThread->PauseThread();
				else if (NewThreadState == ThreadState::eRun)
					TempAsyncThread->WakeUpThread();
			}
		}
		else {
			AT_AsyncBindEvent::ThreadMap.Remove(ThreadNameIn);
		}
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("没有找到\"%s\"线程，因此设置线程状态失败!"), *ThreadNameIn);
	return false;
}
