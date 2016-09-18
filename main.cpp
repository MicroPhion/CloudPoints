#include "stdafx.h"
#include "ply/ply.h"
#include "MxImage.h"
#include "MxIsoSurface.h"
#include "SurfaceRendering.h"
#include "MxOpenGL.h"
#include "MxImageIO.h"
#include "MxFileIO.h"
typedef struct Vertex {
	float x,y,z;
	float r,g,b;
	float nx,ny,nz;
	void *other_props;       /* other properties */
} Vertex;

typedef struct Face {
	unsigned char nverts;    /* number of vertex indices in list */
	int *verts;              /* vertex index list */
	void *other_props;       /* other properties */
} Face;

char *elem_names[] = { /* list of the elements in the object */
	"vertex", "face"
};

PlyProperty vert_props[] = { /* list of property information for a vertex */
	{"x", Float32, Float32, offsetof(Vertex,x), 0, 0, 0, 0},
	{"y", Float32, Float32, offsetof(Vertex,y), 0, 0, 0, 0},
	{"z", Float32, Float32, offsetof(Vertex,z), 0, 0, 0, 0},
	{"r", Float32, Float32, offsetof(Vertex,r), 0, 0, 0, 0},
	{"g", Float32, Float32, offsetof(Vertex,g), 0, 0, 0, 0},
	{"b", Float32, Float32, offsetof(Vertex,b), 0, 0, 0, 0},
	{"nx", Float32, Float32, offsetof(Vertex,nx), 0, 0, 0, 0},
	{"ny", Float32, Float32, offsetof(Vertex,ny), 0, 0, 0, 0},
	{"nz", Float32, Float32, offsetof(Vertex,nz), 0, 0, 0, 0},
};

PlyProperty face_props[] = { /* list of property information for a face */
	{"vertex_indices", Int32, Int32, offsetof(Face,verts),
	1, Uint8, Uint8, offsetof(Face,nverts)},
};


/*** the PLY object ***/

static int nverts,nfaces;
static Vertex **vlist;
static Face **flist;

static PlyOtherProp *vert_other,*face_other;

static int per_vertex_color = 0;
static int has_normals = 0;
/******************************************************************************
Read in the PLY file from standard in.
******************************************************************************/

void read_file(FILE* file)
{
	int i,j;
	int elem_count;
	char *elem_name;
	PlyFile *in_ply;

	/*** Read in the original PLY object ***/

	in_ply  = read_ply (file);

	for (i = 0; i < in_ply->num_elem_types; i++) {

		/* prepare to read the i'th list of elements */
		elem_name = setup_element_read_ply (in_ply, i, &elem_count);

		if (equal_strings ("vertex", elem_name)) {

			/* create a vertex list to hold all the vertices */
			vlist = (Vertex **) malloc (sizeof (Vertex *) * elem_count);
			nverts = elem_count;

			/* set up for getting vertex elements */

			setup_property_ply (in_ply, &vert_props[0]);
			setup_property_ply (in_ply, &vert_props[1]);
			setup_property_ply (in_ply, &vert_props[2]);

			for (j = 0; j < in_ply->elems[i]->nprops; j++) {
				PlyProperty *prop;
				prop = in_ply->elems[i]->props[j];
				if (equal_strings ("r", prop->name)) {
					setup_property_ply (in_ply, &vert_props[3]);
					per_vertex_color = 1;
				}
				if (equal_strings ("g", prop->name)) {
					setup_property_ply (in_ply, &vert_props[4]);
					per_vertex_color = 1;
				}
				if (equal_strings ("b", prop->name)) {
					setup_property_ply (in_ply, &vert_props[5]);
					per_vertex_color = 1;
				}
				if (equal_strings ("nx", prop->name)) {
					setup_property_ply (in_ply, &vert_props[6]);
					has_normals = 1;
				}
				if (equal_strings ("ny", prop->name)) {
					setup_property_ply (in_ply, &vert_props[7]);
					has_normals = 1;
				}
				if (equal_strings ("nz", prop->name)) {
					setup_property_ply (in_ply, &vert_props[8]);
					has_normals = 1;
				}
			}

			vert_other = get_other_properties_ply (in_ply, 
				offsetof(Vertex,other_props));

			std::cout<<"elem_count==="<<elem_count<<std::endl;
			/* grab all the vertex elements */
			for (j = 0; j < elem_count; j++) {
				vlist[j] = (Vertex *) malloc (sizeof (Vertex));
				vlist[j]->r = 1;
				vlist[j]->g = 1;
				vlist[j]->b = 1;
				get_element_ply (in_ply, (void *) vlist[j]);

			}
		}
		else if (equal_strings ("face", elem_name)) {

			/* create a list to hold all the face elements */
			flist = (Face **) malloc (sizeof (Face *) * elem_count);
			nfaces = elem_count;

			/* set up for getting face elements */

			setup_property_ply (in_ply, &face_props[0]);
			face_other = get_other_properties_ply (in_ply, offsetof(Face,other_props));

			/* grab all the face elements */
			for (j = 0; j < elem_count; j++) {
				flist[j] = (Face *) malloc (sizeof (Face));
				get_element_ply (in_ply, (void *) flist[j]);
			}
		}
		else
			get_other_element_ply (in_ply);
	}

	close_ply (in_ply);
	free_ply (in_ply);
}


