/**
 * File: KDTree.h
 * Author: Zach Gu
 * ------------------------
 * An interface representing a kd-tree in some number of dimensions. The tree
 * can be constructed from a set of data and then queried for membership and
 * nearest neighbors.
 */

#ifndef KDTREE_INCLUDED
#define KDTREE_INCLUDED

#include "Point.h"
#include "BoundedPQueue.h"
#include <stdexcept>
#include <cmath>
#include <set>

// "using namespace" in a header file is conventionally frowned upon, but I'm
// including it here so that you may use things like size_t without having to
// type std::size_t every time.
using namespace std;

template <size_t N, typename ElemType>
class KDTree {
public:
    // Constructor: KDTree();
    // Usage: KDTree<3, int> myTree;
    // ----------------------------------------------------
    // Constructs an empty KDTree.
    KDTree();
    
    // Destructor: ~KDTree()
    // Usage: (implicit)
    // ----------------------------------------------------
    // Cleans up all resources used by the KDTree.
    ~KDTree();
    
    // KDTree(const KDTree& rhs);
    // KDTree& operator=(const KDTree& rhs);
    // Usage: KDTree<3, int> one = two;
    // Usage: one = two;
    // -----------------------------------------------------
    // Deep-copies the contents of another KDTree into this one.
    KDTree(const KDTree& rhs);
    KDTree& operator=(const KDTree& rhs);
    
    // size_t dimension() const;
    // Usage: size_t dim = kd.dimension();
    // ----------------------------------------------------
    // Returns the dimension of the points stored in this KDTree.
    size_t dimension() const;
    
    // size_t size() const;
    // bool empty() const;
    // Usage: if (kd.empty())
    // ----------------------------------------------------
    // Returns the number of elements in the kd-tree and whether the tree is
    // empty.
    size_t size() const;
    bool empty() const;
    
    // bool contains(const Point<N>& pt) const;
    // Usage: if (kd.contains(pt))
    // ----------------------------------------------------
    // Returns whether the specified point is contained in the KDTree.
    bool contains(const Point<N>& pt) const;
    
    // void insert(const Point<N>& pt, const ElemType& value);
    // Usage: kd.insert(v, "This value is associated with v.");
    // ----------------------------------------------------
    // Inserts the point pt into the KDTree, associating it with the specified
    // value. If the element already existed in the tree, the new value will
    // overwrite the existing one.
    void insert(const Point<N>& pt, const ElemType& value);
    
    // ElemType& operator[](const Point<N>& pt);
    // Usage: kd[v] = "Some Value";
    // ----------------------------------------------------
    // Returns a reference to the value associated with point pt in the KDTree.
    // If the point does not exist, then it is added to the KDTree using the
    // default value of ElemType as its key.
    ElemType& operator[](const Point<N>& pt);
    
    // ElemType& at(const Point<N>& pt);
    // const ElemType& at(const Point<N>& pt) const;
    // Usage: cout << kd.at(v) << endl;
    // ----------------------------------------------------
    // Returns a reference to the key associated with the point pt. If the point
    // is not in the tree, this function throws an out_of_range exception.
    ElemType& at(const Point<N>& pt);
    const ElemType& at(const Point<N>& pt) const;
    
    // ElemType kNNValue(const Point<N>& key, size_t k) const
    // Usage: cout << kd.kNNValue(v, 3) << endl;
    // ----------------------------------------------------
    // Given a point v and an integer k, finds the k points in the KDTree
    // nearest to v and returns the most common value associated with those
    // points. In the event of a tie, one of the most frequent value will be
    // chosen.
    ElemType kNNValue(const Point<N>& key, size_t k) const;


private:
    // TODO: Add implementation details here.
    // structure Node
    struct Node {
        Point<N> pt_;    // Point
        ElemType value_; // Value, mapped with Point
        size_t level_;   // Level of the node

        Node *left_;     // Left sub tree
        Node *right_;    // Right sub tree
    };

    Node *root_;

    size_t size_;

private:
    //A helper function to find node
    Node* findNode(const Point<N>& pt) const;

    //A helper function to traverse and copy tree
    Node *copyRe(const Node* current_node);

    //A helper function to traverse and delete tree
    void deleteRe(Node *current_node);

    // kNNValueRecursion function
    void kNNValueRe(const Point<N>& pt, BoundedPQueue<Node*>& bpq, Node* current_node) const;

};

/** KDTree class implementation details */
// TODO: finish the implementation of the rest of the KDTree class

// Construct function
template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree() {
    // TODO: Fill this in.
    size_ = 0;
    root_ = NULL;
}

// Desstructor function
template <size_t N, typename ElemType>
KDTree<N, ElemType>::~KDTree() {
    // TODO: Fill this in.
    size_ = 0;
    deleteRe(root_);
}

// Copy constructor
template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree(const KDTree &other) {
    size_ = other.size();
    root_ = copyRe(other.root_);
}


template <size_t N, typename ElemType>
KDTree<N, ElemType> &KDTree<N, ElemType>::operator =(const KDTree & other) {
    if (this != &other) {
        deleteRe(root_);
        root_ = copyRe(other.root_);
        size_ = other.size();
    }

    return *this;
}


// Get dimension of Point
template <size_t N, typename ElemType>
inline size_t KDTree<N, ElemType>::dimension() const {
    // TODO: Fill this in.
    return N;
}

// Get size of KDTree
template <size_t N, typename ElemType>
inline size_t KDTree<N, ElemType>::size() const {
    // TODO: Fill this in.
    return size_;
}

template <size_t N, typename ElemType>
inline bool KDTree<N, ElemType>::empty() const {
    return size() == 0;
}

