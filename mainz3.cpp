
#include "stdafx.h"//把所有关于vtk 的头文件都放到stdafx.h中了

//#include "vtkUnstructuredGridMapper.h" 
void maine()
{


	FILE *Datai = fopen("D:\\右心室整型.txt","rt");		//D:\\2DpointDatas.txt 右心室整型.txt Rightventricle.txt
	
	int PointCount=0;						//_计算坐标点的个数
	//double Bounds[6];
	
	vtkPolyData *polydata = vtkPolyData::New(); //_以点坐标形式存储数据
	vtkPoints *points = vtkPoints::New();		//_用于装入坐标点的数组

	while(!feof(Datai))
	{
		//_fprintf(stdout,"reading points: %i\r", vtkPointCount);
		float x, y, z;							//_用于临时存储坐标的变量
		fscanf(Datai,"%f %f %f\n", &x, &y, &z);
		points->InsertPoint(PointCount++, x/1000, y/1000, z/1000);
	}
	fclose(Datai);
	

	polydata->SetPoints(points);				//_将所有点坐标放入Data中
	vtkCellArray *cell = vtkCellArray::New();
	int iNum = points->GetNumberOfPoints();		//_计算做标数
	//for(int i=0;i<iNum;i++)
	//	cell->InsertNextCell(1,&i);				//_输入单元数量

	polydata->SetPolys(cell);
	//polydata->GetBounds(Bounds);				//??什么意思不明白
												/*for (int j=0;j<6;j++)
												{
												cout<<Bounds[j]<<endl;
												}*/
	
	
	///////////////////////////////////////////////////////////////
// 	vtkPolyDataMapper *polydataMapper = vtkPolyDataMapper::New();
// 	polydataMapper->SetInput(polydata);
// 	
// 	
// 	//_设置actor
// 	vtkActor *actor = vtkActor::New();
// 	actor->SetMapper(polydataMapper);
// 	actor->GetProperty()->SetPointSize(3);
// 	actor->GetProperty()->SetRepresentationToPoints();
// 	actor->GetProperty()->SetColor(0,0,1);
	
	//_点连接
	vtkSurfaceReconstructionFilter *Surffilter = vtkSurfaceReconstructionFilter::New();
	Surffilter->SetInputData(polydata);
	
	//_图形轮廓过滤
	vtkContourFilter *Confilter = vtkContourFilter::New();
	Confilter->SetInputData((vtkDataSet*)Surffilter->GetOutput());
	//Confilter->SetValue(0,0.0);
	
	//_表面布局
	vtkDecimatePro *Decimatepro = vtkDecimatePro::New();
	Decimatepro->SetInputData(Confilter->GetOutput());
	Decimatepro->SetTargetReduction(0.9);
	Decimatepro->PreserveTopologyOn();
	
	//_表面光滑处理
	vtkSmoothPolyDataFilter *Smoothfilter = vtkSmoothPolyDataFilter::New();
	Smoothfilter->SetInputData(Confilter->GetOutput());
	Smoothfilter->SetNumberOfIterations(50);
	
	//_？？？？？？？
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
	
	//_渲染
	vtkRenderer *ren = vtkRenderer::New();
	//ren->AddActor(actor);
	ren->AddActor(actor2);
	
	//_渲染窗口 
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->SetSize(800,800);
	renWin->AddRenderer(ren);
	
	
	//渲染窗口交互
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	
	renWin->Render();
	iren->Start();
}