// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#include "vtkActor.h"
#include "vtkAxes.h"
#include "vtkContourFilter.h"
#include "vtkDataSet.h"
#include "vtkFloatArray.h"
#include "vtkGaussianSplatter.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleTrackball.h"
#include "vtkRenderer.h"
#include "vtkTubeFilter.h"
#include "vtkUnstructuredGrid.h"
//#include "vtkTestUtilities.h"
#include "vtkConeSource.h"
#include "vtkCamera.h"
#include "vtkCellArray.h"
#include "vtkInteractorStyle.h"
#include "vtkMatrix4x4.h"

#include "vtkWin32RenderWindowInteractor.h"
#include "vtkDelaunay2D.h "
#include "vtkDelaunay3D.h"
#include "vtkShrinkFilter.h"
#include "vtkDataSetMapper.h"
#include "vtkAxesActor.h"

#include "vtkSurfaceReconstructionFilter.h"
#include "vtkContourFilter.h"
#include "vtkDecimatePro.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkPolyDataNormals.h"

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#include <stdio.h>
#include <tchar.h>
#include <conio.h>



// TODO: 在此处引用程序需要的其他头文件
