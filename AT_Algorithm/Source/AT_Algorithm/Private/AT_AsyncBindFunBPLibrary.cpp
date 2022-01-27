// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_AsyncBindFunBPLibrary.h"



UAT_AsyncBindFunBPLibrary::~UAT_AsyncBindFunBPLibrary() {
	
}

bool UAT_AsyncBindFunBPLibrary::CreateAsyncBindEventThread(const FString& ThreadNameIn, const FOnAsyncEvent& BindEventInst, bool bOnceIn, float DelayTimeIn) {
	
	TSharedPtr<FOnAsyncEvent> ExeEvent(new FOnAsyncEvent (BindEventInst));
	return AT_AsyncBindEvent::CreateAsyncBindEventThread(ThreadNameIn, [ExeEvent]() {ExeEvent->ExecuteIfBound(); }, bOnceIn, DelayTimeIn);
}

bool UAT_AsyncBindFunBPLibrary::CreateAsyncBindEventThread(const FString& ThreadNameIn, TFunction<void()> FunInstance, bool bOnceIn, float DelayTimeIn){
	return AT_AsyncBindEvent::CreateAsyncBindEventThread(ThreadNameIn, FunInstance, bOnceIn, DelayTimeIn);
}

bool UAT_AsyncBindFunBPLibrary::SetThreadState(const FString& ThreadNameIn, ThreadState NewThreadState){
	return AT_AsyncBindEvent::SetThreadState(ThreadNameIn, NewThreadState);
}

void UAT_AsyncBindFunBPLibrary::ClearAllAsyncEvent(){

	TSet<FString> Keys;
	AT_AsyncBindEvent::ThreadMap.GetKeys(Keys);
	for (auto K : Keys) {
		AT_AsyncBindEvent::SetThreadState(K, ThreadState::eClose);
	}
}

bool UAT_AsyncBindFunBPLibrary::AsyncEventIsValid(FString ThreadNameIn){
	return AT_AsyncBindEvent::ThreadMap.Find(ThreadNameIn) != nullptr;
}

TArray<FString> UAT_AsyncBindFunBPLibrary::GetAllAsyncThreadName(){
	TSet<FString> Res;
	AT_AsyncBindEvent::ThreadMap.GetKeys(Res);
	return Res.Array();
}
