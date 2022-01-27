// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_AsyncActor.h"

// Sets default values
AAT_AsyncActor::AAT_AsyncActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAT_AsyncActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAT_AsyncActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAT_AsyncActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	this->ClearAllAsyncEvent();
}

bool AAT_AsyncActor::CreateAsyncBindEventThread(const FString& ThreadNameIn, const FOnAsyncEvent2& BindEventInst, bool bOnceIn, float DelayTimeIn) {
	TSharedPtr<FOnAsyncEvent2> ExeEvent(new FOnAsyncEvent2(BindEventInst));
	return AT_AsyncBindEvent::CreateAsyncBindEventThread(ThreadNameIn, [ExeEvent]() {ExeEvent->ExecuteIfBound(); }, bOnceIn, DelayTimeIn);
}

bool AAT_AsyncActor::CreateAsyncBindEventThread(const FString& ThreadNameIn, TFunction<void()> FunInstance, bool bOnceIn, float DelayTimeIn) {
	return AT_AsyncBindEvent::CreateAsyncBindEventThread(ThreadNameIn, FunInstance, bOnceIn, DelayTimeIn);
}

bool AAT_AsyncActor::SetThreadState(const FString& ThreadNameIn, ThreadState NewThreadState) {
	return AT_AsyncBindEvent::SetThreadState(ThreadNameIn, NewThreadState);
}

void AAT_AsyncActor::ClearAllAsyncEvent() {

	TSet<FString> Keys;
	AT_AsyncBindEvent::ThreadMap.GetKeys(Keys);
	for (auto K : Keys) {
		AT_AsyncBindEvent::SetThreadState(K, ThreadState::eClose);
	}
}

bool AAT_AsyncActor::AsyncEventIsValid(FString ThreadNameIn) {
	return AT_AsyncBindEvent::ThreadMap.Find(ThreadNameIn) != nullptr;
}
