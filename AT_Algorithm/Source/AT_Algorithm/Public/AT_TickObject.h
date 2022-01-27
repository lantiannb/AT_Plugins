// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Tickable.h>
#include "AT_TickObject.generated.h"

/**
 * 
 */
UCLASS()
class AT_ALGORITHM_API UAT_TickObject : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	UAT_TickObject();
	virtual ~UAT_TickObject();

	// Begin FTickableGameObject Interface.
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void TickEvent(float DeltaTime);

};