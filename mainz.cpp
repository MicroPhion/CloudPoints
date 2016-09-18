#include "stdafx.h"//把所有关于vtk 的头文件都放到stdafx.h中了

void mainz()
{
	vtkPoints *points = vtkPoints::New();
	vtkPolyData *polyData = vtkPolyData::New();
	vtkPolyDataMapper *pointMapper = vtkPolyDataMapper::New();
	vtkDelaunay2D   *delny=vtkDelaunay2D:: New() ;					//_对离散点进行delaunay_三角化
	vtkActor *pointActor = vtkActor::New();
	vtkRenderer *ren1= vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	vtkInteractorStyleTrackball *istyle = vtkInteractorStyleTrackball::New();


	FILE  *fp; fp=fopen("D:\\右心室整型.txt","r");	//Rightventricle.txt  2DpointDatas.tx
	float x,y,z; int i = 0; 
	while (!feof(fp))  
	{  
		fscanf (fp,"%f%f%f\n" ,&x,&y,&z) ;  
		//points->InsertNextPoint( x, y,z ) ;	//用vtkDelaunay2D可以构两种网：第一种就是完全平面网，设置Z坐标为任意常数；
		points->InsertPoint(i,x,y,z);			//第二种就是数据是三维的，构的网也是三维的，vtkDelaunay3D是用Ten构网了
		i++;	
	}  
	fclose(fp);


	//---------------------------2D建网-----------------------
	polyData->SetPoints(points) ;
	delny->SetInputData(polyData);  
	delny->SetTolerance(0.001);  
	pointMapper->SetInputData(delny->GetOutput()) ; 
	pointActor->SetMapper(pointMapper);  

	pointActor->GetProperty()->SetColor( 1,0,0) ;
	//pointActor->GetProperty()->SetRepresentationToWireframe();
	pointActor->GetProperty()->SetRepresentationToSurface();

	renWin->AddRenderer(ren1) ; 
	iren->SetRenderWindow(renWin);  //交互
	ren1->AddActor(pointActor) ;
	ren1->SetBackground( 0.2,1,0.2 ) ;  
	renWin->SetSize(800,800); 


	renWin->Render();  
	iren->Start() ;


	points->Delete();
	polyData->Delete();
	pointMapper->Delete();
	renWin->Delete();
	iren->Delete();
	delny->Delete();
	pointActor->Delete();
	istyle->Delete();
	ren1->Delete();
}