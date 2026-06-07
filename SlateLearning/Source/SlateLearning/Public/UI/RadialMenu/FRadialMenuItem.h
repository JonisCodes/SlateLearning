#pragma once

#include "CoreMinimal.h"
#include "FRadialMenuItem.generated.h"

class USlateBrushAsset;

USTRUCT(Blueprintable)
struct FRadialMenuItem
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Label;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color = FLinearColor(0.2f, 0.2f, 0.8f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor HoveredColor = FLinearColor(0.5f, 0.5f, 1.f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsvalid = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush Brush;
};