/******************************************************************************
Write out the PLY file to standard out.
******************************************************************************/

void write_file(FILE* file)
{
	int i;
	PlyFile *ply;
	int num_elem_types;

	/*** Write out the transformed PLY object ***/
	static int nelems = 2;
	ply = write_ply (file, nelems, elem_names, PLY_ASCII);

	/* describe what properties go into the vertex elements */

	describe_element_ply (ply, "vertex", nverts);
	describe_property_ply (ply, &vert_props[0]);
	describe_property_ply (ply, &vert_props[1]);
	describe_property_ply (ply, &vert_props[2]);
	describe_property_ply (ply, &vert_props[3]);
	describe_property_ply (ply, &vert_props[4]);
	describe_property_ply (ply, &vert_props[5]);

	describe_element_ply (ply, "face", nfaces);
	describe_property_ply (ply, &face_props[0]);

	append_comment_ply (ply, "created by sphereply");

	header_complete_ply (ply);

	/* set up and write the vertex elements */
	put_element_setup_ply (ply, "vertex");
	for (i = 0; i < nverts; i++)
		put_element_ply (ply, (void *) &vlist[i]);

	/* set up and write the face elements */
	put_element_setup_ply (ply, "face");
	for (i = 0; i < nfaces; i++)
		put_element_ply (ply, (void *) &flist[i]);

	close_ply (ply);
	free_ply (ply);
}



/*
* @Time:20160917 Written By MicroPhion
* @Function:Get the Bound box of the vertex, use the bound to set the resolution of the spacing
* @Input: vlist, the list of vertex
          vVertexNum, the number of the vertex
* @Output: fLeftDownPoint, the left down point of the bound box
           fRightUpPoint, the right up point of the bound box
*/
void GetBoundBox(Vertex** vlist, int nVertexNum, double *fLeftDownPoint,double *fRightUpPoint){
	//1.0 Init the bound box
	for (int i = 0; i < 3; i ++){
		fLeftDownPoint[i] = 1000000;
		fRightUpPoint[i] = -1000000;
	}
	//2.0 Get the bound box
	for (int i = 0; i < nVertexNum; i ++){
		double x = vlist[i]->x;
		double y = vlist[i]->y;
		double z = vlist[i]->z;
		if (x < fLeftDownPoint[0]){
			fLeftDownPoint[0] = x;
		}
		if (y < fLeftDownPoint[1]){
			fLeftDownPoint[1] = y;
		}
		if (z < fLeftDownPoint[2]){
			fLeftDownPoint[2] = z;
		}
		if (x > fRightUpPoint[0]){
			fRightUpPoint[0] = x;
		}
		if (y > fRightUpPoint[1]){
			fRightUpPoint[1] = y;
		}
		if (z > fRightUpPoint[2]){
			fRightUpPoint[2] = z;
		}
	}
	//3.0 test, print the information of the volume
	std::cout<<"nverts==="<<nverts<<std::endl;
	std::cout<<"left down point==="<<fLeftDownPoint[0]<<","<<fLeftDownPoint[1]<<","<<fLeftDownPoint[2]<<std::endl;;
	std::cout<<"right up point==="<<fRightUpPoint[0]<<","<<fRightUpPoint[1]<<","<<fRightUpPoint[2]<<std::endl;
	double volume = (fRightUpPoint[0]-fLeftDownPoint[0])*(fRightUpPoint[1]-fLeftDownPoint[1])*(fRightUpPoint[2]-fLeftDownPoint[2]);
	std::cout<<"volume==="<<volume<<std::endl;
	std::cout<<"spacing==="<<volume/(nverts*10)<<std::endl;
}

