#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <concepts>

template<typename T>
concept Monoid = requires(T::Value a, T::Value b, T::Value c) {
  typename T::Value; // hay un tipo de valores
  { T::op(a, b) } -> std::same_as<typename T::Value>; // clausura de la operacion
  // T::op(a, T::op(b, c)) == T::op(T::op(a, b), c) // asociatividad de la operacion
  { T::neut() } -> std::same_as<typename T::Value>; // existencia del neutro
  // T::op(a, T::neut()) == a // neutro por derecha
  // T::op(T::neut(), a) == a // neutro por izquierda
};

#endif // STRUCTURES_H