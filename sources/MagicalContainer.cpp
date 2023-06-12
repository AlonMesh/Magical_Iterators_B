#include "MagicalContainer.hpp"
#include <algorithm>

namespace ariel
{
    // ===============Helpful functions=================

    vector<int> getSortedCopy(const vector<int> &inputVector)
    {
        vector<int> sortedVector(inputVector); // Create a copy of the inputVector

        sort(sortedVector.begin(), sortedVector.end()); // Sort the vector in ascending order

        return sortedVector;
    }

    bool isPrime(int number)
    {
        if (number <= 1)
        {
            return false;
        }

        for (int i = 2; i * i <= number; ++i)
        {
            if (number % i == 0)
            {
                return false;
            }
        }

        return true;
    }

    vector<int> getCrosSortedCopy(const std::vector<int>& inputVector) {
        vector<int> sortedVector(inputVector);  // Create a copy of the input vector
        sort(sortedVector.begin(), sortedVector.end());  // Sort the copy in ascending order

        std::vector<int> result;  // Result vector to store the sorted numbers
        int left = 0;  // Index for the smallest numbers
        int right = sortedVector.size() - 1;  // Index for the biggest numbers

        while (left <= right) {
            // Add the smallest number
            result.push_back(sortedVector[static_cast<std::vector<int>::size_type>(left++)]);

            // Add the biggest number (if different from the smallest)
            if (left <= right) {
                result.push_back(sortedVector[static_cast<std::vector<int>::size_type>(right--)]);
            }
        }

        return result;
    }

    // ===============MagicalContainer=================
    MagicalContainer::MagicalContainer()
    {
        elements = vector<int>();
        iterators = vector<Iterator *>();
    }

    void MagicalContainer::addElement(int element_to_add)
    {
        auto it = lower_bound(elements.begin(), elements.end(), element_to_add);

        if (it != elements.end() && *it == element_to_add)
        {
            throw invalid_argument("Can't add a duplicate element");
        }

        elements.insert(it, element_to_add);

        // Update/notify iterators
        for (auto iterator : iterators)
        {
            AscendingIterator *ascIterator = dynamic_cast<AscendingIterator *>(iterator);
            if (ascIterator)
            {
                auto iter = lower_bound(ascIterator->sortedVec.begin(), ascIterator->sortedVec.end(), element_to_add);
                ascIterator->sortedVec.insert(iter, element_to_add);
            }
            else
            {
                SideCrossIterator *sideIterator = dynamic_cast<SideCrossIterator *>(iterator);
                if (sideIterator)
                {
                    auto iter = lower_bound(sideIterator->sortedVec.begin(), sideIterator->sortedVec.end(), element_to_add);
                    sideIterator->sortedVec.insert(iter, element_to_add);
                }
                else
                {
                    PrimeIterator *primeIterator = dynamic_cast<PrimeIterator *>(iterator);
                    if (primeIterator)
                    {
                        if (isPrime(element_to_add))
                        {
                            auto iter = lower_bound(primeIterator->sortedVec.begin(), primeIterator->sortedVec.end(), element_to_add);
                            primeIterator->sortedVec.insert(iter, element_to_add);
                        }
                    }
                    else
                    {
                        // Unknown iterator type
                        throw runtime_error("Unknown iterator type encountered");
                    }
                }
            }
        }
    }

    void MagicalContainer::removeElement(int element_to_remove)
    {
        auto it = std::find(elements.begin(), elements.end(), element_to_remove);

        if (it != elements.end())
        {
            elements.erase(it);
            return;
        }

        throw std::runtime_error("Can't remove a non-existing element");
    }

    int MagicalContainer::size() const
    {
        return elements.size();
    }

    // ===============Iterator=================

    MagicalContainer::Iterator::Iterator(MagicalContainer *original_container)
    : Iterator(original_container, 0) 
    {}

    MagicalContainer::Iterator::Iterator(MagicalContainer *original_container, int index)
        : original_container(original_container), endOfIteration(false)
    {
        if (!original_container->elements.empty()) {
            sortedVec = original_container->elements;
            currentElement = &sortedVec[static_cast<std::vector<int>::size_type>(index)];

            if (sortedVec.size() == index) {
                endOfIteration = true;
            }
        }
    }

    vector<int> MagicalContainer::Iterator::getSortedVec() const
    {
        return sortedVec;
    }

    MagicalContainer::Iterator::Iterator(const Iterator &other)
        : original_container(other.original_container), currentElement(other.currentElement), endOfIteration(other.endOfIteration)
    {
        sortedVec = other.sortedVec;
    }

    MagicalContainer::Iterator &MagicalContainer::Iterator::operator=(const Iterator &other)
    {
        if (this->original_container != other.original_container) {
            throw runtime_error("Can't use = with iterators which points on diffrent containers");
        }

        if (this != &other)
        {
            original_container = other.original_container;
            currentElement = other.currentElement;
            sortedVec = other.sortedVec;
            endOfIteration = other.endOfIteration;
        }
        return *this;
    }

