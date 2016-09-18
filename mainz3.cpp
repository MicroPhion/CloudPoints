
#include "stdafx.h"//�����й���vtk ��ͷ�ļ����ŵ�stdafx.h����

//#include "vtkUnstructuredGridMapper.h" 
void maine()
{


	FILE *Datai = fopen("D:\\����������.txt","rt");		//D:\\2DpointDatas.txt ����������.txt Rightventricle.txt
	
	int PointCount=0;						//_���������ĸ���
	//double Bounds[6];
	
	vtkPolyData *polydata = vtkPolyData::New(); //_�Ե�������ʽ�洢����
	vtkPoints *points = vtkPoints::New();		//_����װ������������

	while(!feof(Datai))
	{
		//_fprintf(stdout,"reading points: %i\r", vtkPointCount);
		float x, y, z;							//_������ʱ�洢����ı���
		fscanf(Datai,"%f %f %f\n", &x, &y, &z);
		points->InsertPoint(PointCount++, x/1000, y/1000, z/1000);
	}
	fclose(Datai);
	

	polydata->SetPoints(points);				//_�����е��������Data��
	vtkCellArray *cell = vtkCellArray::New();
	int iNum = points->GetNumberOfPoints();		//_����������
	//for(int i=0;i<iNum;i++)
	//	cell->InsertNextCell(1,&i);				//_���뵥Ԫ����

	polydata->SetPolys(cell);
	//polydata->GetBounds(Bounds);				//??ʲô��˼������
												/*for (int j=0;j<6;j++)
												{
												cout<<Bounds[j]<<endl;
												}*/
	
	
	///////////////////////////////////////////////////////////////
// 	vtkPolyDataMapper *polydataMapper = vtkPolyDataMapper::New();
// 	polydataMapper->SetInput(polydata);
// 	
// 	
// 	//_����actor
// 	vtkActor *actor = vtkActor::New();
// 	actor->SetMapper(polydataMapper);
// 	actor->GetProperty()->SetPointSize(3);
// 	actor->GetProperty()->SetRepresentationToPoints();
// 	actor->GetProperty()->SetColor(0,0,1);
	
	//_������
	vtkSurfaceReconstructionFilter *Surffilter = vtkSurfaceReconstructionFilter::New();
	Surffilter->SetInputData(polydata);
	
	//_ͼ����������
	vtkContourFilter *Confilter = vtkContourFilter::New();
	Confilter->SetInputData((vtkDataSet*)Surffilter->GetOutput());
	//Confilter->SetValue(0,0.0);
	
	//_���沼��
	vtkDecimatePro *Decimatepro = vtkDecimatePro::New();
	Decimatepro->SetInputData(Confilter->GetOutput());
	Decimatepro->SetTargetReduction(0.9);
	Decimatepro->PreserveTopologyOn();
	
	//_����⻬����
	vtkSmoothPolyDataFilter *Smoothfilter = vtkSmoothPolyDataFilter::New();
	Smoothfilter->SetInputData(Confilter->GetOutput());
	Smoothfilter->SetNumberOfIterations(50);
	
	//_��������������
	vtkPolyDataNormals *polydatanormal = vtkPolyDataNormals::New();
	polydatanormal->SetInputData(Smoothfilter->GetOutput());
	polydatanormal->FlipNormalsOn();
	
	vtkPolyDataMapper *polydataMapper2 = vtkPolyDataMapper::New();
	polydataMapper2->SetInputData(polydatanormal->GetOutput());
	polydataMapper2->ScalarVisibilityOff();
	
	vtkActor *actor2 = vtkActor::New();
	actor2->SetMapper(polydataMapper2);
	//actor2->GetProperty()->SetRepresentationToWireframe();
	actor2->GetProperty()->SetRepresentationToSurface();
	actor2->GetProperty()->SetColor(0,0,1) ;
	
	//_��Ⱦ
	vtkRenderer *ren = vtkRenderer::New();
	//ren->AddActor(actor);
	ren->AddActor(actor2);
	
	//_��Ⱦ���� 
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->SetSize(800,800);
	renWin->AddRenderer(ren);
	
	
	//��Ⱦ���ڽ���
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	
	renWin->Render();
	iren->Start();
}