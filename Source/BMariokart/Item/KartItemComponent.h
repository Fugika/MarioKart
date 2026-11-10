// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KartItemComponent.generated.h"

class UKartItemData;
class UKartItemPoolData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemChanged, UKartItemData*, Item, int32, UsesLeft);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRouletteStarted, UKartItemPoolData*, Pool, float, Duration);

UCLASS( ClassGroup=(Kart), meta=(BlueprintSpawnableComponent) )
class BMARIOKART_API UKartItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKartItemComponent();
	
	UFUNCTION( BlueprintCallable, Category = "Kart|Item")
	bool TryGiveRandomItem( UKartItemPoolData* Pool );
	
	UFUNCTION( BlueprintCallable, Category = "Kart|Item")
	void UseItem();
	
	UFUNCTION( BlueprintCallable, Category = "Kart|Item")
	void ClearItem();
	
	UFUNCTION( BlueprintPure, Category = "Kart|Item")
	bool HasItem() const { return CurrentItem != nullptr; }
	
	UFUNCTION( BlueprintPure, Category = "Kart|Item")
	bool IsRouletteRunning() const { return bRouletteRunning; }
	
	UFUNCTION( BlueprintPure, Category = "Kart|Item")
	UKartItemData* GetCurrentItem() const { return CurrentItem; }
	
	UFUNCTION( BlueprintPure, Category = "Kart|Item")
	int32 GetUsesLeft() const { return UsesLeft; }
	
	UPROPERTY( BlueprintAssignable, Category = "Kart|Item" )
	FOnItemChanged OnItemChanged;
	
	UPROPERTY( BlueprintAssignable, Category = "Kart|Item" )
	FOnRouletteStarted OnRouletteStarted;

protected:
	
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Kart|Item", meta = (ClampMin = "0"))
	float RouletteDuration = 2.f;
	
	UPROPERTY( EditAnywhere, Category = "Kart|Item|Debug" )
	bool bShowDebugText = true;

	
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Item" )
	TObjectPtr<UKartItemData> CurrentItem;
	
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Item" )
	int32 UsesLeft = 0;
	
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Kart|Item" )
	bool bRouletteRunning = false;
	
private:
	
	void FinishRoulette();
	
	void SetItem(UKartItemData* NewItem, int32 NewUses);
	
	UPROPERTY()
	TObjectPtr<UKartItemData> PendingItem;
	
	FTimerHandle RouletteTimer;
	
};
