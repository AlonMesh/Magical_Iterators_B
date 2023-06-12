#ifndef MAGICAL_CONTAINER_HPP
#define MAGICAL_CONTAINER_HPP
#include <iostream>
#include <vector>
using namespace std;

namespace ariel
{
    class MagicalContainer
    {
        class Iterator
        {
        public:
            vector<int> sortedVec;
            MagicalContainer *original_container;
            int* currentElement;
            bool endOfIteration;

            Iterator() : currentElement(nullptr), original_container(nullptr), sortedVec(), endOfIteration(false) {} // Initialize currentElement to nullptr
            Iterator(MagicalContainer* original_container);
            Iterator(MagicalContainer* original_container, int index);
            vector<int> getSortedVec() const;
            Iterator(const Iterator& other);
            Iterator& operator=(const Iterator& other);

            virtual ~Iterator() = default;

            int &operator*();
            Iterator &operator++();

            bool operator==(const Iterator &other) const;
            bool operator!=(const Iterator &other) const;
            bool operator<(const Iterator &other) const;
            bool operator>(const Iterator &other) const;
        };

        std::vector<int> elements;
        std::vector<Iterator*> iterators; // Collection of iterators

    public:
        MagicalContainer();
        ~MagicalContainer() = default;

        void addElement(int element);
        void removeElement(int element);
        int size() const;

        class AscendingIterator : public Iterator
        {
        public:
            AscendingIterator();
            AscendingIterator(MagicalContainer &container);
            AscendingIterator(MagicalContainer &container, int index);
            AscendingIterator(const AscendingIterator &other);
            ~AscendingIterator() override = default;

            AscendingIterator &operator=(const AscendingIterator &other);

            AscendingIterator begin();
            AscendingIterator end();
        };

        class SideCrossIterator : public Iterator
        {
        public:
            SideCrossIterator();
            SideCrossIterator(MagicalContainer &container);
            SideCrossIterator(MagicalContainer &container, int index);
            SideCrossIterator(const SideCrossIterator &other);
            ~SideCrossIterator() override = default;

            SideCrossIterator &operator=(const SideCrossIterator &other);

            SideCrossIterator begin();
            SideCrossIterator end();
        };

        class PrimeIterator : public Iterator
        {
        public:
            PrimeIterator();
            PrimeIterator(MagicalContainer &container);
            PrimeIterator(MagicalContainer &container, int index);
            PrimeIterator(const PrimeIterator &other);
            ~PrimeIterator() override = default;

            PrimeIterator &operator=(const PrimeIterator &other);

            PrimeIterator begin();
            PrimeIterator end();
        };
    };
}

#endif