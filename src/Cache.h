#ifndef CACHE_H
#define CACHE_H

#include "main.h"

enum BalanceValue
{
	LH = -1,
	EH = 0,
	RH = 1
};
class Node
{
public:
	Elem* element;
	Node *left;
	Node *right;
	BalanceValue balance;
	Node(const int &addr, Data* cont)
	{
		element = new Elem(addr, cont, true);
		left = right = NULL;
		balance = EH;
	}
    
};
class AVL
{
private:
    int size;
	Node *root;
protected:
	Node *rotateRight(Node *&node);
	Node *rotateLeft(Node *&node);
	Node *leftBalance(Node *&node, bool &taller);
	Node *rightBalance(Node *&node, bool &taller);
    Node *removeLeftBalance(Node *&node, bool &shorter);
	Node *removeRightBalance(Node *&node, bool &shorter);
    
	//TODO Methods
	Node *insertRec(Node *&node, const int &addr, Data* cont, bool &taller);
    Node *removeRec(Node *&node, const int &addr, bool &shorter, bool &success);
    void inOrderRec(Node* node);
    void preOrderRec(Node* node);
    Elem* searchRec(Node* node, const int &addr);
public:
	AVL()
	{
        this->size = 0;
		this->root = NULL;
	}
	
	// TODO Methods
	void insert(const int &addr, Data* cont);
	void remove(const int &addr);
    void inOrder();
    void preOrder();
    int getSize(){
        return this->size;
    }
    Elem* search(const int &addr);
};


class Cache {
		AVL avlTree;
        queue<Elem*> arrLifetime; 
		int maxSize;
	public:
		Cache(int s) {
			this->maxSize = s;
		}
		~Cache() {
            while(!(this->arrLifetime.empty())){
                Elem* elemOut= this->arrLifetime.front();
                this->arrLifetime.pop();
                this->avlTree.remove(elemOut->addr);
            }
		}
		Data* read(int addr);
		Elem* put(int addr, Data* cont);
		Elem* write(int addr, Data* cont);
		void print();
		void preOrder();
		void inOrder();	
};


#endif