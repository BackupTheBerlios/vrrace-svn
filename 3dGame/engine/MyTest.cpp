#include "MyTest.h"

void MyTest::drawKS(LPDIRECT3DDEVICE9 givenDevice)
{
	int counter	= 0;
	CUSTOMVERTEX Vertices[120];
	HRESULT hresult;
	
	for (int count = 0; count < 10; count++)
	{
		Vertices[counter].x	= 0;
		Vertices[counter].y	= 0;
		Vertices[counter].z = count;
		counter++;
		Vertices[counter].x	= 9;
		Vertices[counter].y	= 0;
		Vertices[counter].z = count;
		counter++;

		Vertices[counter].x	= count;
		Vertices[counter].y	= 0;
		Vertices[counter].z = 0;
		counter++;
		Vertices[counter].x	= count;
		Vertices[counter].y	= 0;
		Vertices[counter].z = 9;
		counter++;

		Vertices[counter].x	= 0;
		Vertices[counter].y	= count;
		Vertices[counter].z = 0;
		counter++;
		Vertices[counter].x	= 9;
		Vertices[counter].y	= count;
		Vertices[counter].z = 0;
		counter++;

		Vertices[counter].x	= count;
		Vertices[counter].y	= 0;
		Vertices[counter].z = 0;
		counter++;
		Vertices[counter].x	= count;
		Vertices[counter].y	= 9;
		Vertices[counter].z = 0;
		counter++;

		Vertices[counter].x	= 0;
		Vertices[counter].y	= count;
		Vertices[counter].z = 0;
		counter++;
		Vertices[counter].x	= 0;
		Vertices[counter].y	= count;
		Vertices[counter].z = 9;
		counter++;

		Vertices[counter].x	= 0;
		Vertices[counter].y	= 0;
		Vertices[counter].z = count;
		counter++;
		Vertices[counter].x	= 0;
		Vertices[counter].y	= 9;
		Vertices[counter].z = count;
		counter++;

	}
	
	if (FAILED(hresult = givenDevice->SetFVF(D3DFVF_XYZ)))
	{
	}
	if (FAILED(hresult = givenDevice->DrawPrimitiveUP(D3DPT_LINELIST, 60, Vertices, sizeof(CUSTOMVERTEX))))
	{
	}
}
