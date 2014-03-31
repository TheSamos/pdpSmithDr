
#ifndef MATHEMATICALS_TYPES_HPP
#define MATHEMATICALS_TYPES_HPP

#include <eigen/Eigen/Eigen>

#include <core/Point3d.hpp>
#include <core/Vector3d.hpp>

#include <utils/sdTypeUtils.hpp>

#include <iostream> //DEBUG

#ifndef PI
#define PI 3.141592653589793238462
#endif

static const unsigned int DIMENSION_SPACE = 3;

namespace sd {
  
  template<typename T>
  class Math {
  
  public:
    
    typedef typename Eigen::Matrix<T, Eigen::Dynamic, 1> vector_dyn;
    typedef typename Eigen::Matrix<T, 1, Eigen::Dynamic> vector_dyn_t;
    
    typedef typename Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> matrix_dyn;
    
    typedef typename Eigen::Matrix<T, DIMENSION_SPACE, 1> vector_3d;
    typedef typename Eigen::Matrix<T, 1, DIMENSION_SPACE> vector_3d_t;
    typedef typename Eigen::Matrix<T, DIMENSION_SPACE, DIMENSION_SPACE> matrix_3d;
    
    typedef typename Eigen::Matrix<T, Eigen::Dynamic, DIMENSION_SPACE> matrix_3d_dyn;
    typedef typename Eigen::Matrix<T, DIMENSION_SPACE, Eigen::Dynamic> matrix_3d_dyn_t;
    
    
#define DECLARE_VECTOR(N)						\
    typedef typename Eigen::Matrix<T, N, 1> vector_##N##d;		\
    typedef typename Eigen::Matrix<T, 1, N> vector_##N##d_t;		\
    typedef typename Eigen::Matrix<T, N, N> matrix_##N##d;		\
									\
    typedef typename Eigen::Matrix<T, Eigen::Dynamic, N> matrix_##N##d_dyn; \
    typedef typename Eigen::Matrix<T, N, Eigen::Dynamic> matrix_##N##d_dyn_t;

    DECLARE_VECTOR(2)
    DECLARE_VECTOR(4)
    DECLARE_VECTOR(5)
    DECLARE_VECTOR(6)
    DECLARE_VECTOR(7)
    DECLARE_VECTOR(8)

#undef DECLARE_VECTOR
    
    static inline vector_3d
    sdPoint3dToMathVector3d(const sd::core::Point3d<T>& p) {
      return vector_3d(p.x(), p.y(), p.z());
    }
  
    static inline vector_3d
    sdVector3dToMathVector3d(const sd::core::Vector3d<T>& v) {
      return vector_3d(v.x(), v.y(), v.z());
    }
  
    static inline sd::core::Vector3d<T>
    mathVector3dToSdVector3d(const vector_3d& v) {
      return sd::core::Vector3d<T>(v[0], v[1], v[2]);
    }
  
    static inline sd::core::Point3d<T>
    mathVector3dToSdPoint3d(const vector_3d& v) {
      return sd::core::Point3d<T>(v[0], v[1], v[2]);
    }
    
    static inline T
    compute_voronoi_area(const sd::core::Point3d<T>& target, const sd::core::Point3d<T>& prev,
			 const sd::core::Point3d<T>& current, const sd::core::Point3d<T>& next) {
      return sd::core::voronoiArea(target, prev, current, next);
    }

    inline static void
    print_vector(const typename Math<T>::vector_3d& tmp) {
      std::cout << "(";
      for(unsigned int i= 0; i < DIMENSION_SPACE; ++i) {
	std::cout << tmp(i);
	if (i+1 < DIMENSION_SPACE)
	  std::cout << ", ";
      }
      std::cout << ")" << std::endl;
    }
  
    inline static void
    print_matrix(const typename Math<T>::matrix_3d& tmp) {
      std::cout << std::endl << "(" ;
      for(unsigned int i= 0; i < DIMENSION_SPACE; ++i) {
	for(unsigned int j= 0; j < DIMENSION_SPACE; ++j) {
	  std:: cout << tmp(j, i);
	  if (j+1 < DIMENSION_SPACE) std::cout << "\t";
	}
	if (i+1 < DIMENSION_SPACE) std::cout << std::endl;
      }
      std::cout << ")" << std::endl;
    }
    
    inline static void
    computeRotation(const T angle,
		    typename Math<T>::matrix_3d& r) {
      r = sd::Math<T>::matrix_3d(cos(angle), -sin(angle), 0.,
				 sin(angle), cos(angle), 0.,
				 0., 0., 1.);
    }
    
