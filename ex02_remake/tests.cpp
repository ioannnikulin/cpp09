#include "PmergeMe.hpp"

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <cmath>

static bool parseUint(const std::string &token, unsigned int &out) {
  if (token.empty())
    return false;
  for (std::string::size_type i = 0; i < token.size(); ++i) {
    if (token[i] < '0' || token[i] > '9')
      return false;
  }

  errno = 0;
  char *end = NULL;
  unsigned long value = std::strtoul(token.c_str(), &end, 10);
  if (errno != 0 || end == NULL || *end != '\0')
    return false;
  if (value == 0 || value > UINT_MAX)
    return false;

  out = static_cast<unsigned int>(value);
  return true;
}

bool parse(
	int argc, char **argv,
                  std::vector<myuint> &vec
) {
  for (int i = 1; i < argc; ++i) {
    std::istringstream iss(argv[i]);
    std::string token;
    while (iss >> token) {
      unsigned int value = 0;
      if (!parseUint(token, value))
        return false;
      vec.push_back(myuint(value));
    }
  }
  return !vec.empty();
}

static bool isSorted(const std::vector<myuint> &v) {
  for (std::vector<myuint>::size_type i = 1; i < v.size(); ++i) {
    if (v[i] < v[i - 1])
      return false;
  }
  return true;
}

static bool sameElements(std::vector<myuint> a,
                         std::vector<myuint> b) {
  std::sort(a.begin(), a.end());
  std::sort(b.begin(), b.end());
  return a == b;
}

static bool hasDuplicates(const std::vector<unsigned int> &v) {
  std::vector<unsigned int> sorted(v);
  std::sort(sorted.begin(), sorted.end());
  for (std::vector<unsigned int>::size_type i = 1; i < sorted.size(); ++i) {
    if (sorted[i] == sorted[i - 1])
      return true;
  }
  return false;
}

// Upper bound on comparisons for an optimal-comparison sort of n elements.
// Formula: sum_{i=1}^{n} ceil(log2(3*i / 4))
// Reference: OEIS A001768, Knuth TAOCP Vol.3 "Merge Insertion"
static unsigned int expectedFordJohnsonComparisons(unsigned int n) {
  unsigned int comparisons = 0;
  for (unsigned int i = 1; i <= n; ++i) {
    double value = (3.0 * static_cast<double>(i)) / 4.0;
    double lg = std::log(value) / std::log(2.0);
    int c = static_cast<int>(std::ceil(lg));
    if (c > 0)
      comparisons += static_cast<unsigned int>(c);
  }
  return comparisons;
}

static bool runTest(const std::vector<unsigned int> &input,
                    const std::string &name) {
	PmergeMe sorter;
	vector<myuint> vecIn = wrap(input);
  vector<myuint> vecOut = sorter.sort(vecIn);

  unsigned int vecComps = comparisonCounter;
  comparisonCounter = 0;

  unsigned int expected = 0;
  if (!input.empty())
    expected = expectedFordJohnsonComparisons(
        static_cast<unsigned int>(input.size()));

  bool pass = true;
  std::string failures;

  if (!isSorted(vecOut)) {
    pass = false;
    failures += " vec-not-sorted";
  }
  if (!sameElements(vecIn, vecOut)) {
    pass = false;
    failures += " vec-wrong-elements";
  }

  // Ford-Johnson comparison bound only holds for distinct elements
  bool checkComps = !hasDuplicates(input);
  if (checkComps && vecComps > expected) {
    pass = false;
    failures += " vec-too-many-comps";
  }

  if (pass) {
    std::cout << "[PASS] " << name
              << " (n=" << input.size()
              << ", comps=" << vecComps
              << "/" << expected
              << std::endl;
  } else {
    std::cout << "[FAIL] " << name
              << " (n=" << input.size()
              << ", vec_comps=" << vecComps
              << ", expected_max=" << expected
              << "):" << failures << std::endl;
  }
  return pass;
}

// Generates a vector of 'size' unique values via Fisher-Yates shuffle
// of [1..size], ensuring no duplicates so the comparison bound applies.
static std::vector<unsigned int> randomUniqueVec(unsigned int size) {
  std::vector<unsigned int> v;
  v.reserve(size);
  for (unsigned int i = 1; i <= size; ++i)
    v.push_back(i);
  for (unsigned int i = size; i > 1; --i) {
    unsigned int j = static_cast<unsigned int>(std::rand()) % i;
    unsigned int tmp = v[i - 1];
    v[i - 1] = v[j];
    v[j] = tmp;
  }
  return v;
}

int runTestSuite() {
  unsigned int passed = 0;
  unsigned int failed = 0;

  std::cout << "=== PmergeMe Debug Tests ===" << std::endl;
  std::cout << std::endl;

  struct TestCase {
    size_t size;
    const char* name;
    unsigned int data[10]; // 10 is the max size needed here
  };

  const TestCase tests[] = {
    { 1,  "single element",    { 42 } },
    { 2,  "two elements",      { 4, 2 } },
    { 2,  "two sorted",        { 2, 4 } },
    { 3,  "three elements",    { 3, 1, 2 } },
    { 5,  "subject example",   { 3, 5, 9, 7, 4 } },
    { 8,  "reverse sorted 8",  { 8, 7, 6, 5, 4, 3, 2, 1 } },
    { 8,  "already sorted 8",  { 1, 2, 3, 4, 5, 6, 7, 8 } },
    { 5,  "duplicates",        { 3, 1, 3, 2, 1 } },
    { 5,  "all same",          { 5, 5, 5, 5, 5 } },
    { 10, "reverse sorted 10", { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 } },
    { 7,  "interleaved 7",     { 1, 3, 2, 5, 4, 7, 6 } }
  };
  const size_t numTests = sizeof(tests) / sizeof(tests[0]);

  for (size_t i = 0; i < numTests; ++i) {
    std::vector<unsigned int> vec(tests[i].data, tests[i].data + tests[i].size);
    if (runTest(vec, tests[i].name))
      ++passed;
    else
      ++failed;
  }

  std::cout << std::endl;

  const unsigned int sizes[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    15, 20, 31, 50, 63, 100, 127, 255,
    500, 1000, 3000
  };
  const unsigned int numSizes = sizeof(sizes) / sizeof(sizes[0]);

  // Capture the seeds before running
  const unsigned int seeds[] = { 42, static_cast<unsigned int>(time(NULL)) };
  const char* const prefixes[] = { "rand(42) n=", "rand(?) n=" };

  for (int run = 0; run < 2; ++run) {
    std::cout << "--- Starting random tests batch " << (run + 1)
              << " (Seed: " << seeds[run] << ") ---" << std::endl;

    std::srand(seeds[run]);
    for (unsigned int s = 0; s < numSizes; ++s) {
      std::ostringstream name;
      name << prefixes[run] << sizes[s];
      if (runTest(randomUniqueVec(sizes[s]), name.str()))
        ++passed;
      else
        ++failed;
    }
  }

  std::cout << std::endl;
  std::cout << "Tests passed: " << passed << "/" << (passed + failed) << std::endl;

  if (failed > 0) {
    std::cout << "SOME TESTS FAILED" << std::endl;
    return 1;
  }
  std::cout << "ALL TESTS PASSED" << std::endl;
  return 0;
}
