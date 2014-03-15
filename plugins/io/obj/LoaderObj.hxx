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

#ifndef SD_OBJ_LOADER_OBJ_HXX
#define SD_OBJ_LOADER_OBJ_HXX

#include <fstream>
#ifdef DEBUG
#include <iostream>
#endif
#include <vector>

#include <utils/sdRawUtils.hpp>
#include <utils/sdTypeUtils.hpp>

template<typename T, unsigned int N>
sd::core::MeshView_<T, N>*
sd::obj::LoaderObj<T, N>::loadObjMesh(const std::string& fileURI) const {
  std::ifstream infile(fileURI.c_str());
  if (!infile) {
#ifdef DEBUG
    std::cerr << "LoaderObj::loadObjMesh() : unable to open file: " << fileURI << std::endl;
#endif
    return NULL;
  }
  
  sd::core::Vector<T> vertices;
  sd::core::Vector<typename sd::core::Normal<T>::type> vertexNormals;
  sd::core::Vector<unsigned char> vertexColors;
  sd::core::Vector<double> vertexTextures;
  sd::core::Vector<unsigned int> faces;
  sd::core::Vector<unsigned int> vertexNormalsIds;
  sd::core::Vector<unsigned int> vertexTexturesIds;
  sd::core::Vector<double> faceNormals;
  sd::core::Vector<unsigned char> faceColors;
  
  char buff[256];
  
  while (!infile.eof()) {
    char commandChar1 = '\n';
    char commandChar2 = '\n';
    
    commandChar1 = infile.peek();
    
    while (commandChar1 != 'f' && commandChar1 != 'v' && !infile.eof()) {
      infile.ignore(256, '\n');
      commandChar1 = infile.peek();
    }
    
    infile >> commandChar1;
    switch (commandChar1) {
    case 'v': {
      commandChar2 = infile.get();
      switch (commandChar2) {
      case ' ': {
	infile.getline(buff, 256);
	sd::core::Vector<std::string> result;
	sd::raw::split(buff, " ", result);
	sd::utils::removeIf(result, "");
	sd::utils::removeIf(result, "\r");
#ifdef DEBUG
	if (result.size() != 3 && result.size() != 6) std::cerr << buff << std::endl;
#endif
	if (result.size() != 3 && result.size() != 6) return NULL;
	vertices.push_back(sd::raw::readAcsii<T>(result[0]));
	vertices.push_back(sd::raw::readAcsii<T>(result[1]));
	vertices.push_back(sd::raw::readAcsii<T>(result[2]));
	if (result.size() > 3) {
	  vertexColors.push_back(sd::raw::readAcsii<unsigned char>(result[3]));
	  vertexColors.push_back(sd::raw::readAcsii<unsigned char>(result[4]));
	  vertexColors.push_back(sd::raw::readAcsii<unsigned char>(result[5]));
	  if (result.size() == 6)
	    vertexColors.push_back(255);
	  else
	    vertexColors.push_back(sd::raw::readAcsii<unsigned char>(result[6]));
	}
	break;
      }
      case 'n': {
	infile.get();
	infile.getline(buff, 256);
	sd::core::Vector<std::string> result;
	sd::raw::split(buff, " ", result);
	sd::utils::removeIf(result, "");
	sd::utils::removeIf(result, "\r");
#ifdef DEBUG
	if (result.size() != 3) std::cerr << buff << std::endl;
#endif
	if (result.size() != 3) return NULL;
	vertexNormals.push_back(sd::raw::readAcsii<double>(result[0]));
	vertexNormals.push_back(sd::raw::readAcsii<double>(result[1]));
	vertexNormals.push_back(sd::raw::readAcsii<double>(result[2]));
	break;
      }
      case 't': {
	infile.get();
	infile.getline(buff, 256);
	sd::core::Vector<std::string> result;
	sd::raw::split(buff, " ", result);
	sd::utils::removeIf(result, "");
	sd::utils::removeIf(result, "\r");
	vertexTextures.push_back(sd::raw::readAcsii<float>(result[0]));
	vertexTextures.push_back((result.size() >= 2) ? sd::raw::readAcsii<float>(result[1]) : 0.);
	vertexTextures.push_back((result.size() == 3) ? sd::raw::readAcsii<float>(result[2]) : 0.);
	break;
      }
      case 'c': {
	infile.get();
	infile.getline(buff, 256);
	sd::core::Vector<std::string> result;
	sd::raw::split(buff, " ", result);
	sd::utils::removeIf(result, "");
	sd::utils::removeIf(result, "\r");
#ifdef DEBUG
	if (result.size() < 3) std::cerr << buff << std::endl;
#endif
	if (result.size() < 3) return NULL;
	vertexColors.push_back(sd::raw::readAcsii<float>(result[0])*255.);
	vertexColors.push_back(sd::raw::readAcsii<float>(result[1])*255.);
	vertexColors.push_back(sd::raw::readAcsii<float>(result[2])*255.);
	if (result.size() > 3)
	  vertexColors.push_back(sd::raw::readAcsii<float>(result[3])*255.);
	else
	  vertexColors.push_back(255.);
	break;
      }
      default: {
	infile.ignore(256, '\n');
	break;
      }
      }
      break;
    }
    case 'f': {
      commandChar2 = infile.get();
      switch (commandChar2) {
      case ' ': {
	if (N == 3) {
	  infile.getline(buff, 256);
	  sd::core::Vector<std::string> result;
	  std::string buffStr = "";
	  sd::raw::split(buff, " ", result);
	  sd::utils::removeIf(result, "");
	  sd::utils::removeIf(result, "\r");
	  sd::raw::merge(result, " ", buffStr);
	  sd::raw::split(buffStr.c_str(), " /", result);
	  const size_t faceCard = result.size()/(1+sd::raw::numberOf(buffStr.c_str(), '/', ' '));
	  const size_t nbFields = result.size()/faceCard;
	  if (faceCard == 4) {
	    assert(sd::raw::readAcsii<size_t>(result[0*nbFields]) > 0);
	    assert(sd::raw::readAcsii<size_t>(result[1*nbFields]) > 0);
	    assert(sd::raw::readAcsii<size_t>(result[2*nbFields]) > 0);
	    assert(sd::raw::readAcsii<size_t>(result[3*nbFields]) > 0);
	    faces.push_back(sd::raw::readAcsii<size_t>(result[0*nbFields])-1);
	    faces.push_back(sd::raw::readAcsii<size_t>(result[1*nbFields])-1);
	    faces.push_back(sd::raw::readAcsii<size_t>(result[2*nbFields])-1);
	    faces.push_back(sd::raw::readAcsii<size_t>(result[2*nbFields])-1);
	    faces.push_back(sd::raw::readAcsii<size_t>(result[3*nbFields])-1);
	    faces.push_back(sd::raw::readAcsii<size_t>(result[0*nbFields])-1);
	    if ((nbFields >= 2) && !(result[0*nbFields+1].empty())) {
	      assert(sd::raw::readAcsii<size_t>(result[0*nbFields+1]) > 0);
	      assert(sd::raw::readAcsii<size_t>(result[1*nbFields+1]) > 0);
	      assert(sd::raw::readAcsii<size_t>(result[2*nbFields+1]) > 0);
	      assert(sd::raw::readAcsii<size_t>(result[3*nbFields+1]) > 0);
	      vertexTexturesIds.push_back(sd::raw::readAcsii<size_t>(result[0*nbFields+1])-1);
	      vertexTexturesIds.push_back(sd::raw::readAcsii<size_t>(result[1*nbFields+1])-1);
	      vertexTexturesIds.push_back(sd::raw::readAcsii<size_t>(result[2*nbFields+1])-1);
	      vertexTexturesIds.push_back(sd::raw::readAcsii<size_t>(result[2*nbFields+1])-1);
	      vertexTexturesIds.push_back(sd::raw::readAcsii<size_t>(result[3*nbFields+1])-1);
	      vertexTexturesIds.push_back(sd::raw::readAcsii<size_t>(result[0*nbFields+1])-1);
	    }
	    if ((nbFields == 3) && !(result[0*nbFields+2].empty())) {
	      assert(sd::raw::readAcsii<size_t>(result[0*nbFields+2]) > 0);
	      assert(sd::raw::readAcsii<size_t>(result[1*nbFields+2]) > 0);
	      assert(sd::raw::readAcsii<size_t>(result[2*nbFields+2]) > 0);
	      assert(sd::raw::readAcsii<size_t>(result[3*nbFields+2]) > 0);
	      vertexNormalsIds.push_back(sd::raw::readAcsii<size_t>(result[0*nbFields+2])-1);
	      vertexNormalsIds.push_back(sd::raw::readAcsii<size_t>(result[1*nbFields+2])-1);
	      vertexNormalsIds.push_back(sd::raw::readAcsii<size_t>(result[2*nbFields+2])-1);
	      vertexNormalsIds.push_back(sd::raw::readAcsii<size_t>(result[2*nbFields+2])-1);
	      vertexNormalsIds.push_back(sd::raw::readAcsii<size_t>(result[3*nbFields+2])-1);
	      vertexNormalsIds.push_back(sd::raw::readAcsii<size_t>(result[0*nbFields+2])-1);
	    }
	  } else if (faceCard == 3) {
	    faces.push_back(sd::raw::readAcsii<size_t>(result[0*nbFields])-1);
	    faces.push_back(sd::raw::readAcsii<size_t>(result[1*nbFields])-1);
	    faces.push_back(sd::raw::readAcsii<size_t>(result[2*nbFields])-1);
	    if ((nbFields >= 2) && !(result[0*nbFields+1].empty())) {
	      assert(sd::raw::readAcsii<size_t>(result[0*nbFields+1]) > 0);
	      assert(sd::raw::readAcsii<size_t>(result[1*nbFields+1]) > 0);
	      assert(sd::raw::readAcsii<size_t>(result[2*nbFields+1]) > 0);
	      vertexTexturesIds.push_back(sd::raw::readAcsii<size_t>(result[0*nbFields+1])-1);
	      vertexTexturesIds.push_back(sd::raw::readAcsii<size_t>(result[1*nbFields+1])-1);
	      vertexTexturesIds.push_back(sd::raw::readAcsii<size_t>(result[2*nbFields+1])-1);
	    }
	    if ((nbFields == 3) && !(result[0*nbFields+2].empty())) {
	      assert(sd::raw::readAcsii<size_t>(result[0*nbFields+2]) > 0);
	      assert(sd::raw::readAcsii<size_t>(result[1*nbFields+2]) > 0);
	      assert(sd::raw::readAcsii<size_t>(result[2*nbFields+2]) > 0);
	      vertexNormalsIds.push_back(sd::raw::readAcsii<size_t>(result[0*nbFields+2])-1);
	      vertexNormalsIds.push_back(sd::raw::readAcsii<size_t>(result[1*nbFields+2])-1);
	      vertexNormalsIds.push_back(sd::raw::readAcsii<size_t>(result[2*nbFields+2])-1);
	    }
	  } else {
#ifdef DEBUG
	    std::cerr << "ERROR CAN NOT BE PARSED >" << buff << "< " << result.size() << " faceCard = "  << faceCard << std::endl;
#endif
	  }
	} else {
	  infile.getline(buff, 256);
	  sd::core::Vector<std::string> result;
	  std::string buffStr = "";
	  sd::raw::split(buff, " ", result);
	  sd::utils::removeIf(result, "");
	  sd::utils::removeIf(result, "\r");
	  sd::raw::merge(result, " ", buffStr);
	  sd::raw::split(buffStr.c_str(), " /", result);
	  const size_t faceCard = result.size()/(1+sd::raw::numberOf(buffStr.c_str(), '/', ' '));
	  const size_t nbFields = result.size()/faceCard;
	  if (faceCard%4 == 0) {
	    faces.push_back(sd::raw::readAcsii<size_t>(result[0*nbFields])-1);
	    faces.push_back(sd::raw::readAcsii<size_t>(result[1*nbFields])-1);
	    faces.push_back(sd::raw::readAcsii<size_t>(result[2*nbFields])-1);
	    faces.push_back(sd::raw::readAcsii<size_t>(result[2*nbFields])-1);
	    faces.push_back(sd::raw::readAcsii<size_t>(result[3*nbFields])-1);
	    faces.push_back(sd::raw::readAcsii<size_t>(result[0*nbFields])-1);
	  } else {
#ifdef DEBUG
	    std::cerr << "ERROR CAN NOT BE PARSED >" << buff << "< " << result.size() << " faceCard = "  << faceCard << std::endl;
#endif
	  }
	}
	break;
      }
      case 'n': {
	infile.get();
	infile.getline(buff, 256);
	sd::core::Vector<std::string> result;
	sd::raw::split(buff, " ", result);
	sd::utils::removeIf(result, "");
	sd::utils::removeIf(result, "\r");
	if (result.size() != 3) return NULL;
	faceNormals.push_back(sd::raw::readAcsii<double>(result[0]));
	faceNormals.push_back(sd::raw::readAcsii<double>(result[1]));
	faceNormals.push_back(sd::raw::readAcsii<double>(result[2]));
	break;
      }
      case 'c': {
	infile.get();
	infile.getline(buff, 256);
	sd::core::Vector<std::string> result;
	sd::raw::split(buff, " ", result);
	sd::utils::removeIf(result, "");
	sd::utils::removeIf(result, "\r");
	if (result.size() != 3) return NULL;
	faceColors.push_back(sd::raw::readAcsii<float>(result[0])*255.);
	faceColors.push_back(sd::raw::readAcsii<float>(result[1])*255.);
	faceColors.push_back(sd::raw::readAcsii<float>(result[2])*255.);
	break;
      }
      default: {
	infile.ignore(256, '\n');
	break;
      }
      }
      break;
    }
    default: {
      infile.ignore(256, '\n');
      break;
    }
    }
  }

  infile.close();
  
  if (vertexNormalsIds.size() != 0 && vertexNormalsIds.size() == faces.size()) {
    sd::core::Vector<typename sd::core::Normal<T>::type> vertexNormalsCopy(vertexNormals);
    vertexNormals.resize(vertices.size());
    for (unsigned int i=0; i<vertexNormalsIds.size(); ++i)
      for (unsigned int j=0; j<3; ++j)
	vertexNormals[faces[i]*3+j] = vertexNormalsCopy[(vertexNormalsIds[i]*3+j)];
  } else {
#ifdef DEBUG
    if (vertexNormalsIds.size() == 0) {
      std::cerr << "NO NORMALS ON FACES" << std::endl;
    } else {
      vertexNormals.resize(0);
      std::cerr << "NO NORMALS ON ALL FACES" << std::endl;
    }
#endif
  }
  
  if (vertexTexturesIds.size() != 0 && vertexTexturesIds.size() == faces.size()) {
    sd::core::Vector<double> vertexTexturesCopy(vertexTextures);
    vertexTextures.resize(vertices.size());
    for (unsigned int i=0; i<vertexTexturesIds.size(); ++i)
      for (unsigned int j=0; j<3; ++j)
	vertexTextures[faces[i]*3+j] = vertexTexturesCopy[(vertexTexturesIds[i]*3+j)];
  } else {
#ifdef DEBUG
    if (vertexTexturesIds.size() == 0) {
      std::cerr << "NO TEXTURES ON FACES" << std::endl;
    } else {
      vertexTextures.resize(0);
      std::cerr << "NO TEXTURES ON ALL FACES" << std::endl;
    }
#endif
  }
  
  return new sd::core::MeshView_<T, N>(fileURI,
				       vertices,
				       faces,
				       sd::core::Vector<unsigned int>(),
				       vertexNormals,
				       //faceNormals,
				       vertexColors,
				       //faceColors,
				       vertexTextures);
}

#endif /* ! SD_OBJ_LOADER_OBJ_HXX */
