// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AT_AsyncBindEvent.h"
#include "AT_AsyncActor.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnAsyncEvent2);
UCLASS()
class AT_ALGORITHM_API AAT_AsyncActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAT_AsyncActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "创建异步绑定事件", Keywords = "CreateAsyncBindEventThread"), Category = "AT_Async")
		bool CreateAsyncBindEventThread(const FString& ThreadNameIn, const FOnAsyncEvent2& BindEventInst, bool bOnceIn = true, float DelayTimeIn = 0.1f);
	// CreateAsyncBindEventThread重载，C++中调用
	static bool CreateAsyncBindEventThread(const FString& ThreadNameIn, TFunction<void()> FunInstance, bool bOnceIn = true, float DelayTimeIn = 0.1f);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "设置异步绑定事件状态", Keywords = "SetThreadState"), Category = "AT_Async")
		bool SetThreadState(const FString& ThreadNameIn, ThreadState NewThreadState);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "清除所有异步事件", Keywords = "ClearAllAsyncEvent"), Category = "AT_Async")
		void ClearAllAsyncEvent();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "异步事件已存在", Keywords = "AsyncEventIsValid"), Category = "AT_Async")
		bool AsyncEventIsValid(FString ThreadNameIn);
};
