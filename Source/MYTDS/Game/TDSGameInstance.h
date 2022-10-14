#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MYTDS/FunctionLibrary/Type.h"
#include "Engine/DataTable.h"
#include "Weapons/WeaponDefault.h"
#include "TDSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYTDS_API UTDSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " WeaponSetting ")
	UDataTable* WeaponInfoTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " WeaponSetting ")
	UDataTable* DropItemInfoTable = nullptr;
	UFUNCTION(BlueprintCallable)
	bool GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo);
	UFUNCTION(BlueprintCallable)
	bool GetDropItemInfoByName(FName NameItem, FDropItem& OutInfo);
};