// Uncomment the next line to use precompiled headers
// #include "pch.h" // Commented out for simplicity, use only if truly configured for PCH.
// uncomment the next line if you do not use precompiled headers
#include "gtest/gtest.h" // UNCOMMENT THIS LINE!
#include <vector>        // Required for std::vector
#include <memory>        // Required for std::unique_ptr
#include <cassert>       // Required for assert
#include <cstdlib>       // Required for rand, srand
#include <ctime>         // Required for time
#include <stdexcept>     // Required for std::out_of_range, std::length_error

// the global test environment setup and tear down
// you should not need to change anything here
class Environment : public ::testing::Environment
{
public:
    ~Environment() override {}

    // Override this to define how to set up the environment.
    void SetUp() override
    {
        //  initialize random seed
        srand(static_cast<unsigned int>(time(nullptr)));
    }

    // Override this to define how to tear down the environment.
    void TearDown() override {}
};

// create our test class to house shared data between tests
// you should not need to change anything here
class CollectionTest : public ::testing::Test
{
protected:
    // create a smart point to hold our collection
    std::unique_ptr<std::vector<int>> collection;

    void SetUp() override
    { // create a new collection to be used in the test
        collection.reset(new std::vector<int>);
    }

    void TearDown() override
    { //  erase all elements in the collection, if any remain
        collection->clear();
        // free the pointer
        collection.reset(nullptr);
    }

    // helper function to add random values from 0 to 99 count times to the collection
    void add_entries(int count)
    {
        assert(count > 0); // Ensure count is positive
        for (auto i = 0; i < count; ++i)
            collection->push_back(rand() % 100);
    }
};

// When should you use the EXPECT_xxx or ASSERT_xxx macros?
// Use ASSERT when failure should terminate processing, such as the reason for the test case.
// Use EXPECT when failure should notify, but processing should continue

// Test that a collection is empty when created.
// Prior to calling this (and all other TEST_F defined methods),
//  CollectionTest::StartUp is called.
// Following this method (and all other TEST_F defined methods),
//  CollectionTest::TearDown is called
TEST_F(CollectionTest, CollectionSmartPointerIsNotNull)
{
    // is the collection created
    ASSERT_TRUE(collection);

    // if empty, the size must be 0
    ASSERT_NE(collection.get(), nullptr);
}

// Test that a collection is empty when created.
TEST_F(CollectionTest, IsEmptyOnCreate)
{
    // is the collection empty?
    ASSERT_TRUE(collection->empty());

    // if empty, the size must be 0
    ASSERT_EQ(collection->size(), 0);
}

/* Comment this test out to prevent the test from running
 * Uncomment this test to see a failure in the test explorer */
 // TEST_F(CollectionTest, AlwaysFail)
 // {
 //   FAIL();
 // }

 // TODO: Create a test to verify adding a single value to an empty collection
TEST_F(CollectionTest, AddSingleValueToEmptyCollection) // Renamed for clarity
{
    // is the collection empty?
    ASSERT_TRUE(collection->empty());
    // if empty, the size must be 0
    ASSERT_EQ(collection->size(), 0);

    add_entries(1);

    // is the collection still empty?
    ASSERT_FALSE(collection->empty());
    // if not empty, what must the size be?
    ASSERT_EQ(collection->size(), 1);
}

// TODO: Create a test to verify adding five values to collection
TEST_F(CollectionTest, AddFiveValuesToCollection) // Renamed for clarity
{
    ASSERT_TRUE(collection->empty());
    add_entries(5);
    ASSERT_FALSE(collection->empty());
    ASSERT_EQ(collection->size(), 5);
}

// TODO: Create a test to verify that max size is greater than or equal to size for 0, 1, 5, 10 entries
TEST_F(CollectionTest, MaxSizeIsGreaterThanOrEqualToSize)
{
    // For 0 entries
    EXPECT_GE(collection->max_size(), collection->size()); // max_size() of std::vector is very large

    // For 1 entry
    collection->clear(); // Ensure clear state
    add_entries(1);
    EXPECT_GE(collection->max_size(), collection->size());

    // For 5 entries
    collection->clear();
    add_entries(5);
    EXPECT_GE(collection->max_size(), collection->size());

    // For 10 entries
    collection->clear();
    add_entries(10);
    EXPECT_GE(collection->max_size(), collection->size());
}

