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

#ifndef SD_STL_LOADER_STL_HXX
#define SD_STL_LOADER_STL_HXX

#include <cstring>
#include <fstream>

#include <core/mesh/MeshView.hpp>
#include <core/mesh/MeshView_.hpp>

#include <utils/sdRawUtils.hpp>

namespace sd {
  namespace stl {
    namespace {

      static const UINT BINARY_NAME_LENGTH = 80;
      //nb of byte per data (float => 4 bytes)
      static const UINT BYTE_LEN = 4;
      // Facet : 1 normal + 3 vertices (3 coord per data, 4byte per coord) + attribute count ( 2 byte )
      static const UINT BYTE_PER_FACES= BYTE_LEN*3*4+2;
      //check chExpected & chFound equality
      bool  
      checkStr(std::string chExpected, std::string chFound, std::string & errMsg, bool print)
      {
	if(chExpected.compare(chFound) != 0)
	  {
	    if(print)
	      {
		std::ostringstream ostr;
		ostr << "-Fail to load mesh as Ascii StL file.\n";
		ostr << "Expected Keyword: \"" << chExpected.c_str() << "\"\n";
		ostr << "\"" <<chFound.c_str() << "\" found\n";
		errMsg = ostr.str();
	      }
	    return false;
	  }
	return true;
      }
      bool
      checkStr(std::string chExpected, std::string chFound, std::string& errMsg)
      {
	return checkStr(chExpected, chFound, errMsg, true);
      }
      //create vertex normal
      template<typename T, unsigned int N>
      sd::core::VertexFromMeshView<T>
      buildN(core::MeshView_<T, N>* mesh, sd::core::VertexFromMeshView<T> vertex){
	auto normal = new sd::core::VertexFromMeshView<T>();
	normal->set(0,0,0);
	double val[3] = {0,0,0};
	std::vector<sd::core::FaceFromMeshView<T, 3>* > result;
	for (auto it = mesh->beginFaces(); it != mesh->endFaces(); ++it) {
	   auto face = *it;
	   for (int i = 0; i < 3; ++i) {
	     auto vert = face->getAt(mesh, i);
	     if (vertex.x() == vert.x() && vertex.y() == vert.y() && vertex.z() == vert.z())
	       result.push_back(face);
	   }
	}
	for(unsigned int i=0; i<result.size(); ++i){
	  auto face = result[i];
	  auto norm = face->getNormal();
	  val[0] += norm[0];
	  val[1] += norm[1];
	  val[2] += norm[2];
	}
	double n = (float) sqrt(val[0]*val[0] + val[1]*val[1] + val[2]*val[2]);
	if (n==0) return *normal;
	normal->set(val[0]/n,val[1]/n,val[2]/n);	
	return *normal;
       }
      
