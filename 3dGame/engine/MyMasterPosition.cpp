#include "MyMasterPosition.h"

MyMasterPosition::MyMasterPosition(void)
{
	m_pPosition				= new MyVertex();
	m_pAbsolutePosition		= new MyVertex();
	m_pDirection			= new MyVertex();
	m_pRotation				= new MyVertex();
	m_pLocalRotation		= new MyVertex();
	m_pRotDir				= new MyVertex();
	m_pScaleFactor			= new MyVertex();
	m_bScale				= false;

	m_pMatrix				= new D3DXMATRIX;
	m_pFinalTransRotMatrix	= new D3DXMATRIX;
	m_pRotationMatrix		= new D3DXMATRIX;

	D3DXMatrixRotationYawPitchRoll(
			m_pRotationMatrix,
			0.0f,
			0.0f,
			0.0f);

	m_pFinalMatrix			= NULL;

	m_pMaster				= NULL;
}

MyMasterPosition::~MyMasterPosition(void)
{
	delete m_pPosition;
	delete m_pAbsolutePosition;
	delete m_pDirection;
	delete m_pRotation;
	delete m_pLocalRotation;
	delete m_pRotDir;
	delete m_pScaleFactor;

	delete m_pMatrix;
	delete m_pFinalTransRotMatrix;
	delete m_pFinalMatrix;
}

void	MyMasterPosition::calcOwnMatrix()
{
	D3DXMATRIX	tempRot;
	D3DXMATRIX	tempRotLoc;
	D3DXMATRIX	tempRotSum;
	D3DXMATRIX	tempTrans;

	if (!m_bControl)
	{

		//rotiere lokal
		D3DXMatrixRotationYawPitchRoll(
			m_pRotationMatrix,
			m_pRotation->getY(),
			m_pRotation->getX(),
			m_pRotation->getZ());
	}

	//transliere lokal
	D3DXMatrixTranslation(
			&tempTrans,
			m_pPosition->getX(),
			m_pPosition->getY(),
			m_pPosition->getZ()
			);

	D3DXMatrixMultiply(
			m_pFinalTransRotMatrix,
			m_pRotationMatrix,
			&tempTrans);

	if (m_pMaster == NULL)
	{
		//delete m_pFinalMatrix;
		m_pFinalMatrix = m_pFinalTransRotMatrix;
	}
}

void	MyMasterPosition::rotate(float givenX, float givenY, float givenZ)
{
	D3DXMATRIX tempMatrixNeu;
	D3DXMATRIX* tempMatrixAlt = m_pRotationMatrix;

	m_pRotationMatrix = new D3DXMATRIX;

	D3DXMatrixRotationYawPitchRoll(
			&tempMatrixNeu,
			givenY,
			givenX,
			givenZ);

	D3DXMatrixMultiply(
		m_pRotationMatrix,
		&tempMatrixNeu,
		tempMatrixAlt);
	
	delete tempMatrixAlt;
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
			m_pFinalTransRotMatrix,
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
	D3DXMATRIX	tempMatrix;
	D3DXMATRIX	tempScale;
	if (m_bScale)
	{
		//Skaliere
		D3DXMatrixScaling(
			&tempScale,
			m_pScaleFactor->getX(),
			m_pScaleFactor->getY(),
			m_pScaleFactor->getZ());

		
		D3DXMatrixMultiply(
			&tempMatrix,
			&tempScale,
			m_pFinalMatrix);

		_D3DDevice->SetTransform(D3DTS_WORLD, &tempMatrix);
	} else {
		_D3DDevice->SetTransform(D3DTS_WORLD, m_pFinalMatrix);
	}
}

void	MyMasterPosition::calcClients()
{
	D3DXVECTOR3*	pVector	= new D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR4*	pOut	= new D3DXVECTOR4();

	if (m_pMaster != NULL)
	{
		D3DXVec3Transform(pOut, pVector, m_pFinalMatrix);
		m_pAbsolutePosition->setValues(pOut->x, pOut->y, pOut->z);
	} 

	delete pVector;
	delete pOut;

	if (m_pClients.size() > 0)
	{
		for (DWORD count = 0; count < m_pClients.size(); count++)
		{
			m_pClients[count]->calcMatrix(m_pFinalMatrix);
		}
	}
}

void	MyMasterPosition::addClient(MyMasterPosition* givenClient)
{
	m_pClients.push_back(givenClient);
}

void	MyMasterPosition::setMaster(MyMasterPosition* givenMaster)
{
	m_pMaster	= givenMaster;
	m_pMaster->addClient(this);
	m_pFinalMatrix	= new D3DXMATRIX;
}

MyVertex*	MyMasterPosition::getScale()
{
	return m_pScaleFactor;
}

MyVertex*	MyMasterPosition::getAbsolutePosition()
//erst nach moove aufrufen
{
	if (m_pMaster == NULL) {
		return m_pPosition;
	} else {
		return m_pAbsolutePosition;
	}
}

D3DXMATRIX*	MyMasterPosition::getRotationMatrix()
{
	return m_pRotationMatrix;
}