/*
* @Time: 20160917 Written By MicroPhion
* @Function: build the volume data by cloud points
* @Input: vlist, the list of the vertex
          nVertexNum, the number of the vertex
		  fLeftDownPoint, the left down point of the bound box
		  fRightUpPoint, the right up point of the bound box
* @Output:im, volume data in 3d format
*
*/
void BuildVolumeData(Vertex** vlist, int nVertexNum, double *fLeftDownPoint,double *fRightUpPoint,UCharImage &im){
	//the spacing confirm by the number of the points and boundbox of the points
	double spacing[3] = {0.01,0.01,0.01};
	int size[3] = {1,1,1};
	for (int i = 0; i < 3;i ++){
		size[i] = (fRightUpPoint[i] - fLeftDownPoint[i])/spacing[i]+10;
	}
	int nDataLength = size[0]*size[1]*size[2];
	unsigned char *mask = new unsigned char[nDataLength+1];
	im.Create(size);
	im.SetSpacing(spacing);
	im.Flush(0);
	//TODO we should save the color information here, for example ,we can create an IntImage to save the rgb
	//when we get the surface of the volume data, we should get the rgb from IntImage
	for(int i = 0; i < nVertexNum; i ++){
		int x = (int)((vlist[i]->x-fLeftDownPoint[0])/spacing[0]+0.5);
		int y = (int)((vlist[i]->y-fLeftDownPoint[1])/spacing[1]+0.5);
		int z = (int)((vlist[i]->z-fLeftDownPoint[2])/spacing[2]+0.5);
		int nIndex = x + y * size[0] + z * size[0]*size[1];
		im.SetPixelValue(nIndex,1);
	}
	//std::cout<<"size==="<<size[0]<<","<<size[1]<<","<<size[2]<<std::endl;
}
/*
* @Time: 20160917 Written By MicroPhion
* @Function:a.Render Surface and Save the Frame to Image
* @Input: width, the width of the output image
          height, the height of the output image
		  outputpath, the path of the output image
		  imageNum,the number of the output image
		  surface, the surface want to be rendered
*/


void Create3DImages(int width, int height, std::string outputpath, int imageNum,std::vector<MxSurface*> surface){
	//1.0 Init gl context
	GLUtils::xInitGlut();
	int id = glutCreateWindow("MicroPhion");

	if(wglGetCurrentContext() == NULL)return;

	//2.0 Render the surface
	FloatPoint box[2];
	int fSpaceScope = MxSurface::xGetSpaceScope(surface,box);
	//2.1 set the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-fSpaceScope/2*1.1, fSpaceScope/2*1.1 , -fSpaceScope/2*1.1 , fSpaceScope/2*1.1 , -fSpaceScope , fSpaceScope );
	SurfaceRendering * surfaceRender = new SurfaceRendering();
	surfaceRender->xSetMxSurface(surface);
	for(int i = 0; i < imageNum; i ++){
		//2.2 set the viewmodel matrix
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glRotated(360.0*i/imageNum, 0.0f, 1.0f, 0.0f);//ÈÆYÖáÐý×ª   
		glTranslated(-(box[0].x + box[1].x)/2,-(box[0].y + box[1].y)/2,-(box[0].z + box[1].z)/2);

		MxOpenGL::xglEnable(GL_BLEND);
		surfaceRender->xCreateSRViewData(512,512,1);
		glPopMatrix();
		//2.3 Save the image to file
		IntImage im;
		surfaceRender->xGetSRViewData(&im,1);
		UCharImage im2;
		MxImageIO::Int2RGB(im,&im2);
		std::string path = outputpath+std::string("\\")+MxFileIO::ConvertToString(i)+std::string(".jpg");
		std::cout<<"nodule path ====" <<path<<std::endl;
		MxImageIO::SaveRGBFile(im2,path.c_str());

	}


	SAFEDELETEObject(surfaceRender);


	//3.0 release the gl context
	glutHideWindow();
	glutDestroyWindow(id);
}
/*
* @Time:20160917 Written by Microphion
* @Function: Create the surface from volume image by MarchingCube algorithm
* @Input:image, the volume data in 3d format
* @Output:surface the surface of the volume data
*/
void CreateSurface(UCharImage& image,MxSurface &surface){
	std::cout << image.GetDataLength() << std::endl;
	//1.0 Create the Surface by MarchingCube
	TriangleSurface m_triangles;
	MxIsoSurface<unsigned char> isos;
	int size = image.GetDataLength();
	unsigned char* data = new unsigned char[size];
	memcpy(data,image.GetData(),size);
	isos.SetScalarData(data,image.GetWidth()-1,image.GetHeight()-1,image.GetDepth()-1,2,2,2);
	isos.SetThreshold(0.5);
	isos.Update();
	isos.GetTriangleSurface(m_triangles);
	isos.ClearAllData();
	//2.0 Set the color of the surface
	surface.Copy(m_triangles);
	surface.color[0] = 1;surface.color[1] = 1;surface.color[2] = 0;surface.color[3] = 1;
	//std:;cout<<"m_surface point==="<<m_triangles.pointSize()<<std::endl;

	//3.0 Testing, save the surface to image2d
	std::vector<MxSurface*> surfaces;
	surfaces.push_back(&surface);
	Create3DImages(1024,1024,"D:\\",30,surfaces);
}





