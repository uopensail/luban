#include <iostream>
#include <type_traits>
#include <vector>
typedef std::decay<int>::type A;         // int
typedef std::decay<int&>::type B;        // int
typedef std::decay<int&&>::type C;       // int
typedef std::decay<const int&>::type D;  // int
typedef std::decay<int[2]>::type E;      // int*
typedef std::decay<int(int)>::type F;    // int(*)(int)
typedef std::decay<std::vector<int>> G;  // int(*)(int)

typedef int X[3];

namespace std {
template <class T>
struct remove_vector {
  typedef T type;
};
template <class T>
struct remove_vector<std::vector<T>> {
  typedef T type;
};
template <class T>
struct remove_vector<std::vector<T*>> {
  typedef T type;
};
}  // namespace std

template <class T>
typename T::value_type get() {
  typename T::value_type v;
  return v;
}

int main() {
  std::cout << std::boolalpha;
  std::cout << "typedefs of int:" << std::endl;
  std::cout << "A: " << std::is_same<int, A>::value << std::endl;
  std::cout << "B: " << std::is_same<int, B>::value << std::endl;
  std::cout << "C: " << std::is_same<int, C>::value << std::endl;
  std::cout << "D: " << std::is_same<int, D>::value << std::endl;
  std::cout << "E: " << std::is_same<int, E>::value << std::endl;
  std::cout << "F: " << std::is_same<int, F>::value << std::endl;
  std::cout
      << "G: "
      << std::is_same<int, std::remove_vector<std::vector<int>>::type>::value
      << std::endl;

  auto tmp = get<std::vector<int>>();
  std::cout << tmp << std::endl;
  // for (int i = 0; i < 100; i++) {
  //   tmp.push_back(i);
  // }
  // for (int i = 0; i < 100; i++) {
  //   std::cout << tmp[i] << std::endl;
  // }
  return 0;
}