// Fill out your copyright notice in the Description page of Project Settings.


#include "KartCountDown.h"

#include "Components/TextBlock.h"
#include "KartGameMode.h"
#include "TimerManager.h"

void UKartCountDown::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetVisibility(ESlateVisibility::Collapsed);
	
	if (AKartGameMode* GameMode = GetWorld()->GetAuthGameMode<AKartGameMode>())
	{
		GameMode->OnCountdownTick.AddUniqueDynamic(this, &UKartCountDown::HandleCountDownTick);
		
		if (!GameMode->IsRaceStarted() && GameMode->GetCountdownNumber() > 0)
		{
			HandleCountDownTick(GameMode->GetCountdownNumber());
		}
	}
}

void UKartCountDown::NativeDestruct()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(HideTimer);
		
		if (AKartGameMode* GameMode = World->GetAuthGameMode<AKartGameMode>())
		{
			GameMode->OnCountdownTick.RemoveDynamic(this, &UKartCountDown::HandleCountDownTick);
		}
		
	}
	
	Super::NativeDestruct();
}

void UKartCountDown::HandleCountDownTick(int32 Number)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	GetWorld()->GetTimerManager().ClearTimer(HideTimer);
	
	if (Number > 0)
	{
		CountDownText->SetText(FText::AsNumber(Number));
	}
	else
	{
		CountDownText->SetText(GoText);
		GetWorld()->GetTimerManager().SetTimer(HideTimer, this, &UKartCountDown::HideCountDown, FMath::Max(GoDisplayTime,0.01f), false);
	}
	
	OnCountDownNumberShown(Number);
}

void UKartCountDown::HideCountDown()
{
	//Créé et présent mais non visible
	SetVisibility(ESlateVisibility::Collapsed);
}