    // Implementation of operator*()
    int &MagicalContainer::Iterator::operator*()
    {
        if (currentElement == nullptr)
        {
            throw std::runtime_error("Iterator is not pointing to a valid element");
        }

        return *currentElement;
    }

    // Implementation of operator++()
    MagicalContainer::Iterator &MagicalContainer::Iterator::operator++()
    {
        if (endOfIteration || sortedVec.empty()) {
            // If the iterator has reached the end, throw an exception or handle it according to your requirement.
            throw std::runtime_error("Iterator has reached the end");

        }

        if (currentElement == &sortedVec.back()) 
        {   
            endOfIteration = true;
        }

        ++currentElement; // Move to the next element
        return *this;
    }

    // Implementation of operator==()
    bool MagicalContainer::Iterator::operator==(const MagicalContainer::Iterator &other) const
    {
        if (original_container != other.original_container)
        {
            throw std::runtime_error("Can't compare Iterators with different MagicalContainers");
        }

        // Convert this & other to AscendingIterator
        const AscendingIterator *thisAscending = dynamic_cast<const AscendingIterator *>(this);
        const AscendingIterator *otherAscending = dynamic_cast<const AscendingIterator *>(&other);

        // If both iterators are AscendingIterators, compare them
        if (thisAscending && otherAscending)
        {
            // If both iterators are empty, they're equal
            if(sortedVec.empty() && other.sortedVec.empty()) {
                return true;
            }

            // If both iterators got to the end, they're equal
            if (endOfIteration && other.endOfIteration) {
                return true;
            }

            // Check if the iterators points on the same element
            return (*thisAscending->currentElement == *otherAscending->currentElement);
        }

        // Convert this & other to SideCrossIterator
        const SideCrossIterator *thisSideCross = dynamic_cast<const SideCrossIterator *>(this);
        const SideCrossIterator *otherSideCross = dynamic_cast<const SideCrossIterator *>(&other);

        // If both iterators are SideCrossIterators, compare them
        if (thisSideCross && otherSideCross)
        {
            if(sortedVec.empty() && other.sortedVec.empty()) {
                return true;
            }

            if (endOfIteration && other.endOfIteration) {
                return true;
            }

            return (*thisSideCross->currentElement == *otherSideCross->currentElement);
        }

        // Convert this & other to PrimeIterator
        const PrimeIterator *thisPrime = dynamic_cast<const PrimeIterator *>(this);
        const PrimeIterator *otherPrime = dynamic_cast<const PrimeIterator *>(&other);

        // If both iterators are PrimeIterators, compare them
        if (thisPrime && otherPrime)
        {
            if(sortedVec.empty() && other.sortedVec.empty()) {
                return true;
            }

            if (endOfIteration && other.endOfIteration) {
                return true;
            }

            return (*thisPrime->currentElement == *otherPrime->currentElement);
        }

        // If the iterators are not of the same type, throw exception
        throw std::runtime_error("The iterators are of different types");
    }

    // Implementation of operator!=()
    bool MagicalContainer::Iterator::operator!=(const MagicalContainer::Iterator &other) const
    {
        return !MagicalContainer::Iterator::operator==(other);
    }

    // Implementation of operator<()
    bool MagicalContainer::Iterator::operator<(const MagicalContainer::Iterator &other) const
    {
        if (original_container != other.original_container)
        {
            throw std::runtime_error("Can't compare Iterators with different MagicalContainers");
        }

        // Convert this & other to AscendingIterator
        const AscendingIterator *thisAscending = dynamic_cast<const AscendingIterator *>(this);
        const AscendingIterator *otherAscending = dynamic_cast<const AscendingIterator *>(&other);

        // If both iterators are AscendingIterators, compare them
        if (thisAscending && otherAscending)
        {
            return (*thisAscending->currentElement < *otherAscending->currentElement);
        }

        // Convert this & other to SideCrossIterator
        const SideCrossIterator *thisSideCross = dynamic_cast<const SideCrossIterator *>(this);
        const SideCrossIterator *otherSideCross = dynamic_cast<const SideCrossIterator *>(&other);

        // If both iterators are SideCrossIterators, compare them
        if (thisSideCross && otherSideCross)
        {
            return (*thisSideCross->currentElement < *otherSideCross->currentElement);
        }

        // Convert this & other to PrimeIterator
        const PrimeIterator *thisPrime = dynamic_cast<const PrimeIterator *>(this);
        const PrimeIterator *otherPrime = dynamic_cast<const PrimeIterator *>(&other);

        // If both iterators are PrimeIterators, compare them
        if (thisPrime && otherPrime)
        {
            return (*thisPrime->currentElement < *otherPrime->currentElement);
        }

        // If the iterators are not of the same type, throw exception
        throw std::runtime_error("The iterators are of different types");
    }

