#include "SMyHUD.h"

#include "SArmorRing.h"
#include "SStatList.h"
#include "MySlateCharacter.h"
#include "Animation/CurveSequence.h"
#include "FSlatePracticeStyle.h"
#include "SCustomComboBox.h"
#include "SRadialMenu.h"
#include "SStatBar.h"
#include "Inventory/SInventoryGrid.h"
#include "Styling/AppStyle.h"
#include "Widgets/SInvalidationPanel.h"
#include "Widgets/Notifications/SProgressBar.h"

void SMyHUD::Construct(const FArguments& InArgs)
{
	OwningCharacter = InArgs._OwningCharacter;

	PulseSequence = FCurveSequence();
	PulseHandle = PulseSequence.AddCurve(0.f, 0.6f, ECurveEaseFunction::Linear);
	RegisterActiveTimer(0.f, FWidgetActiveTimerDelegate::CreateSP(this, &SMyHUD::TimerCallback));

	ChildSlot
	[
		SNew(SInvalidationPanel)
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
					.Style(&FSlatePracticeStyle::Get(), "SlatePractice.HealthBar")
					.ForceVolatile(true)
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
					.ForceVolatile(true)
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

			// + SOverlay::Slot()
			// .HAlign(HAlign_Center)
			// .VAlign(VAlign_Center)
			// [
			// 	SNew(SBox)
			// 	.WidthOverride(600.f)
			// 	.HeightOverride(600.f)
			// 	[
			// 		// SNew(SCustomComboBox)
			// 		// .DesiredWidth(300.f)
			// 		// .HeaderHeight(24.f)
			// 		SNew(SRadialMenu)
			// 		.NumSegments(8)
			// 		.OuterRadius(300)
			//
			// 	]
			// ]

			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Bottom)
			.Padding(0.f, 0.f, 0.f, 30.f)
			[
				SAssignNew(ArmorRingWidget, SArmorRing)
				.ArmorPercent(this, &SMyHUD::GetArmorPercent)
			]


			// + SOverlay::Slot()
			// .HAlign(HAlign_Left)
			// .VAlign(VAlign_Top)
			// [
			// 	SNew(SStatList)
			// 	.StatRows(this, &SMyHUD::GetStatRows)
			// ]
		]

	];
}

void SMyHUD::OnHealthChanged(const float NewHealth, float const MaxHealth)
{
	if (const auto NewPercent = NewHealth / MaxHealth; !FMath::IsNearlyEqual(CachedHealthPercent, NewPercent))
	{
		if (!StatBarWidget.IsValid()) return;

		CachedHealthPercent = NewPercent;
		StatBarWidget->SetCurrentValue(NewHealth);
		Invalidate(EInvalidateWidgetReason::Paint);
	}
}

void SMyHUD::OnAmmoChanged(int32 NewAmmo, int32 MaxAmmo)
{
	const auto NewText = FText::Format(
		INVTEXT("{0} / {1}"), NewAmmo, MaxAmmo);

	if (!NewText.EqualTo(CachedAmmoText))
	{
		CachedAmmoText = NewText;
		Invalidate(EInvalidateWidgetReason::Paint);
	}
}

void SMyHUD::OnArmorChanged(float NewArmor, float MaxArmor)
{
	if (const auto NewPercent = NewArmor / MaxArmor; FMath::Abs(NewPercent - CachedArmorPercent) > 0.01f)
	{
		CachedArmorPercent = NewPercent;
		if (ArmorRingWidget.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalidating Armor ring"));
			ArmorRingWidget->Invalidate(EInvalidateWidgetReason::Paint);
		}
	}
}

void SMyHUD::UpdateHealthState()
{
	if (!OwningCharacter.IsValid()) return;

	if (const auto Percent = OwningCharacter->Health / OwningCharacter->MaxHealth; Percent <= 0.25f && !bIsLowHealth)
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
	if (!OwningCharacter.IsValid()) return 0.f;

	return FMath::Clamp(CachedHealthPercent, 0.f, 1.f);
}

TArray<TSharedPtr<FStatRow>> SMyHUD::GetStatRows() const
{
	if (!OwningCharacter.IsValid()) return {};

	return OwningCharacter->GetStatRows();
}

float SMyHUD::GetArmorPercent() const
{
	if (!OwningCharacter.IsValid()) return 0.f;

	return CachedArmorPercent;
}

FText SMyHUD::GetAmmoText() const
{
	if (!OwningCharacter.IsValid()) return FText::GetEmpty();

	return CachedAmmoText;
}

FSlateColor SMyHUD::GetHealthBarColor() const
{
	if (bIsLowHealth)
	{
		const auto CurveValue = PulseHandle.GetLerp();
		return FSlateColor(FLinearColor::LerpUsingHSV(FLinearColor::White, FLinearColor::Red, CurveValue));
	}

	return FSlateColor(FLinearColor::White);
}
