#pragma once

struct FComboBoxItem
{
	FText DisplayName;
	FLinearColor BackgroundColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.f);
	FLinearColor TextColor = FLinearColor::White;
};
