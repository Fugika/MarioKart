// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/KartItemComponent.h"

#include "Engine/Engine.h"
#include "Item/KartItemData.h"
#include "Item/KartItemPoolData.h"
#include "Pawny.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UKartItemComponent::UKartItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

bool UKartItemComponent::TryGiveRandomItem(UKartItemPoolData* Pool)
{
	
	if (Pool == nullptr || HasItem() || bRouletteRunning)
	{
		return false;
	}
	
	PendingItem = Pool->PickRandomItem();
	if (PendingItem == nullptr)
	{
		return false;
	}
	
	bRouletteRunning = true;
	OnRouletteStarted.Broadcast(Pool, RouletteDuration);
	
	if (RouletteDuration > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(RouletteTimer, this, &UKartItemComponent::FinishRoulette, RouletteDuration, false);
	}
	else
	{
		FinishRoulette();
	}
	
	return true;
	
}

void UKartItemComponent::FinishRoulette()
{
	bRouletteRunning = false;
	
	if (PendingItem != nullptr)
	{
		SetItem(PendingItem, PendingItem->Uses);
		PendingItem = nullptr;
	}
}

void UKartItemComponent::UseItem()
{
	if (bRouletteRunning||CurrentItem == nullptr)
	{
		return;
	}
	
	APawny* Kart = Cast<APawny>(GetOwner());
	
	if (!CurrentItem->Activate(Kart))
	{
		return;
	}
	
	if (UsesLeft > 1)
	{
		SetItem(CurrentItem, UsesLeft - 1);
	}
	else
	{
		SetItem(nullptr, 0);
	}
	
}

void UKartItemComponent::ClearItem()
{
	GetWorld()->GetTimerManager().ClearTimer(RouletteTimer);
	bRouletteRunning = false;
	PendingItem = nullptr;
	SetItem(nullptr, 0);
	
}

void UKartItemComponent::SetItem(UKartItemData* NewItem, int32 NewUses)
{
	CurrentItem = NewItem;
	UsesLeft = NewUses;
	
	OnItemChanged.Broadcast(CurrentItem, UsesLeft);
	
	if (bShowDebugText && GEngine != nullptr)
	{
		const FString ItemName = CurrentItem != nullptr ? CurrentItem->DisplayName.ToString() : TEXT("aucun");
		GEngine->AddOnScreenDebugMessage(10,2.f, FColor::Green, FString::Printf(TEXT("Objet : %s x%d"), *ItemName, UsesLeft));
	}
}


