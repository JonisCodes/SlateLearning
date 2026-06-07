// Fill out your copyright notice in the Description page of Project Settings.


#include "Minimap/SMinimapWidget.h"

#include "SlateOptMacros.h"
#include "DSP/ChannelMap.h"
#include "Minimap/MapComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Minimap/MinimapSubsystem.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMinimapWidget::Construct(const FArguments& InArgs)
{
	MinimapSize = InArgs._MinimapSize;
	MinimapSubsystem = InArgs._MinimapSubsystem;
	MapRadius = InArgs._MapRadius;
	BackgroundColor = InArgs._BackgroundColor;
	MapRenderTarget = InArgs._MapRenderTarget;
	MapMin = InArgs._MapMin;
	MapMax = InArgs._MapMax;
}

const FVector2D SMinimapWidget::WorldToMinimap(const FVector& WorldPos, const FVector& PlayerPos,
                                               const FVector2d& MinimapSize, float MapRadius)
{
	const FVector2D RelativePos = FVector2D(WorldPos.Y - PlayerPos.Y, -(WorldPos.X - PlayerPos.X));
	const FVector2D NormalizedPos = RelativePos / MapRadius;
	return NormalizedPos * (MinimapSize * 0.5f) + MinimapSize * 0.5f;
}

void SMinimapWidget::SetMinimapSize(const FVector2D& NewMinimapSize)
{
	MinimapSize = NewMinimapSize;
	Invalidate(EInvalidateWidgetReason::Layout);
}

void SMinimapWidget::SetMapRadius(const float NewMapRadius)
{
	MapRadius = NewMapRadius;
	Invalidate(EInvalidateWidgetReason::Layout);
}

