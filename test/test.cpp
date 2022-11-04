
#include <functional>

template <class T0, class T1>
std::function<T0(T1 &)> call(T0 (*func)(T1 &), const int &params) {}

template <class T0, class T1>
*(T0(T1 &))call(T0 (*func)(T1 &), const SharedArgumentsPtr &params) {}
