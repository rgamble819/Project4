#ifndef AVL_TREE_ENUMERATOR
#define AVL_TREE_ENUMERATOR

#include <stack>
#include "Enumerator.h"
#include "AVLTreeOrder.h"
#include "AVLTree.h"

template <typename T>
class AVLTreeEnumerator : public Enumerator<T>
{
private:
    AVLTreeOrder order;
    std::stack<const AVLTree<T>*> traversalStack;
    void buildTraversalStack(const AVLTree<T>* current);
public:
    AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order);
    virtual ~AVLTreeEnumerator();
    bool hasNext() const;
    T next();                        // throws ExceptionEnumerationBeyondEnd if no next item is available
    T peek() const;                    // throws ExceptionEnumerationBeyondEnd if no next item is available
    AVLTreeOrder getOrder();        // returns the order of this enumerator (preorder, inorder, or postorder)
};

// put implementation here

template<typename T>
AVLTreeEnumerator<T>::AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order)
{
    this->order = order;
    traversalStack = buildTraversalStack(root);
}

template<typename T>
AVLTreeEnumerator<T>::~AVLTreeEnumerator()
{
    // Delete object...
}

// Returns true if the AVLTree has a next value according to the order specified.
template<typename T>
bool AVLTreeEnumerator<T>::hasNext() const
{
    return traversalStack->hasNext();
}

// Returns the next value and progresses enumerator according to the order.
template<typename T>
T AVLTreeEnumerator<T>::next()
{
    return traversalStack->next();
}

// Returns the next value without progressing the enumerator according to the order.
template<typename T>
T AVLTreeEnumerator<T>::peek() const
{
    return traversalStack->peek();
}

template<typename T>
AVLTreeOrder AVLTreeEnumerator<T>::getOrder()
{ 
    return order;
}

template<typename T>
void AVLTreeEnumerator<T>::buildTraversalStack(const AVLTree<T>* current)
{
    // Build the stack with the correct order.
}

#endif // !AVL_TREE_ENUMERATOR
