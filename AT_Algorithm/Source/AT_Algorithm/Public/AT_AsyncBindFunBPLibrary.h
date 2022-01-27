// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AT_AsyncBindEvent.h"
#include "AT_AsyncBindFunBPLibrary.generated.h"


/*************************************************

Author:蓝天

Date:2022-1-27

Description:
异步绑定事件的静态调用
CreateAsyncBindEventThread函数有一个重载，公开到蓝图的则是采用lambda绑定事件去执行，而未公开的则是在c++中直接绑定函数调用
其余函数均有写注释标注作用

**************************************************/

DECLARE_DYNAMIC_DELEGATE(FOnAsyncEvent);
UCLASS()
class AT_ALGORITHM_API UAT_AsyncBindFunBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	~UAT_AsyncBindFunBPLibrary();

public:
	// 创建后必须要调用SetThreadState(ThreadName, eClose)销毁，否则请调用AT_AsyncActor, 但其需要创建才能使用，因此使用前需斟酌一下
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
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "获取所有存在的线程", Keywords = "GetAllAsyncThreadName"), Category = "AT_Async")
		static TArray<FString> GetAllAsyncThreadName();
};
