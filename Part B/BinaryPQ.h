// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#ifndef BINARYPQ_H
#define BINARYPQ_H


#include <algorithm>
#include "Eecs281PQ.h"

// A specialized version of the priority queue ADT implemented as a binary
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty PQ with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
            this->compare = comp;
        // TODO: Implement this function, or verify that it is already done
    } // BinaryPQ


    // Description: Construct a PQ out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, data{start, end} {
            int dataSize = static_cast<int>(data.size());
            for(int i = dataSize; i > 0; i--){
                fixDown(dataSize, i);
            }
        // TODO: Implement this function
        
        // These lines are present only so that this provided file compiles.
        //(void)start; // TODO: Delete this line
        //(void)end;   // TODO: Delete this line
    } // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automatically.
    virtual ~BinaryPQ() {
    } // ~BinaryPQ()


    // Description: Assumes that all elements inside the heap are out of
    //              order and 'rebuilds' the heap by fixing the heap
    //              invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        // TODO: Implement this function.
          int dataSize = static_cast<int>(data.size());
            for(int i = dataSize; i > 0; i--){
                fixDown(dataSize, i);
            }
    } // updatePriorities()


    // Description: Add a new element to the PQ.
    // Runtime: O(log(n))
    virtual void push(const TYPE &val) {
        // TODO: Implement this function.
        data.push_back(val);
        int finalIndex = static_cast<int>(data.size());
        fixUp(finalIndex);
        // This line is present only so that this provided file compiles.
        //(void)val; // TODO: Delete this line
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the PQ.
    // Note: We will not run tests on your code that would require it to pop
    //       an element when the PQ is empty. Though you are welcome to if
    //       you are familiar with them, you do not need to use exceptions in
    //       this project.
    // Runtime: O(log(n))
    virtual void pop() {
        // TODO: Implement this function.
        int dataSize = static_cast<int>(data.size());
        getElement(1) = data[data.size() - 1]; 
        fixDown(dataSize, 1);
        data.pop_back();

    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the PQ. This should be a reference for speed. It MUST
    //              be const because we cannot allow it to be modified, as
    //              that might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        // TODO: Implement this function.
        return getElement(1);

        // These lines are present only so that this provided file compiles.
        //static TYPE temp; // TODO: Delete this line
        //return temp;      // TODO: Delete or change this line
    } // top()


    // Description: Get the number of elements in the PQ.
    // Runtime: O(1)
    virtual std::size_t size() const {
        // TODO: Implement this function. Might be very simple,
        // depending on your implementation.
        return data.size(); // TODO: Delete or change this line
    } // size()


    // Description: Return true if the PQ is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        // TODO: Implement this function. Might be very simple,
        // depending on your implementation.
        return data.empty(); // TODO: Delete or change this line
    } // empty()


private:
    // Note: This vector *must* be used for your PQ implementation.
    std::vector<TYPE> data;
    // NOTE: You are not allowed to add any member variables. You don't need
    //       a "heapSize", since you can call your own size() member
    //       function, or check data.size().

    // TODO: Add any additional member functions you require here. For
    //       instance, you might add fixUp() and fixDown().

    TYPE &getElement(int i){
        return data[i-1];
    }

    const TYPE &getElement(int i) const {
        return data[i-1];
    }

    void fixUp(int index){
        while(index > 1 && this->compare(getElement(index/2), getElement(index))){
            std::swap(getElement(index), getElement(index/2));
            index/=2;
        }
    }

    void fixDown(int dataSize, int index){
        while(2*index <= dataSize){
            int childIdx = 2*index;
            if(childIdx < dataSize && this->compare(getElement(childIdx), getElement(childIdx + 1))){
                childIdx++;
            }
            if(!this->compare(getElement(index), getElement(childIdx))){
                break;
            }
            std::swap(getElement(index), getElement(childIdx));
            index = childIdx;
        }
    };
}; // BinaryPQ


#endif // BINARYPQ_H
