#include "SMyHUD.h"

#include "SArmorRing.h"
#include "Kismet/GameplayStatics.h"
#include "SlateLearning/Player/MySlateCharacter.h"
#include "Animation/CurveSequence.h"
#include "Styling/AppStyle.h"
#include "Widgets/Notifications/SProgressBar.h"

void SMyHUD::Construct(const FArguments& InArgs)
{
	BarStyle = FProgressBarStyle()
	.SetBackgroundImage(FSlateNoResource())
	.SetFillImage(FSlateColorBrush(FLinearColor::White))
	.SetMarqueeImage(FSlateNoResource());
	
	OwningCharacter = InArgs._OwningCharacter;
	
	PulseSequence = FCurveSequence();
	PulseHandle = PulseSequence.AddCurve(0.f, 0.6f, ECurveEaseFunction::Linear);
	RegisterActiveTimer(0.f, FWidgetActiveTimerDelegate::CreateSP(this, &SMyHUD::TimerCallback));
	
	
	ChildSlot
	[
		SNew(SOverlay)

		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Bottom)
		.Padding(30.f, 0.f, 0.f, 30.f)
		[
			SNew(SBox)
			.WidthOverride(300.f)
			.HeightOverride(24.f)
			[
				SNew(SProgressBar)
				.Percent(this, &SMyHUD::GetHealthPercent)
				.BarFillType(EProgressBarFillType::LeftToRight)
				.FillColorAndOpacity(this, &SMyHUD::GetHealthBarColor)
				.Style(&BarStyle)
			]
		]

		+ SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		.Padding(0.f, 0.f, 30.f, 30.f)
		[
			SNew(SBorder)
			.ColorAndOpacity(FLinearColor::Blue)
			[
				SNew(STextBlock)
				.Text(this, &SMyHUD::GetAmmoText)
			]
		]

		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Top)
		.Padding(0.f, 30.f, 0.f, 0.f)
		[
			SNew(STextBlock)
			.Text(FText::FromString("PLAYER NAME"))
			.ColorAndOpacity(FLinearColor::White)
		]

		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Bottom)
		.Padding(0.f, 0.f, 0.f, 30.f)
		[
			SNew(SArmorRing)
			.ArmorPercent(this, &SMyHUD::GetArmorPercent)
		]
	];
}

void SMyHUD::UpdateHealthState()
{
	if (!OwningCharacter.IsValid()) return;
	
	const float Percent = OwningCharacter->Health / OwningCharacter->MaxHealth;
	
	if (Percent <= 0.25f && !bIsLowHealth)
	{
		bIsLowHealth = true;
		PulseSequence.Play(this->AsShared(), true, 0.f, true);
	}
	else if (Percent > 0.25f && bIsLowHealth)
	{
		bIsLowHealth = false;
		PulseSequence.Pause();
	}
}

EActiveTimerReturnType SMyHUD::TimerCallback(double InCurrentTime, float InDeltaTime)
{
	UpdateHealthState();
	return EActiveTimerReturnType::Continue;
}

TOptional<float> SMyHUD::GetHealthPercent() const
{
	if (!OwningCharacter.IsValid()) return 1.f;

	const auto Percent = OwningCharacter->Health / OwningCharacter->MaxHealth;
	return FMath::Clamp(Percent, 0.f, 1.f);
}

float SMyHUD::GetArmorPercent() const
{
	if (!OwningCharacter.IsValid()) return 0.f;

	return OwningCharacter->Armor / OwningCharacter->MaxArmor;
}

FText SMyHUD::GetAmmoText() const
{
	if (!OwningCharacter.IsValid()) return FText::GetEmpty();

	return FText::Format(
		INVTEXT("{0} / {1}"),
		OwningCharacter->Ammo,
		OwningCharacter->MaxAmmo
	);
}

FSlateColor SMyHUD::GetHealthBarColor() const
{
	if (bIsLowHealth)
	{
		const float CurveValue = PulseHandle.GetLerp();
		return FSlateColor(FLinearColor::LerpUsingHSV(FLinearColor::White, FLinearColor::Red, CurveValue));
	}
	
	return FSlateColor(FLinearColor::White);
}
