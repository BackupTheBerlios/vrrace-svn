#define STRICT
#include "polyClass.h"

/*surfaceClass::surfaceClass()
{
}*/

bool polyClass::init(pointClass* givenOne, pointClass* givenTwo, pointClass* givenThree)
{
/*	for (int count = 0; count < 3; count++)
	{
		m_points.push_back(new pointClass());
//		m_edges.push_back(new edgeClass());
	}

	//m_points.at(0)->setX
/*	m_edges.at(0)->init(givenOne, givenTwo);
	m_edges.at(1)->init(givenTwo, givenThree);
	m_edges.at(2)->init(givenThree, givenOne);
*/

	p1	= givenOne;
	p2	= givenTwo;
	p3	= givenThree;
	return true;
}
/*
CUSTOMVERTEX	polyClass::getVertex()
{
	CUSTOMVERTEX temp[] =
	{
		{
			p1->x, p1->y, p1->z, 1.0f, 0xffff0000,
		},
		{
			p2->x, p2->y, p2->z, 1.0f, 0xff00ff00,
		},
		{
			p3->x, p3->y, p3->z, 1.0f, 0xff00ffff,
		},
	};


	return
}*/

bool polyClass::init(pointClass* givenOne, pointClass* givenTwo, pointClass* givenThree, pointClass* givenFour)
{
/*	for (int count = 0; count < 4; count++)
	{
		m_edges.push_back(new edgeClass());
	}
	m_edges.at(0)->init(givenOne, givenTwo);
	m_edges.at(1)->init(givenTwo, givenThree);
	m_edges.at(2)->init(givenThree, givenFour);
	m_edges.at(3)->init(givenFour, givenOne);
*/	return true;
}
/*
polyClass::~surfaceClass()
{
	
}*/