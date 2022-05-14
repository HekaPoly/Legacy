#ifndef Chart_H
#define Chart_H

#include "vtkSmartPointer.h"

// Forward VTK class declarations
class vtkQtTableView;
class vtkChartXY;
class vtkContextView;
class vtkFloatArray;
class QVTKWidget;

class Chart{
public:
	Chart() = delete;
	Chart(const Chart& other) = default;
	Chart(Chart&& other) = default;
	Chart& operator=(const Chart& other) = default;
	Chart& operator=(Chart&& other) = default;
	~Chart() = default;

	Chart(
		vtkSmartPointer<vtkChartXY> chart, 
		vtkSmartPointer<vtkContextView> contextView, 
		QVTKWidget* widget);

	vtkSmartPointer<vtkContextView> getContextView() const;
	vtkSmartPointer<vtkChartXY> getChart() const;
	QVTKWidget* getWidget() const;

	void setXRange(const double& minimum, const double& maximum) const;
	void slideXAxis(const float& inc) const;
	void slideXAxisMinimal() const;

private:
	vtkSmartPointer<vtkChartXY> chart_;
	vtkSmartPointer<vtkContextView> contextView_;
	QVTKWidget* widget_;
};

#endif