#include "MYTDSGameMode.h"
#include "MYTDSPlayerController.h"
#include "MYTDS/Character/MYTDSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMYTDSGameMode::AMYTDSGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMYTDSPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Character/BP_Character"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprints/Environment/BP_PlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class; // Cast to Player Controller Class;
	}
}