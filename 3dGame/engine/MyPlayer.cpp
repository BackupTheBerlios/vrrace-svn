#include "MyPlayer.h"

MyPlayer::MyPlayer()
{
	m_Name			= new string();
	m_pPlayerMesh	= new MyMesh();
//	m_pCamera		= NULL;//new MyView();
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
/*
MyView*	MyPlayer::getCamera()
{
	return m_pCamera;
}

void	MyPlayer::setCamera(MyView* givenCamera)
{
	m_pCamera = givenCamera;
}*/