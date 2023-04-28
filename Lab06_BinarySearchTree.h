#ifndef __MYBST_H__
#define __MYBST_H__

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>

enum TreeTravOrderType
{
    PreOrder,
    InOrder,
    PostOrder
};

enum NodePosType
{
    Left,
    Right,
    Root
};

template <typename ComparableType>
class MyBST
{
  private:
    struct BinaryNode
    {
        ComparableType element;
        size_t height;
        BinaryNode *left;
        BinaryNode *right;
        
        BinaryNode(const ComparableType & x, const size_t h, BinaryNode *l, BinaryNode *r) : 
            element{x}, 
            height{h},
            left{l}, 
            right{r} 
        { ; }

        BinaryNode(ComparableType && x, const size_t h, BinaryNode *l, BinaryNode *r) : 
            element{std::move(x)}, 
            height{h},
            left{l},
            right{r}
        { ; }
    };

    BinaryNode *root;       // the root node
    size_t theSize;         // the number of data elements in the tree

    // finds the minimum data element from the tree rooted at t
    BinaryNode* findMin(BinaryNode *t) const
    {
        if (t == nullptr)
            return nullptr;
        if (t->left == nullptr)
            return t;
        return findMin(t->left);
    }

    // finds the maximum data element from the tree rooted at t
    BinaryNode* findMax(BinaryNode *t) const
    {
        if (t == nullptr)
            return nullptr;
        if (t->right == nullptr)
            return t;
        return findMin(t->right);
    }

    // checks if x is contained in the tree rooted at t
    bool contains(const ComparableType& x, BinaryNode *t) const
    {
        if (t == nullptr)
            return false;
        else if (x < t->element)
            return contains(x, t->left);
        else if (x > t->element)
            return contains(x, t->right);
        else
            return true; // Match found
    }

    // deletes the tree rooted at t
    void clear(BinaryNode*& t)
    {
        if (t != nullptr) {
            clear(t->left);
            clear(t->right);
            delete t;
        }

        t = nullptr;
    }

    // returns the height of the node
    // leave nodes have a height of 1
    size_t height(BinaryNode* t) const
    { return (t == nullptr ? 0 : t->height); }

    // balances tree node t
    void balance(BinaryNode*& t)
    {
        /*if (t != nullptr)
            std::cout << "balance(" << t->element << ")" << std::endl;
        else
            std::cout << "balance(nullptr)" << std::endl;*/

        if (t == nullptr) {
            return; // no change, no action taken
        }
           

        if (int(height(t->left) - height(t->right)) > 1) {
            //std::cout << "Left branch larger than right branch by " << height(t->left) - height(t->right) << std::endl;
            if (height(t->left->left) >= height(t->left->right))
                rotateLeft(t);
            else
                doubleRotateLeft(t);
        } else if (int(height(t->right) - height(t->left)) > 1) {
            //std::cout << "Right branch larger than left branch: " << height(t->right) << " vs " << height(t->left) << std::endl;
            if (height(t->right->right) >= height(t->right->left))
                rotateRight(t);
            else
                doubleRotateRight(t);
        } else {
           // std::cout << "no balancing needed" << std::endl;
        }

        t->height = std::max(height(t->left), height(t->right)) + 1;

    }
    
    // single rotation that reduces left branch depth
    void rotateLeft(BinaryNode*& t)
    {
        //if (t == nullptr)
            //std::cout << "WARNING: nullptr passed to rotateLeft" << std::endl;

        //std::cout << "rotateLeft(" << t->element << ")" << std::endl;

        // t = k2, u = k1, 
        BinaryNode *u = t->left;
        t->left = u->right;
        u->right = t;
        t->height = std::max(height(t->left), height(t->right)) + 1;
        u->height = std::max(height(u->left), height(u->right)) + 1;
        t = u;
        //std::cout << "rotateLeft finished" << std::endl;
    }

    // single rotation that reduces right branch depth
    void rotateRight(BinaryNode*& t)
    {
        //if (t == nullptr)
            //std::cout << "WARNING: nullptr passed to rotateRight" << std::endl;
        BinaryNode *u = t->right;
        t->right = u->left;
        u->left = t;
        t->height = std::max(height(t->left), height(t->right)) + 1;
        u->height = std::max(height(u->left), height(u->right)) + 1;
        t = u;
    }

