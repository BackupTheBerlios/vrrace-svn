#define STRICT
#include "masterClass.h"

masterClass::masterClass(/*String givenName, short givenType, long givenNumber*/)
{
	m_sName				= "";
/*	m_nType				= givenType;
	m_lNumber			= givenNumber;*/
}

bool masterClass::initObject()
{
//	m_xPosition			= new pointClass();
	m_xViewPoint		= new pointClass();
	m_xDirection		= new pointClass();
//	m_xRotation			= new pointClass();
	
//	m_Polygons			= NULL;
//	m_Surfaces			= NULL;
	m_SecondaryObjects	= NULL;

	m_VisSurfaces		= NULL;
	m_VisSecObjs		= NULL;

	return true;
}

bool masterClass::setPosition()
{
	setPosition(0.0, 0.0, 0.0);
	return true;
}

bool masterClass::setPosition(pointClass* givenPosition)
{
	setPosition(givenPosition->x, givenPosition->y, givenPosition->z);
	return true;
}

bool masterClass::setPosition(double givenX, double givenY, double givenZ)
{
	m_xPosition.x	= givenX;
	m_xPosition.y	= givenY;
	m_xPosition.z	= givenZ;
	return true;
}

masterClass::~masterClass()
{
//	delete m_xPosition;
	delete m_xViewPoint;
	delete m_xDirection;
//	delete m_xRotation;

//	m_xPosition			= NULL;
	m_xViewPoint		= NULL;
	m_xDirection		= NULL;
//	m_xRotation			= NULL;
}

short masterClass::addSurface()
{
	surfaceClass*	t_surface	= new surfaceClass();
//	m_Surfaces.push_back(t_surface);
	return 0;
}

/*
Fuegt einen Punkt in die MeshListe wenn er noch nicht drin ist
*/
void	masterClass::newMeshPoint(pointClass* givenPoint)
{
	bool isnew = true;

	for (unsigned int count = 0; count < m_MeshPoints.size(); count++)
	{
		if ((m_MeshPoints.at(count)->x == givenPoint->x)
		&& (m_MeshPoints.at(count)->y == givenPoint->y)
		&& (m_MeshPoints.at(count)->z == givenPoint->z))
			isnew = false;
	}

	if (isnew)
		m_MeshPoints.push_back(givenPoint);
}

short	masterClass::addPoint(pointClass* givenPoint)
/*
Fügt dem Objekt einen Festen Punkt hinzu, der im objekt direkt in einem Vector gespeichert wird
*/
{
	pointClass new_point;//	= new pointClass();
	new_point.set(givenPoint->x, givenPoint->y, givenPoint->z);
	newMeshPoint(&new_point);
	return 0;
}

short	masterClass::addPoint(double givenX, double givenY, double givenZ)
{
	pointClass* new_point = new pointClass();
	new_point->set(givenX, givenY, givenZ);
	newMeshPoint(new_point);
	return 0;
}

short	masterClass::addPoly(int given1, int given2, int given3)
{
	polyClass* new_poly = new polyClass();
	new_poly->init(m_MeshPoints.at(given1), m_MeshPoints.at(given2), m_MeshPoints.at(given3));
	m_Polygons.push_back(new_poly);
	return 0;
}

polyClass*	masterClass::getPoly(int givenIndex)
{
	return m_Polygons.at(givenIndex);
}

void	masterClass::newEdge(edgeClass* givenEdge)
{
	bool	isnew	= true;

	for (unsigned int count = 0; count < m_MeshEdges.size(); count++)
	{
		if (((m_MeshEdges.at(count)->m_Start == givenEdge->m_Start)
			&& (m_MeshEdges.at(count)->m_End == givenEdge->m_End))
			|| ((m_MeshEdges.at(count)->m_Start == givenEdge->m_End)
			&& (m_MeshEdges.at(count)->m_End == givenEdge->m_Start)))
			isnew = false;
	}

	if (isnew)
		m_MeshEdges.push_back(givenEdge);
	else
		delete givenEdge;
}

short	masterClass::addEdge(pointClass* givenStart, pointClass* givenEnd)
{
	edgeClass* new_edge = new edgeClass();
	new_edge->init(givenStart, givenEnd);
	newEdge(new_edge);
	return 0;
}

short	masterClass::addEdge(int givenStart, int givenEnd)
{
	edgeClass* new_edge = new edgeClass();
	if ((givenStart < m_MeshPoints.size())
		&& (givenEnd < m_MeshPoints.size())
		&& (givenStart != givenEnd))
	{
		new_edge->init(m_MeshPoints.at(givenStart), m_MeshPoints.at(givenEnd));
		newEdge(new_edge);
	}

	return 0;
}

short masterClass::addSecObj(masterClass* givenObject)
{
	m_SecondaryObjects->push_back(givenObject);
	return 0;
}

int masterClass::getNumPoints()
{
	return (int)m_MeshPoints.size();
}

int masterClass::getNumEdges()
{
	return (int)m_MeshEdges.size();
}

int	masterClass::getNumPolys()
{
	return (int)m_Polygons.size();
}

int masterClass::getNumSurfaces()
{
	return (int)m_Surfaces.size();
}

double masterClass::getMeshPointX(int givenIndex)
{
	return m_MeshPoints.at(givenIndex)->x + m_xPosition.x + m_xRotation.x;
}

double masterClass::getMeshPointY(int givenIndex)
{
	return m_MeshPoints.at(givenIndex)->y + m_xPosition.y + m_xRotation.y;
}

double masterClass::getMeshPointZ(int givenIndex)
{
	return m_MeshPoints.at(givenIndex)->z + m_xPosition.z + m_xRotation.z;
}

double masterClass::getMeshEstartX(int givenIndex)
{
	return m_MeshEdges.at(givenIndex)->m_Start->x + m_xPosition.x + m_xRotation.x;
}

double masterClass::getMeshEstartY(int givenIndex)
{
	return m_MeshEdges.at(givenIndex)->m_Start->y + m_xPosition.y + m_xRotation.y;
}

double masterClass::getMeshEstartZ(int givenIndex)
{
	return m_MeshEdges.at(givenIndex)->m_Start->z + m_xPosition.z + m_xRotation.z;
}

double masterClass::getMeshEendX(int givenIndex)
{
	return m_MeshEdges.at(givenIndex)->m_End->x + m_xPosition.x + m_xRotation.x;
}

double masterClass::getMeshEendY(int givenIndex)
{
	return m_MeshEdges.at(givenIndex)->m_End->y + m_xPosition.y + m_xRotation.y;
}

double masterClass::getMeshEendZ(int givenIndex)
{
	return m_MeshEdges.at(givenIndex)->m_End->z + m_xPosition.z + m_xRotation.z;
}

double masterClass::getPosX()
{
	return m_xPosition.x;
}

double masterClass::getPosY()
{
	return m_xPosition.y;
}

double masterClass::getPosZ()
{
	return m_xPosition.z;
}