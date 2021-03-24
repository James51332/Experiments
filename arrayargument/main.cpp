#include <iostream>
using namespace std;

// We can log any number of elements using this method
// This design pattern is used in graphics apis such
// as OpenGL or Vulkan.
void log(int size, int* array)
{
  for (int i = 0; i < size; i++)
  {
    cout << array[i];

    if (i + 1 < size)
    {
      cout << ", ";
    }
  }
  cout << endl;
}

double average(int size, int* array) {
  double total = 0;
  for (int i = 0; i < size; i++)
    total += array[i];

  return total / static_cast<double>(size);
}

int main() {
  int num = 32;

  cout << "Logging individual number." << endl;
  log(1, &num);
  cout << "Average of single number is " << average(1, &num) << "." << endl;

  constexpr int arraySize = 5;
  int nums[arraySize] = { 1, 2, 33, 5, 3 };

  cout << "Logging array with size 5." << endl;
  log(arraySize, nums);
  cout << "Average of array is " << average(arraySize, nums) << "." << endl;
}