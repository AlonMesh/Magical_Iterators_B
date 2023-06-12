#include "doctest.h"
#include "sources/MagicalContainer.hpp"
using namespace std;
using namespace ariel;

TEST_CASE("Initialize MagicalContainer")
{
    MagicalContainer container;

    // Check if the size of an empty container is 0
    CHECK_EQ(container.size(), 0);
}

TEST_CASE("Adding elements increase container's size")
{
    MagicalContainer container;
    container.addElement(2);
    CHECK_EQ(container.size(), 1);

    container.addElement(4);
    CHECK_EQ(container.size(), 2);
}

TEST_CASE("Removing elements decrease container's size")
{
    MagicalContainer container;
    container.addElement(2);
    container.addElement(4);

    container.removeElement(2);
    CHECK_EQ(container.size(), 1);

    container.removeElement(4);
    CHECK_EQ(container.size(), 0);
}

TEST_CASE("Add a duplicate element")
{
    MagicalContainer container;
    container.addElement(2);

    // It should throw an exception and the size of the container should remain the same
    CHECK_THROWS_AS(container.addElement(2), std::invalid_argument);
    CHECK_EQ(container.size(), 1);
}

TEST_CASE("Remove an existing element")
{
    MagicalContainer container;
    // Add some elements to the container
    container.addElement(2);
    container.addElement(4);
    container.addElement(6);

    container.removeElement(4);

    // Check if the size has decreased and the element is removed
    CHECK_EQ(container.size(), 2);
}

TEST_CASE("Remove a non-existing element")
{
    MagicalContainer container;
    // Add some elements to the container
    container.addElement(2);
    container.addElement(4);
    container.addElement(6);

    CHECK_THROWS_AS(container.removeElement(10), std::invalid_argument);

    // Check if the size remains the same
    CHECK_EQ(container.size(), 3);
}

TEST_CASE("AscendingIterator initialize")
{
    MagicalContainer container;
    MagicalContainer::AscendingIterator asc(container);

    container.addElement(3);
    container.addElement(1);
    container.addElement(2);
    CHECK_EQ(asc, asc.begin()); // Check if iterator is equal to begin()
    CHECK_EQ(*asc, 1);

    ++asc;
    CHECK_EQ(*asc, 2);

    ++asc;
    CHECK_EQ(*asc, 3);

    ++asc;
    CHECK_EQ(asc, asc.end()); // Check if iterator reaches the end
    CHECK_THROWS(*asc);

    // Passing the end of the iterator should throw runtime error
    CHECK_THROWS_AS(++asc, std::runtime_error);
}

TEST_CASE("AscendingIterator - Copy Constructor")
{
    MagicalContainer container;
    MagicalContainer::AscendingIterator asc(container);
    container.addElement(3);
    container.addElement(1);
    container.addElement(2);

    MagicalContainer::AscendingIterator ascCopy(asc);

    // Copied iterator has the same state as the original iterator
    CHECK_EQ(*asc, *ascCopy);

    ++asc;
    ++ascCopy;

    CHECK_EQ(*asc, *ascCopy);

    // Modifying the original iterator does not affect the copied iterator
    ++asc;
    CHECK_NE(*asc, *ascCopy);
}

TEST_CASE("AscendingIterator - Assignment Operator")
{
    MagicalContainer container;
    MagicalContainer::AscendingIterator asc1(container);
    container.addElement(3);
    container.addElement(1);
    container.addElement(2);

    MagicalContainer::AscendingIterator asc2;
    asc2 = asc1; // Assign 'asc' to 'asc2'

    // Assigned iterator has the same state as the source iterator
    CHECK_EQ(*asc1, *asc2);

    ++asc1;
    ++asc2;

    CHECK_EQ(*asc1, *asc2);

    // Modifying the source iterator does not affect the assigned iterator
    ++asc1;
    CHECK_NE(*asc1, *asc2);
}

TEST_CASE("AscendingIterator - Equality, Inequality, Less Than and Great Than Comparison")
{
    MagicalContainer container;
    MagicalContainer::AscendingIterator asc1(container);
    container.addElement(3);
    container.addElement(1);
    container.addElement(2);

    MagicalContainer::AscendingIterator asc2(container);

    // Equality comparison
    CHECK(asc1 == asc2);
    CHECK_FALSE(asc1 != asc2);

    // Move 'asc' to a different position within the container
    ++asc1;

    // Inequality comparison
    CHECK(asc1 != asc2);
    CHECK_FALSE(asc1 == asc2);

    // Less Than comparison
    CHECK(asc2 < asc1);
    CHECK_FALSE(asc1 < asc2);

    // Great Than comparison
    CHECK(asc1 > asc2);
    CHECK_FALSE(asc2 > asc1);
}