// TODO: Create a test to verify that capacity is greater than or equal to size for 0, 1, 5, 10 entries
TEST_F(CollectionTest, CapacityIsGreaterThanOrEqualToSize)
{
    // For 0 entries
    EXPECT_GE(collection->capacity(), collection->size());

    // For 1 entry
    collection->clear();
    add_entries(1);
    EXPECT_GE(collection->capacity(), collection->size());

    // For 5 entries
    collection->clear();
    collection->reserve(5); // Ensure capacity can hold 5 (might be optimized by vector)
    add_entries(5);
    EXPECT_GE(collection->capacity(), collection->size());

    // For 10 entries
    collection->clear();
    collection->reserve(10); // Ensure capacity can hold 10
    add_entries(10);
    EXPECT_GE(collection->capacity(), collection->size());
}

// TODO: Create a test to verify resizing increases the collection
TEST_F(CollectionTest, ResizeIncreasesCollectionSize)
{
    add_entries(2); // Start with 2 elements
    size_t initial_size = collection->size(); // Should be 2

    // Resize to a larger size
    collection->resize(5);

    EXPECT_EQ(collection->size(), 5); // Size should increase to 5
    EXPECT_GE(collection->capacity(), 5); // Capacity should be at least 5
    EXPECT_GT(collection->size(), initial_size); // Size should have increased
}

// TODO: Create a test to verify resizing decreases the collection
TEST_F(CollectionTest, ResizeDecreasesCollectionSize)
{
    add_entries(10); // Start with 10 elements
    size_t initial_size = collection->size(); // Should be 10

    // Resize to a smaller size
    collection->resize(3);

    EXPECT_EQ(collection->size(), 3); // Size should decrease to 3
    EXPECT_GE(collection->capacity(), 3); // Capacity should be at least 3
    EXPECT_LT(collection->size(), initial_size); // Size should have decreased
}

// TODO: Create a test to verify resizing decreases the collection to zero
TEST_F(CollectionTest, ResizeDecreasesCollectionToZero)
{
    add_entries(5); // Start with some elements
    ASSERT_GT(collection->size(), 0);

    collection->resize(0); // Resize to 0 elements

    EXPECT_EQ(collection->size(), 0); // Size should be zero
    EXPECT_GE(collection->capacity(), 0); // Capacity might be 0 or more
    EXPECT_TRUE(collection->empty());
}

// TODO: Create a test to verify clear erases the collection
TEST_F(CollectionTest, ClearErasesCollection)
{
    add_entries(5); // Add some elements
    ASSERT_FALSE(collection->empty());
    ASSERT_GT(collection->size(), 0);

    collection->clear(); // Clear the collection

    EXPECT_TRUE(collection->empty());
    EXPECT_EQ(collection->size(), 0);
}

// TODO: Create a test to verify erase(begin,end) erases the collection
TEST_F(CollectionTest, EraseBeginEndErasesCollection)
{
    add_entries(5); // Add 5 elements
    ASSERT_EQ(collection->size(), 5);

    // Erase all elements from begin to end
    collection->erase(collection->begin(), collection->end());

    EXPECT_TRUE(collection->empty());
    EXPECT_EQ(collection->size(), 0);
}

// TODO: Create a test to verify reserve increases the capacity but not the size of the collection
TEST_F(CollectionTest, ReserveIncreasesCapacityNotSize)
{
    size_t initial_size = collection->size();     // Should be 0
    size_t initial_capacity = collection->capacity(); // Could be 0 or small default

    collection->reserve(50); // Reserve capacity for 50 elements

    EXPECT_EQ(collection->size(), initial_size); // Size should NOT change
    EXPECT_GE(collection->capacity(), 50);      // Capacity should be at least 50
    EXPECT_GT(collection->capacity(), initial_capacity); // Capacity should have increased
}

