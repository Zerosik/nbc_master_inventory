// Fill out your copyright notice in the Description page of Project Settings.


#include "InvGameMode.h"
#include "MyCharacter.h"

AInvGameMode::AInvGameMode()
{
	DefaultPawnClass = AMyCharacter::StaticClass();
}
