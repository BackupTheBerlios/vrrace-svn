#include "MyView.h"

MyView::MyView(void)
{
	m_pPosition		= new MyVertex();
	m_pLocalPos		= new MyVertex();
	m_pViewPoint	= new MyVertex();
	m_pUpVector		= new MyVertex();
	m_pMaster		= NULL;
}

MyVertex*	MyView::getLP()
{
	return m_pLocalPos;
}

MyVertex*	MyView::getPos()
{
	return m_pPosition;
}

MyVertex*	MyView::getVP()
{
	return m_pViewPoint;
}

MyVertex*	MyView::getUV()
{
	return m_pUpVector;
}

MyView::~MyView(void)
{
	delete m_pPosition;
	delete m_pLocalPos;
	delete m_pViewPoint;
	delete m_pUpVector;

	m_pPosition		= NULL;
	m_pLocalPos		= NULL;
	m_pViewPoint	= NULL;
	m_pUpVector		= NULL;
	m_pMaster		= NULL;
}

void	MyView::setMaster(MyMasterPosition* givenMaster)
{
	m_pMaster	= givenMaster;
	m_pViewPoint->setPValues(
		m_pMaster->getAbsolutePosition()->getPX(),
		m_pMaster->getAbsolutePosition()->getPY(),
		m_pMaster->getAbsolutePosition()->getPZ());
}

MyMasterPosition*	MyView::getMaster()
{
	return m_pMaster;
}

void	MyView::move()
{
	D3DXMATRIX	tempTransLP;
	D3DXMATRIX	tempTransUV;
	D3DXMATRIX	tempTransb;
	MyVertex	tempMesh;
	D3DXVECTOR4*	pOut	= new D3DXVECTOR4();

	if (m_pMaster != NULL)
	{

	//wenn sich das schiff in eine richtung bewegt, muss die camera hinterherfliegen

		//eigendlich sollte der localplayer die position anpassen
		D3DXMatrixTranslation(
			&tempTransLP,
			m_pLocalPos->getX(),
			m_pLocalPos->getY(),
			m_pLocalPos->getZ()
			);

		D3DXMatrixTranslation(
			&tempTransUV,
			m_pUpVector->getX(),
			m_pUpVector->getY(),
			m_pUpVector->getZ()
			);

		D3DXMatrixMultiply(
			&tempTransb,
			&tempTransLP,
			m_pMaster->getRotationMatrix());

		D3DXVec3Transform(pOut, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &tempTransb);
		tempMesh.setX(pOut->x);
		tempMesh.setY(pOut->y);
		tempMesh.setZ(pOut->z);

		m_pPosition->setValues(
			m_pMaster->getAbsolutePosition()->getX() + tempMesh.getX(), 
			m_pMaster->getAbsolutePosition()->getY() + tempMesh.getY(), 
			m_pMaster->getAbsolutePosition()->getZ() + tempMesh.getZ());
//UPVECTORBERECHNUNG
//wird hier jedesmal ausgeführt, was dazu führt, dass der upvector nicht fest bleibt		
/*		D3DXMatrixMultiply(
			&tempTransb,
			&tempTransUV,
			m_pMaster->getRotationMatrix());

		D3DXVec3Transform(pOut, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &tempTransb);
		tempMesh.setX(pOut->x);
		tempMesh.setY(pOut->y);
		tempMesh.setZ(pOut->z);

		m_pUpVector->setValues(tempMesh.getX(), tempMesh.getY(), tempMesh.getZ());*/
	} 


	delete pOut;
	
}