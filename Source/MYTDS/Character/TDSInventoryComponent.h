#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MYTDS/FunctionLibrary/Type.h"
#include "TDSInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSwitchWeapon, FName, WeaponIdName, FAdditionalWeaponInfo, WeaponAdditionalInfo, int32, NewCurrentIndexWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChange, EWeaponType, TypeAmmo, int32, Cout);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponAdditionalInfoChange, int32, IndexSlot, FAdditionalWeaponInfo, AdditionalInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponAmmoEmpty, EWeaponType, WeaponType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponAmmoAviable, EWeaponType, WeaponType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateWeaponSlots, int32, IndexSlotChange, FWeaponSlot, NewInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponNotHaveRound, int32, IndexSlotWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponHaveRound, int32, IndexSlotWeapon);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTDS_API UTDSInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTDSInventoryComponent();
	
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FOnSwitchWeapon OnSwitchWeapon;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	//Event on change ammo in slots by weaponType
	FOnAmmoChange OnAmmoChange;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnWeaponAdditionalInfoChange OnWeaponAdditionalInfoChange;
	//Event Ammo slots after change still empty rounds
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnWeaponAmmoEmpty OnWeaponAmmoEmpty;
	//Event Ammo slots after chage have rounds
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnWeaponAmmoAviable OnWeaponAmmoAviable;
	//Event weapon was change by slotIndex
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnUpdateWeaponSlots OnUpdateWeaponSlots;

	//Event current weapon not have additional_Rounds 
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnWeaponNotHaveRound OnWeaponNotHaveRound;
	//Event current weapon have additional_Rounds 
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnWeaponHaveRound OnWeaponHaveRound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	TArray<FWeaponSlot> WeaponSlots;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	TArray<FAmmoSlot> AmmoSlots;

	int32 MaxSlotsWeapon = 0;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//TODO OMG Refactoring need!!!
	bool SwitchWeaponToIndex(int32 ChangeToIndex, int32 OldIndex, FAdditionalWeaponInfo OldInfo, bool bIsForward);

	FAdditionalWeaponInfo GetAdditionalInfoWeapon(int32 IndexWeapon);
	int32 GetWeaponIndexSlotByName(FName IdWeaponName);
	FName GetWeaponNameBySlotIndex(int32 indexSlot);
	void SetAdditionalInfoWeapon(int32 IndexWeapon, FAdditionalWeaponInfo NewInfo);

	UFUNCTION(BlueprintCallable)
	void AmmoSlotChangeValue(EWeaponType TypeWeapon, int32 CoutChangeAmmo);

	bool CheckAmmoForWeapon(EWeaponType TypeWeapon, int8 &AviableAmmForWeapon);

	//Interface PickUp Actors
	UFUNCTION(BlueprintCallable, Category = "Interface")
	bool CheckCanTakeAmmo(EWeaponType AmmoType);
	UFUNCTION(BlueprintCallable, Category = "Interface")
	bool CheckCanTakeWeapon(int32 &FreeSlot);
	UFUNCTION(BlueprintCallable, Category = "Interface")
	bool SwitchWeaponToInventory(FWeaponSlot NewWeapon, int32 IndexSlot, int32 CurrentIndexWeaponChar, FDropItem &DropItemInfo);
	UFUNCTION(BlueprintCallable, Category = "Interface")
	bool TryGetWeaponToInventory(FWeaponSlot NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Interface")
	bool GetDropItemInfoFromInventory(int32 IndexSlot, FDropItem &DropItemInfo);
};