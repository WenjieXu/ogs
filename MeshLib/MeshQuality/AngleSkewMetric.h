/**
 * \file   AngleSkewMetric.h
 * \author Thomas Fischer
 * \date   2011-03-17
 * \brief  Definition of the AngleSkewMetric class.
 *
 * \copyright
 * Copyright (c) 2013, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 */

#ifndef ANGLESKEWMETRIC_H_
#define ANGLESKEWMETRIC_H_

#include "ElementQualityMetric.h"

namespace MeshLib
{

/** 
 * Calculates the quality of mesh elements based on the EquiAngleSkew measure
 */
class AngleSkewMetric : public ElementQualityMetric
{
public:
	AngleSkewMetric(Mesh const* const mesh);
	virtual ~AngleSkewMetric();

	virtual void calculateQuality ();

private:
	double checkTriangle(Element const* const elem) const;
	double checkQuad(Element const* const elem) const;
	double checkTetrahedron(Element const* const elem) const;
	double checkHexahedron(Element const* const elem) const;
	double checkPrism (Element const* const elem) const;
	void getMinMaxAngleFromQuad(double const* const n0,
	                            double const* const n1, double const* const n2,
	                            double const* const n3, double &min_angle,
	                            double &max_angle) const;
	void getMinMaxAngleFromTriangle(double const* const n0,
	                                double const* const n1, double const* const n2,
	                                double &min_angle, double &max_angle) const;

	const double M_PI_THIRD;
	const double TWICE_M_PI;
};
}

#endif /* ANGLESKEWMETRIC_H_ */
