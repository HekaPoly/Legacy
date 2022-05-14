/*=========================================================================

  Program:   Visualization Toolkit
  Module:    SimpleView.h
  Language:  C++

  Copyright 2009 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
  license for use of this work by or on behalf of the
  U.S. Government. Redistribution and use in source and binary forms, with
  or without modification, are permitted provided that this Notice and any
  statement of authorship are reproduced on all copies.

=========================================================================*/
#ifndef SimpleView_H
#define SimpleView_H

#include <QMainWindow>
#include "Chart.h"
#include <QTimer>

// Forward Qt class declarations
class Ui_SimpleView;

// Forward VTK class declarations
class vtkQtTableView;
class vtkChartXY;
class vtkContextView;
class QVTKWidget;

class SimpleView : public QMainWindow
{
	Q_OBJECT

public:

	// Constructor/Destructor
	SimpleView();
	~SimpleView() override;
	void createCharts();
	void slide(const float& inc);

public slots:

	virtual void slotOpenFile();
	virtual void slotExit();
	void startSlider() const;
	void stopSlider();
	//void slide(const float& inc);

signals:
	void incremented(const float& inc);

private:
	std::vector<Chart> charts_;
	QTimer* timer_;
	bool toggle_;
	// Designer form
	Ui_SimpleView *ui;
};

#endif // SimpleView_H
