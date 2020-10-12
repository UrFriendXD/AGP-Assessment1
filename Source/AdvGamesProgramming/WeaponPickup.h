// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "WeaponPickup.generated.h"

UENUM(BlueprintType)
enum class WeaponPickupRarity : uint8
{
    //NONE UMETA(DisplayName = "No Pattern"),
    LEGENDARY = 3,
    MASTER = 2,
    RARE = 1,
    COMMON = 0,

    Num UMETA(Hidden)
};

UCLASS()
class ADVGAMESPROGRAMMING_API AWeaponPickup : public APickup
{
    GENERATED_BODY()
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    AWeaponPickup();

    UFUNCTION(BlueprintImplementableEvent)
    void OnPickup(AActor* ActorThatPickedUp) override;

    UFUNCTION(BlueprintCallable)
    void OnGenerate() override;

    void ChooseGoodStats();
    void GenerateStats();

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    float BulletDamage;
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    float MuzzleVelocity;
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    int32 MagazineSize;
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    float WeaponAccuracy;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    WeaponPickupRarity Rarity;

    UPROPERTY(VisibleAnywhere)
    TArray<int> GoodStatPool = {0, 1, 2, 3};
    UPROPERTY(VisibleAnywhere)
    TMap<FName, int> StatQualities;
};
