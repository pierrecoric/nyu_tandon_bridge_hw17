/*
INSTRUCTIONS
In this assignment, it is required that you fill out areas under comments labeled as "TODO" appropriately based on the accompanying directions. 
You are also required to follow any directions accompanying comments such as "NOTE".
You can add/modify code anywhere, with the exception of the provided "main" (which we will use for testing).
You can use the constants RED and BLACK, instead of the ints 0 and 1, when appropriate.
*/

#include <iostream>
#include <math.h> // for asserting height
#include <queue>
#include <cassert>

using namespace std;

#define RED 0
#define BLACK 1

template <class T>
class RBT;

// swapColor swaps the color from red to black and vice versa
int swapColor(int c) {
    return (c == 0) ? 1 : 0;
}

//RBT Node. Three pointers, data and color.
template <class T>
class RBTNode {
    RBTNode<T> *parent, *left, *right;
    T data;
    int color;

public:
    //Default constructor.
    RBTNode(T data)
        : data(data),
          color(RED),
          parent(nullptr),
          left(nullptr),
          right(nullptr) {}
    friend class RBT<T>;
    void prettyPrint(int indent) const;

    template <class T1>
    friend void swapColor(RBTNode<T1> *);

    template <class T1>
    friend int getColor(RBTNode<T1> *);

    int height() const;
};

//Recursively finds the height of the tree by finding the height of the heighest leaf.
template <class T>
int RBTNode<T>::height() const {
    int left_h = 0;
    if (left != nullptr) {
        left_h = left -> height();
    }
    int right_h = 0;
    if (right != nullptr) {
        right_h = right -> height();
    }
    return 1 + max(left_h, right_h);
}

//Recursively prints out the tree.
template <class T>
void RBTNode<T>::prettyPrint(int indent) const {
    if (right != nullptr) {
        right -> prettyPrint(indent + 1);
    }
    int margin = indent * 2;
    for (int i = 0; i < margin; ++i) {
        cout << '\t';
    }
    cout << "DATA: " << data << endl;
    for (int i = 0; i < margin; ++i) {
        cout << '\t';
    }
    cout << "COLOR: " << (color == RED ? "RED" : "BLACK") << endl;
    if (left != nullptr) {
        left -> prettyPrint(indent + 1);
    }
}

//Simple function to swap the colors of two nodes.
template <class T>
void swapColor(RBTNode<T> *node) {
    if (node != nullptr) {
        node -> color = swapColor(node -> color);
    }
}

//GetColor handles null nodes
template <class T>
int getColor(RBTNode<T> *node) {
    if (node != nullptr) {
        return node -> color;
    }
    return BLACK;
}

//RBT class.
template <class T>
class RBT {
    RBTNode<T> *root;
    //Rotation functions.
    void singleCCR(RBTNode<T> *&point);
    void doubleCR(RBTNode<T> *&point);
    void singleCR(RBTNode<T> *&point);
    void doubleCCR(RBTNode<T> *&point);

public:
    //Default constructor.
    RBT() : root(nullptr) {}

    void insert(const T &);
    void insert(const T &toInsert, RBTNode<T> *&point, RBTNode<T> *parent);
    //Calls the print funciton defined in node on the root.
    void prettyPrint() const { root -> prettyPrint(0); }
    //Calls the height function defined in node on the root.
    int height() const { return root -> height(); }
};

//Double Counter Clockwise Rotation, towards the left.
template <class T>
void RBT<T>::doubleCCR(RBTNode<T> *&point) {
    singleCR(point -> right);
    singleCCR(point);
}

//Double clocwise rotation, towards the right.
template <class T>
void RBT<T>::doubleCR(RBTNode<T> *&point) {
    singleCCR(point -> left);
    singleCR(point);
}

//Single clocwise rotation, towards the right
template <class T>
void RBT<T>::singleCR(RBTNode<T> *&point) {
    RBTNode<T> *grandparent = point;
    RBTNode<T> *parent = grandparent -> left;
    // TODO: ADD ROTATION CODE HERE
    //Nothing to rotate:
    if(parent == nullptr) {
        return;
    }

    grandparent -> left = parent -> right; 

    if(parent -> right != nullptr) {
        parent -> right -> parent = grandparent;
    }

    //Prommote the parent to the grandparent's position.
    parent -> parent = grandparent -> parent;
    grandparent -> parent = parent;
    parent -> right = grandparent;

    if(parent -> parent != nullptr) {
        if(parent -> parent -> left == grandparent) {
            parent -> parent -> left = parent;
        }
        else {
            parent -> parent -> right = parent;
        }
    }

    if(grandparent == root) {
        root = parent;
    }

    point = parent;
}

