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

#ifndef SD_MESH_PROCESSING_CURVATURE_ANALYSIS_AVAILABLE_TOOLS_MANAGER_HPP
#define SD_MESH_PROCESSING_CURVATURE_ANALYSIS_AVAILABLE_TOOLS_MANAGER_HPP

namespace sd {

  namespace meshprocessing {
    
    template<typename A>
    class AvailableToolsManager {
      
    public:
      
      virtual ~AvailableToolsManager() {
	for (unsigned int i=0; i<m_data.size(); ++i)
	  delete m_data[i];
      }
      
      A* get(const std::string& name) {
	unsigned int idx = std::find(m_names.begin(), m_names.end(),
				     name)-m_names.begin();
	if (idx < m_data.size())
	  return m_data[idx];
	return NULL;
      }
      
      void add(A* a) {
	m_data.push_back(a);
	loadNames();
      }
      
      void remove(const std::string& name) {
	assert(get(name) != NULL);
	assert(std::find(get(name)) != m_data.end());
	const typename std::vector<A*>::iterator it = std::find(get(name));
	delete *it;
	m_data.erase(*it);
	loadNames();
      }
      
      const std::vector<A*>& availables() const {return m_data;}
      const std::vector<std::string>& names() const {return m_names;}
      
    protected:
      
      AvailableToolsManager() : m_data(), m_names() {}
      
      std::vector<A*>& data() {return m_data;}
      virtual void load() = 0;
      
      void loadNames() {
	m_names.resize(m_data.size());
	for (unsigned int i=0; i<m_data.size(); ++i)
	  m_names[i] = m_data[i]->name();
      }
      
    private:
      
      std::vector<A*> m_data;
      std::vector<std::string> m_names;
      
    };
    
  }
  
}

#endif /*! SD_MESH_PROCESSING_CURVATURE_ANALYSIS_AVAILABLE_TOOLS_MANAGER_HPP */