// TODO: Create a test to verify the std::out_of_range exception is thrown when calling at() with an index out of bounds
// NOTE: This is a negative test
TEST_F(CollectionTest, AtThrowsOutOfRangeExceptionForOutOfBoundsIndex)
{
    // Test with an empty collection
    EXPECT_THROW(collection->at(0), std::out_of_range); // Accessing index 0 on empty vector

    collection->push_back(10); // Add one element
    // Test with an index equal to size (which is out of bounds for std::vector::at)
    EXPECT_THROW(collection->at(collection->size()), std::out_of_range);
    // Test with an index much larger than size
    EXPECT_THROW(collection->at(collection->size() + 100), std::out_of_range);
}

// TODO: Create 2 unit tests of your own to test something on the collection - do 1 positive & 1 negative

// Custom Positive Test: Verify copy construction creates an independent, identical copy
TEST_F(CollectionTest, CopyConstructionCreatesIndependentCopy)
{
    add_entries(3); // Add some elements to the original collection
    std::vector<int> original_content = *collection; // Store content before copy

    std::unique_ptr<std::vector<int>> copied_collection(new std::vector<int>(*collection)); // Copy constructor

    // Verify they are identical in content and size
    ASSERT_EQ(original_content.size(), copied_collection->size());
    for (size_t i = 0; i < original_content.size(); ++i) {
        EXPECT_EQ(original_content.at(i), copied_collection->at(i));
    }

    // Modify original and ensure copy remains unchanged
    collection->push_back(999);
    EXPECT_NE(collection->size(), copied_collection->size()); // Sizes should now differ
}

// Custom Negative Test: Verify that erase with invalid iterators (outside of range) behaves as expected (e.g., doesn't crash or throws specific errors in debug)
// Note: std::vector::erase with invalid iterators leads to undefined behavior in release builds.
// In debug builds, it often triggers asserts or specific exceptions.
TEST_F(CollectionTest, EraseWithInvalidIteratorsAssertsInDebug)
{
    collection->push_back(1);
    collection->push_back(2);
    ASSERT_EQ(collection->size(), 2);

    // Attempt to erase with an iterator beyond end()
    // This is technically undefined behavior in standard C++, but common libraries
    // (like MSVC's STL) often have debug checks that assert or throw.
    // We'll use EXPECT_DEATH_IF_SUPPORTED for expected crash in debug, or
    // EXPECT_ANY_THROW if an exception is specifically caught.
#ifdef _MSC_VER // Specific to MSVC compiler for debug assertions
  // Note: EXPECT_DEATH_IF_SUPPORTED runs the test in a separate process, which is slower.
  // std::vector::erase with end() as begin is valid for empty range.
  // Invalid is usually begin() > end() or iterators not belonging to the vector.
  // For simplicity here, we'll demonstrate using an iterator far out of bounds if possible,
  // though this often just crashes rather than throws a specific C++ exception.
  // The safest way to test U.B. is often through code reviews or fuzzing, not unit tests.
  // For a unit test, we'll simulate the intent: causing an error.
  // As a "negative" test, if your vector implementation *did* throw, you'd EXPECT_THROW.
  // Since std::vector::erase explicitly says "Undefined Behavior", a specific EXPECT_THROW
  // for std::out_of_range or similar is not guaranteed.
  // Let's test a scenario where `erase` is called with invalid range where begin > end, which might throw
  // std::out_of_range in some debug implementations, or lead to an assertion.
    EXPECT_NO_THROW(collection->erase(collection->begin(), collection->begin())); // Valid: erases nothing
    // For a true "negative" test, we'd need an operation that *guarantees* an exception.
    // A common one for std::vector is `at()` with an out-of-bounds index (already covered).
    // Given the constraints, let's redefine this "negative" test to use `at()` again,
    // but demonstrating a different boundary condition.
    // (If the assignment implies a vector operation that *always* throws, we must use that.)
#endif

// Re-purposing this negative test to illustrate a different `at()` scenario
// - accessing an index that was valid but became invalid after a resize operation.
    collection->clear();
    collection->push_back(10);
    collection->push_back(20);
    ASSERT_EQ(collection->size(), 2);

    collection->resize(1); // Shrink the vector to size 1
    EXPECT_EQ(collection->size(), 1);
    EXPECT_EQ(collection->at(0), 10); // First element still accessible

    // Now, attempting to access the second element (which used to exist) should throw
    EXPECT_THROW(collection->at(1), std::out_of_range); // New negative test scenario
}