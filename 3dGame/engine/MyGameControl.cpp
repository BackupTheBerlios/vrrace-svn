#include "MyGameControl.h"

MyGameControl::MyGameControl(void)
{
	m_initCount		= 0;
	m_pView			= new MyView();
	m_bShowStatus	= false;
	m_pShip			= new MyMesh();
}

MyGameControl::~MyGameControl(void)
{
	delete m_pView;
	delete m_pShip;
}

bool	MyGameControl::loadObjects()
{
	if (m_initCount > 0)
	{
		m_initCount++;
	} else {
		return false;
	}

	m_pShip->load();

	return true;
}

bool	MyGameControl::drawObjects(D3DXMATRIX* givenMatWorld)
{
	if (m_initCount < 2)
	{
		return false;
	}

	CUSTOMVERTEX*	_tempPosition	= m_pShip->getPosition();	//Pointer zurueckgeben macht keinen sinn, hebelt private wieder aus
	//---------------------------------------------------------------------------
	D3DXMatrixTranslation(givenMatWorld, 10, 10, 10);//vermutlich falsche stelle oder matWorld is nich korrekt, funzt ned
	//---------------------------------------------------------------------------
	m_pShip->draw();
	
	return true;
}

bool	MyGameControl::init(LPDIRECT3DDEVICE9 givenDevice)
{
	if (m_initCount == 0)
	{
		m_initCount++;
	} else {
		return false;
	}

	_D3DDevice				= givenDevice;

	m_pView->m_Position.x	= 0.0f;
	m_pView->m_Position.y	= 1.0f;
	m_pView->m_Position.z	= 0.0f;

	m_pView->m_UpVector.x	= 0.0f;
	m_pView->m_UpVector.y	= 1.0f;
	m_pView->m_UpVector.z	= 0.0f;

	m_pView->m_ViewPoint.x	= 0.0f;
	m_pView->m_ViewPoint.y	= 0.0f;
	m_pView->m_ViewPoint.z	= 0.0f;

	m_pShip->init(_D3DDevice);

	return true;
}
