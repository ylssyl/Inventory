// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/PotionEffectInfoWidget.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

void UPotionEffectInfoWidget::Init(FName PropertyName, FPropertyEffect PropertyEffect)
{
	PropertyNameText->SetText(FText::FromName(PropertyName));

	AddAmountText->SetText(FText::FromString(FString::FromInt(FMath::CeilToInt(PropertyEffect.Amount))));

	DurationPanel->SetVisibility((PropertyEffect.Duration != 0) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	DurationText->SetText(FText::FromString(FString::FromInt(FMath::CeilToInt(PropertyEffect.Duration))));

	EffectDurationPanel->SetVisibility((PropertyEffect.EffectDuration != 0) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	EffectDurationText->SetText(FText::FromString(FString::FromInt(FMath::CeilToInt(PropertyEffect.EffectDuration))));
}
