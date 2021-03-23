#include <iostream>
#include <cstdlib>
#include <cstring>
#include <memory>
using namespace std;

// Step 0: Retrieve info about array (pass location by reference to change for user. this could be replicated using int** in c)
void resize(int*& arrayLocation, int curSize, int newSize) {
  // Step 1: allocate new memory for our resized array
  int* newArray = (int*)malloc(sizeof(int) * newSize);

  // Step 2: copy the content of our old array into the new array
  int safeSize = curSize < newSize ? curSize : newSize; // figure out how much memory is safe for both arrays
  memcpy(newArray, arrayLocation, sizeof(int) * safeSize); // copy the actualy data

  // Step 3: free the old array to prevent leaks
  free(arrayLocation);

  // Step 4: point the user to our new location
  arrayLocation = newArray;
}

int main() {
  int size = 10;
  int* array = (int*)malloc(sizeof(int) * size);

  cout << "Creating array with " << size << " elements." << endl;
  for (int i = 0; i < size; i++) {
    array[i] = i + 1;
    cout << array[i];
    if (i + 1 < size) cout << ", ";
  }
  cout << endl;

  int newSize = 12;
  cout << "Resizing array to " << newSize << " elements." << endl;

  resize(array, size, newSize);
  size = newSize;
  for (int i = 0; i < size; i++) {
    array[i] = i + 1;
    cout << array[i];
    if (i + 1 < size) cout << ", ";
  }
  cout << endl;

  free(array);
}