      template<typename T, unsigned int N>
      core::MeshView_<T, N>*
      stlBinaryParse( char*& data, size_t& readable, std::string& errMsg)
      {

	sd::core::Vector<T> vertices;
    	sd::core::Vector<DOUBLE> faceNormals;	
	sd::core::Vector<UINT> faces;
	std::string name;
	UINT nb_facet;
	


	if (readable < BYTE_PER_FACES){
	  errMsg += "-Fail to load mesh as Binary StL file. Incorrect size\n" ;
	  return 0;
	}
	//mesh name (read 80byte)
	for (size_t i = 0; i < BINARY_NAME_LENGTH ; ++i){
	  if(*data < 127 && *data != 0 && *data !='\n')
	  	name += *data;
	  ++data;
	  --readable;
	  
	}

	//nb faces
	memcpy(&nb_facet, data, BYTE_LEN);
	data += BYTE_LEN;
	readable -= BYTE_LEN;
	
	std::string err;
	raw::checkCorrupted(readable + BINARY_NAME_LENGTH + BYTE_LEN, nb_facet*BYTE_PER_FACES + BINARY_NAME_LENGTH + BYTE_LEN, err);

	//reserve and resize vectors
	try
	{
	  vertices.resize(nb_facet*3*3);
	  faceNormals.reserve(nb_facet*3);
	  faces.reserve(nb_facet*3);
	}
	catch(...)
	{
	  std::ostringstream ostr;
	  ostr << "-Fail to load mesh as Binary StL file. May not be a binary file or too many faces for memory: "<<nb_facet<<"\n";
	  errMsg += ostr.str();
	  if (err.size() != 0) errMsg += "Incorrect size if binary STL file : \n" + err;	
	  return 0;
	}
	if (readable < nb_facet*BYTE_PER_FACES){
	  errMsg += "-Fail to load mesh as Binary StL file \n" + err;
	  return 0;
	}
	errMsg = err;
	float no [3];
	for (size_t i = 0; i < nb_facet; ++i) {
	  //Normal
	  memcpy(&no[0], data, 3*BYTE_LEN);
	  faceNormals.insert(faceNormals.end(), no, no+3);
	  data += 3*BYTE_LEN;
	  
	  //vert1 vert2 vert3 => 9 vertices of 4byte each
	  memcpy(&vertices[9*i], data,9*BYTE_LEN);
	  data += 9*BYTE_LEN;	   

	  //attribute byte count
	  data += 2;
	  //readable => readable-byte read
	  readable -= BYTE_PER_FACES;
	}
	for (UINT i = 0; i < nb_facet*3; ++i)
	  faces.push_back(i);
	
	sd::core::MeshView_<T, N>* mesh = new sd::core::MeshView_<T, N>(name, vertices, faces);

	// fill vertices normal ...take too long time because of builN(...) very long
	/*
	auto mesh = new sd::core::MeshView_<T, 3>(vertices, faces, std::vector<unsigned int>(), std::vector<DOUBLE>(vertices.size()),colors, name);

	//add vertices normal

	for (auto it = mesh->beginVertices(); it != mesh->endVertices(); ++it) {
	  auto vertex = *it;
	  auto normal = buildN<T>(mesh,vertex);
	  auto normalVect = sd::core::Vector3d<DOUBLE>(normal.x(),normal.y(),normal.z());
	  mesh->setNormal(vertex,normalVect);

	}
	*/
	
	return mesh;
      }

      template<typename T, unsigned int N>
      core::MeshView_<T, N>*
      stlParse( char*& data, size_t& readable, std::string& errMsg)
      {
	
	sd::core::Vector<T> vertices;
    	sd::core::Vector<DOUBLE> faceNormals;
	sd::core::Vector<UINT> faces;

	//stl keywords
	static const std::string SOLID = "solid";
	static const std::string FACET = "facet";
	static const std::string NORMAL = "normal";
	static const std::string OUTER = "outer";	
	static const std::string LOOP = "loop";	
	static const std::string VERTEX = "vertex";
	static const std::string ENDLOOP = "endloop";
	static const std::string ENDFACET = "endfacet";
	static const std::string ENDSOLID = "endsolid";

	std::string name;
	std::string in;
	int nbFacet = 0;
	int nbVert = 0;
	T maxVal = 0;
	T minVal = 0;
	T val;

	//start
	raw::skipAsciiSpaces(data, readable);
	raw::skipAsciiComments(data, readable);
	in = raw::readAsciiString(data,readable);
	//solid name
	if( !checkStr(SOLID, in, errMsg)) return 0;
	raw::skipAsciiSpaces(data, readable);
	name = raw::readAsciiString(data, readable);
	raw::skipAsciiEndOfLine(data, readable);
	raw::skipAsciiSpaces(data, readable);
	//2nd line => facet
	in = raw::readAsciiString(data, readable);
	while(readable && data) {
	  nbFacet++;
	  if( !checkStr(FACET, in, errMsg)) return 0;//string facet
	  raw::skipAsciiSpaces(data, readable);
	  if( !checkStr(NORMAL, raw::readAsciiString(data, readable), errMsg)) return 0; //string normal
	  raw::skipAsciiSpaces(data, readable);
	  for(int i = 0; i < 3; ++i){
  	    raw::readAsciiFloat(data, readable);
	    faceNormals.push_back( -T (raw::readAsciiFloat(data, readable)));
	    raw::skipAsciiSpaces(data, readable);
	  }
	  
	  if( !checkStr(OUTER, raw::readAsciiString(data, readable), errMsg)) return 0;//string outer
	  raw::skipAsciiSpaces(data, readable);
	  if( !checkStr(LOOP, raw::readAsciiString(data, readable), errMsg)) return 0;//string loop
	  raw::skipAsciiSpaces(data, readable);
	  
	  for(int i = 0; i < 3; ++i){
	    if( !checkStr(VERTEX, raw::readAsciiString(data, readable), errMsg)) return 0; //string vertex
	    raw::skipAsciiSpaces(data, readable);
	    //3 coord
	    for(int j = 0; j < 3; ++j){
	      val =  T (raw::readAsciiFloat(data, readable));
	      if(val < minVal) minVal = val;
	      if(val > maxVal) maxVal = val;
	      vertices.push_back(val);
	      raw::skipAsciiSpaces(data, readable);
	    }
	    nbVert++;
	  }
	  if( !checkStr(ENDLOOP, raw::readAsciiString(data, readable), errMsg)) return 0;//string endloop
	  raw::skipAsciiSpaces(data, readable);
	  if( !checkStr(ENDFACET, raw::readAsciiString(data, readable), errMsg)) return 0;//string endfacet
	  raw::skipAsciiSpaces(data, readable);
	  //test endsolid: in = endsolid, or in = facet
	  in=raw::readAsciiString(data,readable);
	  if(checkStr(in,ENDSOLID,errMsg,false)) break;
	}
	//faces:
	faces.reserve(nbFacet*3);
	for(int i = 0; i < nbVert; ++i)
	  faces.push_back(i);

	sd::core::MeshView_<T, N>* mesh = new sd::core::MeshView_<T, N>(name, vertices, faces);
	// fill vertices normal ...take too long time because of builN(...) very long
	/*
	auto mesh = new sd::core::MeshView_<T, 3>(vertices, faces, std::vector<unsigned int>(), std::vector<DOUBLE>(vertices.size()),colors, name);

	//add vertices normal

	for (auto it = mesh->beginVertices(); it != mesh->endVertices(); ++it) {
	  auto vertex = *it;
	  auto normal = buildN<T>(mesh,vertex);
	  auto normalVect = sd::core::Vector3d<DOUBLE>(normal.x(),normal.y(),normal.z());
	  mesh->setNormal(vertex,normalVect);

	}
	*/
	
	return mesh;
      }
    }

