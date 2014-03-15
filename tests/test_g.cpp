/*
  Copyright (C) 2011 The SmithDR team, smithdr@labri.fr

  This file is part of SmithDR.

  SmithDR is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  SmithDR is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with SmithDR.
  If not, see <http://www.gnu.org/licenses/>.
*/

#include <SmithDRDefs.hpp>
#include <core/image/ImageView.hpp>
#include <core/image/Image.hpp>
#include <frontend/lib/plugins/PluginManager.hpp>
#include <frontend/lib/LoaderManager.hpp>
#include <frontend/lib/SaverManager.hpp>

#include <cstdlib>

#include <time.h>

#include <meshprocessing/curvatureanalysis/AvailableCurvatureEstimators.hpp>
#include <meshprocessing/normalestimator/AvailableNormalEstimators.hpp>
#include <meshprocessing/generator/neighborsdealer/AvailableNeighborsDealersFromMesh.hpp>
#include <meshprocessing/generator/neighborsdealer/AvailableNeighborsDealersStandAlone.hpp>

#include <iostream>

typedef sd::DOUBLE T;
const unsigned int N=3;

int
main(int argc, char* argv[]) {
  
  // std::cerr << sd::meshprocessing::normalestimator::SineFittingNormalComputer<double>().description() << std::endl;
  
  unsigned int targetId = 1;
  
  sd::core::MeshView* mv = NULL;
  if (argc == 2) {
    std::string inFilename(argv[1]);
    sd::frontend::plugins::PluginManager& pm = sd::frontend::plugins::PluginManager::instance();
    std::cout << "Load LoaderPlyPlugin\n";
    pm.loadPlugin(PLUGIN_DIR, "LoaderPlyPlugin");
    std::cout << "Load LoaderObjPlugin\n";
    pm.loadPlugin(PLUGIN_DIR, "LoaderObjPlugin");
    // load
    bool loaded = sd::frontend::loadFile(inFilename, mv);
    if (!loaded || !mv) {
      std::cerr << "Unable to load file: " << inFilename << ".\n\n";
      return EXIT_FAILURE;
    }
  } else {
  
    const unsigned int NB_NEIGHBORS = 6;
    const T RADIUS = 2.;
    const T STEP = .5;
    const T DMIN = 0.;
    const T DMAX = 1.;
  
    // TODO mv = ;
    switch (2) {
    case 0 :
      mv = sd::meshprocessing::generator::SphereNeighborsDealer<T>(NB_NEIGHBORS, RADIUS, STEP, DMIN, DMAX).getNeighboringMesh();
      break;
    case 1 :
      mv = sd::meshprocessing::generator::SphereNeighborsDealer<T>(NB_NEIGHBORS, RADIUS, STEP, DMIN, DMAX, 1).getNeighboringMesh();
      break;
    case 2 :
      mv = sd::meshprocessing::generator::SphereNeighborsDealer<T>(NB_NEIGHBORS, RADIUS, STEP, DMIN, DMAX, 2).getNeighboringMesh();
      break;
    case 3 :
      mv = sd::meshprocessing::generator::CylinderNeighborsDealer<T>(NB_NEIGHBORS, RADIUS, STEP, DMIN, DMAX).getNeighboringMesh();
      break;
    case 4 :
      mv = sd::meshprocessing::generator::CylinderNeighborsDealer<T>(NB_NEIGHBORS, RADIUS, STEP, DMIN, DMAX, 2).getNeighboringMesh();
      break;
    }
    
    targetId = static_cast<sd::core::MeshView_<T, N>* >(mv)->nbVertices()-1;
    // return EXIT_FAILURE;
  } 
  
  assert(sd::core::DataType_<T>() == mv->getVertexDataType());
  assert(N == mv->getFaceDegree());
  
  sd::core::MeshView_<T, N>* mesh = static_cast<sd::core::MeshView_<T, N>* >(mv);

  sd::meshprocessing::curvatureanalysis::SineFittingAlgorithm<T, N> algo;
  // sd::meshprocessing::curvatureanalysis::SineFittingExtendedAlgorithm<T, N> algo;
  // sd::meshprocessing::curvatureanalysis::SimpleQuadricFittingAlgorithm<T, N> algo;
  // algo.setNormalComputer(new sd::meshprocessing::normalestimator::SineFittingExtendedNormalComputer<T>());
  // algo.setNormalComputer(new sd::meshprocessing::normalestimator::SineFittingNormalComputer2<T>());
  algo.setNormalComputer(new sd::meshprocessing::normalestimator::SineFittingNormalComputer3<T>());
  // algo.setNormalComputer(new sd::meshprocessing::normalestimator::SineFittingNormalComputer<T>());
  
  sd::core::VertexFromMeshView<T> vertexTarget = mesh->getVertexAt(std::min(targetId, mesh->nbVertices()-1));
  
  sd::meshprocessing::generator::NeighborsDealerFromMeshByRing<T, N> neighborsDealer(mesh, vertexTarget);
  const std::vector<std::vector<sd::core::VertexFromMeshView<T> > >& neighbors = neighborsDealer.getNeighbors();
  
  if (neighbors.size() > 0
      && neighbors[0].size() >
      algo.getMinimumNeighborhoodSize()
      && neighborsDealer.getValidations()[0]) {
    
    T h=0., k=0., k1=0., k2=0.;
    sd::core::Vector3d<T> e1, e2;
    algo.computeCurvature(h, k, k1, k2,
			  e1, e2, neighbors,
			  vertexTarget, sd::core::Normal<T>(vertexTarget.getNormal()));
    // std::cerr << "H=" << h << ", K=" << k << ", k1=" << k1 << ", k2=" << k2 << std::endl;
  }
  
  //sd::frontend::plugins::PluginManager& pm = sd::frontend::plugins::PluginManager::instance();
  //pm.loadPlugin(PLUGIN_DIR, "SaverPlyPlugin");
  //std::cout << "Load SaverPlyPlugin\n";
  //sd::frontend::saveFile(mv, "/tmp/out.ply", false);
  
  delete mesh;
  
  return EXIT_SUCCESS;  
}
