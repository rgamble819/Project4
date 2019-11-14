#ifndef AVL_TREE
#define AVL_TREE

#include "Comparator.h"
#include "AVLTreeEnumerator.h"

template <typename T>
class AVLTree {
    template <typename U>
    friend class AVLTreeEnumerator;
private:
    Comparator<T>* comparator = NULL;               // used to determine tree order and item equality
    unsigned long size = 0;                         // actual number of items currently in tree
    T data{};                                       // data item of any type
    bool empty = true;                              // flag to indicate whether node contains data
    int diff = 0;                                   // height of right minus height of left
    AVLTree<T>* left = NULL;                        // pointer to left subtree
    AVLTree<T>* right = NULL;                       // pointer to right subtree
    void zig();                                     // right rotation
    void zag();                                     // left rotation
    void zigzag();                                  // left rotation on left subtree, followed by right rotation
    void zagzig();                                  // right rotation on right subtree, followed by left rotation
    void rebalance();                               // check for and rebalance this node, if needed
    // Additional private helper methods
    bool isEmpty();
    AVLTree<T>* createSubTree();
    void deleteSubTree(int &_old);
    T look(AVLTree<T>* subtree, const T* item) const;
    void swap(AVLTree<T>* subtree1, AVLTree<T>* subtree2);
public:
    AVLTree(Comparator<T>* comparator);             // creates empty linked tree with comparator
    virtual ~AVLTree();                             // deletes subtrees

    // if an equivalent item is not already present, insert item in order and return true
    // if an equivalent item is already present, leave tree unchanged and return false
    bool insert(const T item);

    // if an equivalent item is already present, replace item and return true
    // if an equivalent item is not already present, leave tree unchanged and return false
    bool replace(const T item);

    // if an equivalent item is already present, remove item and return true
    // if an equivalent item is not already present, leave tree unchanged and return false
    bool remove(const T item);

    // if an equivalent item is present, return true
    // if an equivalent item is not present, false
    bool contains(T item) const;

    // if an equivalent item is present, return a copy of the item
    // if an equivalent item is not present, throw a new ExceptionLinkedtreeAccess
    T find(const T item) const;

    unsigned long getSize() const;                  // returns the current number of items in the tree

    AVLTreeEnumerator<T> enumerator(AVLTreeOrder order = AVLTreeOrder::inorder) const;    // create an enumerator for this AVL tree
};
// Constructor/Destructor.

template<typename T>
AVLTree<T>::AVLTree(Comparator<T>* comparator)
{
    this->comparator = comparator;
}

template<typename T>
AVLTree<T>::~AVLTree()
{
    // DELETE ALL NODES...
}

// add public functions here

// Returns the current number of items in the tree.
template<typename T>
unsigned long AVLTree<T>::getSize() const
{
    return size;
}

// Returns the value, if found. Throws a LinkedTreeAcess error.
template<typename T>
T AVLTree<T>::find(const T item) const
{
    // Throw an error if we reach a leaf without finding the value.
    if(data == NULL) throw new ExceptionAVLTreeAccess();
    // Returning the data if it is found.
    if(comparator->compare(item, data) == 0) return *data;
    
    // Recursivly look in the next tree (left or right) depending on the value.
    if(comparator->compare(data, item) > 0) return look(right, item);
    else return look(left, item);
}

// Removes a node while maintaining balance
template<typename T>
bool AVLTree<T>::remove(const T item)
{
    // If the tree is empty we cannot remove any items, return false.
    if(isEmpty()) return false;
    
    // Track old difference value and if we actually removed our value.
    int _old;
    bool isRemoved = false;
    
    // Check which tree to progress down or delete the value if found.
    if(comparator->compare(data, item) > 0)
    {
        _old = left->diff;
        isRemoved = left->remove(data);
        
        if((left->isEmpty() || ((left->diff != _old) && (left->diff == 0)))) diff++;
    }
    else if(comparator->compare(data, item) < 0)
    {
        _old = right->diff;
        isRemoved = right->remove(item);
        if((right->isEmpty()) || ((right->diff != _old) && (right->diff == 0))) diff--;
    }
    else
    {
        deleteSubTree(_old);
        isRemoved = true;
        size--;
    }
    rebalance();
    return isRemoved;
}

// Replaces a node while maintaining balance.
template<typename T>
bool AVLTree<T>::replace(const T item)
{
    // If the node is empty or the same return false/reject duplicate.
    if(isEmpty()) return false;
    else if(comparator->compare(item, data) == 0) return false;
    
    // Check which tree to replace the value in.
    if(comparator->compare(data, item) > 0)
    {
        if(left->isEmpty()) return false;
        else return left->insert(item);
    }
    else
    {
        if(right->isEmpty()()) return false;
        else return right->insert(item);
    }
}

