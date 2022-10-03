// Copyright Epic Games, Inc. All Rights Reserved.

#include "MYTDSPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "MYTDS/Character/MYTDSCharacter.h"
#include "Engine/World.h"

AMYTDSPlayerController::AMYTDSPlayerController()
{
	bShowMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Default;	
}

void AMYTDSPlayerController::PlayerTick(float DeltaTime)
{
	
	Super::PlayerTick(DeltaTime);
	
}

void AMYTDSPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

}