    // double rotation that reduces left branch depth
    void doubleRotateLeft(BinaryNode*& t)
    {
        //if (t == nullptr)
            //std::cout << "WARNING: nullptr passed to doubleRotateLeft" << std::endl;

        rotateRight(t->left);
        rotateLeft(t);
    }

    // double rotation that reduces right branch depth
    void doubleRotateRight(BinaryNode*& t)
    {
        //if (t == nullptr)
            //std::cout << "WARNING: nullptr passed to doubleRotateRight" << std::endl;
        rotateLeft(t->right);
        rotateRight(t);
    }

    // inserts x to the tree rooted at t (copy)
    // if x exists, the do nothing
    void insert(const ComparableType& x, BinaryNode*& t)
    {
        /*if (t != nullptr)
            std::cout << "insert(" << x << ", " << t->element << ")" << std::endl;
        else
            std::cout << "insert(" << x << ", " << "nullptr)" << std::endl;*/

        if (t == nullptr) {
            t = new BinaryNode {x, 0, nullptr, nullptr}; // set height to 0 for now, will be updated when balance is called or when the output bugs out lol
           // std::cout << "node created" << std::endl;
        }
        else if (x < t->element) {
            //std::cout << "left" << std::endl;
            insert(x, t->left);
        }
        else if (x > t->element) {
            //std::cout << "right" << std::endl;
            insert(x, t->right);
        }

        balance(t);
    }

    // inserts x to the tree rooted at t (move)
    void insert(ComparableType && x, BinaryNode*& t)
    {
        if (t == nullptr)
            t = new BinaryNode {std::move(x), 0, nullptr, nullptr}; // set height to 0 for now, will be updated when balance is called or when the output bugs out lol
        else if (x < t->element)
            insert(std::move(x), t->left);
        else if (x > t->element)
            insert(std::move(x), t->right);

        balance(t);
    }

    // removes x from the tree rooted at t
    // when deleting a node with two children, replace it with the smallest child of the right subtree
    void remove(const ComparableType& x, BinaryNode*& t)
    {
        if (t == nullptr)
            return; // Item not found, do nothing

        if (x < t->element)
            remove(x, t->left);
        else if (x > t->element)
            remove(x, t->right);
        else if (t->left != nullptr && t->right != nullptr) { //two child case
            t->element = (findMin(t->right))->element;
            remove(t->element, t->right); // go to "replacement" node and clear
        } else { // 0 and 1 child case
            BinaryNode *oldNode = t;
            t = (t->left != nullptr) ? t->left : t->right;
            delete oldNode;
        }

        balance(t);
    }

    // clones the node t and returns the clone
    BinaryNode* clone(BinaryNode* t) const
    {
        if( t == nullptr ) return nullptr;
        else
            return new BinaryNode{t->element, clone(t->left), clone(t->right)};
    }

    // prints all data elements rooted at t pre-order
    void printPreOrder(BinaryNode* t, std::ostream& out) const
    {
        const char delim = ' ';
        if(t != nullptr)
        {
            out << t->element<< delim;
            printPreOrder(t->left, out);
            printPreOrder(t->right, out);
        }
        return;
    }

    // prints all data elements rooted at t in-order
    void printInOrder(BinaryNode* t, std::ostream& out) const    
    {
        const char delim = ' ';
        if(t != nullptr)
        {
            printInOrder(t->left, out);
            out << t->element<< delim;
            printInOrder(t->right, out);
        }
        return;
    }

    // prints all data elements rooted at t post-order
    void printPostOrder(BinaryNode* t, std::ostream& out) const
    {
        const char delim = ' ';
        if(t != nullptr)
        {
            printPostOrder(t->left, out);
            printPostOrder(t->right, out);
            out << t->element << delim;
        }
        return;
    }

