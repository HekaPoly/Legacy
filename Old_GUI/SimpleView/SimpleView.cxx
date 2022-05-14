/*
 * Copyright 2007 Sandia Corporation.
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the
 * U.S. Government. Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that this Notice and any
 * statement of authorship are reproduced on all copies.
 */


#include "ui_SimpleView.h"
#include "SimpleView.h"
#include "Chart.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkSmartPointer.h"
#include <vtkNew.h>
#include <vtkQtTableView.h>
#include <vtkRenderer.h>
#include <QtWidgets/QApplication>
#include <QtGui/QSurfaceFormat>
#include <vtkIntArray.h>
#include <vtkFloatArray.h>
#include <vtkContextView.h>
#include <vtkPen.h>
#include <QVTKWidget.h>
#include <vtkAxis.h>
#include <vtkTable.h>
#include <vtkPlot.h>
#include <vtkChart.h>
#include <vtkContextScene.h>
#include <vtkChartXY.h>

#include <QThread>
#include <chrono>

#include"cnpy.h"

// Constructor
SimpleView::SimpleView():
    toggle_(true)
{
	// Init ui
	this->ui = new Ui_SimpleView;
	this->ui->setupUi(this);

	// Init auto-scrolling
	timer_ = new QTimer();
	connect(timer_, &QTimer::timeout, this, [this] { slide(0.1); });

	// Init VTK Renderer
	vtkNew<vtkRenderer> ren;

	// Init charts in a 3x3 grid
	createCharts(10);

    // Set up action signals and slots
    connect(this->ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(startSlider()));
    connect(this->ui->actionSave, SIGNAL(triggered()), this, SLOT(stopSlider()));
    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));

    QTimer::singleShot(15000, this,
                       [this]()
                       {
                           for (auto& chart : charts_)
                           {
                               chart.setXRange(0, 5);
                               chart.setYRange(400, 600);
                               emit this->ui->actionOpenFile->triggered();
                           }
                           system(R"(C:\Users\hazbo\Desktop\freestyle_2.mp4)");
                           system(R"(python.exe C:\Users\hazbo\Documents\git\metis_duplicate\Metis-Exoskeleton\Classification\Src\real_time_predict.py)");
                       }
    );
};

SimpleView::~SimpleView()
{
    // The smart pointers should clean up for up
}

// Create chart
Chart createChart()
{
    // Set up the view
    auto contextView = vtkSmartPointer<vtkContextView>::New();
    contextView->GetRenderer()->SetBackground(1.0, 1.0, 1.0);

	// X-axis
	auto arrX = vtkSmartPointer<vtkFloatArray>::New();
	arrX->SetName("X Axis");
	table->AddColumn(arrX);

	// Y-axis
	auto arrC = vtkSmartPointer<vtkFloatArray>::New();
	arrC->SetName("Cosine");
	table->AddColumn(arrC);

	// Fill the table with some example values
	int numPoints = 100000;
	float inc = 27.5 / (numPoints - 1);
	table->SetNumberOfRows(numPoints);
	for (int i = 0; i < numPoints; ++i)
	{
		table->SetValue(i, 0, 0.5 * i * inc);
		table->SetValue(i, 1, 0.5 * cos(i * inc));
	} 

void SimpleView::createCharts()
{
    // auto arr = cnpy::npy_load(R"(C:\Users\hazbo\Documents\build_gui\elbow_extension_3s_2000Hz_1.npy)");
    auto arr = cnpy::npy_load(
        R"(C:\Users\hazbo\Documents\git\metis_duplicate\Metis-Exoskeleton\Acquisition\ringBufferDMA\hostReception\freestyle_45s_2000Hz_2.npy)");
    auto loaded_data = arr.data<int>();

    auto table = vtkSmartPointer<vtkTable>::New();
    table->SetNumberOfRows(arr.shape.at(1));

    auto arrX = vtkSmartPointer<vtkFloatArray>::New();
    arrX->SetName("Time(s)");
    auto xAxis = new float[arr.shape.at(1)];
    for (auto i = 0u; i < arr.shape.at(1); ++i)
    {
        xAxis[i] = i / 2000.0f;
    }
    arrX->SetArray(xAxis, arr.shape.at(1), 1);
    table->AddColumn(arrX);

// Create x number of charts (max. 9 charts)
void SimpleView::createCharts(const int& nbCharts)
{
	for (int i = 0; i < nbCharts; i++)
	{
		auto chart = createChart();

		// adds chart to a 3x3 grid
		auto widget = chart.getWidget();
		this->ui->gridLayout->addWidget(widget, i / 3, i % 3);
		widget->setMinimumSize({300, 300});

		// binds chart to window (?)
		chart.getContextView()->SetInteractor(widget->GetInteractor());
		widget->SetRenderWindow(chart.getContextView()->GetRenderWindow());
		widget->show();
		charts_.push_back(std::move(chart));
	}
}

// Micro-slide all the charts 
// OR
// Slide all the charts minimally
void SimpleView::slide(const float& inc)
{
	for (auto& chart : charts_)
	{
		//chart.slideXAxis(inc);
		chart.slideXAxisMinimal();
	}

}

// Action to be taken upon file open - will be used to load data
void SimpleView::slotOpenFile()
{
}

// Auto-scrolling mode is enabled - charts micro-slide every 10ms
void SimpleView::startSlider() const
{
	timer_->start(1000);
}

// Auto-scrolling mode is disabled - charts stop sliding
void SimpleView::stopSlider()
{
    timer_->stop();
}

// Exit program
void SimpleView::slotExit()
{
    qApp->exit();
}
