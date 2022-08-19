#ifndef NODE_H
#define NODE_H

enum Type{
    Vertex,
    Edge,
    Face
};

class Node{

private:
    Type type;
    int leftChild, rightChild;
    int element;

public:
    Node(const int element, Type type);

    Type getType();
    int getLeftChild();
    int getRightChild();
    int getElement();

    void setElement(const int element);
    void setType(Type element);
    void setLeftChild(const int leftChild);
    void setRightChild(const int rightChild);
};

#endif // NODE_H
