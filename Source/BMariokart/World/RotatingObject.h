// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingObject.generated.h"

class USceneComponent;

UCLASS()
class BMARIOKART_API ARotatingObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatingObject();
	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Rotating")
	void SetAnimationEnabled(bool bEnabled);
	
	UFUNCTION(BlueprintPure, Category = "Rotating")
	bool IsAnimationEnabled() const { return bAnimate; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rotating|Components")
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rotating|Components")
	TObjectPtr<USceneComponent> AnimatedRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotating|Rotation")
	FRotator RotationSpeed= FRotator(30.0f, 90.0f, 45.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotating|Bobbing")
	bool bEnableBobbing = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotating|Bobbing", meta = (ClampMin = 0))
	float BobHeight = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotating|Bobbing", meta = (ClampMin = 0.01))
	float BobFrequency = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotating")
	bool bRandomStartPhase = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotating")
	bool bAnimate = true;
	

private:
	
	float AnimationTime = 0.f;
	
	FVector AnimatedRootBaseLocation = FVector::ZeroVector;

};
