#pragma once
#include "connector.h"
#include "ellipse.h"
#include "DatasHandler.h"

class CentraliseurConnecteur : public Connector
{
public:
	CentraliseurConnecteur(DatasHandler *handler);
	~CentraliseurConnecteur();
	virtual AEllipse get_dataset(Point2D p);
	virtual Vector3D get_wind(Point2D p);
	virtual Vector3D get_slope(Point2D p);
	virtual double get_aspect(Point2D p);
	virtual double get_slope_value(Point2D p);
	virtual bool is_center(Point2D p);
	virtual bool is_border(Point2D p);
	virtual bool is_angle(Point2D p);

private:
	DatasHandler *m_handler;
};