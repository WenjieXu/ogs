/**
 * \file
 * \author Karsten Rink
 * \date   2013-07-05
 * \brief  Implementation of  of mesh to geometry conversion.
 *
 * \copyright
 * Copyright (c) 2013, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 */

#include "convertMeshToGeo.h"

// ThirdParty/logog
#include "logog/include/logog.hpp"

#include "GEOObjects.h"
#include "Surface.h"

#include "Mesh.h"
#include "Elements/Tri.h"
#include "Elements/Quad.h"
#include "Node.h"

namespace MeshLib {

bool convertMeshToGeo(const MeshLib::Mesh &mesh, GeoLib::GEOObjects* geo_objects)
{
	if (mesh.getDimension() != 2)
	{
		ERR ("Mesh to geometry conversion is only working for 2D meshes.");
		return false;
	}
	
	// nodes to points conversion
	const std::size_t nNodes (mesh.getNNodes());
	std::vector<GeoLib::Point*> *points = new std::vector<GeoLib::Point*>(nNodes);
	const std::vector<MeshLib::Node*> &nodes = mesh.getNodes();

	for (unsigned i=0; i<nNodes; ++i)
		(*points)[i] = new GeoLib::Point(static_cast<GeoLib::Point>(*nodes[i]));

	std::string mesh_name (mesh.getName());
	geo_objects->addPointVec(points, mesh_name);
	const std::vector<std::size_t> id_map (geo_objects->getPointVecObj(mesh_name)->getIDMap());

	// elements to surface triangles conversion
	const std::vector<MeshLib::Element*> &elements = mesh.getElements();
	GeoLib::Surface* sfc = new GeoLib::Surface(*points);
	const std::size_t nElems (mesh.getNElements());

	for (unsigned i=0; i<nElems; ++i)
	{
		MeshLib::Element* e (elements[i]);
		if (e->getGeomType() == MeshElemType::TRIANGLE)
			sfc->addTriangle(id_map[e->getNodeIndex(0)], id_map[e->getNodeIndex(1)], id_map[e->getNodeIndex(2)]);
		if (e->getGeomType() == MeshElemType::QUAD)
		{
			sfc->addTriangle(id_map[e->getNodeIndex(0)], id_map[e->getNodeIndex(1)], id_map[e->getNodeIndex(2)]);
			sfc->addTriangle(id_map[e->getNodeIndex(0)], id_map[e->getNodeIndex(2)], id_map[e->getNodeIndex(3)]);
		}
		// all other element types are ignored (i.e. lines)
	}

	std::vector<GeoLib::Surface*> *sfcs = new std::vector<GeoLib::Surface*>(1);
	(*sfcs)[0] = sfc;

	geo_objects->addSurfaceVec(sfcs, mesh_name);
	return true;
}


}

