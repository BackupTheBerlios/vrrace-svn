#include "MyView.h"

MyView::MyView(void)
{
	m_pPosition		= new MyVertex();
	m_pViewPoint	= new MyVertex();
	m_pUpVector		= new MyVertex();
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
	delete m_pViewPoint;
}