//Single counter clocwise rotation, towards the left
template <class T>
void RBT<T>::singleCCR(RBTNode<T> *&point) {
    RBTNode<T> *grandparent = point;
    RBTNode<T> *parent = grandparent -> right;
    // TODO: ADD ROTATION CODE HERE
    //Nothing to rotate:
    if(parent == nullptr) {
        return;
    }

    grandparent -> right = parent -> left; 
    if(parent -> left != nullptr) {
        parent -> left -> parent = grandparent;
    }

    //Prommote the parent to the grandparent's position.
    parent -> parent = grandparent -> parent;
    grandparent -> parent = parent;
    parent -> left = grandparent;

    if(parent -> parent != nullptr) {
        if(parent -> parent -> left == grandparent) {
            parent -> parent -> left = parent;
        }
        else {
            parent -> parent -> right = parent;
        }
    }

    if(grandparent == root) {
        root = parent;
    }

    
    point = parent;
}

//4 scenarios:
//1: point = root -> color black
//2: aunt of point = red -> recolor
//3: aunt of point = black in a triangle pattern -> rotate point's parent
//4: aunt of point = black in a line patter -> rotate point's parent

template <class T>
void RBT<T>::insert(const T &toInsert, RBTNode<T> *&point, RBTNode<T> *parent) {
    if (point == nullptr) {               // leaf location is found so insert node
        point = new RBTNode<T>(toInsert); // modifies the pointer itself since *point is passed by reference
        point -> parent = parent;

        RBTNode<T> *curr_node = point; // curr_node will be set appropriately when walking up the tree
        // TODO: ADD RBT RULES HERE
        //The node is RED by default.
        //Scenario 1: root is null -> so color it black.
        if(curr_node -> parent == nullptr) {
            point -> color = BLACK;
            root = point;
            return;
        }

        while(curr_node != root && getColor(curr_node -> parent) == RED) {
            RBTNode<T> *currParent =  curr_node -> parent;
            RBTNode<T> *grandparent = currParent -> parent;

            //Case in which the parent of curr_node is the left child of grandparent.
            if(currParent == grandparent -> left) {
                RBTNode<T>* aunt = grandparent -> right;
                //if aunt is red
                if(getColor(aunt) == RED) {
                    currParent -> color = BLACK;
                    aunt -> color = BLACK;
                    grandparent -> color = RED;
                    //Move the pointer.
                    curr_node = grandparent;
                }
                //Else aunt is black
                else {
                    //Triangle scenario
                    if(curr_node == currParent -> right) {
                        curr_node = currParent;
                        singleCCR(curr_node);
                        currParent = curr_node -> parent;
                        grandparent = currParent -> parent;
                    }
                    currParent -> color = BLACK;
                    grandparent -> color = RED;
                    singleCR(grandparent);
                }

            }
            //Case in which the parent of curr_node is the right child of grandparent.
            else {
                RBTNode<T>* aunt = grandparent -> left;
                //if aunt is red
                //Same logic but with the other node.
                if(getColor(aunt) == RED) {
                    currParent -> color = BLACK;
                    aunt -> color = BLACK;
                    grandparent -> color = RED;
                    //Move the pointer.
                    curr_node = grandparent;
                }
                else {
                    //Triangle scenario.
                    if(curr_node == currParent -> left) {
                        curr_node = currParent;
                        singleCR(curr_node);
                        currParent = curr_node -> parent;
                        grandparent = currParent -> parent;
                    }
                    currParent -> color = BLACK;
                    grandparent -> color = RED;
                    singleCCR(grandparent);
                }
            }
        }
        //Just to be sure.
        root -> color = BLACK;


    } else if (toInsert < point -> data) { // recurse down the tree to left to find correct leaf location
        insert(toInsert, point -> left, point);
    } else { // recurse down the tree to right to find correct leaf location
        insert(toInsert, point -> right, point);
    }
}


template <class T>
void RBT<T>::insert(const T &toInsert) {
    insert(toInsert, root, nullptr);
}

// NOTE: DO NOT MODIFY THE MAIN FUNCTION BELOW
int main() {
    RBT<int> b;
    int count = 10;
    for (int i = 0; i < count; i++) {
        b.insert(i);
    }
    b.prettyPrint();
    /* EXPECTED OUTPUT:
                                                                    Data: 9
                                                                    COLOR: RED
                                                    Data: 8
                                                    COLOR: BLACK
                                    Data: 7
                                    COLOR: RED
                                                    Data: 6
                                                    COLOR: BLACK
                    Data: 5
                    COLOR: BLACK
                                    Data: 4
                                    COLOR: BLACK
    Data: 3
    COLOR: BLACK
                                    Data: 2
                                    COLOR: BLACK
                    Data: 1
                    COLOR: BLACK
                                    Data: 0
                                    COLOR: BLACK
    */
    // TEST
    // the below tests the validity of the height of the RBT
    // if the assertion fails, then your tree does not properly self-balance
    int height = b.height();
    assert(height <= 2 * log2(count));
    cout<<endl;
    cout<<"---------------------"<<endl;
    cout<<endl;
    RBT<int> c;
    count = 5;
    for (int i = count; i > 0; i--) {
        c.insert(i);
    }
    c.prettyPrint();
    /* EXPECTED OUTPUT:
                    Data: 5
                    COLOR: BLACK
    Data: 4
    COLOR: BLACK
                                    Data: 3
                                    COLOR: RED
                    Data: 2
                    COLOR: BLACK
                                    Data: 1
                                    COLOR: RED
    */
    height = c.height();
    assert(height <= 2 * log2(count));
}