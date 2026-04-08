// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroBossHUDWidget.h"

void UHeroBossHUDWidget::InitializeBossHUD(FHeroBossAttributes InAttributes)
{
	BossData = InAttributes;
}

void UHeroBossHUDWidget::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	if (MaxHealth <= 0.f) return;

	TargetHealthPercent = FMath::Clamp(CurrentHealth / MaxHealth, 0.f, 1.f);
	OnBossDamaged();
}

void UHeroBossHUDWidget::UpdatePower(float CurrentPower, float MaxPower)
{
	if (MaxPower <= 0.f) return;

	TargetPowerPercent = FMath::Clamp(CurrentPower / MaxPower, 0.f, 1.f);
}