// Insert a Node into the tree
template <size_t N, typename ElemType>
void KDTree<N, ElemType>::insert(const Point<N> &pt,
                                 const ElemType &value) {
    Node *current_node = root_;
    Node *parent_node = NULL;

    size_t level = 0;

    while (current_node != NULL) {
        // The point is found and replace the value
        if (pt == current_node->pt_) {
            current_node->value_ = value;
            return;
        }

        // Determine which subtree to search
        size_t index = current_node->level_ % N;

        parent_node = current_node;
        if(pt[index] < current_node->pt_[index]) {
            current_node = current_node->left_;
        }
        else if (pt[index] >= current_node->pt_[index]) {
            current_node = current_node->right_;
        }

        ++level;
    }

    Node *node = new Node;
    node->pt_ = pt;
    node->value_ = value;
    node->level_ = level;
    node->left_ = NULL;
    node->right_ = NULL;
    ++size_;

    if (current_node == root_) {
        root_ = node;
    }
    else if (parent_node->pt_[parent_node->level_ % N] > pt[parent_node->level_ % N]) {
        parent_node->left_ = node;
    }
    else {
        parent_node->right_ = node;
    }
}

// A helper function to find the node which has the Point pt
template <size_t N, typename ElemType>
typename KDTree<N, ElemType>::Node* KDTree<N, ElemType>::findNode(const Point<N> &pt) const {
    Node *current_node = root_;
    while(current_node != NULL) {
        if (current_node->pt_ == pt)
            return current_node;

        // Continue to search sub trees
        size_t index = current_node->level_ % N;
        if (current_node->pt_[index] > pt[index]) {
            current_node = current_node->left_;
        }
        else
            current_node = current_node->right_;
    }
    return NULL;
}

// A helper function to traverse and copy tree
template <size_t N, typename ElemType>
typename KDTree<N, ElemType>::Node* KDTree<N, ElemType>::copyRe(const Node *current_node) {
    if (current_node == NULL)
        return NULL;

    Node *copy_node = new Node;
    copy_node->pt_ = current_node->pt_;
    copy_node->value_ = current_node->value_;
    copy_node->level_ = current_node->level_;
    copy_node->left_ = copyRe(current_node->left_);
    copy_node->right_ = copyRe(current_node->right_);

    return copy_node;

}

// A helper function to traverse and delete tree
template <size_t N, typename ElemType>
void KDTree<N, ElemType>::deleteRe(Node *current_node) {
    if (current_node == NULL)
        return ;

    deleteRe(current_node->left_);
    deleteRe(current_node->right_);

    delete current_node;
}

// Determine the node whether in the tree
template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::contains(const Point<N> &pt) const {
    return findNode(pt) != NULL;
}

// operation [] , this implementation isn't efficient
template <size_t N, typename ElemType>
ElemType &KDTree<N, ElemType>::operator [](const Point<N> &pt) {
    Node *found_node = findNode(pt);

    if (found_node != NULL) {
        return found_node->value_;
    }
    else {
        insert(pt, ElemType());
        Node *tmp_node = findNode(pt);
        return tmp_node->value_;
    }
}

// at function
template <size_t N, typename ElemType>
ElemType &KDTree<N, ElemType>::at(const Point<N> &pt) {
    Node *found_node = findNode(pt);

    if (found_node != NULL) {
        return found_node->value_;
    }
    else
        throw out_of_range("This point doesn't exist!");
}

// at function , const
template <size_t N, typename ElemType>
const ElemType &KDTree<N, ElemType>::at(const Point<N> &pt) const {
    Node *found_node = findNode(pt);

    if (found_node != NULL) {
        return found_node->value_;
    }
    else
        throw out_of_range("This point doesn't exist!");
}

// kNNValue function
template <size_t N, typename ElemType>
ElemType KDTree<N, ElemType>::kNNValue(const Point<N> &key, size_t k) const {
    BoundedPQueue<Node*> bpq(k);
    kNNValueRe(key, bpq, root_);

    std::multiset<ElemType> kValues;
    while (!bpq.empty()) {
        kValues.insert(bpq.dequeueMin()->value_);
    }

    ElemType most_freq;
    size_t count = 0;
    for (auto it = kValues.begin(), ie = kValues.end(); it != ie; it++) {
        if (kValues.count(*it) > count) {
            most_freq = *it;
            count = kValues.count(*it);
        }
    }

    return most_freq;
}

// kNNValueRe function
template <size_t N, typename ElemType>
void KDTree<N, ElemType>::kNNValueRe(const Point<N> &pt, BoundedPQueue<Node *> &bpq, Node *current_node) const {
    // Starting at the root
    if (current_node == NULL)
        return ;

    // Add the current node to the bpq
    bpq.enqueue(current_node, Distance(current_node->pt_, pt));

    // Recursively search the half of the tree that contains the point
    size_t index = current_node->level_ % N;
    if (pt[index] < current_node->pt_[index]) {
        kNNValueRe(pt, bpq, current_node->left_);
        // If the candiate hypersphere crosses this splitting plane,
        // look on the other side of plane
        if (bpq.size() != bpq.maxSize() || fabs(pt[index] - current_node->pt_[index]) < bpq.worst())
            kNNValueRe(pt, bpq, current_node->right_);
    }
    else {
        kNNValueRe(pt, bpq, current_node->right_);
        if (bpq.size() != bpq.maxSize() || fabs(pt[index] - current_node->pt_[index]) < bpq.worst())
            kNNValueRe(pt, bpq, current_node->left_);
    }
}

#endif // KDTREE_INCLUDED
