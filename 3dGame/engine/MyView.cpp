#include "MyView.h"

MyView::MyView(void)
{
	m_pPosition		= new MyVertex();
	m_pLocalPos		= new MyVertex();
	m_pViewPoint	= new MyVertex();
	m_pUpVector		= new MyVertex();
	m_pDirection	= new MyVertex();
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

MyVertex*	MyView::getDirection()
{
	return m_pDirection;
}

MyView::~MyView(void)
{
	delete m_pPosition;
	delete m_pLocalPos;
	delete m_pViewPoint;
	delete m_pUpVector;
	delete m_pDirection;

	m_pPosition		= NULL;
	m_pLocalPos		= NULL;
	m_pViewPoint	= NULL;
	m_pUpVector		= NULL;
	m_pMaster		= NULL;
	m_pDirection	= NULL;
}

void	MyView::setMaster(MyMasterPosition* givenMaster)
{
	m_pMaster	= givenMaster;
	m_pViewPoint->setPValues(
		m_pMaster->getAbsolutePosition()->getPX(),
		m_pMaster->getAbsolutePosition()->getPY(),
		m_pMaster->getAbsolutePosition()->getPZ());
	m_pDirection->setPValues(
		m_pMaster->m_pDirection->getPX(),
		m_pMaster->m_pDirection->getPY(),
		m_pMaster->m_pDirection->getPZ());
}

MyMasterPosition*	MyView::getMaster()
{
	return m_pMaster;
}

void	MyView::move()
{
//	D3DXMATRIX	tempTransLP;
//	D3DXMATRIX	tempTransb;
	D3DXVECTOR4	pOut;

	if (m_pMaster != NULL)
	{
/*		D3DXMatrixTranslation(
			&tempTransLP,
			m_pLocalPos->getX(),
			m_pLocalPos->getY(),
			m_pLocalPos->getZ()
			);

		D3DXMatrixMultiply(
			&tempTransb,
			&tempTransLP,
			m_pMaster->getRotationMatrix());
*/
		//D3DXVec3Transform(&pOut, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &tempTransb);
		D3DXVec3Transform(&pOut, &D3DXVECTOR3(m_pLocalPos->getX(), m_pLocalPos->getY(), m_pLocalPos->getZ()), m_pMaster->getRotationMatrix());

		m_pPosition->setValues(
			m_pMaster->getAbsolutePosition()->getX() + pOut.x, 
			m_pMaster->getAbsolutePosition()->getY() + pOut.y, 
			m_pMaster->getAbsolutePosition()->getZ() + pOut.z);
	}
}

void	MyView::rotate()
{
	D3DXVECTOR4	pOut;
	D3DXVec3Transform(&pOut, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), m_pMaster->getRotationMatrix());
	m_pUpVector->setValues(pOut.x, pOut.y, pOut.z);
}