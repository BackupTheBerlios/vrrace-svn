#include "MyMasterPosition.h"

MyMasterPosition::MyMasterPosition(void)
{
	m_pPosition		= new MyVertex();
	m_pDirection	= new MyVertex();
	m_pRotation		= new MyVertex();
	m_pRotDir		= new MyVertex();
}

MyMasterPosition::~MyMasterPosition(void)
{
	delete m_pPosition;
	delete m_pDirection;
	delete m_pRotation;
	delete m_pRotDir;
}