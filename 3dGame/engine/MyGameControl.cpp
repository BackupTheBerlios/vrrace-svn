#include "MyGameControl.h"

MyGameControl::MyGameControl(void)
{
	m_initCount		= 0;
	m_pView			= new MyView();
	m_bShowStatus	= false;
}

MyGameControl::~MyGameControl(void)
{
	delete m_pView;
}

bool	MyGameControl::loadObjects()
{
	if (m_initCount > 0)
	{
		m_initCount++;
		for (int count = 0; count < m_pMeshes.capacity(); count++)
		{
			m_pMeshes[count]->load();
		}
	} else {
		return false;
	}
	return true;
}

bool	MyGameControl::drawObjects(D3DXMATRIX* givenMatWorld)
{
	for (int count = 0; count < m_pMeshes.capacity(); count++)
	{
           D3DXMatrixTranslation(
			givenMatWorld,
			m_pMeshes[count]->getPosition()->x,
			m_pMeshes[count]->getPosition()->y,
			m_pMeshes[count]->getPosition()->z
			);
			_D3DDevice->SetTransform(D3DTS_WORLD, givenMatWorld);
			m_pMeshes[count]->draw();
	}

	return true;
}

bool	MyGameControl::moveObjects()
{
	for (int count = 0; count < m_pMeshes.capacity(); count++)
	{
		m_pMeshes[count]->move();
	}
	
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

	this->initObjects();

	return true;
}

bool	MyGameControl::initObjects()
{
		/*MyMesh*	temp	= new MyMesh();

		temp->init(_D3DDevice, "resources/shusui.x", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.01f);
*/
/*		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/shusui.x", 0.0f, 0.0f, 0.0f, 0.01f, 0.0f, -0.05f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/star sail.x", 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, -0.051f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 2.x", 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, -0.01f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 5.x", 0.0f, 0.0f, 1.0f, 0.0f, 0.01f, -0.01f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 8.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 13.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 8.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 13.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 8.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 13.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 8.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 13.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 8.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		m_pMeshes.push_back(new MyMesh(_D3DDevice, "resources/spaceship 13.x", 1.0f, 1.0f, 1.0f, 0.0f, 0.05f, 0.0f));
		
		/*
		MyMesh*	temp2	= new MyMesh();
		temp2->init(_D3DDevice, "resources/shusui.x", 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, -0.01f);

		m_pMeshes.push_back(temp2);
*/

	return true;
}

void	MyGameControl::addObject()
{
	m_pMeshes.push_back(new MyMesh());
	m_pMeshes[m_pMeshes.capacity()-1]->init(_D3DDevice, "resources/shusui.x", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.05f);
	m_pMeshes[m_pMeshes.capacity()-1]->load();
}

