#include "MyPlayer.h"

MyPlayer::MyPlayer()
{
	m_Name			= new string();
	m_pPlayerMesh	= new MyMesh();
}

MyPlayer::~MyPlayer()
{
	delete m_Name;
	delete m_pPlayerMesh;

	m_Name			= NULL;
	m_pPlayerMesh	= NULL;
}

void	MyPlayer::setMesh(MyMesh* givenMesh)
{
	delete m_pPlayerMesh;
	m_pPlayerMesh	= givenMesh;
}

MyMesh*	MyPlayer::getMesh()
{
	return m_pPlayerMesh;
}