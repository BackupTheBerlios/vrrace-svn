#include "MyPlayer.h"

MyPlayer::MyPlayer()
{
	m_Name			= new string();
	m_pPlayerMesh	= new MyMesh();
	m_pPlayerSound	= new MySound();
	m_pCamera		= NULL;//new MyView();
	m_points		= 0;
}

MyPlayer::~MyPlayer()
{
	delete m_Name;
	//if(m_pPlayerMesh)
	//	delete m_pPlayerMesh;
	//if(m_pPlayerSound)
	//	delete m_pPlayerSound;

	m_Name			= NULL;
	m_pPlayerMesh	= NULL;
	m_pPlayerSound	= NULL;
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

MySound* MyPlayer::getSound()
{
	return m_pPlayerSound;
}

MyView*	MyPlayer::getCamera()
{
	return m_pCamera;
}

void	MyPlayer::setCamera(MyView* givenCamera)
{
	m_pCamera = givenCamera;
}