TEST_CASE("AscendingIterator - Comparing iterators of diffrent containers")
{
    MagicalContainer container;
    MagicalContainer::AscendingIterator asc1(container);
    container.addElement(3);
    container.addElement(1);
    container.addElement(2);

    MagicalContainer container2;
    container2.addElement(5);
    container2.addElement(4);
    MagicalContainer::AscendingIterator asc2(container2);

    CHECK_THROWS_AS({ bool comparison = asc1.operator<(asc2); }, std::runtime_error);
    CHECK_THROWS_AS({ bool comparison = asc1.operator>(asc2); }, std::runtime_error);
    CHECK_THROWS_AS({ bool comparison = asc1.operator==(asc2); }, std::runtime_error);
    CHECK_THROWS_AS({ bool comparison = asc1.operator!=(asc2); }, std::runtime_error);
}

TEST_CASE("SideCrossIterator initialize")
{
    MagicalContainer container;
    MagicalContainer::SideCrossIterator sci(container);
    container.addElement(3);
    container.addElement(1);
    container.addElement(2);

    CHECK_EQ(sci, sci.begin()); // Check if iterator is equal to begin()
    CHECK_EQ(*sci, 1);

    ++sci;
    CHECK_EQ(*sci, 3);

    ++sci;
    CHECK_EQ(*sci, 2);

    ++sci;
    CHECK_EQ(sci, sci.end()); // Check if iterator reaches the end

    // Passing the end of the iterator should throw runtime error
    CHECK_THROWS_AS(++sci, std::runtime_error);
}

TEST_CASE("SideCrossIterator - Copy Constructor")
{
    MagicalContainer container;
    MagicalContainer::SideCrossIterator sci(container);
    container.addElement(3);
    container.addElement(1);
    container.addElement(2);

    MagicalContainer::SideCrossIterator sciCopy(sci);

    // Copied iterator has the same state as the original iterator
    CHECK_EQ(*sci, *sciCopy);

    ++sci;
    ++sciCopy;

    CHECK_EQ(*sci, *sciCopy);

    // Modifying the original iterator does not affect the copied iterator
    ++sci;
    CHECK_NE(*sci, *sciCopy);
}

TEST_CASE("SideCrossIterator - Assignment Operator")
{
    MagicalContainer container;
    MagicalContainer::SideCrossIterator sci(container);
    container.addElement(3);
    container.addElement(1);
    container.addElement(2);

    MagicalContainer::SideCrossIterator sci2;
    sci2 = sci; // Assign 'sci' to 'sci2'

    // Assigned iterator has the same state as the source iterator
    CHECK_EQ(*sci, *sci2);

    ++sci;
    ++sci2;

    CHECK_EQ(*sci, *sci2);

    // Modifying the source iterator does not affect the assigned iterator
    ++sci;
    CHECK_NE(*sci, *sci2);
}

TEST_CASE("SideCrossIterator - Equality, Inequality, Less Than and Great Than Comparison")
{
    MagicalContainer container;
    MagicalContainer::SideCrossIterator sci(container);
    container.addElement(3);
    container.addElement(1);
    container.addElement(2);

    MagicalContainer::SideCrossIterator sci2(container);

    // Equality comparison
    CHECK(sci == sci2);
    CHECK_FALSE(sci != sci2);

    // Move 'asc' to a different position within the container
    ++sci;

    // Inequality comparison
    CHECK(sci != sci2);
    CHECK_FALSE(sci == sci2);

    // Less Than comparison
    CHECK(sci2 < sci);
    CHECK_FALSE(sci < sci2);

    // Great Than comparison
    CHECK(sci > sci2);
    CHECK_FALSE(sci2 > sci);
}

TEST_CASE("SideCrossIterator - Comparing iterators of diffrent containers")
{
    MagicalContainer container;
    MagicalContainer::SideCrossIterator sci(container);
    container.addElement(3);
    container.addElement(1);
    container.addElement(2);

    MagicalContainer container2;
    container2.addElement(5);
    container2.addElement(4);
    MagicalContainer::SideCrossIterator sci2(container2);

    CHECK_THROWS_AS({ bool comparison = sci.operator<(sci2); }, std::runtime_error);
    CHECK_THROWS_AS({ bool comparison = sci.operator>(sci2); }, std::runtime_error);
    CHECK_THROWS_AS({ bool comparison = sci.operator==(sci2); }, std::runtime_error);
    CHECK_THROWS_AS({ bool comparison = sci.operator!=(sci2); }, std::runtime_error);
}

TEST_CASE("PrimeIterator - initialize")
{
    MagicalContainer container;
    MagicalContainer::PrimeIterator pi(container);
    container.addElement(4);
    container.addElement(5);
    container.addElement(2);

    CHECK_EQ(pi, pi.begin()); // Check if iterator is equal to begin()
    CHECK_EQ(*pi, 5);

    ++pi;
    CHECK_EQ(*pi, 2);

    ++pi;
    CHECK_EQ(pi, pi.end()); // Check if iterator reaches the end

    // Passing the end of the iterator should throw runtime error
    CHECK_THROWS_AS(++pi, std::runtime_error);
}