void SMinimapWidget::SetBackgroundColor(const FLinearColor NewBackgroundColor)
{
	BackgroundColor = NewBackgroundColor;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void SMinimapWidget::SetRenderTarget(UTextureRenderTarget2D* NewRenderTarget)
{
	MapRenderTarget = NewRenderTarget;

	if (IsValid(NewRenderTarget))
	{
		CachedRenderTargetBrush = MakeShared<FSlateImageBrush>(
			MapRenderTarget.Get(),
			MinimapSize);
		// CachedRenderTargetBrush->SetResourceObject(NewRenderTarget);
		// CachedRenderTargetBrush->ImageSize = MinimapSize;
		// CachedRenderTargetBrush->DrawAs = ESlateBrushDrawType::Image;
		// CachedRenderTargetBrush->ImageType = ESlateBrushImageType::FullColor;
	}
}

void SMinimapWidget::SetMapMin(const FVector2D& NewMapMin)
{
	MapMin = NewMapMin;
}

void SMinimapWidget::SetMapMax(const FVector2D& NewMapMax)
{
	MapMax = NewMapMax;
}

int32 SMinimapWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                              const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                              const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// ----- Background Box -----
	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId, AllottedGeometry.ToPaintGeometry(),
		FCoreStyle::Get().GetBrush("WhiteBrush"),
		ESlateDrawEffect::None,
		BackgroundColor
	);
	LayerId++;
	// ----- END -----

	// ----- Render Target -----
	if (MapRenderTarget.IsValid() && CachedRenderTargetBrush.IsValid())
	{
		FSlateDrawElement::MakeBox(
			OutDrawElements, LayerId,
			AllottedGeometry.ToPaintGeometry(),
			CachedRenderTargetBrush.Get(),
			ESlateDrawEffect::None,
			FLinearColor::White
		);
	}
	LayerId++;

	// ----- END -----

	if (!MinimapSubsystem.IsValid())
	{
		if (CachedRenderTargetBrush.IsValid())
		{
			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(),
				CachedRenderTargetBrush.Get(),
				ESlateDrawEffect::None,
				FLinearColor::White
			);
		}
		LayerId++;
		return LayerId;
	}

	const TWeakObjectPtr<UMapComponent>* PlayerCompPtr = MinimapSubsystem->GetRegisteredComponents().FindByPredicate(
		[](const TWeakObjectPtr<UMapComponent>& Component)
		{
			return Component.IsValid() && Component->ActorType == EMapActorType::Player;
		});

	if (!PlayerCompPtr || !PlayerCompPtr->IsValid())
	{
		if (CachedRenderTargetBrush.IsValid())
		{
			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(),
				CachedRenderTargetBrush.Get(),
				ESlateDrawEffect::None,
				FLinearColor::White
			);
		}
		LayerId++;
		return LayerId;
	}

	const UMapComponent* PlayerComp = PlayerCompPtr->Get();
	const FVector PlayerWorldPos = PlayerComp->GetOwner()->GetActorLocation();

	if (CachedRenderTargetBrush.IsValid())
	{
		const float ViewFraction = (MapRadius * 2.f) / (MapMax.X - MapMin.X);

		const FVector2D UVCenter = FVector2D(
			(PlayerWorldPos.Y - MapMin.Y) / (MapMax.Y - MapMin.Y),
			1.0f - (PlayerWorldPos.X - MapMin.X) / (MapMax.X - MapMin.X)
		);

		const FVector2D UVMin = UVCenter - FVector2D(ViewFraction * 0.5f);
		const FVector2D UVMax = UVCenter + FVector2D(ViewFraction * 0.5f);
		CachedRenderTargetBrush->SetUVRegion(FBox2D(UVMin, UVMax));
	}

	// ----- Component renders -----
	for (const TWeakObjectPtr<UMapComponent>& WeakComponent : MinimapSubsystem->GetRegisteredComponents())
	{
		if (!WeakComponent.IsValid()) continue;

		if (WeakComponent->ActorType == EMapActorType::Player) continue;

		const FVector ActorWorldPos = WeakComponent->GetOwner()->GetActorLocation();
		const FVector2D MinimapPos = WorldToMinimap(ActorWorldPos, PlayerWorldPos, MinimapSize, MapRadius);

		if (const FVector2D LocalSize = AllottedGeometry.GetLocalSize();
			MinimapPos.X < 0.f || MinimapPos.X > LocalSize.X || MinimapPos.Y < 0.f || MinimapPos.Y > LocalSize.Y)
			continue;

		DrawMinimapIcon(OutDrawElements, LayerId, AllottedGeometry, MinimapPos, WeakComponent->IconSize,
		                WeakComponent->IconColor, WeakComponent->Icon);
	}
	LayerId++;
	// ----- END -----

	// ----- Ping renders -----
	for (const FMinimapPing& Ping : MinimapSubsystem->GetPings())
	{
		const FVector2D MinimapPos = WorldToMinimap(Ping.WorldLocation, PlayerWorldPos, MinimapSize, MapRadius);

		if (const FVector2D LocalSize = AllottedGeometry.GetLocalSize();
			MinimapPos.X < 0.f || MinimapPos.X > LocalSize.X ||
			MinimapPos.Y < 0.f || MinimapPos.Y > LocalSize.Y)
			continue;

		DrawMinimapIcon(OutDrawElements, LayerId, AllottedGeometry, MinimapPos, Ping.Size, Ping.Color, Ping.Icon);
	}
	LayerId++;
	// ----- END -----

	// ----- Player Box -----
	const FVector2D Center = AllottedGeometry.GetLocalSize() / 2.f;

	DrawMinimapIcon(OutDrawElements, LayerId, AllottedGeometry, Center, PlayerComp->IconSize, PlayerComp->IconColor,
	                PlayerComp->Icon);
	// ----- END -----

	// ----- Player line -----
	const FRotator PlayerRotation = PlayerComp->GetOwner()->GetActorRotation();
	const float YawRad = FMath::DegreesToRadians(PlayerRotation.Yaw - 90.f);
	const FVector2D ForwardDir = FVector2D(FMath::Cos(YawRad), FMath::Sin(YawRad));

	const float ArrowLength = 12.f;
	TArray<FVector2D> ArrowPoints;
	ArrowPoints.Add(Center);
	ArrowPoints.Add(Center + ForwardDir * ArrowLength);

	FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		ArrowPoints,
		ESlateDrawEffect::None,
		PlayerComp->IconColor,
		true, 2.f);
	// ----- END -----

	return LayerId;
}

FVector2D SMinimapWidget::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return MinimapSize;
}

const void SMinimapWidget::DrawMinimapIcon(FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                           const FGeometry& AllottedGeometry, const FVector2D& Position, float Size,
                                           const FLinearColor& Color,
                                           UTexture2D* Icon) const
{
	FSlateBrush MarkerBrush;
	if (IsValid(Icon))
	{
		MarkerBrush.SetResourceObject(Icon);
		MarkerBrush.DrawAs = ESlateBrushDrawType::Image;
	}
	else
	{
		MarkerBrush = *FCoreStyle::Get().GetBrush("WhiteBrush");
	}

	const float CenterSize = Size * 0.5f;

	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(
			FVector2D(Size, Size),
			FSlateLayoutTransform(Position - FVector2D(CenterSize, CenterSize)),
			FSlateRenderTransform()),
		&MarkerBrush,
		ESlateDrawEffect::None,
		Color);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
