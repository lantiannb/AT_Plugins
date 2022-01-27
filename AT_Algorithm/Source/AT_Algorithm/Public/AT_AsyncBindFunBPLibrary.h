// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AT_AsyncBindEvent.h"
#include "AT_AsyncBindFunBPLibrary.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE(FOnAsyncEvent);
UCLASS()
class AT_ALGORITHM_API UAT_AsyncBindFunBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	~UAT_AsyncBindFunBPLibrary();

public:
	

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "创建异步绑定事件", Keywords = "CreateAsyncBindEventThread"), Category = "AT_Async")
		static bool CreateAsyncBindEventThread(const FString& ThreadNameIn, const FOnAsyncEvent& BindEventInst, bool bOnceIn = true, float DelayTimeIn = 0.1f);
	// CreateAsyncBindEventThread重载，C++中调用
	static bool CreateAsyncBindEventThread(const FString& ThreadNameIn, TFunction<void()> FunInstance, bool bOnceIn = true, float DelayTimeIn = 0.1f);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "设置异步绑定事件状态", Keywords = "SetThreadState"), Category = "AT_Async")
		static bool SetThreadState(const FString& ThreadNameIn, ThreadState NewThreadState);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "清除所有异步事件", Keywords = "ClearAllAsyncEvent"), Category = "AT_Async")
		static void ClearAllAsyncEvent();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "异步事件已存在", Keywords = "AsyncEventIsValid"), Category = "AT_Async")
		static bool AsyncEventIsValid(FString ThreadNameIn);
		
};