// Inserts a node while maintaining balance. Duplicates are rejected.
template<typename T>
bool AVLTree<T>::insert(const T item)
{
    // If tree is empty return false. Or if a duplicate is found, reject and return false.
    if(isEmpty())
    {
        data = item;
        this->left = createSubTree();
        this->right = createSubTree();
        return true;
    }
    else if(comparator->compare(item, data) == 0)
        return false;
    
    // Check which tree to go down.
    int _diff;
    if(comparator->compare(data, item) > 0)
    {
        // If left is empty insert item.
        if(left->isEmpty())
        {
            left->insert(item);
            diff--;
        }
        // If left is not empty, we have to manipulate the difference because we are adding height.
        else
        {
            _diff = left->diff ;
            left->insert(item);
            if((_diff != left->diff) && (left->diff != 0))
            {
                diff--;
            }
        }
    }
    else
    {
        // If right is empty insert item.
        if(right->isEmpty())
        {
            right->insert(item);
            diff++;
        }
        // If right is not empty, we have to maniplate the difference because we are adding height.
        else
        {
            _diff = right->diff;
            right->insert(data);
            if((_diff != right->diff) && (right->diff != 0))
            {
                diff++;
            }
        }
    }
    size ++;
    rebalance();
    return true;
}

// add additional private functions here


// Rebalance tree when the difference exceeds two
template<typename T>
void AVLTree<T>::rebalance()
{
    if(abs(diff) >= 1) return;
    
    // Change how these are calculated *************
    if(diff < 0 && left->diff <= 0)
    {
        zig();
    }
    else if(diff < 0 && left->dif > 0)
    {
        zigzag();
    }
    else if(diff > 0 && right->diff < 0)
    {
        zagzig();
    }
    else if(diff > 0 && right->diff >= 0)
    {
        zag();
    }
    
}

// zag on right child then zig on root.
template<typename T>
void AVLTree<T>::zigzag()
{
    if(isEmpty()) return;
    right->zag();
    zig();
}

// zig on right child then zag on root.
template<typename T>
void AVLTree<T>::zagzig()
{
    if(isEmpty()) return;
    right->zig();
    zag();
}

// A counter-clockwise rotation.
template<typename T>
void AVLTree<T>::zag()
{
    // Check if we can proceed with a right rotation.
    if(isEmpty()) return;
    else if(right->isEmpty()) return;
    
    AVLTree<T>* rightSubtree = right;
    
    right = rightSubtree->right;
    rightSubtree->right = rightSubtree->left;
    rightSubtree->left  = left;
    left = rightSubtree;
    
    swap(this, rightSubtree);
}

// A clockwise rotation.
template<typename T>
void AVLTree<T>::zig()
{
    // Check if we can proceed with a left rotation.
    if(left->isEmpty()) return;
    else if(right->isEmpty()) return;
    
    AVLTree<T>* leftSubtree = left;
    
    left = leftSubtree->left;
    leftSubtree->left = leftSubtree->right;
    leftSubtree->right = right;
    right = leftSubtree;
    
    swap(this, leftSubtree);
}

//---- Helper Methods ----\\

// Swap the values of nodes.
template<typename T>
void AVLTree<T>::swap(AVLTree<T>* subtree1, AVLTree<T>* subtree2)
{
    T* temp = new T(subtree1->data);
    
    subtree1 = subtree2;
    subtree2 = temp;
    
    delete temp;
    temp = NULL;
}

// Looks to the specified tree by using the find method.
template<typename T>
T AVLTree<T>::look(AVLTree<T>* subtree, const T* item) const
{
    return subtree->find(item);
}

// Deletes the subtree
template<typename T>
void AVLTree<T>::deleteSubTree(int &_old)
{
    if(right->isEmpty())
    {
        // Right tree is empty, so only need to remove the node and replace it with left.
        AVLTree<T>* origLeft = left;
        delete right;
        delete data;
        this = new AVLTree<T>(left);
        origLeft = NULL;
        delete origLeft;
        diff = 0;
    }
    else if(left->isEmpty())
    {
        // Left node is empty, so only need to remove the node and replace it with right
        AVLTree<T>* origRight = right;
        delete left;
        delete data;
        this = new AVLTree<T>(right);
        origRight = NULL;
        delete origRight;
        diff = 0;
    }
    else
    {
        // Must find and replace node with the inorder successor. Must delete the duplicate value too.
        AVLTree<T>* inorder = right;
        while(!inorder->left->isEmpty())
        {
            inorder = inorder->left;
        }
        delete data;
        data = new T(inorder->data);
        
        _old = right->diff;
        
        right->remove(inorder->data);
        
        if((right->isEmpty()) || ((right->diff != _old))) diff--;
    }
}

// Return false if data is null
template<typename T>
bool AVLTree<T>::isEmpty()
{
    if(data == NULL) return false;
    else return true;
}

// Returns a new, empty subtree
template<typename T>
AVLTree<T>* AVLTree<T>::createSubTree()
{
    return new AVLTree<T>(comparator);
}

#endif // !AVL_TREE
