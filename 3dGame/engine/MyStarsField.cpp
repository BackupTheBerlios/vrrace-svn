#include "MyStarsField.h"
#include "includes.h"

MyStarsField::MyStarsField(void)
{
	m_pPosition->setValues(0.0f, 0.0f, 0.0f);
	m_pDirection->setValues(0.0f, 0.0f, 0.0f);
	m_pRotation->setValues(0.0f, 0.0f, 0.0f);
	m_pRotDir->setValues(0.0f, 0.0f, 0.0f);
	m_pScaleFactor->setValues(1.0f, 1.0f, 1.0f);
}

MyStarsField::~MyStarsField(void)
{
	MyTools::deleteArray(m_pStars);
}

bool MyStarsField::init(LPDIRECT3DDEVICE9 givenDevice,
								D3DXMATRIX* givenMatWorld,
								float posX,
								float posY,
								float posZ,
								float dirX,
								float dirY,
								float dirZ,
								float rotX,
								float rotY,
								float rotZ,
								float rotDirX,
								float rotDirY,
								float rotDirZ,
								int numberStars)
{
	if (givenDevice != NULL)
	{
		_D3DDevice			= givenDevice;
	} else {
		return false;
	}

	if (givenMatWorld != NULL)
	{
		_matWorld			= givenMatWorld;
	} else {
		return false;
	}

	m_iNumberStars = numberStars;

	m_pPosition->setValues(posX, posY, posZ);
	m_pDirection->setValues(dirX, dirY, dirZ);
	m_pRotation->setValues(rotX, rotY, rotZ);
	m_pRotDir->setValues(rotDirX, rotDirY, rotDirZ);

	m_pStars = new CUSTOMVERTEX[(size_t)m_iNumberStars];

	srand( (unsigned)time( NULL ) );

	for(int count = 0; count < m_iNumberStars; count++)
	{
		m_pStars[count].x	= (float)pow((double)-1.0f,(double)count) * (-1.0f + (1.0f + 1.0f) * ((float)(rand() % 10001) / 10000.0f)) * 10000.0f;//(float)pow((double)-1.0f, (double)count) * (float)rand() / 100.0f;
		m_pStars[count].y	= (float)pow((double)-1.0f,(double)count) * (-1.0f + (1.0f + 1.0f) * ((float)(rand() % 10001) / 10000.0f)) * 10000.0f;
		m_pStars[count].z	= (float)pow((double)-1.0f,(double)count) * (-1.0f + (1.0f + 1.0f) * ((float)(rand() % 10001) / 10000.0f)) * 10000.0f;
		/*char temp[118];
		sprintf(temp,"%f, %f, %f",m_pStars[count].x,m_pStars[count].y,m_pStars[count].z);
		MessageBox(NULL,temp,"Message",MB_OK);*/
		m_pStars[count].dwColor	= D3DCOLOR_XRGB(255, 255, 255);
	}

	return true;
}

bool MyStarsField::draw(void)
{
	_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	_D3DDevice->SetTexture(0, NULL);

    this->transform();

	_D3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	_D3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST/*LINELIST*/, m_iNumberStars, m_pStars, sizeof(CUSTOMVERTEX));

	//_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	return true;
}
