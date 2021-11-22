

//Mark Capka
	//VR & Data Visualization - Assignment 4: 3D Scalar Data Visualization


		//velocity.cxx is built from the assignment 3 file medical.cxx which is the base file from assignment 3 that I modified for the beginning of assignment 4. 

	//isosurface extraction


/*=========================================================================

Mark Capka
CSCD 477 Virtual Reality and Visualization
Assignment 4: 3D Vector Visualization

  Program:   Visualization Toolkit
  Module:   Velocity.cxx
  

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

	 This software is distributed WITHOUT ANY WARRANTY; without even
	 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
	 PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//
// This example will visualize the depiction of velocity of airflow within a rectangle chamber
//
#include <vtkTextActor.h> 
#include <vtkCamera.h>
#include <vtkContourFilter.h>
#include <vtkLookupTable.h>
#include <vtkNamedColors.h>
#include <vtkOutlineFilter.h>
#include <vtkPointData.h>
#include <vtkPointSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkStreamTracer.h>
#include <vtkStructuredPointsReader.h>
#include <vtkThresholdPoints.h>
#include <vtkMaskPoints.h>
#include <vtkGlyph3D.h>
#include <vtkArrowSource.h> //below this are added includes from assign 3 or for assign 4 beyond carotid flow example
#include <vtkVolume16Reader.h>
#include <vtkTextProperty.h>
#include <vtkActor.h>
#include <vtkStripper.h>
#include <vtkPolyDataNormals.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include <vtkSmartPointer.h>
#include <vtkImageMapper3D.h>
#include <vtkAxesActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkScalarBarActor.h>
#include <vtkAlgorithm.h>





int main(int argc, char* argv[])
{
	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
	


	// Create the renderer, the render window, and the interactor. The renderer
	// draws into the render window, the interactor enables mouse- and
	// keyboard-based interaction with the data within the render window.
	//
	vtkSmartPointer<vtkRenderer> aRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	
	vtkSmartPointer<vtkRenderWindow> renWin =
		vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(aRenderer);




	vtkSmartPointer<vtkRenderWindowInteractor> iren =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);


//assignment 3 commented out
	// The following reader is used to read a series of 2D slices (images)
	// that compose the volume. The slice dimensions are set, and the
	// pixel spacing. The data Endianness must also be specified. The reader
	// uses the FilePrefix in combination with the slice number to construct
	// filenames using the format FilePrefix.%d. (In this case the FilePrefix
	// is the root name of the file: quarter.)

  //mark - removed per assignment instructions -> replaced with lines below
	//vtkSmartPointer<vtkVolume16Reader> v16 =
	//  vtkSmartPointer<vtkVolume16Reader>::New();
	//v16->SetDataDimensions (64,64);
	//v16->SetImageRange (1,93);
	//v16->SetDataByteOrderToLittleEndian();
	//v16->SetFilePrefix ("headsq/quarter");
	//v16->SetDataSpacing (3.2, 3.2, 2.5);

//assignment 3 end

//	



	//assignment 4: 1. Global Velocity Distribution 4pts
	//use vtkStructuredPointsReader (like in assingment 3) to read velocity data from velocity.dat
	//use vtkArrowSource  as a glyph




	vtkSmartPointer<vtkStructuredPointsReader> v16 = vtkSmartPointer<vtkStructuredPointsReader>::New();
	v16->SetFileName("velocity.dat");	//our file containing data for velocity
	v16->Update();


	// create pipeline
////	we are reading in the velocity.dat above so no need to read from the velocity.vtk file.
//	vtkSmartPointer<vtkStructuredPointsReader> reader = vtkSmartPointer<vtkStructuredPointsReader>::New();
//	reader->SetFileName("velocity.vtk");
//	reader->Update();


	//assignment 4 par t2 
	////this is local velocity distribution -> we will use with streamtracer
	//vtkSmartPointer<vtkPointSource> psource = vtkSmartPointer<vtkPointSource>::New();
	//psource->SetNumberOfPoints(25);
	//psource->SetCenter(133.1, 116.3, 5.0);
	//psource->SetRadius(2.0);


	vtkSmartPointer<vtkThresholdPoints> threshold = vtkSmartPointer<vtkThresholdPoints>::New();
	threshold->SetInputConnection(v16->GetOutputPort());
	threshold->ThresholdByUpper(0);//teria is cells whose scalars are greater than or equal to threshold

	//TODO confirm that we need a mask
	vtkSmartPointer<vtkMaskPoints> mask = vtkSmartPointer<vtkMaskPoints>::New();
	mask->SetInputConnection(threshold->GetOutputPort());
	mask->SetOnRatio(10);
	mask->Update();


	//using vtkArrowSource glyph   TODO - confirm at project specs
	vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
		arrow->SetTipResolution(6);
		arrow->SetTipRadius(0.2);
		arrow->SetTipLength(0.4);
		arrow->SetShaftResolution(6);
		arrow->SetShaftRadius(0.075);

	vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
		glyph->SetInputConnection(threshold->GetOutputPort());
		glyph->SetSourceConnection(arrow->GetOutputPort()); //TODO when i comment out this line, the dots appear, do need arrows though...
		glyph->SetScaleFactor(1.5);
		glyph->SetScaleModeToScaleByScalar();
		glyph->SetVectorModeToUseVector();
		glyph->OrientOn();
		glyph->Update();


	//particle tracing: using particle advection: week 8 lecture 2
		//steps:
			//find cell containing initial position{point location}
			//
			//while particle in grid
				//determine velocity at current position{interpolation}
				//calculate new position{integration}
				//find cell containing new position{point location}
			//end of while

		//TODO continue from week 8 lecture 2 notes from slide 11




		//double range[2];
		//range[0] = threshold->GetOutput()->GetPointData()->GetScalars()->GetRange()[0];
		//range[1] = threshold->GetOutput()->GetPointData()->GetScalars()->GetRange()[1];

		//printf("Range[0] is %f\n", range[0]);
		//printf("Range[1] is %f\n", range[1]);
// 
//





//
//	// Now create a lookup table that consists of the full hue circle
//// (from HSV).
	vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
	lut->SetHueRange(.667, 0.0);
	lut->Build();

					 
	vtkSmartPointer<vtkPolyDataMapper> vecMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vecMapper->SetInputConnection(glyph->GetOutputPort());
	//vecMapper->SetScalarRange(range[0], range[1]);
	vecMapper->SetLookupTable(lut);
	

	
			
vtkSmartPointer<vtkActor> streamerActor = vtkSmartPointer<vtkActor>::New();
                            streamerActor->SetMapper(vecMapper);



	//	// An outline provides context around the data.
//	//
	vtkSmartPointer<vtkOutlineFilter> outlineData =
		vtkSmartPointer<vtkOutlineFilter>::New();
	outlineData->SetInputConnection(v16->GetOutputPort());


	vtkSmartPointer<vtkPolyDataMapper> outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	outlineMapper->SetInputConnection(outlineData->GetOutputPort());

	vtkSmartPointer<vtkActor> outlineActor = vtkSmartPointer<vtkActor>::New();
	outlineActor->SetMapper(outlineMapper);
	outlineActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());



 



	// Setup the text and add it to the renderer	//added per assignment 3 step 3: scale bars and labels
	vtkSmartPointer<vtkTextActor> textActor = vtkSmartPointer<vtkTextActor>::New();
	textActor->SetInput("3D Vector Visualization: Velocity:");	//title
	textActor->SetPosition(200, 40);						//location in the program from the bottom left corner
	textActor->GetTextProperty()->SetFontSize(24);			//font size
	textActor->GetTextProperty()->SetColor(1.0, 1.0, 1.0);	//color of text

	//
	////adding axes - assignment 3 part 3 
	vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();

	//x axis
	axes->GetXAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
	axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);	
	axes->SetXAxisLabelText("X");
	axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(15);

	//y axis
	axes->GetYAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
	axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);
	axes->SetYAxisLabelText("Y");
	axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(15);

	//z axis
	axes->GetZAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
	axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);
	axes->SetZAxisLabelText("Z");
	axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(15);
	
	
	axes->SetTotalLength(5.0, 5.0, 5.0);	//setting the length of the x, y, z axes


	//scalar bar added to scene: 
	vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();

	scalarBar->UnconstrainedFontSizeOn();
	scalarBar->SetTitle("Velocity"); //scalar bar label
	scalarBar->SetNumberOfLabels(4);	//number of labels displayed on the scalar bar

	scalarBar->SetLookupTable(lut);
	scalarBar->GetPositionCoordinate()->SetValue(.125, .125);

	scalarBar->SetWidth(0.125);	//bar width
	scalarBar->SetHeight(0.75);	//bar heigth




	

	// Now we are creating three orthogonal planes passing through the
	// volume. Each plane uses a different texture map and therefore has
	// different coloration.




	// It is convenient to create an initial view of the data. The FocalPoint
	// and Position form a vector direction. Later on (ResetCamera() method)
	// this vector is used to position the camera to look at the data in
	// this direction.
	vtkSmartPointer<vtkCamera> aCamera =
		vtkSmartPointer<vtkCamera>::New();
	aCamera->SetViewUp(0, 1, 0); //changed per week 7 lab 1 slide 15
	aCamera->SetPosition(0, 0, 1); //changed per week 7 lab 1 slide 15
	aCamera->SetFocalPoint(0, 0, 0);
	aCamera->ComputeViewPlaneNormal();
	aCamera->Azimuth(0.0); //changed per week 7 lab 1 slide 15
	aCamera->Elevation(0.0);//changed per week 7 lab 1 slide 15
	aRenderer->SetActiveCamera(aCamera);
	// Actors are added to the renderer. An initial camera view is created.
	// The Dolly() method moves the camera towards the FocalPoint,
	// thereby enlarging the image.
	
	
	
	//assignment 3 
	//ISO-SURFACE uncomment the below line along with any of the skin methods
	//aRenderer->AddActor(skin); //uncomment for isosurface display



	//assignment 3 
	////PROBING: have the below 3 lines uncommented, along with the colors and sagittal, axial, and coronal methods
	//aRenderer->AddActor(sagittal);
	//aRenderer->AddActor(axial);
	//aRenderer->AddActor(coronal);

	
	//labels and titles and annotations
	aRenderer->AddActor(textActor);
	aRenderer->AddActor(axes);
	aRenderer->AddActor(scalarBar);
	
	aRenderer->AddActor(outlineActor);
	aRenderer->AddActor(streamerActor);
	//aRenderer->AddActor(glyphActor);
	aRenderer->SetBackground(colors->GetColor3d("Gray").GetData());
	renWin->SetSize(640, 480);
	
	aRenderer->SetActiveCamera(aCamera);

	aRenderer->ResetCamera();
	aCamera->Dolly(1.0); //changed per week 7 lab 1 slide 15


	// Note that when camera movement occurs (as it does in the Dolly()
	// method), the clipping planes often need adjusting. Clipping planes
	// consist of two planes: near and far along the view direction. The
	// near plane clips out objects in front of the plane; the far plane
	// clips out objects behind the plane. This way only what is drawn
	// between the planes is actually rendered.
	aRenderer->ResetCameraClippingRange();
	
	// Initialize the event loop and then start it.
	iren->Initialize();
	renWin->Render();
	iren->Start();

	return EXIT_SUCCESS;
}
