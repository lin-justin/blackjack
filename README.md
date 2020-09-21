# Blackjack

My implementation of the Blackjack Game in C++.

This was mainly for me to practice Object-Oriented Programming, classes, inheritance, and polymorphism in C++.

## Usage

```
g++ -o blackjack blackjack.cpp
./blackjack
```

## Takeaways

- One of the key elements of OOP is inheritance, which allows deriving a new class from an existing one. The new class automatically inherits data members and member functions from the existing class.
- A derived class does not inherit contructors, copy constructors, destructors, or an overloaded asssignment operator.
- Base class constructors are automatically called before the derived class constructor when a derived class object is instantiated.
- Base class destructors are automatically called after the derived class destructor when a derived class object is destroyed.
- Protected members are accessible only in their own class and certain derived classes, depending upon the derivation access level.
- Using public derivation means that public members in the base class becomes public members in the derived class, protected members in the basee class become protected members in the derived class, and prive members are (as always) inaccessible.
- Base class member functions can be overrided by giving them new definitions in a derived class.
- A base class member function can be explicitly called from a derived class.
- A base class constructor can be explicitly called from a derived class constructor.
- Polymorphism is the quality whereby a member function will produce different results depending on the type of object for which it is called.
- Virtual functions allow for polymorphic behavior.
- Once a member function is defined as virtual, it is virtual in any derived class.
- A pure virtual function is a function to which a definition is not given. A pure virtual function is specified by placing an equal sign and a zero at the end of the function header.
- An abstract class has at least one pure virtual member function.
- An abstract class cannot be used to instantiate an object.
