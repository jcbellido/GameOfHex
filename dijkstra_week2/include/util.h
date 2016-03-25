#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED
#include<assert.h>
#include <vector>
#include <iostream>

typedef unsigned int node;
typedef unsigned int distance;

/** \brief check if the element x_elt is inside the vector x_vector
 *  \fn bool exist(std::vector<T> const& x_vector, T const& x_elt)
 *  \param [in] x_vector The vector where to search in
 *  \param [in] x_elt The element to be searched
 *  \return bool
 *
 */
template <class T>
bool exist(std::vector<T> const& x_vector, T const& x_elt);

/** \brief overloaded operator<< for vector<T>
 *  \fn std::ostream& operator<<(std::ostream& x_out, std::vector<T> const& x_vector)
 *  \param [in, out] x_out The output stream where the flow is printed to
 *  \param [in] x_vector The vector of elements to output
 *  \return std::ostream&
 *
 */
template <class T>
std::ostream& operator<<(std::ostream& x_out, std::vector<T> const& x_vector);

/** \brief return the smallest element in a non empty vector of comparable element
 *  \fn const T smallest(std::vector<T> const& x_vector)
 *  \param [in] x_vector The vector in which the smallest element is looking
 *  \exception x_vector is empty
 *  \return const T
 *
 */
template <class T>
const T smallest(std::vector<T> const& x_vector);

template <class T>
const T smallest(std::vector<T> const& x_vector)
{
  assert(!x_vector.empty());
  T loc_small(x_vector[0]);
  for(auto loc_elt: x_vector){
    if(loc_elt < loc_small){
      loc_small = loc_elt;
    }
  }
  return loc_small;
}

template <class T>
bool exist(std::vector<T> const& x_vector, T const& x_elt)
{
  bool loc_result(false);
  for(auto loc_elt: x_vector){
    if(loc_elt == x_elt){
      loc_result = true;
      break;
    }
  }
  return loc_result;
}

template<class T>
std::ostream& operator<<(std::ostream& x_out, std::vector<T> const& x_vector)
{
  bool loc_isFirst(true);
  for(auto loc_elt: x_vector){
    if(loc_isFirst){
      x_out << loc_elt;
    }else{
      x_out << ",\t" << loc_elt;
    }
    loc_isFirst = false;
  }
  return x_out;
}

#endif // UTIL_H_INCLUDED
