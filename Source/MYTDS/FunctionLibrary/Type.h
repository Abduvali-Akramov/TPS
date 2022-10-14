#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Type.generated.h"

UENUM(BlueprintType)
enum class EmovementState : uint8
{
	Aim_State UMETA(DisplayName = "Aim State"),
	AimWalk_State UMETA(DisplayName = "AimWalk State"),
	Walk_State UMETA(DisplayName = "Walk State"),
	Run_State UMETA(DisplayName = "Run State"),
	SprintRun_State UMETA(DisplayName = "SprintRun State")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	RifleType UMETA(DisplayName = "Rifle"),
	ShotGunType UMETA(DisplayName = "ShotGun"),
	SniperRifle UMETA(DisplayName = "SniperRifle"),
	Pistol UMETA(DisplayName = "Pistol"),
	GrenadeLauncher UMETA(DisplayName = "GrenadeLauncher"),
	RocketLauncher UMETA(DisplayName = "RocketLauncher")
};

USTRUCT(BlueprintType)
struct FCharacterSpeed
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AimSpeedNormal = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeedNormal = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunSpeedNormal = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AimSpeedWalk = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintRunSpeedRun = 800.0f;
};

USTRUCT(BlueprintType)
struct FProjectileInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
	TSubclassOf<class AProjectileDefault> Projectile = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
	UStaticMesh* ProjectileStaticMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
	FTransform ProjectileStaticMeshOffset = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
	UParticleSystem* ProjectileTrailFX = nullptr; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
	FTransform ProjectileTrailFixOffset = FTransform();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
	float ProjectileDamage = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
	float ProjectileLifeTime = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
	float ProjectileInitSpeed = 2000.0f;

	//material to decal on hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
	TMap<TEnumAsByte<EPhysicalSurface>, UMaterialInterface*> HitDecals;
	//Sound when hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
	USoundBase* HitSound = nullptr;
	//fx when hit check by surface
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	TMap<TEnumAsByte<EPhysicalSurface>, UParticleSystem*> HitFXs;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explode")
	UParticleSystem* ExploseFX = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explode")
	USoundBase* ExploseSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explode")
	float ProjectileMaxRadiusDamage = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explode")
	float ProjectileMinRadiusDamage = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explode")
	float ExplodeMaxDamage = 40.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explode")
	float ExplodeFallofCoef = 1.0f;
	//Timer add
};

USTRUCT(BlueprintType)
struct FWeaponDispersion
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
	float Aim_StateDispersionAimMax = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
	float Aim_StateDispersionAimMin = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
	float Aim_StateDispersionAimRecoil = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
	float Aim_StateDispersionReduction = .3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
	float AimWalk_StateDispersionAimMax = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
	float AimWalk_StateDispersionAimMin = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
	float AimWalk_StateDispersionAimRecoil = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
	float AimWalk_StateDispersionReduction = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
	float Walk_StateDispersionAimMax = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
	float Walk_StateDispersionAimMin = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
	float Walk_StateDispersionAimRecoil = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
	float Walk_StateDispersionReduction = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
	float Run_StateDispersionAimMax = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
	float Run_StateDispersionAimMin = 4.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
	float Run_StateDispersionAimRecoil = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
	float Run_StateDispersionReduction = 0.1f;
};

USTRUCT(BlueprintType)
struct FAnimationWeaponInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Char")
	UAnimMontage* AnimCharFire = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Char")
	UAnimMontage* AnimCharFireAim = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Char")
	UAnimMontage* AnimCharReload = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Char")
	UAnimMontage* AnimCharReloadAim = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Weapon")
	UAnimMontage* AnimWeaponReload = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Weapon")
	UAnimMontage* AnimWeaponReloadAim = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Weapon")
	UAnimMontage* AnimWeaponFire = nullptr;
};

USTRUCT(BlueprintType)
struct FDropMeshInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropMesh")
	UStaticMesh* DropMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropMesh")
	float DropMeshTime = -1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropMesh")
	float DropMeshLifeTime = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropMesh")
	FTransform DropMeshOffset = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropMesh")
	FVector DropMeshImpulseDir = FVector(0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropMesh")
	float PowerImpulse = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropMesh")
	float ImpulseRandomDispersion = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropMesh")
	float CustomMass = 0.0f;
};

USTRUCT(BlueprintType)
struct FWeaponInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
	TSubclassOf<class AWeaponDefault> WeaponClass = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float RateOfFire = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float ReloadTime = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int32 MaxRound = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int32 NumberProjectileByShot = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion ")
	FWeaponDispersion DispersionWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound ")
	USoundBase* SoundFireWeapon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound ")
	USoundBase* SoundReloadWeapon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX ")
	UParticleSystem* EffectFireWeapon = nullptr;
	//if null use trace logic (TSubclassOf<class AProjectileDefault> Projectile = nullptr), use projectile setting damage, FX and other for trace logic
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile ")
	FProjectileInfo ProjectileSetting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace ")
	float DistacneTrace = 2000.0f;
	//one decal on all?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitEffect ")
	UDecalComponent* DecalOnHit = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim ")
	FAnimationWeaponInfo AnimWeaponInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh ")
	FDropMeshInfo ClipDropMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh ")
	FDropMeshInfo ShellBullets;

	//inv
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory ")
	float SwitchTimeToWeapon = 1.0f;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory ")
	UTexture2D* WeaponIcon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory ")
	EWeaponType WeaponType = EWeaponType::RifleType;
};

USTRUCT(BlueprintType)
struct FAdditionalWeaponInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	int32 Round = 0;
};

USTRUCT(BlueprintType)
struct FWeaponSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSlot")
	FName NameItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSlot")
	FAdditionalWeaponInfo AdditionalInfo;
};

USTRUCT(BlueprintType)
struct FAmmoSlot
{
	GENERATED_BODY()

	///Index Slot by Index Array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmmoSlot")
	EWeaponType WeaponType = EWeaponType::RifleType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmmoSlot")
	int32 Cout = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmmoSlot")
	int32 MaxCout = 100;
};

USTRUCT(BlueprintType)
struct FDropItem : public FTableRowBase
{
	GENERATED_BODY()

	///Index Slot by Index Array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropWeapon")
	UStaticMesh* WeaponStaticMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropWeapon")
	USkeletalMesh* WeaponSkeletMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropWeapon")
	FWeaponSlot WeaponInfo;
};

UCLASS()
class MYTDS_API UTypes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};