TEST_CASE("PrimeIterator - Copy Constructor")
{
    MagicalContainer container;
    MagicalContainer::PrimeIterator pi(container);
    container.addElement(4);
    container.addElement(5);
    container.addElement(2);

    MagicalContainer::PrimeIterator piCopy(pi);

    // Copied iterator has the same state as the original iterator
    CHECK_EQ(*pi, *piCopy);

    ++pi;
    ++piCopy;

    CHECK_EQ(*pi, *piCopy);

    // Modifying the original iterator does not affect the copied iterator
    ++pi;
    CHECK_NE(*pi, *piCopy);
}

TEST_CASE("PrimeIterator - Assignment Operator")
{
    MagicalContainer container;
    MagicalContainer::PrimeIterator pi(container);
    container.addElement(4);
    container.addElement(5);
    container.addElement(2);

    MagicalContainer::PrimeIterator pi2;
    pi2 = pi; // Assign 'pi' to 'pi2'

    // Assigned iterator has the same state as the source iterator
    CHECK_EQ(*pi, *pi2);

    ++pi;
    ++pi2;

    CHECK_EQ(*pi, *pi2);

    // Modifying the source iterator does not affect the assigned iterator
    ++pi;
    CHECK_NE(*pi, *pi2);
}

TEST_CASE("PrimeIterator - Equality, Inequality, Less Than and Great Than Comparison")
{
    MagicalContainer container;
    MagicalContainer::PrimeIterator pi(container);
    container.addElement(4);
    container.addElement(5);
    container.addElement(2);

    MagicalContainer::PrimeIterator pi2(container);

    // Equality comparison
    CHECK(pi == pi2);
    CHECK_FALSE(pi != pi2);

    // Move 'asc' to a different position within the container
    ++pi;

    // Inequality comparison
    CHECK(pi != pi2);
    CHECK_FALSE(pi == pi2);

    // Less Than comparison
    CHECK(pi2 < pi);
    CHECK_FALSE(pi < pi2);

    // Great Than comparison
    CHECK(pi > pi2);
    CHECK_FALSE(pi2 > pi);
}

TEST_CASE("PrimeIterator - Comparing iterators of diffrent containers")
{
    MagicalContainer container;
    MagicalContainer::PrimeIterator pi(container);
    container.addElement(3);
    container.addElement(1);
    container.addElement(2);

    MagicalContainer container2;
    container2.addElement(5);
    container2.addElement(4);
    MagicalContainer::PrimeIterator pi2(container2);

    CHECK_THROWS_AS({ bool comparison = pi.operator<(pi2); }, std::runtime_error);
    CHECK_THROWS_AS({ bool comparison = pi.operator>(pi2); }, std::runtime_error);
    CHECK_THROWS_AS({ bool comparison = pi.operator==(pi2); }, std::runtime_error);
    CHECK_THROWS_AS({ bool comparison = pi.operator!=(pi2); }, std::runtime_error);
}

TEST_CASE("Empty Container")
{
    MagicalContainer container;
    MagicalContainer::AscendingIterator asc(container);

    // Check if the iterator is equal to begin() and end()
    CHECK_EQ(asc, asc.begin());
    CHECK_EQ(asc, asc.end());

    // Verify that dereferencing the iterator throws an exception
    CHECK_THROWS(*asc);
}

TEST_CASE("Single Element Container")
{
    MagicalContainer container;
    MagicalContainer::AscendingIterator asc(container);
    container.addElement(42); // Add a single element

    // Verify that the iterator points to the single element
    CHECK_EQ(*asc, 42);

    // Increment the iterator and verify that the iterator is at the end
    ++asc;
    CHECK_EQ(asc, asc.end());
}

TEST_CASE("Removing Current Element")
{
    MagicalContainer container;
    MagicalContainer::AscendingIterator asc(container);
    container.addElement(3);
    container.addElement(1);
    container.addElement(2);

    // Increment the iterator such that it'll point to the 2nd element
    ++asc;
    CHECK_EQ(*asc, 2);

    container.removeElement(2);

    // After removing the element, the iterator should still point to the remaining element
    CHECK_EQ(*asc, 3);
}

TEST_CASE("Adding element before")
{
    MagicalContainer container;
    MagicalContainer::AscendingIterator asc(container);
    container.addElement(3);
    container.addElement(2);

    // Increment the iterator such that it'll point to the 2nd element
    ++asc;
    CHECK_EQ(*asc, 3);

    container.addElement(1);

    // After adding the element, the iterator should still point to the same place
    CHECK_EQ(*asc, 2);
}

TEST_CASE("Removing the element that the iterator points on")
{
    MagicalContainer container;
    MagicalContainer::AscendingIterator asc(container);
    container.addElement(3);
    container.addElement(1);
    container.addElement(2);

    ++asc;
    ++asc;
    CHECK_EQ(*asc, 3); // Assert

    // Any removing will force the iterator to point on garbage
    container.removeElement(3);
    CHECK_THROWS(*asc);
}
