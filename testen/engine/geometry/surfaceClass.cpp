#define STRICT
#include "surfaceClass.h"

surfaceClass::surfaceClass()
{

}

bool surfaceClass::init(pointClass* givenOne, pointClass* givenTwo, pointClass* givenThree)
{
	for (int count = 0; count < 3; count++)
	{
		m_edges.push_back(new edgeClass());
	}
	m_edges.at(0)->init(givenOne, givenTwo);
	m_edges.at(1)->init(givenTwo, givenThree);
	m_edges.at(2)->init(givenThree, givenOne);
	return true;
}

bool surfaceClass::init(pointClass* givenOne, pointClass* givenTwo, pointClass* givenThree, pointClass* givenFour)
{
	for (int count = 0; count < 4; count++)
	{
		m_edges.push_back(new edgeClass());
	}
	m_edges.at(0)->init(givenOne, givenTwo);
	m_edges.at(1)->init(givenTwo, givenThree);
	m_edges.at(2)->init(givenThree, givenFour);
	m_edges.at(3)->init(givenFour, givenOne);
	return true;
}

surfaceClass::~surfaceClass()
{
	
}
short surfaceClass::addPolygon(
		double givenX1, double givenY1, double givenZ1,
		double givenX2, double givenY2, double givenZ2,
		double givenX3, double givenY3, double givenZ3)
{
//	pointClass point1	= new pointClass();
//	pointClass point2	= new pointClass();
//	pointClass point3	= new pointClass();
//	polyClass t_poly	= new polyClass();

//	point1.set(givenX1, givenY1, givenZ1);
//	point2.set(givenX2, givenY2, givenZ2);
//	point3.set(givenX3, givenY3, givenZ3);

//	t_poly.init(point1, point2, point3);
//	m_Polygons.push_back(t_poly);

	return 0;
}
