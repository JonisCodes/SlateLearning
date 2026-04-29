#include "FSlatePracticeStyle.h"

#include "Styling/SlateStyleRegistry.h"

TSharedPtr<FSlateStyleSet> FSlatePracticeStyle::StyleInstance = nullptr;

void FSlatePracticeStyle::Initialize()
{
	if (StyleInstance.IsValid()) return;
	
	StyleInstance = MakeShared<FSlateStyleSet>(GetStyleSetName());
	
	// Progress bar styles
	FProgressBarStyle HealthBarStyle = FProgressBarStyle()
	.SetBackgroundImage(FSlateNoResource())
	.SetFillImage(FSlateColorBrush(FLinearColor::White))
	.SetMarqueeImage(FSlateNoResource());
	
	FProgressBarStyle ArmorBarStyle = FProgressBarStyle()
	.SetBackgroundImage(FSlateNoResource())
	.SetFillImage(FSlateColorBrush(FLinearColor(0.f, 0.6f, 1.f)))
	.SetMarqueeImage(FSlateNoResource());
	
	StyleInstance->Set("SlatePractice.HealthBar", HealthBarStyle);
	StyleInstance->Set("SlatePractice.ArmorBar", ArmorBarStyle);
	
	// Text styles
	FTextBlockStyle DefaultText = FTextBlockStyle()
	.SetFont(FCoreStyle::GetDefaultFontStyle("Regular", 12))
	.SetColorAndOpacity(FLinearColor::White);
	
	FTextBlockStyle HeaderText = FTextBlockStyle()
	.SetFont(FCoreStyle::GetDefaultFontStyle("Bold", 16))
	.SetColorAndOpacity(FLinearColor::White); 
	
	StyleInstance->Set("SlatePractice.Text.Default", DefaultText);
	StyleInstance->Set("SlatePractice.Text.Header", HeaderText);
	
	FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
}

void FSlatePracticeStyle::Shutdown()
{
	if (StyleInstance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
		StyleInstance.Reset();
	}
}

const ISlateStyle& FSlatePracticeStyle::Get()
{
	return *StyleInstance;
}

FName FSlatePracticeStyle::GetStyleSetName()
{
	static FName StyleSetName("FSlatePracticeStyle");
	return StyleSetName;
}
