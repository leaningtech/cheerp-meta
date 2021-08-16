/// @file     segmented_sieve.cpp
/// @author   Kim Walisch, <kim.walisch@gmail.com>
/// @brief    This is a simple implementation of the segmented sieve of
///           Eratosthenes with a few optimizations. It generates the
///           primes below 10^9 in 0.8 seconds (single-threaded) on an
///           Intel Core i7-6700 3.4 GHz CPU.
/// @license  Public domain.

/// Slightly modified by Carlo Piovesan <carlo@leaningtech.com>
/// with the purpose of providing a tutorial for Cheerp

#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <stdint.h>
#include <chrono>

/// Set your CPU's L1 data cache size (in bytes) here
const int64_t L1D_CACHE_SIZE = 32768;

/// Generate primes using the segmented sieve of Eratosthenes.
/// This algorithm uses O(n log log n) operations and O(sqrt(n)) space.
/// @param limit  Sieve primes <= limit.
///
int64_t segmented_sieve(int64_t limit)
{
  int64_t sqrt = (int64_t) std::sqrt(limit);
  int64_t segment_size = std::max(sqrt, L1D_CACHE_SIZE);
  int64_t count = (limit < 2) ? 0 : 1;

  // we sieve primes >= 3
  int64_t i = 3;
  int64_t n = 3;
  int64_t s = 3;

  std::vector<char> sieve(segment_size);
  std::vector<char> is_prime(sqrt + 1, true);
  std::vector<int64_t> primes;
  std::vector<int64_t> multiples;

  for (int64_t low = 0; low <= limit; low += segment_size)
  {
    std::fill(sieve.begin(), sieve.end(), true);

    // current segment = [low, high]
    int64_t high = low + segment_size - 1;
    high = std::min(high, limit);

    // generate sieving primes using simple sieve of Eratosthenes
    for (; i * i <= high; i += 2)
      if (is_prime[i])
        for (int64_t j = i * i; j <= sqrt; j += i)
          is_prime[j] = false;

    // initialize sieving primes for segmented sieve
    for (; s * s <= high; s += 2)
    {
      if (is_prime[s])
      {
           primes.push_back(s);
        multiples.push_back(s * s - low);
      }
    }

    // sieve the current segment
    for (std::size_t i = 0; i < primes.size(); i++)
    {
      int64_t j = multiples[i];
      for (int64_t k = primes[i] * 2; j < segment_size; j += k)
        sieve[j] = false;
      multiples[i] = j - segment_size;
    }

    for (; n <= high; n += 2)
      if (sieve[n - low]) // n is a prime
        count++;
  }

  return count;
}

/// Wraps segmented_sieve(limit) function taking
/// care of measuring time elapsed and giving output
/// @param limit  Sieve primes <= limit.
///
double wrapper_segmented_sieve(int64_t limit)
{
  // Record start time
  auto start = std::chrono::high_resolution_clock::now();

  // Portion of code to be timed
  int64_t count = segmented_sieve(limit);

  // Record end time
  auto finish = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> elapsed = finish - start;
  double duration = elapsed.count();

  std::cout << duration \
	  << "s\tto sieve in the interval (1, " \
	  << (double)limit << ")\t" \
	  << count << " primes found" << std::endl;

  return duration;
}

/// Usage: ./segmented_sieve
///
int main()
{
  int64_t limit = 10;

  //Stop whenever the segmented_sieve call last more than 1.0 seconds
  while (wrapper_segmented_sieve(limit) < 1.0)
  {
    limit *= 10;
  }
  return 0;
}
