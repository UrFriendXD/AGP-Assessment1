// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"

AWeaponPickup::AWeaponPickup()
{
    StatQualities.Add("BulletDamage", 0);
    StatQualities.Add("MuzzleVelocity", 0);
    StatQualities.Add("MagazineSize", 0);
    StatQualities.Add("WeaponAccuracy", 0);
}

void AWeaponPickup::BeginPlay()
{
    Super::BeginPlay();
    //OnGenerate();
}

void AWeaponPickup::OnGenerate()
{
    APickup::OnGenerate();
    int random = FMath::RandRange(0, 100);
    if (random < 50)
    {
        Rarity = WeaponPickupRarity::COMMON;
    }
    else if (random >= 50 && random < 80)
    {
        Rarity = WeaponPickupRarity::RARE;
    }
    else if (random >= 80 && random < 95)
    {
        Rarity = WeaponPickupRarity::MASTER;
    }
    else if (random >= 95)
    {
        Rarity = WeaponPickupRarity::LEGENDARY;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Random rarity value out of bounds %i"), random);
    }

    ChooseGoodStats();
    GenerateStats();
}

void AWeaponPickup::ChooseGoodStats()
{
    //UE_LOG(LogTemp, Error, TEXT("RandomStatIndex out of bounds %i"), int(Rarity))
    for (int i = 0; i < static_cast<int>(Rarity) * 4 + 2; i++)
    {
        if (GoodStatPool.Num() != 0)
        {
            // Picks a num 0-4
            int RandomStatIndex = FMath::RandRange(0, GoodStatPool.Num() - 1);

            // Get value from pool
            int StatIndex = GoodStatPool[RandomStatIndex];

            // Add value for stat
            switch (StatIndex)
            {
            case 0:
                StatQualities["BulletDamage"] += 1;
                if (StatQualities["BulletDamage"] == 4)
                {
                    GoodStatPool.Remove(StatIndex);
                }
                break;
            case 1:
                StatQualities["MuzzleVelocity"] += 1;
                if (StatQualities["MuzzleVelocity"] == 4)
                {
                    GoodStatPool.Remove(StatIndex);
                }
                break;
            case 2:
                StatQualities["MagazineSize"] += 1;
                if (StatQualities["MagazineSize"] == 4)
                {
                    GoodStatPool.Remove(StatIndex);
                }
                break;
            case 3:
                StatQualities["WeaponAccuracy"] += 1;
                if (StatQualities["WeaponAccuracy"] == 4)
                {
                    GoodStatPool.Remove(StatIndex);
                }
                break;
            default:
                UE_LOG(LogTemp, Error, TEXT("RandomStatIndex out of bounds %i"), RandomStatIndex)
            }
        }
    }
}

void AWeaponPickup::GenerateStats()
{
    switch (StatQualities["BulletDamage"])
    {
    case 0:
        BulletDamage = FMath::RandRange(2.0f, 7.0f);
        break;
    case 1:
        BulletDamage = FMath::RandRange(7.0f, 14.0f);
        break;
    case 2:
        BulletDamage = FMath::RandRange(14.0f, 20.0f);
        break;
    case 3:
        BulletDamage = FMath::RandRange(20.0f, 25.0f);
        break;
    case 4:
        BulletDamage = FMath::RandRange(25.0f, 30.0f);
        break;
    default: ;
    }
    switch (StatQualities["MuzzleVelocity"])
    {
    case 0:
        MuzzleVelocity = FMath::RandRange(5000.0f, 7000.0f);
        break;
    case 1:
        MuzzleVelocity = FMath::RandRange(7000.0f, 10000.0f);
        break;
    case 2:
        MuzzleVelocity = FMath::RandRange(10000.0f, 15000.0f);
        break;
    case 3:
        MuzzleVelocity = FMath::RandRange(15000.0f, 18000.0f);
        break;
    case 4:
        MuzzleVelocity = FMath::RandRange(18000.0f, 20000.0f);
        break;
    default: ;
    }
    switch (StatQualities["MagazineSize"])
    {
    case 0:
        MagazineSize = FMath::RandRange(5, 15);
        break;
    case 1:
        MagazineSize = FMath::RandRange(15, 30);
        break;
    case 2:
        MagazineSize = FMath::RandRange(30, 55);
        break;
    case 3:
        MagazineSize = FMath::RandRange(55, 80);
        break;
    case 4:
        MagazineSize = FMath::RandRange(80, 100);
        break;
    default: ;
    }
    switch (StatQualities["WeaponAccuracy"])
    {
    case 0:
        WeaponAccuracy = FMath::RandRange(20.0f, 80.0f);
        break;
    case 1:
        WeaponAccuracy = FMath::RandRange(80.0f, 200.0f);
        break;
    case 2:
        WeaponAccuracy = FMath::RandRange(200.0f, 400.0f);
        break;
    case 3:
        WeaponAccuracy = FMath::RandRange(400.0f,700.0f);
        break;
    case 4:
        WeaponAccuracy = FMath::RandRange(700.0f, 1000.0f);
        break;
    default: ;
    }
}
