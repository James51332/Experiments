#include <iostream>
using namespace std;

struct Vec3
{
  float X;
  float Y;
  float Z;
};

int main()
{
  cout << "Size of Vec3 is " << sizeof(Vec3) << endl;

  Vec3 array[5];
  cout << "Expected size of array is sizeof(Vec3) * 5 which is " << sizeof(Vec3) * 5 << endl;
  cout << "Actual size is " << sizeof(array) << endl;

  return 0;
}