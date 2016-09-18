// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
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

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#include <stdio.h>
#include <tchar.h>
#include <conio.h>



// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
