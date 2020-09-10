// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

AEnemyAIController::AEnemyAIController()
{
    SetGenericTeamId(FGenericTeamId(2));
}

ETeamAttitude::Type AEnemyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
    if (const APawn* OtherPawn = Cast<APawn>(&Other))
    {
        if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
        {
            FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
            switch (OtherTeamID)
            {
                case 1:
                    return ETeamAttitude::Hostile;
                case 2:
                    return ETeamAttitude::Friendly;
                default:
                    return ETeamAttitude::Neutral;
            }    
        }
    }
    return ETeamAttitude::Neutral;
}
