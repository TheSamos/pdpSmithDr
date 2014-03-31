# Check availability of C++ shared_ptr implementation.
#
# Sets the following variables:
#
# SHARED_PTR_FOUND
# HAVE_CPP0X_SHARED_PTR
# HAVE_TR1_SHARED_PTR
# HAVE_BOOST_TR1_SHARED_PTR
# HAVE_BOOST_SHARED_PTR

# We need to have at least this version to support the VERSION_LESS argument to 'if' (2.6.2) and unset (2.6.3)
cmake_policy(PUSH)
cmake_minimum_required(VERSION 2.6.3)
cmake_policy(POP)

include(CheckCXXSourceCompiles)

check_cxx_source_compiles(
  "
        #include <memory>
        int main() {
            std::shared_ptr<int> ptr;
            return 0;
        }
    "
  HAVE_CPP0X_SHARED_PTR)

check_cxx_source_compiles(
  "
        #include <tr1/memory>
        int main() {
            std::tr1::shared_ptr<int> ptr;
            return 0;
        }
    "
  HAVE_TR1_SHARED_PTR)

check_cxx_source_compiles(
  "
        #include <boost/tr1/memory.hpp>
        int main() {
            std::tr1::shared_ptr<int> ptr;
            return 0;
        }
    "
  HAVE_BOOST_TR1_SHARED_PTR)

check_cxx_source_compiles(
  "
        #include <boost/shared_ptr.hpp>
        int main() {
            boost::shared_ptr<int> ptr;
            return 0;
        }
    "
  HAVE_BOOST_SHARED_PTR)


# ---------------------------------------------------------------------------
set(SHARED_PTR_FOUND FALSE)
if (HAVE_CPP0X_SHARED_PTR OR HAVE_TR1_SHARED_PTR OR HAVE_BOOST_TR1_SHARED_PTR OR HAVE_BOOST_SHARED_PTR)
  set(SHARED_PTR_FOUND TRUE)
else()
  message("** No shared_ptr implementation found **")
endif()


include(FindPackageHandleStandardArgs)

# handle the QUIETLY and REQUIRED arguments
find_package_handle_standard_args(SharedPtr DEFAULT_MSG SHARED_PTR_FOUND)

mark_as_advanced(HAVE_CPP0X_SHARED_PTR)
mark_as_advanced(HAVE_TR1_SHARED_PTR)
mark_as_advanced(HAVE_BOOST_TR1_SHARED_PTR)
mark_as_advanced(HAVE_BOOST_SHARED_PTR)
