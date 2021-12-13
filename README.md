
# Simple nonlinear equation system solver with std::function
Author: Leanne Dong

Date: 30 Aug 2021

This is a small network equation solver demonstrate the small  network example in Figure 2.1 [Combined analysis of electricity and heat networks](https://orca.cardiff.ac.uk/57830/)

## Build instruction

### Dependency required

- CMake >= 3.12
- A Modern C++ compiler
- Eigen3.3
- doctest

  [The fastest feature-rich C++11/14/17/20 single-header testing framework](https://raw.githubusercontent.com/onqtam/doctest/master/doctest/doctest.h)

### Linux

#### Build from terminal

```
git clone git@github.com:leannejdong/EngineSim.git
cd SimpleSolve/
mkdir build
cd build
cmake ..
make
```
To run test
```
./EngineSim
```

## Documentation

### Motivation: Callback function 

* C (function pointer) : `void (*callbackFunc)(int)`
* C++ : `std::function<void(int)> callbackFunc`
* template parameter
* 
Use `std::function` unless we have a reason not to (i.e. C compatible API).

C-like callbacks have the drawback not being able to capture some context. For instance, you won't be able 
to pass lambda function as a callback which capture some context variables.

See more discussion [here](https://stackoverflow.com/questions/25848690/should-i-use-stdfunction-or-a-function-pointer-in-c/25848890)

### Motivation: Why `std::function`

* Those who want to support callbacks might have different return types(`double`, `int`, `VectorXd`, `MatrixXd`)
* The need for a function wrapper stores various return type

* Type Erasure: this allows one to use various concrete types through a single generic interface

* A good example of **Type Erasure** is `std::function`.
  (a technique for allowing different types of objects to all be treated as a single type)
* 
```
Genrally, any place in which a function pointer would be used to defer a call
or make a callback
```

### Polymorphism

* The provision of a single interface to entities of different type
* Entities are the callable objects (function objects, function pointers, lambdas)
* A very handy utility: `std::function` provides such an interface

### std::function

* A polymorphic function wrapper

* A class template from STL

* Can store any copyable entity that may be invoked as a function

* Is itself a callable object that support `operator()`

* Need to specify support signature upfront
  * objects are stored as double while return type is void : `std::function<void<double>>`
  * the function takes no parameter and return void: `std::function<int()>`

* Caution: `std::function` erases the types it holds

* Is type safe
  * The argument types must be convertible to the declared parameter types
  
    * Implicitly convert it
  
    * entirely disregard it
* Flexible

#### What does std::function allocate? 

* Small buffer op: If the callable is smaller than than the stack space, 
then the stack space will be used for allocation. However,
* If the thing we store is bigger than the stack space, then `std::function` will fraction some memory from the heap.
  * The size of the stack space is implementation defined. (Can't tell apriori)


[CppCon2018: Roll your own `std::function` implementation](https://www.youtube.com/watch?v=VY83afAJUIg)
