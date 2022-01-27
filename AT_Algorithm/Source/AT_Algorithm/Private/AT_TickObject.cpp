// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_TickObject.h"

UAT_TickObject::UAT_TickObject()
{
}


UAT_TickObject::~UAT_TickObject()
{
}

void UAT_TickObject::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);	
	TickEvent(DeltaTime);
}



bool UAT_TickObject::IsTickable() const
{
	return true;
}
TStatId UAT_TickObject::GetStatId() const
{
	return Super::GetStatID();
}

void UAT_TickObject::TickEvent_Implementation(float DeltaTime)
{
}