    template<typename T, unsigned int N>
    core::MeshView_<T, N>*
    loadFile(const std::string& filename, std::string& errMsg) {
      
      std::ifstream is(filename.c_str(), std::ios::binary);
      if ( !is.is_open())
	return NULL;
      is.seekg(0L, std::ios::end);
      size_t file_len = is.tellg();
      is.seekg(0L, std::ios::beg);
      
      std::vector<char> pdata(file_len+1);
      char *data = &pdata[0];
      char *saved_data = data;
      is.read(data, file_len);
      size_t readable = file_len;
      size_t saved_readable = readable;
      
      bool binary = false;
      is.seekg(BINARY_NAME_LENGTH, std::ios::beg);
      UINT nb_faces;
      //if binary => readable = BYTE_PER_FACES*nb_faces + BINARY_NAME_LENGTH + BYTE_LEN ( = nb_faces)
      size_t expected_size = BYTE_PER_FACES*nb_faces + BINARY_NAME_LENGTH + BYTE_LEN;
      is.read((char *)&nb_faces, BYTE_LEN);

      //high chances to be a binary file
      if (readable == expected_size ) binary = true;
      is.close();

      sd::core::MeshView_<T, N>* mesh = stlParse<T, N>(data, readable, errMsg); 
      if(mesh == 0){
	data = saved_data;
	readable = saved_readable;
	if(binary) errMsg = "";
	mesh = stlBinaryParse<T, N>(data, readable, errMsg);
      }

      if(mesh == 0){
	//load fail!
	std::ostringstream ostr;
	ostr << "-Unable to load \""<<filename.c_str()<<"\n\n";
	errMsg += ostr.str();
      }
      
      //show errMsg
      /*if(errMsg.size() > 0)
	printf("%s\n", errMsg.c_str());
*/
      return mesh;
    }
    
    template<typename T, unsigned int N>
    core::MeshView_<T, N>*
    LoaderStl<T, N>::loadStlImage(const std::string& filename) const {
      std::string errMsg; // :TODO: do something with it!
      return loadFile<T, N>(filename, errMsg); 
    }
    
  }
}
#endif /*! SD_STL_LOADER_STL_HXX */
