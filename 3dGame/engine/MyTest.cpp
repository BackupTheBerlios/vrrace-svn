#include "MyTest.h"

void MyTest::drawKS(LPDIRECT3DDEVICE9 givenDevice)
{
	int max		= 100;
	const int maxv	= 1200;
	int counter	= 0;
	CUSTOMVERTEX Vertices[maxv];
	HRESULT hresult;
	
	for (int count = 0; count < max; count++)
	{
		Vertices[counter].x	= 0;
		Vertices[counter].y	= 0;
		Vertices[counter].z = (float)count - (float)max/2;
		counter++;
		Vertices[counter].x	= (float)max-1;
		Vertices[counter].y	= 0;
		Vertices[counter].z = (float)count - (float)max/2;
		counter++;

		Vertices[counter].x	= (float)count - (float)max/2;
		Vertices[counter].y	= 0;
		Vertices[counter].z = 0;
		counter++;
		Vertices[counter].x	= (float)count - (float)max/2;
		Vertices[counter].y	= 0;
		Vertices[counter].z = (float)max-1;
		counter++;

		Vertices[counter].x	= 0;
		Vertices[counter].y	= (float)count - (float)max/2;
		Vertices[counter].z = 0;
		counter++;
		Vertices[counter].x	= (float)max-1;
		Vertices[counter].y	= (float)count - (float)max/2;
		Vertices[counter].z = 0;
		counter++;

		Vertices[counter].x	= (float)count - (float)max/2;
		Vertices[counter].y	= 0;
		Vertices[counter].z = 0;
		counter++;
		Vertices[counter].x	= (float)count - (float)max/2;
		Vertices[counter].y	= (float)max-1;
		Vertices[counter].z = 0;
		counter++;

		Vertices[counter].x	= 0;
		Vertices[counter].y	= (float)count - (float)max/2;
		Vertices[counter].z = 0;
		counter++;
		Vertices[counter].x	= 0;
		Vertices[counter].y	= (float)count - (float)max/2;
		Vertices[counter].z = (float)max-1;
		counter++;

		Vertices[counter].x	= 0;
		Vertices[counter].y	= 0;
		Vertices[counter].z = (float)count - (float)max/2;
		counter++;
		Vertices[counter].x	= 0;
		Vertices[counter].y	= (float)max-1;
		Vertices[counter].z = (float)count - (float)max/2;
		counter++;

	}
	
	if (FAILED(hresult = givenDevice->SetFVF(D3DFVF_XYZ)))
	{
	}
	if (FAILED(hresult = givenDevice->DrawPrimitiveUP(D3DPT_LINELIST, max*6, Vertices, sizeof(CUSTOMVERTEX))))
	{
	}
}