    // prints the subtree rooted at t
    // lv is the level of t (the root corresponds to level 0)
    // p is the relative topological position of t (left/right/root) 
    void printTree(BinaryNode* t, const int lv, const NodePosType p)
    {
        if(t == nullptr)
            return;

        char pos;
        switch(p)
        {
            case Left:
                pos = 'L';
                break;
            case Right:
                pos = 'R';
                break;
            case Root:
                pos = 'T';
                break;
            default:
                std::cout << "Error: MyBST::printTree: unrecognized position type." << std::endl;
        }        
        std::cout << t->element << "|" << lv << "|" << pos << std::endl;
        printTree(t->left, lv + 1, Left);
        printTree(t->right, lv + 1, Right);
        return;
    }

  public:

    // default constructor
    MyBST() : 
        root{nullptr},
        theSize{0}
    { ; }

    // copy constructor
    MyBST(const MyBST& rhs) : 
        root{nullptr},
        theSize{rhs.theSize}
    { root = clone(rhs.root); }

    // move constructor
    MyBST(MyBST && rhs) : 
        root{rhs.root},
        theSize{rhs.theSize}
    { rhs.root = nullptr; }
      
    // destructor
    ~MyBST() { clear(); }

    // finds the minimum data element in the tree
    const ComparableType& findMin() const
    { findMin(root); }

    // finds the maximum data element in the tree
    const ComparableType& findMax( ) const
    { findMax(root); }

    // checks whether x is contained in the tree
    bool contains(const ComparableType& x) const
    { return contains(x, root); }
    
    // returns the number of data elements in the tree
    size_t size(void) const
    { return theSize; }

    // returns the depth of the tree
    // depth defined as the longest path length from the root to any leaf
    // e.g. an empty tree has a depth of 0, a tree with a single node has a depth of 1
    size_t depth(void) const
    { return height(root); }

    // checks whether the tree is empty
    bool empty() const
    { return theSize == 0; }

    // delete all data elements in the tree
    void clear()
    { clear(root); }

    // insert x into the tree (copy)
    void insert(const ComparableType& x)
    { 
        if (!contains(x))
            ++theSize;
        insert(x, root);   
    }

    // insert x into the tree (move)
    void insert(ComparableType && x)
    {
        if (!contains(x))
            ++theSize;
        insert(std::move(x), root);
        
    }

    // removes x from the tree
    void remove(const ComparableType& x)
    { 
        if (contains(x))
            --theSize;
        remove(x, root);
    }

    // copy assignment
    MyBST& operator=(const MyBST& rhs)
    {
        MyBST copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    // move assignment
    MyBST& operator=(MyBST && rhs)
    {
        std::swap(theSize, rhs.theSize);
        std::swap(root, rhs.root);
        return *this;
    }

    // finds the lowest common ancestor (LCA) of x and y
    // if x and y are both in the tree, return true and store their LCA in lca
    // otherwise, return false
    bool lowestCommonAncestor(const ComparableType& x, const ComparableType& y, ComparableType& lca)
    {
        if (!contains(x) || !contains(y))
            return false;

        BinaryNode cur = *root;
        lca = root->element;

        while (cur.right != nullptr || cur.left != nullptr) {
            if (cur.element == x || cur.element == y) {
                lca = cur.element;
                return true;
            }

            bool x_in_lst = contains(x, cur.left);
            bool y_in_lst = contains(y, cur.left);

            // its already been established the current element is not x or y
            if (x_in_lst && y_in_lst) {
                cur = *(cur.left);
                lca =  cur.element; 
            } else if (!x_in_lst && !y_in_lst) {
                cur = *(cur.right);
                lca = cur.element;
            } else { // x and y in different subtrees
                lca = cur.element;
                return true;
            }
                
        }

        return true;

    }

    // print all data elements in the tree
    void print(TreeTravOrderType order, std::ostream& out = std::cout) const
    {
        switch (order)
        {
            case PreOrder:
                printPreOrder(root, out);
                out << std::endl;
                break;
            case InOrder:
                printInOrder(root, out);
                out << std::endl;
                break;
            case PostOrder:
                printPostOrder(root, out);
                out << std::endl;
                break;
            default:
                out << "Error: MyBST::print: Unsupported print order." << std::endl;
                break;
        }
        return;
    }

    // print all data elements in the tree
    // including partial topological information (layer and relative position among siblings)
    void printTree(void)
    {
        printTree(root, 0, Root);
        return;
    }
      
};

#endif // __MYBST_H__