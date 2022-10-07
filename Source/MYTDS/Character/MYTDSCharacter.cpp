#include "MYTDSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "MYTDS/Game/TDSGameInstance.h"

AMYTDSCharacter::AMYTDSCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 50.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMYTDSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentCursor)
	{
		APlayerController* myPC = Cast<APlayerController>(GetController());
		if (myPC)
		{
			FHitResult TraceHitResult;
			myPC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();

			CurrentCursor->SetWorldLocation(TraceHitResult.Location);
			CurrentCursor->SetWorldRotation(CursorR);
		}
	}

	MovementTick(DeltaSeconds);
}

void AMYTDSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMYTDSCharacter::InputAxisX);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMYTDSCharacter::InputAxisY);

	PlayerInputComponent->BindAction(TEXT("FireEvent"), EInputEvent::IE_Pressed, this, &AMYTDSCharacter::InputAttackPressed);
	PlayerInputComponent->BindAction(TEXT("FireEvent"), EInputEvent::IE_Released, this, &AMYTDSCharacter::InputAttackReleased);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Released, this, &AMYTDSCharacter::TryReloadWeapon);

}

void AMYTDSCharacter::InputAxisX(float Value)
{
	AxisX=Value;
	
}

void AMYTDSCharacter::InputAxisY(float Value)
{
	AxisY=Value;
}

void AMYTDSCharacter::InputAttackPressed()
{
	AttackCharEvent(true);
}

void AMYTDSCharacter::InputAttackReleased()
{
	AttackCharEvent(false);
}

void AMYTDSCharacter::MovementTick(float DeltaTime)
{
	AddMovementInput(FVector(1.0f,0.0f,0.0f), AxisX);
	AddMovementInput(FVector(0.0f,1.0f,0.0f), AxisY);

	if (MovementState == EmovementState::SprintRun_State)
	{
		FVector myRotationVector = FVector(AxisX, AxisY, 0.0f);
		FRotator myRotator = myRotationVector.ToOrientationRotator();
		SetActorRotation((FQuat(myRotator)));
	}
	else
	{
		APlayerController* myController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (myController)
		{
			FHitResult ResultHit;
			myController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);

			float FindRotaterResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
			SetActorRotation(FQuat(FRotator(0.0f, FindRotaterResultYaw, 0.0f)));
			
			if (CurrentWeapon)
			{
				FVector Displacement = FVector(0);
				switch (MovementState)
				{
				case EmovementState::Aim_State:
					Displacement = FVector(0.0f, 0.0f, 160.0f);
					break;
				case EmovementState::AimWalk_State:
					Displacement = FVector(0.0f, 0.0f, 160.0f);
					break;
				case EmovementState::Walk_State:
					Displacement = FVector(0.0f, 0.0f, 120.0f);
					break;
				case EmovementState::Run_State:
					Displacement = FVector(0.0f, 0.0f, 120.0f);
					break;
				case EmovementState::SprintRun_State:
					break;
				default:
					break;
				}

				CurrentWeapon->ShootEndLocation = ResultHit.Location + Displacement;
			}
		}
	}
	
	if (CurrentWeapon)
	{
		if (FMath::IsNearlyZero(GetVelocity().Size(), 0.5f))
			CurrentWeapon->ShouldReduceDispersion = true;
		else
		{
			CurrentWeapon->ShouldReduceDispersion = false;
		}
	}
	
}
void AMYTDSCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitWeapon(InitWeaponName);

	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}
}

void AMYTDSCharacter::CharacterUpdate()
{
	float ResSpeed = 600.0f;
	switch (MovementState)
	{
	case EmovementState::Aim_State:
		ResSpeed = MovementSpeedInfo.AimSpeedNormal;
		break;
	case EmovementState::AimWalk_State:
		ResSpeed = MovementSpeedInfo.AimSpeedWalk;
		break;
	case EmovementState::Walk_State:
		ResSpeed = MovementSpeedInfo.WalkSpeedNormal;
		break;
	case EmovementState::Run_State:
		ResSpeed = MovementSpeedInfo.RunSpeedNormal;
		break;
	case EmovementState::SprintRun_State:
		ResSpeed = MovementSpeedInfo.SprintRunSpeedRun;
		break;
	default:
		break;
	}

	GetCharacterMovement()->MaxWalkSpeed = ResSpeed;
}

