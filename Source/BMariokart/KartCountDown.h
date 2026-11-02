// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KartCountDown.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class BMARIOKART_API UKartCountDown : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CountDownText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kart|CountDown")
	FText GoText = NSLOCTEXT("Kart", "Go", "GO!!!");
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kart|CountDown", meta = (ClampMin = "0"))
	float GoDisplayTime = 1.f;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Kart|CountDown")
	void OnCountDownNumberShown(int32 Number);
	
private:
	UFUNCTION()
	void HandleCountDownTick(int32 Number);
	
	void HideCountDown();
	
	FTimerHandle HideTimer;
	
};