/*
* @Time:20160917 Wirtten by MicroPhion
* @Function: render the cloud points by vtk
*/
void ShowCloudPoints(int nverts,Vertex** vlist)
{
	//1.0 Create the Objects
	vtkPoints *m_Points = vtkPoints::New();
	vtkCellArray *vertices = vtkCellArray::New();
	vtkPolyData *polyData = vtkPolyData::New();
	vtkPolyDataMapper *pointMapper = vtkPolyDataMapper::New();
	vtkActor *pointActor = vtkActor::New();
	vtkRenderer *ren1= vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	vtkInteractorStyleTrackball *istyle = vtkInteractorStyleTrackball::New();

	//2.0 put points to vertices
	for (int i = 0; i < nverts; i ++)
	{
		double x = vlist[i]->x;
		double y = vlist[i]->y;
		double z = vlist[i]->z;
		m_Points->InsertPoint(i,x,y,z);	
		vertices->InsertNextCell(1);
		vertices->InsertCellPoint(i);
	}

	//3.0 put the data to Mapper
	polyData->SetPoints(m_Points);	
	polyData->SetVerts(vertices);
	pointMapper->SetInputData(polyData);
	
	//4.0 Set Property of the render
	pointActor->SetMapper( pointMapper );
	pointActor->GetProperty()->SetColor(0.0,0.1,1.0);
	pointActor->GetProperty()->SetAmbient(0.5);
	pointActor->GetProperty()->SetPointSize(2);
	pointActor->GetProperty()->SetRepresentationToWireframe();

	//5.0 Render
	ren1->AddActor( pointActor );
	ren1->SetBackground( 0, 0, 0);
	renWin->AddRenderer( ren1 );
	renWin->SetSize(800,800);

	iren->SetInteractorStyle(istyle);  
	iren->SetRenderWindow(renWin); 

	renWin->Render();
	iren->Start();

	//6.0 Release the Objects
	m_Points->Delete();
	vertices->Delete();
	polyData->Delete();
	pointMapper->Delete();
	pointActor->Delete();
	ren1->Delete();
	renWin->Delete();
	iren->Delete();
	istyle->Delete();
}



void main(){
	//1.0 Read Ply File 
	std::string path = "D:\\densify.ply";
	FILE *file = fopen(path.c_str(), "r");
	read_file(file);


	//2.0 Convert Cloud Points to Volume Data
	//2.1 Get the BoundBox from Cloud Points
	double fLeftDown[3],fRightUp[3];
	GetBoundBox(vlist,nverts,fLeftDown,fRightUp);
	UCharImage image;
	//2.2 Set Resolution Spacings
	//2.3 Create the Volume Data
	BuildVolumeData(vlist,nverts,fLeftDown,fRightUp,image);

	//3.0 Build Surfaces By MarchingCube 
	MxSurface surface;
	CreateSurface(image,surface);

	//4.0 Save the Surface to File
	double *sp = image.GetSpacing();
	for (int i = 0; i < surface.pointSize(); i ++){
		surface.points[i] = surface.points[i].Spacing(sp) + FloatPoint(fLeftDown[0],fLeftDown[1],fLeftDown[2]);
	}
	surface.Save("D:\\1.obj");

}