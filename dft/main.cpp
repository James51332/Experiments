#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

struct complex {
  float real;
  float imag;

  complex(float a = 0.0f, float b = 0.0f)
    : real(a), imag(b) {}
};

inline complex operator+(const complex &one, const complex &two)
{
  // (a + bi) + (c + di) = (a + c) + (b + d)i
  complex num;
  num.real = one.real + two.real;
  num.imag = one.imag + two.imag;

  return num;
}

inline complex& operator+=(complex& one, const complex& two)
{
  one = one + two;
  return one;
}

inline complex operator*(const complex& one, const complex& two)
{
  // (a + bi)(c + di) = ac + bci + adi + bd(-1) = ac - bd + (bc + ad)i
  complex num;
  num.real = one.real * two.real - one.imag * two.imag;
  num.imag = one.imag * two.real + one.real * two.imag;

  return num;
}

complex eulers(float theta)
{
  return { cos(theta), sin(theta) };
}

struct fourier
{
  // number of samples in the period
  float samples;

  // we'll convert between these two matrics
  vector<complex> amplitudes;
  vector<complex> timeValues;
};

void inverseTransform(fourier& fourier)
{
  // zero initialize the values
  fourier.samples = fourier.amplitudes.size();
  fourier.timeValues = vector<complex>(fourier.samples, {0.0f, 0.0f});

  // calculate the basis using the nth roots of unity
  float basis = 2.0f * M_PI / fourier.samples;

  // iterate over roots of unity to calculate their time domain values
  for (int root = 0; root < fourier.samples; root++)
  {
    // this is the time domain value to calculate.
    complex& value = fourier.timeValues[root];

    // iterate over each frequency (same number of frequencies as samples)
    // this nested for loop is what makes the algorithm O(n^2)
    for (int freq = 0; freq < fourier.samples; freq++)
    {
      // the evalution point for the angle is going to be equal to the
      // root number * basis * freq (roots need to move freq times as fast as freq of 1)
      complex pos = eulers(basis * root * freq);

      // add the wave component to the time domain (both real and imag parts)
      value += fourier.amplitudes[freq] * pos;
    }

    // zero out values if precision is unneeded
    if (value.real < 10e-6) value.real = 0.0f;
    if (value.imag < 10e-6) value.imag = 0.0f;
  }
}

// this is the real beast to understand
void fourierTransform(fourier& fourier)
{
  // zero out the amplitudes
  fourier.samples = fourier.timeValues.size();
  fourier.amplitudes = vector<complex>(fourier.samples, {0.0f, 0.0f});

  // calculate the basis angle using the nth roots of unity
  float basis = 2.0f * M_PI / fourier.samples;

  // iterate over all frequencies and add the respective component
  for (int freq = 0; freq < fourier.samples; freq++)
  {
    complex &amplitude = fourier.amplitudes[freq];

    // we're going to iterate over each root and use the direc delta function
    // to find the component for each amplitude that contributes to each term.
    for (int root = 0; root < fourier.samples; root++)
    {
      // find the value we want to get
      complex value = fourier.timeValues[root];

      // formula is simple time [a 0 0 0] => freq (a/N a/N a/N a/N); so we scale
      complex scale = {1.0f / fourier.samples, 0.0f};
      value = value * scale;

      // we'll rotate all of the point back by the number of cycles to wait times their speed
      complex pos = eulers(-basis * root * freq);
      complex ampFactor = value * pos;

      // adjust the amplitude
      amplitude += ampFactor;
    }
  }
}

int main()
{
  fourier fourier;
  fourier.timeValues = { {3.0f, 0.0f}, {3.0f, 3.0f}, {0.0f, 3.0f}, {0.0f, 0.0f}};

  cout << "Performing Fourier Transform:" << endl;
  fourierTransform(fourier);

  for (auto value : fourier.amplitudes)
  {
    float length = sqrt(value.real * value.real + value.imag * value.imag);
    float angle = atan2(value.imag, value.real) * 180.0f / M_PI;
    cout << length << ":" << angle << endl;
  }

  cout << endl << "Performing Inverse Fourier Transform:" << endl;
  inverseTransform(fourier);

  for (auto value : fourier.timeValues)
  {
    cout << value.real << " + " << value.imag << "i" << endl;
  }
}