    inline static void
    computeRotation(const typename Math<T>::vector_3d& normal,
		    typename Math<T>::matrix_3d& r) {
      typename Math<T>::vector_3d r1;
  
      // r3= n
      {
	r(0, 2) = normal(0);
	r(1, 2) = normal(1);
	r(2, 2) = normal(2);
      }
  
      // r1
      {
	typename Math<T>::vector_3d n;
	typename Math<T>::vector_3d_t nt;
	typename Math<T>::matrix_3d id;
	id.setIdentity();
    
	n(0, 0) = normal(0);
	n(1, 0) = normal(1);
	n(2, 0) = normal(2);
    
	nt(0, 0) = normal(0);
	nt(0, 1) = normal(1);
	nt(0, 2) = normal(2);

	typename Math<T>::vector_3d i(1., 0., 0.); // global X axis
	if (normal == i) { // gloal Y axis
	  i(0) = 0.;
	  i(1) = 1.;
	}
	
	typename Math<T>::vector_3d testCol;
	testCol(0) = i(1) * normal(2) - i(2) * normal(1);
	testCol(1) = -i(0) * normal(2) + i(2) * normal(0);
	testCol(2) = i(0) * normal(1) - i(1) * normal(0);
    
	if (testCol.norm() < std::numeric_limits<T>::min()) {
	  i = typename Math<T>::vector_3d(0., 0., 1.);
	}
	
	r1 = (id - n * nt) * i;
	r1 /= r1.norm();
    
	r(0, 0) = r1(0);
	r(1, 0) = r1(1);
	r(2, 0) = r1(2);
      }
  
      // r2 = r3 ^ r1 = n ^ r1
      {
	typename Math<T>::vector_3d r2;
	r2(0) = normal(1) * r1(2) - normal(2) * r1(1);
	r2(1) = -normal(0) * r1(2) + normal(2) * r1(0);
	r2(2) = normal(0) * r1(1) - normal(1) * r1(0);
    
	r(0, 1) = r2(0);
	r(1, 1) = r2(1);
	r(2, 1) = r2(2);
      }
  
      r = r.transpose().eval();
    }

    
    static inline void
    transform(const typename Math<T>::vector_3d& x,
	      const typename Math<T>::vector_3d& translation,
	      const typename Math<T>::matrix_3d& rotation, 
	      typename Math<T>::vector_3d& result) {
      result = rotation * (x - translation);
    }
    /*
    static inline sd::core::Point3d<T>
    intersection(const sd::core::Line3d<T>& l1, const sd::core::Line3d<T>& l2) {
      const typename Math<T>::vector_3d o1 = Math<T>::sdPoint3dToMathVector3d(l1.point());
      const typename Math<T>::vector_3d d1 = Math<T>::sdVector3dToMathVector3d(l1.direction());
      const typename Math<T>::vector_3d o2 = Math<T>::sdPoint3dToMathVector3d(l2.point());
      const typename Math<T>::vector_3d d2 = Math<T>::sdVector3dToMathVector3d(l2.direction());
      
      const typename Math<T>::vector_3d cross = d1.cross(d2);
      assert(cross.norm() > 0.);
      typename Math<T>::matrix_3d M1;
      M1 << o2-o1, d2, cross;
      // const Math<T>::matrix_3d M2 << o2-o1 << d1 << cross;
      T t1 = M1.determinant() / pow(cross.norm(), 2); // Determinant{(o2-o1),d2,d1 X d2} / ||d1 X d2||^2
      // T t2 = M2.determinant() / pow(cross.norm(), 2); // Determinant{(o2-o1),d1,d1 X d2} / ||d1 X d2||^2
      
      return Math<T>::mathVector3dToSdPoint3d(o1 + (d1 * t1));
    }
    
    static inline sd::core::Point3d<T>
    circumcenter(const sd::core::Point3d<T>& a,
		 const sd::core::Point3d<T>& b,
		 const sd::core::Point3d<T>& c) {
      const sd::core::Vector3d<T> v1(b, a);
      const sd::core::Vector3d<T> v2(b, c);

      const sd::core::Point3d<T> m1((a.x()+b.x())/2., (a.y()+b.y())/2., (a.z()+b.z())/2.);
      const sd::core::Point3d<T> m2((b.x()+c.x())/2., (b.y()+c.y())/2., (b.z()+c.z())/2.);
      sd::core::Vector3d<T> normal = v1 ^ v2;
      normal.normalize();
	    
      return intersection(sd::core::Line3d<T>(m1, normal^v1),
			  sd::core::Line3d<T>(m2, normal^v2));
      
    }
  */
  };
  
}

#endif /* ! MATHEMATICALS_TYPES_HPP */