    bool MagicalContainer::Iterator::operator>(const MagicalContainer::Iterator &other) const
    {
        return (!(operator<(other)) && !operator==(other)); // If its not equal and >, then its <
    }

    // ===============AscendingIterator=================
    MagicalContainer::AscendingIterator::AscendingIterator() : Iterator() {}

    MagicalContainer::AscendingIterator::AscendingIterator(MagicalContainer &container)
    : Iterator(&container) {
        if (!sortedVec.empty()) {
            currentElement = &sortedVec[0];
        }
        else {
            currentElement = nullptr;
        }
    }

    MagicalContainer::AscendingIterator::AscendingIterator(MagicalContainer &container, int index)
    : Iterator(&container, index) {
        if (!sortedVec.empty()) {
            currentElement = &sortedVec[static_cast<std::vector<int>::size_type>(index)];
        }
        else {
            currentElement = nullptr;
        }
    }

    MagicalContainer::AscendingIterator::AscendingIterator(const AscendingIterator &other)
    : Iterator(other) {}

    MagicalContainer::AscendingIterator &MagicalContainer::AscendingIterator::operator=(const AscendingIterator &other)
    {
        if (this->original_container != other.original_container) {
            throw runtime_error("Can't use = with iterators which points on diffrent containers");
        }
        
        if (this == &other)
        {
            return *this;
        }

        // Call the base class assignment operator
        Iterator::operator=(other);
        return *this;
    }

    MagicalContainer::AscendingIterator MagicalContainer::AscendingIterator::begin()
    {
        return AscendingIterator(*original_container, 0);
    }

    MagicalContainer::AscendingIterator MagicalContainer::AscendingIterator::end()
    {
        return AscendingIterator(*original_container, sortedVec.size());
    }

    // ===============SideCrossIterator=================
    MagicalContainer::SideCrossIterator::SideCrossIterator() : Iterator()
    {}

    MagicalContainer::SideCrossIterator::SideCrossIterator(MagicalContainer &container)
    : Iterator(&container)
    {
        sortedVec = getCrosSortedCopy(sortedVec);
        currentElement = &sortedVec[0];
    }

    MagicalContainer::SideCrossIterator::SideCrossIterator(MagicalContainer &container, int index)
    : Iterator(&container, index)
    {
        sortedVec = getCrosSortedCopy(sortedVec);
        currentElement = &sortedVec[static_cast<std::vector<int>::size_type>(index)];
    }


    MagicalContainer::SideCrossIterator::SideCrossIterator(const SideCrossIterator &other)
    : Iterator(other)
    {}


    MagicalContainer::SideCrossIterator &MagicalContainer::SideCrossIterator::operator=(const SideCrossIterator &other)
    {
        if (this->original_container != other.original_container) {
            throw runtime_error("Can't use = with iterators which points on diffrent containers");
        }
        
        if (this == &other)
        {
            return *this;
        }

        // Call the base class assignment operator
        Iterator::operator=(other);
        return *this;
    }

    MagicalContainer::SideCrossIterator MagicalContainer::SideCrossIterator::begin()
    {
        return SideCrossIterator(*original_container, 0);
    }

    MagicalContainer::SideCrossIterator MagicalContainer::SideCrossIterator::end()
    {
        return SideCrossIterator(*original_container, sortedVec.size());
    }

    // ===============PrimeIterator=================
    MagicalContainer::PrimeIterator::PrimeIterator() : Iterator() {}

    MagicalContainer::PrimeIterator::PrimeIterator(MagicalContainer &container)
    : Iterator(&container)
    {
        for (auto it = sortedVec.begin(); it != sortedVec.end();) {
            if (!isPrime(*it)) {
                it = sortedVec.erase(it);
            } else {
                ++it;
            }
        }
    }

    MagicalContainer::PrimeIterator::PrimeIterator(MagicalContainer &container, int index)
    : Iterator(&container, index) {
                for (auto it = sortedVec.begin(); it != sortedVec.end();) {
            if (!isPrime(*it)) {
                it = sortedVec.erase(it);
            } else {
                ++it;
            }
        }
    }

    MagicalContainer::PrimeIterator::PrimeIterator(const PrimeIterator &other)
        : Iterator(other) {}


    MagicalContainer::PrimeIterator &MagicalContainer::PrimeIterator::operator=(const PrimeIterator &other)
    {
        if (this->original_container != other.original_container) {
            throw runtime_error("Can't use = with iterators which points on diffrent containers");
        }
        
        if (this == &other)
        {
            return *this;
        }

        // Call the base class assignment operator
        Iterator::operator=(other);
        return *this;
    }

    MagicalContainer::PrimeIterator MagicalContainer::PrimeIterator::begin()
    {
        return PrimeIterator(*original_container, 0);
    }

    MagicalContainer::PrimeIterator MagicalContainer::PrimeIterator::end()
    {
        return PrimeIterator(*original_container, sortedVec.size());
    }
}
