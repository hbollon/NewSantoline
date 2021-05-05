#include "stdafx.h"
#include "CentraliseurConnecteur.h"

CentraliseurConnecteur::CentraliseurConnecteur(DatasHandler *handler) : m_handler(handler)
{
}

CentraliseurConnecteur::~CentraliseurConnecteur()
{
}

AEllipse CentraliseurConnecteur::get_dataset(Point2D p)
{
	return m_handler->get_dataset(p);
}

Vector3D CentraliseurConnecteur::get_wind(Point2D p)
{
	return m_handler->get_wind(p);
}

Vector3D CentraliseurConnecteur::get_slope(Point2D p)
{
	return m_handler->get_slope(p);
}

double CentraliseurConnecteur::get_aspect(Point2D p)
{
	return m_handler->get_aspect(p);
}

double CentraliseurConnecteur::get_slope_value(Point2D p)
{
	return m_handler->get_slope_value(p);
}

bool CentraliseurConnecteur::is_center(Point2D p)
{
	return m_handler->is_center(p);
}

bool CentraliseurConnecteur::is_border(Point2D p)
{
	return m_handler->is_border(p);
}

bool CentraliseurConnecteur::is_angle(Point2D p)
{
	return m_handler->is_angle(p);
}