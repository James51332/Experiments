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

inline complex operator-(const complex &one, const complex &two)
{
  // (a + bi) - (c + di) = (a - c) + (b - d)i
  complex num;
  num.real = one.real - two.real;
  num.imag = one.imag - two.imag;

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
  int samples;

  // we'll convert between these two vectors
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

void fft(fourier& series)
{
  series.samples = series.timeValues.size();
  std::size_t length = series.samples;

  // base case - trivial DFT
  if (length == 1)
  {
    series.amplitudes = { series.timeValues[0] };
    return;
  }

  // otherwise split into evens and odds
  fourier even;
  fourier odd;
  even.timeValues = {};
  odd.timeValues = {};

  for (int i = 0; i < series.samples / 2; i++)
  {
    even.timeValues.push_back(series.timeValues[i * 2]);
    odd.timeValues.push_back(series.timeValues[i * 2 + 1]);
  }

  // run the fft on these values
  fft(even);
  fft(odd);

  // shift and combine
  series.amplitudes = vector<complex>(series.samples, {0.0f, 0.0f});
  for (int freq = 0; freq < even.samples; freq++)
  {
    complex t = eulers(-2.0f * M_PI * freq / series.samples) * odd.amplitudes[freq];
    series.amplitudes[freq] = even.amplitudes[freq] + t;
    series.amplitudes[freq + even.samples] = even.amplitudes[freq] - t;
  }
}

void ifft(fourier &series)
{
  series.samples = series.amplitudes.size();
  std::size_t length = series.samples;

  // base case - trivial DFT
  if (length == 1)
  {
    series.timeValues = { series.amplitudes[0] };
    return;
  }

  // otherwise split into evens and odds
  fourier even;
  fourier odd;
  even.amplitudes = {};
  odd.amplitudes = {};

  for (int i = 0; i < series.samples / 2; i++)
  {
    even.amplitudes.push_back(series.amplitudes[i * 2]);
    odd.amplitudes.push_back(series.amplitudes[i * 2 + 1]);
  }

  // run the fft on these values
  ifft(even);
  ifft(odd);

  // shift and combine
  series.timeValues = vector<complex>(series.samples, {0.0f, 0.0f});
  for (int root = 0; root < even.samples; root++)
  {
    complex t = eulers(2.0f * M_PI * root / series.samples) * odd.timeValues[root];
    series.timeValues[root] = complex(0.5f, 0.0f) * (even.timeValues[root] + t);
    series.timeValues[root + even.samples] = complex(0.5f, 0.0f) * (even.timeValues[root] - t);
  }
}

int main()
{
  fourier fourier;
  fourier.timeValues = { {1.0f, 0.0f}, {2.0f, 0.0}, {3.0f, 0.0f}, {5.0f, 0.0f} };

  cout << "Performing Fourier Transform:" << endl;
  fft(fourier);

  for (auto value : fourier.amplitudes)
  {
    float length = sqrt(value.real * value.real + value.imag * value.imag);
    float angle = atan2(value.imag, value.real) * 180.0f / M_PI;
    cout << length << ":" << angle << " (" << value.real << " + " << value.imag << "i)" << endl;
  }

  cout << endl << "Performing Inverse Fourier Transform:" << endl;
  ifft(fourier);

  for (auto value : fourier.timeValues)
  {
    cout << value.real << " + " << value.imag << "i" << endl;
  }
}