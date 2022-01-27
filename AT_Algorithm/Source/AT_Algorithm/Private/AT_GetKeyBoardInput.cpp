// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_GetKeyBoardInput.h"
#include <conio.h>
#include "AT_AsyncBindFunBPLibrary.h"
#include <AT_AsyncActor.h>


UAT_GetKeyBoardInput::UAT_GetKeyBoardInput() {

}
UAT_GetKeyBoardInput::~UAT_GetKeyBoardInput()
{
	this->EndGetKeyBoardInput();
}

bool UAT_GetKeyBoardInput::StartGetKeyBoardInput()
{
	
	AAT_AsyncActor* AsyncActor;
	TSubclassOf<AAT_AsyncActor> AAT_AsyncActorClass;
	AsyncActor = GetWorld()->SpawnActor<AAT_AsyncActor>(AAT_AsyncActorClass);
	return AsyncActor->CreateAsyncBindEventThread(this->ThreadName, [=]() {
		while (IsRunning) {
			this->ClickKeyBoard.Broadcast(_getch());
		}
		});
	/*return UAT_AsyncBindFunBPLibrary::CreateAsyncBindEventThread(this->ThreadName, [=]() {
		while (IsRunning) {
			this->ClickKeyBoard.Broadcast(_getch());
		}
		});*/
}

void UAT_GetKeyBoardInput::EndGetKeyBoardInput(){
	if (IsRunning) {
		this->IsRunning = false;
		UAT_AsyncBindFunBPLibrary::SetThreadState(this->ThreadName, ThreadState::eClose);
	}
}
