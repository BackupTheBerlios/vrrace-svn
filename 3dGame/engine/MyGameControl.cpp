#include "includes.h"

MyGameControl::MyGameControl(void)
{
	m_pView	= new MyView();
}

MyGameControl::~MyGameControl(void)
{
	delete m_pView;
}

int MyGameControl::init(void)
{
	m_pView->m_Position.x	= 0.0f;
	m_pView->m_Position.y	= 1.0f;
	m_pView->m_Position.z	= 0.0f;

	m_pView->m_UpVector.x	= 0.0f;
	m_pView->m_UpVector.y	= 1.0f;
	m_pView->m_UpVector.z	= 0.0f;

	m_pView->m_ViewPoint.x	= 0.0f;
	m_pView->m_ViewPoint.y	= 0.0f;
	m_pView->m_ViewPoint.z	= 0.0f;

	return 0;
}
