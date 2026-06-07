// Fill out your copyright notice in the Description page of Project Settings.


#include "DraggablePanel/DraggablePanel.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UDraggablePanel::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyDraggablePanel.IsValid())
	{
		MyDraggablePanel->SetBorderColor(BackgroundColor);
		MyDraggablePanel->SetBorderBrush(BackgroundBrush);
		MyDraggablePanel->SetTextPadding(TextPadding);
		MyDraggablePanel->SetText(TitleText);
		MyDraggablePanel->SetTextColor(TextColor);
		MyDraggablePanel->SetFont(TextFont);
		MyDraggablePanel->SetCloseButtonSize(CloseButtonSize);
		MyDraggablePanel->SetButtonContentPadding(CloseButtonContentPadding);
		MyDraggablePanel->SetButtonColor(CloseButtonColor);
		MyDraggablePanel->SetButtonTextColor(CloseButtonTextColor);
		MyDraggablePanel->SetButtonStyle(CloseButtonStyle);
	}
}

void UDraggablePanel::HandleOnDragged(const FVector2D Position) const
{
	OnWidgetDragged.Broadcast(Position);
}

void UDraggablePanel::HandleOnClosePanel()
{
	OnClosePanel.Broadcast();
	SetVisibility(ESlateVisibility::Collapsed);
}

#if WITH_EDITOR
void UDraggablePanel::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

TSharedRef<SWidget> UDraggablePanel::RebuildWidget()
{
	SAssignNew(MyDraggablePanel, SDraggablePanel)
	.OnFocusGained(BIND_UOBJECT_DELEGATE(FOnFocusGained, OnPanelFocused))
	.OnLostFocus(BIND_UOBJECT_DELEGATE(FOnLostFocus, OnPanelUnfocused))
	.TitleBorderColor(BackgroundColor)
	.TitleBorderBrush(BackgroundBrush)
	.TitleTextPadding(TextPadding)
	.TitleText(TitleText)
	.TitleTextColor(TextColor)
	.TitleFont(TextFont)
	.TitleCloseButtonSize(CloseButtonSize)
	.TitleButtonContentPadding(CloseButtonContentPadding)
	.TitleButtonColor(CloseButtonColor)
	.TitleButtonTextColor(CloseButtonTextColor)
	.TitleCloseButtonStyle(CloseButtonStyle)
	.OnDragged(BIND_UOBJECT_DELEGATE(FOnDragged, HandleOnDragged))
	.OnPanelClosed(BIND_UOBJECT_DELEGATE(FOnPanelClosed, HandleOnClosePanel));
	if (GetChildrenCount() > 0)
	{
		UWidget* Child = GetChildAt(0);
		MyDraggablePanel->SetContent(Child->TakeWidget());
	}

	return MyDraggablePanel.ToSharedRef();
}

void UDraggablePanel::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyDraggablePanel.Reset();
}

void UDraggablePanel::OnSlotAdded(UPanelSlot* InSlot)
{
	if (MyDraggablePanel.IsValid() && GetChildrenCount() > 0)
	{
		UWidget* Child = GetChildAt(0);
		MyDraggablePanel->SetContent(Child->TakeWidget());
	}
}

void UDraggablePanel::OnSlotRemoved(UPanelSlot* InSlot)
{
	if (MyDraggablePanel.IsValid())
	{
		MyDraggablePanel->SetContent(SNullWidget::NullWidget);
	}
}

int32 UDraggablePanel::GetCurrentZOrder() const
{
	if (const UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
	{
		return CanvasSlot->GetZOrder();
	}

	return 0;
}

void UDraggablePanel::OnPanelFocused() const
{
	const UUserWidget* OwningWidget = GetTypedOuter<UUserWidget>();
	if (!OwningWidget) return;

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(OwningWidget->Slot))
	{
		CanvasSlot->SetZOrder(CanvasSlot->GetZOrder() + 100);
		OnPanelBroughtToFront.Broadcast();
	}
}

void UDraggablePanel::OnPanelUnfocused() const
{
	const UUserWidget* OwningWidget = GetTypedOuter<UUserWidget>();
	if (!OwningWidget) return;

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(OwningWidget->Slot))
	{
		CanvasSlot->SetZOrder(0);
	}
}
