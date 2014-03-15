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

#ifndef SD_PLY_LOADER_PLY_HXX
#define SD_PLY_LOADER_PLY_HXX

#include <fstream>
#ifdef DEBUG
#include <iostream>
#endif
#include <vector>

#include <utils/sdRawUtils.hpp>
#include <utils/sdTypeUtils.hpp>

template<typename T, unsigned int N>
sd::core::MeshView_<T, N> *
sd::ply::LoaderPly<T, N>::loadPlyMesh(const std::string& fileURI) const {
  std::ifstream infile(fileURI.c_str(), std::ifstream::in | std::ifstream::binary);
  if (!infile) {
#ifdef DEBUG
    std::cerr << "LoaderPly::loadPlyMesh() : unable to open file: " << fileURI << std::endl;
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
  sd::core::Vector<typename sd::core::Normal<T>::type> faceNormals;
  sd::core::Vector<unsigned char> faceColors;
  
  unsigned int nbVertices = 0;
  unsigned int nbFaces = 0;
  sd::core::Vector<std::pair<std::string, std::string> > vertexProperties;
  sd::core::Vector<std::pair<std::string, std::string> > faceProperties;
  
  data_format format;
  
#ifdef DEBUG
  std::cerr << "Load header : ";
#endif
  bool loadable = readHeader(infile,
			     nbVertices, vertexProperties,
			     nbFaces, faceProperties,
			     format);
  if (loadable) {
#ifdef DEBUG
    std::cerr << "ok" << std::endl;
    std::cerr << "nbVertices = " << nbVertices << " nbFaces = " <<  nbFaces << std::endl;
#endif
    if (format == ASCII) {
      loadable &= loadAscii(infile,
			    nbVertices, vertexProperties,
			    vertices, vertexNormals, vertexColors, vertexTextures,
			    nbFaces, faceProperties,
			    faces, faceNormals, faceColors);
    } else { //if (format == BINARY_LITTLE_ENDIAN) {
      loadable &= loadBinary(format, infile,
			     nbVertices, vertexProperties,
			     vertices, vertexNormals, vertexColors, vertexTextures,
			     nbFaces, faceProperties,
			       
			     faces, faceNormals, faceColors);
    }/* else if (format == BINARY_LITTLE_ENDIAN) {
      loadable &= loadBinary(format, infile,
			     nbVertices, vertexProperties,
			     vertices, vertexNormals, vertexColors, vertexTextures,
			     nbFaces, faceProperties,
			     faces, faceNormals, faceColors);
    } else {
      loadable =false;
      }*/
  } else {
#ifdef DEBUG
    std::cerr << "failed" << std::endl;
#endif
  }

  infile.close();
  
  return (loadable)
    ? new sd::core::MeshView_<T, N>(fileURI, vertices, faces,
				    sd::core::Vector<unsigned int>(),
				    vertexNormals,
				    //faceNormals,
				    vertexColors
				    //faceColors
				    )
    : NULL;
}

template<typename T, unsigned int N>
bool
sd::ply::LoaderPly<T, N>::readHeader(std::ifstream& infile,
				     unsigned int& nbVertices,
				     std::vector<std::pair<std::string, std::string> >& vertexProperties,
				     unsigned int& nbFaces,
				     std::vector<std::pair<std::string, std::string> >& faceProperties,
				     data_format& format) {
  
  char buff[256];
  sd::core::Vector<std::string> result(0);
  nbVertices = 0;
  nbFaces = 0;
  
  if (infile.eof()) return false;
  infile.getline(buff, 256);
  if (std::string(buff) != "ply")
    return false;
  
  if (infile.eof()) return false;
  infile.getline(buff, 256);
  sd::raw::split(buff, " ", result);
  if ((result[0] != "format") || (result[2] != "1.0")) return false;
  if (result[1] == "ascii") format = ASCII;
  else if (result[1] == "binary_little_endian") format = BINARY_LITTLE_ENDIAN;
  else if (result[1] == "binary_big_endian") format = BINARY_BIG_ENDIAN;
  else return false;
  
  if (infile.eof()) return false;
  infile.getline(buff, 256);
  sd::raw::split(buff, " ", result);
  while (result[0] == "comment" || result[0] == "obj_info") {
    if (infile.eof()) return false;
    infile.getline(buff, 256);
    sd::raw::split(buff, " ", result);
  }
  
  while (!infile.eof()) {
    if (result[0] == "element") {
      if (result[1] == "vertex") {
	nbVertices = sd::raw::readAcsii<size_t>(result[2]);
	if (infile.eof()) return false;
	infile.getline(buff, 256);
	sd::raw::split(buff, " ", result);
	while (result[0] == "property") {
	  if (result.size() != 3) return false;
	  vertexProperties.push_back(std::pair<std::string, std::string>(result[1], result[2]));
	  if (infile.eof()) return false;
	  infile.getline(buff, 256);
	  sd::raw::split(buff, " ", result);
	}
      } else if (result[1] == "face") {
	nbFaces = sd::raw::readAcsii<size_t>(result[2]);
	if (infile.eof()) return false;
	infile.getline(buff, 256);
	sd::raw::split(buff, " ", result);
	while (result[0] == "property") {
	  if (result.size() != 5) return false;
	  faceProperties.push_back(std::pair<std::string, std::string>(result[2], result[3]));
	  if (infile.eof()) return false;
	  infile.getline(buff, 256);
	  sd::raw::split(buff, " ", result);
	}
      } else {
	// NOT SUPPORTED
	if (infile.eof()) return false;
	infile.getline(buff, 256);
	sd::raw::split(buff, " ", result);
      }
    } else if(result[0] == "end_header") {
      return true;
    } else {
      return false;
    }
  }
  // Never used...
  return false;
}

template<typename T, unsigned int N>
bool
sd::ply::LoaderPly<T, N>::loadAscii(std::ifstream& infile,
				    unsigned int nbVertices,
				    std::vector<std::pair<std::string, std::string> >& vertexProperties,
				    sd::core::Vector<T>& vertices,
				    sd::core::Vector<typename sd::core::Normal<T>::type>& vertexNormals,
				    sd::core::Vector<unsigned char>& vertexColors,
				    sd::core::Vector<double>& /*vertexTextures*/,
				    unsigned int nbFaces,
				    std::vector<std::pair<std::string, std::string> >& /*faceProperties*/,
				    sd::core::Vector<unsigned int>& faces,
				    sd::core::Vector<typename sd::core::Normal<T>::type>& /*faceNormals*/,
				    sd::core::Vector<unsigned char>& /*faceColors*/) {
  char buff[256];
  sd::core::Vector<std::string> result(0);
  vertices.resize(nbVertices*3);
  char xIdx = -1;
  char yIdx = -1;
  char zIdx = -1;
  char nxIdx = -1;
  char nyIdx = -1;
  char nzIdx = -1;
  char rIdx = -1;
  char gIdx = -1;
  char bIdx = -1;
  char aIdx = -1;
  for (unsigned int i=0; i<vertexProperties.size(); ++i) {
    if (vertexProperties[i].second == "x") {
      xIdx = i;
    } else if (vertexProperties[i].second == "y") {
      yIdx = i;
    } else if (vertexProperties[i].second == "z") {
      zIdx = i;
    } else if (vertexProperties[i].second == "nx") {
      nxIdx = i;
    } else if (vertexProperties[i].second == "ny") {
      nyIdx = i;
    } else if (vertexProperties[i].second == "nz") {
      nzIdx = i;
    } else if (vertexProperties[i].second == "red") {
      rIdx = i;
    } else if (vertexProperties[i].second == "green") {
      gIdx = i;
    } else if (vertexProperties[i].second == "blue") {
      bIdx = i;
    } else if (vertexProperties[i].second == "alpha") {
      aIdx = i;
    } 
  }
  
  assert(xIdx != -1 && yIdx != -1 && zIdx != -1);
  
  //if (sd::core::DataType::getDataType(vertexProperties[xIdx].first)
  //!= sd::core::DataType_<T>())
  //return false;
    
  const unsigned int normalCard = sd::core::Normal<T>::card();
  const unsigned int colorCard = sd::core::RGBCoordinates<T>::card();

  const bool vNormalDef = ((nxIdx != -1 && nyIdx != -1 && nzIdx != -1)
			   && (vertexProperties[nxIdx].first == vertexProperties[nyIdx].first)
			   && (vertexProperties[nxIdx].first == vertexProperties[nzIdx].first));
  const bool vColorDef = ((rIdx != -1 && gIdx != -1 && bIdx != -1)
			  && (vertexProperties[rIdx].first == vertexProperties[gIdx].first)
			  && (vertexProperties[rIdx].first == vertexProperties[bIdx].first));
  const bool vColorTypeIsUchar = (vColorDef
				  && sd::core::DataType::getDataType(vertexProperties[rIdx].first)
				  == sd::core::DataType::getDataType("uchar"));
  if (vNormalDef)
    vertexNormals.resize(nbVertices*sd::core::Normal<T>::card());
  if (vColorDef)
    vertexColors.resize(nbVertices*sd::core::RGBCoordinates<T>::card());
  for (unsigned int i=0; i<nbVertices; ++i) {
    if (infile.eof()) return false;
    infile.getline(buff, 256);
    sd::raw::split(buff, " ", result);
    sd::utils::removeIf(result, "");
    vertices[i*3] = sd::raw::readAcsii<T>(result[xIdx]);
    vertices[i*3+1] = sd::raw::readAcsii<T>(result[yIdx]);
    vertices[i*3+2] = sd::raw::readAcsii<T>(result[zIdx]);
    if (vNormalDef) {
      vertexNormals[i*normalCard] = sd::raw::readAcsii<T>(result[nxIdx]);
      vertexNormals[i*normalCard+1] = sd::raw::readAcsii<T>(result[nyIdx]);
      vertexNormals[i*normalCard+2] = sd::raw::readAcsii<T>(result[nzIdx]);
    }
    if (vColorDef) {
      if (vColorTypeIsUchar) {
	vertexColors[i*colorCard] = sd::raw::readAcsii<unsigned int>(result[rIdx]);
	vertexColors[i*colorCard+1] = sd::raw::readAcsii<unsigned int>(result[gIdx]);
	vertexColors[i*colorCard+2] = sd::raw::readAcsii<unsigned int>(result[bIdx]);
	if (aIdx != -1)
	  vertexColors[i*colorCard+3] = sd::raw::readAcsii<unsigned int>(result[aIdx]);
      } else {
	vertexColors[i*colorCard] = sd::raw::readAcsii<T>(result[rIdx])*255.;
	vertexColors[i*colorCard+1] = sd::raw::readAcsii<T>(result[gIdx])*255.;
	vertexColors[i*colorCard+2] = sd::raw::readAcsii<T>(result[bIdx])*255.;
	if (aIdx != -1)
	  vertexColors[i*colorCard+3] = sd::raw::readAcsii<T>(result[aIdx])*255.;
	else
	  vertexColors[i*colorCard+3] = 255.;
      }
    }
  }
  for (unsigned int i=0; i<nbFaces; ++i) {
    if (infile.eof()) return false;
    infile.getline(buff, 256);
    sd::raw::split(buff, " ", result);
    sd::utils::removeIf(result, "");
    const size_t faceCard = sd::raw::readAcsii<size_t>(result[0]);
      if (faceCard == 3) {
	faces.push_back(sd::raw::readAcsii<size_t>(result[1]));
	faces.push_back(sd::raw::readAcsii<size_t>(result[2]));
	faces.push_back(sd::raw::readAcsii<size_t>(result[3]));
      } else if (faceCard == 4) {
	faces.push_back(sd::raw::readAcsii<size_t>(result[1]));
	faces.push_back(sd::raw::readAcsii<size_t>(result[2]));
	faces.push_back(sd::raw::readAcsii<size_t>(result[3]));
	faces.push_back(sd::raw::readAcsii<size_t>(result[3]));
	faces.push_back(sd::raw::readAcsii<size_t>(result[4]));
	faces.push_back(sd::raw::readAcsii<size_t>(result[1]));
      } else {
#ifdef DEBUG
	std::cerr << "ERROR CAN NOT BE PARSED >" << buff << "< " << result.size() << " faceCard = "  << faceCard << std::endl;
#endif
      }
  }
  return true;
}

template<typename T, unsigned int N>
bool
sd::ply::LoaderPly<T, N>::loadBinary(data_format format,
				     std::ifstream& infile,
				     unsigned int nbVertices,
				     std::vector<std::pair<std::string, std::string> >& vertexProperties,
				     sd::core::Vector<T>& vertices,
				     sd::core::Vector<typename sd::core::Normal<T>::type>& vertexNormals,
				     sd::core::Vector<unsigned char>& vertexColors,
				     sd::core::Vector<double>& /*vertexTextures*/,
				     unsigned int nbFaces,
				     std::vector<std::pair<std::string, std::string> >& /*faceProperties*/,
				     sd::core::Vector<unsigned int>& faces,
				     sd::core::Vector<typename sd::core::Normal<T>::type>& /*faceNormals*/,
				     sd::core::Vector<unsigned char>& /*faceColors*/) {
  /*
  vertices.resize(nbVertices*3);
  char xIdx = -1;
  char yIdx = -1;
  char zIdx = -1;
  char nxIdx = -1;
  char nyIdx = -1;
  char nzIdx = -1;
  char rIdx = -1;
  char gIdx = -1;
  char bIdx = -1;
  char aIdx = -1;
  for (unsigned int i=0; i<vertexProperties.size(); ++i) {
    if (vertexProperties[i].second == "x") {
      xIdx = i;
    } else if (vertexProperties[i].second == "y") {
      yIdx = i;
    } else if (vertexProperties[i].second == "z") {
      zIdx = i;
    } else if (vertexProperties[i].second == "nx") {
      nxIdx = i;
    } else if (vertexProperties[i].second == "ny") {
      nyIdx = i;
    } else if (vertexProperties[i].second == "nz") {
      nzIdx = i;
    } else if (vertexProperties[i].second == "red") {
      rIdx = i;
    } else if (vertexProperties[i].second == "green") {
      gIdx = i;
    } else if (vertexProperties[i].second == "blue") {
      bIdx = i;
    } else if (vertexProperties[i].second == "alpha") {
      aIdx = i;
    } 
  }
  
  assert(xIdx != -1 && yIdx != -1 && zIdx != -1);
  
  if (sd::core::DataType::getDataType(vertexProperties[xIdx].first)
      != sd::core::DataType_<T>())
    return false;
  
  const unsigned int normalCard = sd::core::Normal<T>::card();
  const unsigned int colorCard = sd::core::RGBCoordinates<T>::card();

  const bool vNormalDef = ((nxIdx != -1 && nyIdx != -1 && nzIdx != -1)
			   && (vertexProperties[nxIdx].first == vertexProperties[nyIdx].first)
			   && (vertexProperties[nxIdx].first == vertexProperties[nzIdx].first));
  const bool vColorDef = ((rIdx != -1 && gIdx != -1 && bIdx != -1)
			  && (vertexProperties[rIdx].first == vertexProperties[gIdx].first)
			  && (vertexProperties[rIdx].first == vertexProperties[bIdx].first));
  const bool vColorTypeIsUchar = (vColorDef
				  && sd::core::DataType::getDataType(vertexProperties[rIdx].first)
				  == sd::core::DataType::getDataType("uchar"));
  if (vNormalDef)
    vertexNormals.resize(nbVertices*sd::core::Normal<T>::card());
  if (vColorDef)
    vertexColors.resize(nbVertices*sd::core::RGBCoordinates<T>::card());
  
  std::vector<char> buff;
  std::istreambuf_iterator<char> begin(infile);
  std::istreambuf_iterator<char> end;
  std::copy(begin, end, std::back_inserter(buff));
  char* data = buff.data();
  for (unsigned int i=0; i<nbVertices; ++i) {
    unsigned char* dataChar = NULL;
    T* dataTmp = (T*)data;
    vertices[i*3] = *(dataTmp+xIdx);
    vertices[i*3+1] = *(dataTmp+yIdx);
    vertices[i*3+2] = *(dataTmp+zIdx);
    dataTmp += nxIdx;
    data = (char*)dataTmp;
    
    if (vNormalDef) {
      dataTmp = (T*)data;
      vertexNormals[i*normalCard] = *dataTmp;
      vertexNormals[i*normalCard+1] = *(dataTmp+1);
      vertexNormals[i*normalCard+2] = *(dataTmp+2);
      dataTmp += 3;
      data = (char*)dataTmp;
    }
    if (vColorDef) {
      if (vColorTypeIsUchar) {
	dataChar = (unsigned char*)data;
	vertexColors[i*colorCard] = *dataTmp;
	vertexColors[i*colorCard+1] = *(dataTmp+1);
	vertexColors[i*colorCard+2] = *(dataTmp+2);
	if (aIdx != -1)
	  vertexColors[i*colorCard+3] = *(dataTmp+3);
	dataTmp += (aIdx == -1) ? 3 : 4;
	data = (char*)dataChar;
      } else {
	dataTmp = (T*)data;
	vertexColors[i*colorCard] = 255.*(*dataTmp);
	vertexColors[i*colorCard+1] = 255.*(*(dataTmp+1));
	vertexColors[i*colorCard+2] = 255.*(*(dataTmp+2));
	if (aIdx != -1)
	  vertexColors[i*colorCard+3] = 255.*(*(dataTmp+3));
	else
	  vertexColors[i*colorCard+3] = 255.;
	dataTmp += (aIdx == -1) ? 3 : 4;
	data = (char*)dataTmp;
      }
    }
  }
  unsigned int* dataInt = (unsigned int*)data;
  for (unsigned int i=0; i<nbFaces; ++i) {
    unsigned int faceCard = (*dataInt++);
    if (faceCard == 3) {
      faces.push_back(*dataInt);
      faces.push_back(*(dataInt+1));
      faces.push_back(*(dataInt+2));
      dataInt+=3;
    } else if (faceCard == 4) {
      faces.push_back(*dataInt);
      faces.push_back(*(dataInt+1));
      faces.push_back(*(dataInt+2));
      faces.push_back(*(dataInt+2));
      faces.push_back(*(dataInt+3));
      dataInt+=4;
      faces.push_back(*dataInt);
    }
  }
  return true;
  */
  
  char xIdx = -1, yIdx = -1, zIdx = -1,
    nxIdx = -1, nyIdx = -1, nzIdx = -1,
    rIdx = -1, gIdx = -1, bIdx = -1, aIdx = -1,
    globalIdx = -1;
  
  unsigned int xInc = 0, yInc = 0, zInc = 0,
    nxInc = 0, nyInc = 0, nzInc = 0,
    rInc = 0, gInc = 0, bInc = 0, aInc = 0,
    globalInc = 0;
  
  vertices.resize(nbVertices*3);
  indexOf(vertexProperties, "x", xInc, xIdx);
  indexOf(vertexProperties, "y", yInc, yIdx);
  indexOf(vertexProperties, "z", zInc, zIdx);
  indexOf(vertexProperties, "nx", nxInc, nxIdx);
  indexOf(vertexProperties, "ny", nyInc, nyIdx);
  indexOf(vertexProperties, "nz", nzInc, nzIdx);
  indexOf(vertexProperties, "red", rInc, rIdx);
  indexOf(vertexProperties, "green", gInc, gIdx);
  indexOf(vertexProperties, "blue", bInc, bIdx);
  indexOf(vertexProperties, "alpha", aInc, aIdx);
  indexOf(vertexProperties, "", globalInc, globalIdx);
  
  assert(xInc != globalInc && yInc != globalInc && zInc != globalInc);
  
  if (sd::core::DataType::getDataType(vertexProperties[xIdx].first)
      != sd::core::DataType_<T>())
    return false;
  
  const unsigned int normalCard = sd::core::Normal<T>::card();
  const unsigned int colorCard = sd::core::RGBCoordinates<T>::card();

  const bool vNormalDef = ((nxInc != globalInc && nyInc != globalInc && nzInc != globalInc)
			   && (vertexProperties[nxIdx].first == vertexProperties[nyIdx].first)
			   && (vertexProperties[nxIdx].first == vertexProperties[nzIdx].first));
  const bool vColorDef = ((rInc != globalInc && gInc != globalInc && bInc != globalInc)
			  && (vertexProperties[rIdx].first == vertexProperties[gIdx].first)
			  && (vertexProperties[rIdx].first == vertexProperties[bIdx].first));
  const bool vColorTypeIsUchar = (vColorDef
				  && sd::core::DataType::getDataType(vertexProperties[rIdx].first)
				  == sd::core::DataType::getDataType("uchar"));
  if (vNormalDef)
    vertexNormals.resize(nbVertices*sd::core::Normal<T>::card());
  if (vColorDef)
    vertexColors.resize(nbVertices*sd::core::RGBCoordinates<T>::card());
  
  std::vector<char> buff;
  std::istreambuf_iterator<char> begin(infile);
  std::istreambuf_iterator<char> end;
  std::copy(begin, end, std::back_inserter(buff));

  if (format == BINARY_BIG_ENDIAN) {
    char* data = buff.data();
    for (unsigned int i=0; i<nbVertices; ++i) {
      sd::raw::swapEndian(*(T*)(data+xInc));
      sd::raw::swapEndian(*(T*)(data+yInc));
      sd::raw::swapEndian(*(T*)(data+zInc));
      if (vNormalDef) {
	sd::raw::swapEndian(*(T*)(data+nxInc));
	sd::raw::swapEndian(*(T*)(data+nyInc));
	sd::raw::swapEndian(*(T*)(data+nzInc));
      }
      if (vColorDef) {
	if (vColorTypeIsUchar) {
	  sd::raw::swapEndian(*(UINT8*)(data+rInc));
	  sd::raw::swapEndian(*(UINT8*)(data+gInc));
	  sd::raw::swapEndian(*(UINT8*)(data+bInc));
	  if (aInc != globalInc)
	    sd::raw::swapEndian(*(UINT8*)(data+aInc));
	} else {
	  sd::raw::swapEndian(*(T*)(data+rInc));
	  sd::raw::swapEndian(*(T*)(data+gInc));
	  sd::raw::swapEndian(*(T*)(data+bInc));
	  if (aInc != globalInc)
	    sd::raw::swapEndian(*(T*)(data+aInc));
	}
      }
      data += globalInc;
    }
  }


  char* data = buff.data();
  for (unsigned int i=0; i<nbVertices; ++i) {
    vertices[i*3] = *(T*)(data+xInc);
    vertices[i*3+1] = *(T*)(data+yInc);
    vertices[i*3+2] = *(T*)(data+zInc);
    
    if (vNormalDef) {
      vertexNormals[i*normalCard] = *(T*)(data+nxInc);
      vertexNormals[i*normalCard+1] = *(T*)(data+nyInc);
      vertexNormals[i*normalCard+2] = *(T*)(data+nzInc);
    }
    if (vColorDef) {
      if (vColorTypeIsUchar) {
	vertexColors[i*colorCard] = *(UINT8*)(data+rInc);
	vertexColors[i*colorCard+1] = *(UINT8*)(data+gInc);
	vertexColors[i*colorCard+2] = *(UINT8*)(data+bInc);
	if (aInc != globalInc)
	  vertexColors[i*colorCard+3] = *(UINT8*)(data+aInc);
	else
	  vertexColors[i*colorCard+3] = 255.;
      } else {
	vertexColors[i*colorCard] = 255.*(*(T*)(data+rInc));
	vertexColors[i*colorCard+1] = 255.*(*(T*)(data+gInc));
	vertexColors[i*colorCard+2] = 255.*(*(T*)(data+bInc));
	if (aInc != globalInc)
	  vertexColors[i*colorCard+3] = 255.*(*(T*)(data+aInc));
	else
	  vertexColors[i*colorCard+3] = 255.;
      }
    }
    data += globalInc;
  }
  
  unsigned int* dataInt = (unsigned int*)data;
  for (unsigned int i=0; i<nbFaces; ++i) {
    unsigned char* dataChar = (unsigned char*) dataInt;
    unsigned char faceCard = (*dataChar);
    dataInt = (unsigned int*)++dataChar;
    if (faceCard == 3) {
      faces.push_back(*dataInt);
      faces.push_back(*(dataInt+1));
      faces.push_back(*(dataInt+2));
      dataInt+=3;
    } else if (faceCard == 4) {
      faces.push_back(*dataInt);
      faces.push_back(*(dataInt+1));
      faces.push_back(*(dataInt+2));
      faces.push_back(*(dataInt+2));
      faces.push_back(*(dataInt+3));
      faces.push_back(*dataInt);
      dataInt+=4;
    }
  }
  
  if (format == BINARY_BIG_ENDIAN) {
    /*sd::raw::swapEndian(vertexNormals);
      sd::raw::swapEndian(vertices);
      sd::raw::swapEndian(vertexColors);*/
    sd::raw::swapEndian(faces);
  }
  
  return true;
}


template<typename T, unsigned int N>
void
sd::ply::LoaderPly<T, N>::indexOf(const std::vector<std::pair<std::string, std::string> >& properties,
				  const std::string& property,
				  unsigned int& inc,
				  char& idx) {
  inc = 0;
  idx = -1;
  for (unsigned int i=0; i<properties.size(); ++i) {
    if (properties[i].second == property) {
      idx = i;
      return;
    }
    inc += sd::core::DataType::getDataType(properties[i].first).size()/sizeof(char);
  }
}

#endif /* ! SD_PLY_LOADER_PLY_HXX */
