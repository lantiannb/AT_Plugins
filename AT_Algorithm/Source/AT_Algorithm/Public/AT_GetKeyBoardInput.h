// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AT_GetKeyBoardInput.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPressKeyBoard, int32, KeyBoardNum);
UCLASS(Blueprintable)
class AT_ALGORITHM_API UAT_GetKeyBoardInput : public UObject
{
	GENERATED_BODY()
public:
	bool IsRunning = true;
	FString ThreadName = "UAT_GetKeyBoardInputThread";
public:
	UAT_GetKeyBoardInput();
	~UAT_GetKeyBoardInput();
	UPROPERTY(BlueprintAssignable, Category = "Event")
		FPressKeyBoard ClickKeyBoard;
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "开始获取键盘输入", Keywords = "StartGetKeyBoardInput"), Category = "AT_GetKeyBoardInput")
		bool StartGetKeyBoardInput();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "停止获取键盘输入", Keywords = "EndGetKeyBoardInput"), Category = "AT_GetKeyBoardInput")
		void EndGetKeyBoardInput();
};
