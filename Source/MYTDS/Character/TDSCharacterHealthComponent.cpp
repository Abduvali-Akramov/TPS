#include "TDSCharacterHealthComponent.h"

void UTDSCharacterHealthComponent::ChangeHealthValue(float ChangeValue)
{
	float CurrentDamage = ChangeValue * CoefDamage;

	if (Shield > 0.0f && ChangeValue < 0.0f)
	{
		ChangeShieldValue(ChangeValue);
		
		if (Shield < 0.0f)
		{
			//FX
			//UE_LOG(LogTemp, Warning, TEXT("UTPSCharacterHealthComponent::ChangeHealthValue - Sheild < 0"));
		}
	}
	else
	{
		Super::ChangeHealthValue(ChangeValue);
	}
}

float UTDSCharacterHealthComponent::GetCurrentShield()
{
	return Shield;
}

void UTDSCharacterHealthComponent::ChangeShieldValue(float ChangeValue)
{
	Shield += ChangeValue;

	if (Shield > 100.0f)
	{
		Shield = 100.0f;
	}
	else
	{
		if(Shield < 0.0f)
			Shield = 0.0f;
	}

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_CollDownShieldTimer, this, &UTDSCharacterHealthComponent::CoolDownShieldEnd, CoolDownShieldRecoverTime, false);

		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldRecoveryRateTimer);
	}	

	OnShieldChange.Broadcast(Shield, ChangeValue);
}

void UTDSCharacterHealthComponent::CoolDownShieldEnd()
{
	if (GetWorld())
	{		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShieldRecoveryRateTimer, this, &UTDSCharacterHealthComponent::RecoveryShield, ShieldRecoverRate, true);
	}	
}

void UTDSCharacterHealthComponent::RecoveryShield()
{
	float tmp = Shield;
	tmp = tmp + ShieldRecoverValue;
	if (tmp > 100.0f)
	{
		Shield = 100.0f;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldRecoveryRateTimer);
		}
	}
	else
	{
		Shield = tmp;
	}

	OnShieldChange.Broadcast(Shield, ShieldRecoverValue);
}