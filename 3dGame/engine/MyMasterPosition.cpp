#include "MyMasterPosition.h"

MyMasterPosition::MyMasterPosition(void)
{
	m_pPosition				= new MyVertex();
	m_pDirection			= new MyVertex();
	m_pRotation				= new MyVertex();
	m_pRotDir				= new MyVertex();
	m_pScaleFactor			= new MyVertex();
	m_bScale				= false;

	m_pScaleMatrix			= new D3DXMATRIX;
	m_pRotationMatrix		= new D3DXMATRIX;
	m_pTranslationMatrix	= new D3DXMATRIX;
	m_pMatrix				= new D3DXMATRIX;
	m_pFinalMatrix			= new D3DXMATRIX;

	m_pMaster				= NULL;
}

MyMasterPosition::~MyMasterPosition(void)
{
	delete m_pPosition;
	delete m_pDirection;
	delete m_pRotation;
	delete m_pRotDir;
	delete m_pScaleFactor;

	delete m_pScaleMatrix;
	delete m_pRotationMatrix;
	delete m_pTranslationMatrix;
	delete m_pMatrix;
	delete m_pFinalMatrix;
}

void	MyMasterPosition::calcOwnMatrix()
{
	//rotiere lokal
	D3DXMatrixRotationYawPitchRoll(
			m_pRotationMatrix,
			m_pRotation->getY(),
			m_pRotation->getX(),
			m_pRotation->getZ());

	//transliere lokal
	D3DXMatrixTranslation(
			m_pTranslationMatrix,
			m_pPosition->getX(),
			m_pPosition->getY(),
			m_pPosition->getZ()
			);

	if (m_bScale)
	{
		D3DXMATRIX	tempRotTransMatrix;

		//Skaliere
		D3DXMatrixScaling(
			m_pScaleMatrix,
			m_pScaleFactor->getX(),
			m_pScaleFactor->getY(),
			m_pScaleFactor->getZ());

		D3DXMatrixMultiply(
			&tempRotTransMatrix,
			m_pRotationMatrix,
			m_pTranslationMatrix);

		D3DXMatrixMultiply(
			m_pMatrix,
			m_pScaleMatrix,
			&tempRotTransMatrix);

	} else {
		//fasse lokale Matrizen zusammen
		D3DXMatrixMultiply(
			m_pMatrix,
			m_pRotationMatrix,
			m_pTranslationMatrix);
	}

	if (m_pMaster == NULL)
	{
		m_pFinalMatrix = m_pMatrix;
	}
}

void	MyMasterPosition::move()
{
	m_pPosition->addX(m_pDirection->getX());
	m_pPosition->addY(m_pDirection->getY());
	m_pPosition->addZ(m_pDirection->getZ());

	m_pRotation->addX(m_pRotDir->getX());
	m_pRotation->addY(m_pRotDir->getY());
	m_pRotation->addZ(m_pRotDir->getZ());

	this->calcOwnMatrix();
}

void	MyMasterPosition::calcMatrix(D3DXMATRIX* givenMatrix)
{
	D3DXMatrixMultiply(
			m_pFinalMatrix,
			m_pMatrix,
			givenMatrix);

	this->calcClients();
}

void	MyMasterPosition::activateScaling()
{
	m_bScale	= true;
}

void	MyMasterPosition::deactivateScaling()
{
	m_bScale	= false;
}

void	MyMasterPosition::transform()
{
	//fuehre Transformationen durch
	_D3DDevice->SetTransform(D3DTS_WORLD, m_pFinalMatrix);
}

void	MyMasterPosition::calcClients()
{
	for (DWORD count = 0; count < m_pClients.size(); count++)
	{
		m_pClients[count]->calcMatrix(m_pFinalMatrix);
	}
}

void	MyMasterPosition::addClient(MyMasterPosition* givenClient)
{
	m_pClients.push_back(givenClient);
}

void	MyMasterPosition::setMaster(MyMasterPosition* givenMaster)
{
	m_pMaster	= givenMaster;
}

MyVertex*	MyMasterPosition::getScale()
{
	return m_pScaleFactor;
}

MyVertex*	MyMasterPosition::getAbsolutePosition()
{
	MyVertex*	tempVertex	= new MyVertex();

	if (m_pMaster != NULL)
	{
		tempVertex->addX(m_pMaster->getAbsolutePosition()->getX());
		//nur ein ansatz hier!
	}

	return tempVertex;
}