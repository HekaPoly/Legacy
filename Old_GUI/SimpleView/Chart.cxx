#include "Chart.h"
#include "vtkSmartPointer.h"
#include <vtkChartXY.h>
#include <vtkContextView.h>
#include <vtkAxis.h>
#include <vtkRenderWindow.h>

// Constructor
Chart::Chart(
	vtkSmartPointer<vtkChartXY> chart,
	vtkSmartPointer<vtkContextView> contextView,
	QVTKWidget* widget) :
	chart_(chart),
	contextView_(contextView),
	widget_(widget)
{
	auto bottom = chart_->GetAxis(vtkAxis::BOTTOM);
	bottom->SetTitle("t(s)");

	auto left = chart_->GetAxis(vtkAxis::LEFT);
	left->SetTitle("amplitude");
}


// ContextView getter
vtkSmartPointer<vtkContextView> Chart::getContextView() const
{
	return contextView_;
}

// Chart getter
vtkSmartPointer<vtkChartXY> Chart::getChart() const
{
	return chart_;
}

// Widget getter
QVTKWidget* Chart::getWidget() const
{
	return widget_;
}

// Set the X range of chart
void Chart::setXRange(const double& minimum, const double& maximum) const
{
	auto bottom = chart_->GetAxis(vtkAxis::BOTTOM);
	bottom->SetMinimum(minimum);
	bottom->SetMaximum(maximum);

	contextView_->GetRenderWindow()->Render();
}

// Micro-slide chart
void Chart::slideXAxis(const float& inc) const
{
	auto bottom = chart_->GetAxis(vtkAxis::BOTTOM);
	bottom->SetMinimum(bottom->GetMinimum() + inc);
	bottom->SetMaximum(bottom->GetMaximum() + inc);
	contextView_->GetRenderWindow()->Render();
}

// Slide chart (minimal version)
void Chart::slideXAxisMinimal() const
{
	auto bottom = chart_->GetAxis(vtkAxis::BOTTOM);
	double range = bottom->GetMaximum() - bottom->GetMinimum();
	bottom->SetMinimum(bottom->GetMinimum() + range);
	bottom->SetMaximum(bottom->GetMaximum() + range);
	contextView_->GetRenderWindow()->Render();
}