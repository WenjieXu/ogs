/**
 * \file   ElementQualityMetric.h
 * \author Thomas Fischer
 * \date   2010-12-08
 * \brief  Definition of the ElementQualityMetricBase class.
 *
 * \copyright
 * Copyright (c) 2013, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 */

#ifndef ELEMENTQUALITYMETRIC_H_
#define ELEMENTQUALITYMETRIC_H_

#include <vector>

// BaseLib
#include "Histogram.h"

// MSH
#include "Mesh.h"
#include "Elements/Element.h"

#include "logog/include/logog.hpp"

namespace MeshLib
{

/**
 * Base class for calculating the quality of mesh element based on a given metric
 */
class ElementQualityMetric
{
public:
	ElementQualityMetric(Mesh const* const mesh);

	virtual ~ElementQualityMetric () {}

	/// Calculates the quality metric for each element of the mesh
	virtual void calculateQuality () = 0;
	/// Returns the result vector
	std::vector<double> const& getElementQuality () const;
	/// Returns the minimum calculated value
	double getMinValue() const;
	/// Returns the maximum calculated value
	double getMaxValue() const;
	/// Returns a histogram of the result
	virtual BaseLib::Histogram<double> getHistogram (std::size_t nclasses = 0) const;

protected:
	void errorMsg (const Element* elem, std::size_t idx) const;

	double _min;
	double _max;
	Mesh const* const _mesh;
	std::vector<double> _element_quality_metric;
};
}

#endif /* ELEMENTQUALITYMETRIC_H_ */
