/*
  Copyrigh (C) 2011 The SmithDR team, smithdr@labri.fr

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

#include <frontend/lib/ComputedImageViewAlgorithm.hpp>
#include <frontend/lib/Parameter.hpp>

#include <iostream>

class Algo1 : public sd::frontend::ComputedImageViewAlgorithm<double> {
  
public:
  Algo1(int pi1, int pi2, float pf1)
    : sd::frontend::ComputedImageViewAlgorithm<double>(m_name)
  {
    setParams(sd::frontend::Parameter("Pi1", pi1));
    setParams(sd::frontend::Parameter("Pi2", pi2));
    setParams(sd::frontend::Parameter("Pf1", pf1));
  }

  virtual bool run();
  virtual Algo1* clone() { return NULL; }
  virtual const std::string& name() const { return m_name; }

private:
  static const std::string m_name;
};

const std::string Algo1::m_name = "Algo1";

bool
Algo1::run()
{
  std::cout << "Run algo1 with params: " << std::endl;
  std::cout << "--Begin--\n";

  for (int i = 0; i < sd::frontend::ComputedImageViewAlgorithm<double>::MaxNbList; ++i) {
    std::vector<sd::frontend::Parameter> all;
    sd::frontend::ComputedImageViewAlgorithm<double>::getAllParams(i, all);
    if (!all.empty()) {
      std::cout << "All parameters in list " << i << ":\n";
      for (auto itr = all.begin(); itr != all.end(); ++itr)
	std::cout << "   " << *itr << std::endl;
      std::cout << std::endl;
    }
  }
  std::cout << "--End--\n";
  return true;
}

int
main(int /*argc*/, char* /*argv*/[])
{
  std::cout << std::boolalpha;

  sd::frontend::Parameter u, v, w;

  Algo1 a1(5, 4, 3.4);
  sd::frontend::ComputedImageViewAlgorithm<double>& a = a1;

  a.run();
  std::cout << std::endl;

  a.getParams("Pi1", u, "Pi2", v, "Pf1", w);
  std::cout << "params: " << u << "  " << v << "  " << w << std::endl;
  std::cout << std::endl;

  a.setParams_1("Pi2", 3, "Pf1", 1.2f, "Pf2", 2.5, sd::frontend::Parameter("Pi3", -7, "this is pi3"));
  a.setParams("Pb1", true);
  a.run();
  std::cout << std::endl;

  a.getParams("Ps1", u);
  std::cout << u << std::endl;
  a.setParams("Ps1", 3.14);
  a.getParams("Ps1", u);
  std::cout << u << std::endl;
  a.setParams("Ps1", "hello");
  //char* s2;
  //a.getParams("Ps1", s2); // not allowed
  a.getParams("Ps1", u);
  std::cout << u << std::endl;
  std::cout << std::endl;

  //sd::frontend::Parameter pptr("pptr", "test"); // not allowed
  //sd::frontend::Parameter pvec("pvec", std::vector<int>(5)); // not allowed

  std::vector<sd::frontend::Parameter> allP0;
  a.getAllParams_0(allP0);
  std::cout << "All parameters in list 0:\n";
  for (auto itr = allP0.begin(); itr != allP0.end(); ++itr)
    std::cout << "   " << *itr << std::endl;
  std::cout << std::endl;

  std::vector<sd::frontend::Parameter> allP1;
  a.getAllParams_1(allP1);
  std::cout << "All parameters in list 1:\n";
  for (auto itr = allP1.begin(); itr != allP1.end(); ++itr)
    std::cout << "   " << *itr << std::endl;
  std::cout << std::endl;

  a.getParams_0("Pi2", v, "Pf1", w);
  std::cout << "params: " << v << "  " << w << std::endl;
  a.getParams_1("Pi2", v, "Pf1", w);
  std::cout << "params: " << v << "  " << w << std::endl;
  std::cout << std::endl;

  sd::frontend::Parameter p;
  a.getParams_1("Pi3", p);
  std::cout << p << std::endl;
  a.getParams("Pi3", p);
  std::cout << p << std::endl;
  std::cout << std::endl;

  //a.setParams("Pv1", std::vector<int>(5, 0));
  std::vector<int> vec = { 1, 2, 3, 4, 5 };
  a.setParams("Pv1", vec);
  a.getParams("Pv1", p);
  std::cout << p.isVector() << std::endl;
  std::cout << p << std::endl;
  std::vector<int> vec2 = p.getAsVectorOf<int>();
  for (auto it = vec2.begin(); it != vec2.end(); ++it)
    std::cout << *it << "  ";
  std::cout << std::endl;
  std::cout << std::endl;

  a.getParams_1("Pf1", p);
  std::cout << p.isInteger() << "  " << p.isFloat() << std::endl;

  float f = p.getAs<float>();
  std::cout << f << std::endl;
  //double d = p.getAs<double>(); // throw bad_cast
  //int i = p.getAs<int>(); // throw bad_cast

  a.getParams_0("Ps1", p);
  std::string s = p.getAs<std::string>();
  std::cout << s << std::endl;

  return 0;
}
