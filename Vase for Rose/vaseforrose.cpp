struct Vase
{
   virtual void arrange() = 0;
};

template <typename T>
struct VaseFor : public Vase
{
   typedef T value_type;
};

struct Rose {};
struct Tulip {};

typedef VaseFor<Rose> RoseVase;
typedef VaseFor<Tulip> TulipVase;

#include <iostream>

struct BlackRoseVase : public RoseVase
{
   void arrange() { std::cout << "the real thing"; }
};

struct MockTulipVase : public TulipVase
{
   void arrange() { std::cout << "a shade of pale"; }
};

int main()
{
   Vase & vase = * new BlackRoseVase;

   vase.arrange();
}

// g++ -o rosevase rosevase.cpp
