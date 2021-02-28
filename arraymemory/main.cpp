#include <iostream>
using namespace std;

int main() {

// Create a pointer to the base of our array
float* array = new float[5];

// Create a pointer to our access point in the array
float* access = array;

// Log info about our arrays
cout << "Created array with size 5 at address " << array << endl;
cout << "Create access pointer at address " << access << endl;

// Set the first values of the array to 1 and 4
(*access) = 1;
access++; // increments the value sizeof(float)
(*access) = 4;

// Log changing values
cout << "Moved access to address " << access << endl;

// Log expected pointer changes
cout << "The expected change to access is sizeof(float) which is " << sizeof(float) << endl;

// Log actual pointer changes
uint32_t change = static_cast<uint32_t>((uint8_t*)access - (uint8_t*)array); // We cast the pointer to uint8_t because it has a size of one bite (if we don't, it returns 1 because the difference is measured in sizeof(float)'s.

cout << "The actual change to access is access - array which is " << (uint32_t)((uint8_t*)access - (uint8_t*)array) << endl;

// Free the array
access = nullptr;
delete[] array;

}