void AMYTDSCharacter::ChangeMovementState()
{
	if (!WalkEnabled && !SprintRunEnabled && !AimEnabled)
	{
		MovementState = EmovementState::Run_State;
	}
	else
	{
		if (SprintRunEnabled)
		{
			WalkEnabled = false;
			AimEnabled = false;
			MovementState = EmovementState::SprintRun_State;
		}
		if (WalkEnabled && !SprintRunEnabled && AimEnabled)
		{
			MovementState = EmovementState::AimWalk_State;
		}
		else
		{
			if (WalkEnabled && !SprintRunEnabled && !AimEnabled)
			{
				MovementState = EmovementState::Walk_State;
			}
			else
			{
				if (!WalkEnabled && !SprintRunEnabled && AimEnabled)
				{
					MovementState = EmovementState::Aim_State;
				}
			}
		}
	}
	CharacterUpdate();

	//Weapon state update
	AWeaponDefault* myWeapon = GetCurrentWeapon();
	if (myWeapon)
	{
		myWeapon->UpdateStateWeapon(MovementState);
	}
}

UDecalComponent* AMYTDSCharacter::GetCursorToWorld()
{
	return CurrentCursor;
}

AWeaponDefault* AMYTDSCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

void AMYTDSCharacter::InitWeapon(FName IdWeapon)
{
	UTDSGameInstance* myGI = Cast<UTDSGameInstance>(GetGameInstance());
	FWeaponInfo myWeaponInfo;
	if (myGI)
	{
		if (myGI->GetWeaponInfoByName(IdWeapon, myWeaponInfo))
		{
			if (myWeaponInfo.WeaponClass)
			{
				FVector SpawnLocation = FVector(0);
				FRotator SpawnRotation = FRotator(0);

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = GetInstigator();

				AWeaponDefault* myWeapon = Cast<AWeaponDefault>(GetWorld()->SpawnActor(myWeaponInfo.WeaponClass, &SpawnLocation, &SpawnRotation, SpawnParams));
				if (myWeapon)
				{
					FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
					myWeapon->AttachToComponent(GetMesh(), Rule, FName("WeaponSocketRightHand"));
					CurrentWeapon = myWeapon;
					
					myWeapon->WeaponSetting= myWeaponInfo;
					myWeapon->WeaponInfo.Round = myWeaponInfo.MaxRound;

					myWeapon->ReloadTime = myWeaponInfo.ReloadTime;
					myWeapon->UpdateStateWeapon(MovementState);
				
					myWeapon->OnWeaponReloadStart.AddDynamic(this, &AMYTDSCharacter::WeaponReloadStart);
					myWeapon->OnWeaponReloadEnd.AddDynamic(this, &AMYTDSCharacter::WeaponReloadEnd);

					myWeapon->OnWeaponFireStart.AddDynamic(this, &AMYTDSCharacter::WeaponFireStart);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ATPSCharacter::InitWeapon - Weapon not found in table- NULL"))
		}
	}
}

void AMYTDSCharacter::TryReloadWeapon()
{
	if (CurrentWeapon)
	{
		if (CurrentWeapon->GetWeaponRound() <= CurrentWeapon->WeaponSetting.MaxRound)
			CurrentWeapon->InitReload();
	}
}

void AMYTDSCharacter::WeaponFireStart(UAnimMontage* Anim)
{
	
}

void AMYTDSCharacter::WeaponReloadStart(UAnimMontage* Anim)
{
	WeaponReloadStart_BP(Anim);
}

void AMYTDSCharacter::WeaponReloadEnd()
{
	WeaponReloadEnd_BP();
}

void AMYTDSCharacter::WeaponReloadStart_BP_Implementation(UAnimMontage* Anim)
{
	// in BP
}

void AMYTDSCharacter::WeaponReloadEnd_BP_Implementation()
{
	// in BP
}

void AMYTDSCharacter::AttackCharEvent(bool bIsFiring)
{
	AWeaponDefault* myWeapon = nullptr;
	myWeapon = GetCurrentWeapon();
	if (myWeapon)
	{
		//ToDo Check melee or range
		myWeapon->SetWeaponStateFire(bIsFiring);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("AMYTDSCharacter::AttackCharEvent - CurrentWeapon -NULL"));
}