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
    //table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponSetting")
	UDataTable* WeaponInfoTable = nullptr;
	
	UFUNCTION(BlueprintCallable)
	bool GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo);
};