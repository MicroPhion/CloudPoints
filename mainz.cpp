#include "stdafx.h"//�����й���vtk ��ͷ�ļ����ŵ�stdafx.h����

void mainz()
{
	vtkPoints *points = vtkPoints::New();
	vtkPolyData *polyData = vtkPolyData::New();
	vtkPolyDataMapper *pointMapper = vtkPolyDataMapper::New();
	vtkDelaunay2D   *delny=vtkDelaunay2D:: New() ;					//_����ɢ�����delaunay_���ǻ�
	vtkActor *pointActor = vtkActor::New();
	vtkRenderer *ren1= vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	vtkInteractorStyleTrackball *istyle = vtkInteractorStyleTrackball::New();


	FILE  *fp; fp=fopen("D:\\����������.txt","r");	//Rightventricle.txt  2DpointDatas.tx
	float x,y,z; int i = 0; 
	while (!feof(fp))  
	{  
		fscanf (fp,"%f%f%f\n" ,&x,&y,&z) ;  
		//points->InsertNextPoint( x, y,z ) ;	//��vtkDelaunay2D���Թ�����������һ�־�����ȫƽ����������Z����Ϊ���ⳣ����
		points->InsertPoint(i,x,y,z);			//�ڶ��־�����������ά�ģ�������Ҳ����ά�ģ�vtkDelaunay3D����Ten������
		i++;	
	}  
	fclose(fp);


	//---------------------------2D����-----------------------
	polyData->SetPoints(points) ;
	delny->SetInputData(polyData);  
	delny->SetTolerance(0.001);  
	pointMapper->SetInputData(delny->GetOutput()) ; 
	pointActor->SetMapper(pointMapper);  

	pointActor->GetProperty()->SetColor( 1,0,0) ;
	//pointActor->GetProperty()->SetRepresentationToWireframe();
	pointActor->GetProperty()->SetRepresentationToSurface();

	renWin->AddRenderer(ren1) ; 
	iren->SetRenderWindow(renWin);  //����
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