#include "main.h"

Node* AVL::rotateRight(Node* &node){
    Node* temp  = node->left;
    node->left = temp->right;
    temp->right = node;
    return temp;
}
Node* AVL::rotateLeft(Node* &node){
    Node* temp  = node->right;
    node->right = temp->left;
    temp->left = node;
    return temp;
}

Node* AVL::leftBalance(Node *&node, bool &taller){
    Node* leftTree = node->left;
    if(leftTree->balance == LH) {
        node = rotateRight(node);
        node->balance = EH;
        node->right->balance = EH;
        taller = false;
    }
    else {
        Node* rightTree = leftTree->right;
        if(rightTree->balance == LH){
            node->balance = RH;
            leftTree->balance = EH;
        }
        else if(rightTree->balance == EH){
            leftTree->balance = EH;
            node->balance = EH;
        }
        else{
            node->balance = EH;
            leftTree->balance = LH;
        }
        node->left = rotateLeft(leftTree);
        rightTree->balance = EH;
        node = rotateRight(node);
        taller = false;
    }
    return node;
}
Node* AVL::rightBalance(Node *&node, bool & taller){
    Node* rightTree = node->right;
    if(rightTree->balance == RH) {
        node = rotateLeft(node);
        node->balance = EH;
        node->left->balance = EH;
        taller = false;
    }
    else {
        Node* leftTree = rightTree->left;
        if(leftTree->balance == RH){
            node->balance = LH;
            rightTree->balance = EH;
        }
        else if(leftTree->balance == EH){
            rightTree->balance = EH;
            node->balance = EH;
        }
        else {
            node->balance = EH;
            rightTree->balance = RH;
        }
        node->right = rotateRight(rightTree);
        leftTree->balance = EH;
        node = rotateLeft(node);
        taller = false;
    }
    return node;
}

Node* AVL::insertRec(Node *&node, const int &addr, Data* cont, bool &taller){
    
    if(node == NULL) {
        node = new Node(addr, cont);
        taller = true;
        return node;
    }
    if (addr < node->element->addr)
    {
        node->left = insertRec(node->left, addr, cont, taller);
        if (taller)
        {
            if (node->balance == LH){
                node = leftBalance(node, taller);
            }
            else if (node->balance == EH)
                node->balance = LH;
            else
            {
                node->balance = EH;
                taller = false;
            }
        }
        
    }
    else {
        node->right = insertRec(node->right, addr, cont, taller);
        if (taller)
        {
            if (node->balance == LH)
            {
                node->balance = EH;
                taller = false;
            }
            else if (node->balance == EH)
                node->balance = RH; 
            else
                node = rightBalance(node, taller);
        }
    }
    return node;
}
void AVL::insert(const int &addr, Data* cont){
    bool taller = false;
    this->root = this->insertRec(this->root, addr, cont, taller);
    this->size++;
}

Node* AVL::removeLeftBalance(Node *&node, bool &shorter){
    if(node->balance == RH)
        node->balance = EH;
    else if(node->balance == EH){
        node->balance = LH;
        shorter = false;
    }
    else {
        Node* leftTree = node->left;
        if(leftTree->balance == RH) {
            Node* rightTree = leftTree->right;
            if(rightTree->balance == RH){
                leftTree->balance =LH;
                node->balance = EH;
            }
            else if(rightTree->balance == EH){
                node->balance = RH;
                leftTree->balance = EH;
            }
            else {
                node->balance = RH;
                leftTree->balance = EH;
            }
            rightTree->balance = EH;
            node->left = rotateLeft(leftTree);
            node = rotateRight(node);
        }
        else {
            if(leftTree->balance != EH){
                node->balance = EH;
                leftTree ->balance = EH;
            }
            else {
                node->balance = LH;
                leftTree->balance = RH;
                shorter = false;
            }
            node = rotateRight(node);

        }
    }
    return node;
}
Node* AVL::removeRightBalance(Node *&node, bool &shorter){
    if(node->balance == LH)
        node->balance = EH;
    else if(node->balance == EH){
        node->balance = RH;
        shorter = false;
    }
    else {
        Node* rightTree = node->right;
        if(rightTree->balance == LH){
            Node* leftTree = rightTree->left;
            if(leftTree->balance == LH){
                rightTree->balance = RH;
                node->balance = EH;
            }
            else if(leftTree->balance == EH){
                node->balance = LH;
                rightTree->balance = EH;
            }
            else {
                node->balance = LH;
                rightTree->balance = EH;
            } 
            leftTree->balance = EH;
            node->right = rotateRight(rightTree);
            node = rotateLeft(node);
        }
        else {
            if(rightTree->balance != EH){
                node->balance = EH;
                rightTree->balance = EH;
            }
            else {
                node->balance = RH;
                rightTree->balance = LH;
                shorter = false;
            }
            node = rotateLeft(node);
        }
    }
    return node;
}

Node* AVL::removeRec(Node *&node, const int &addr, bool &shorter, bool &success){
    if(node == NULL){
        shorter = false;
        success = false;
        return NULL;
    }
    if(addr < node->element->addr){
        node->left = removeRec(node->left, addr, shorter, success);
        if(shorter)
            node = removeRightBalance(node, shorter);
    }
    else if(addr > node->element->addr) {
        node->right = removeRec(node->right , addr, shorter, success);
        if(shorter)
            node = removeLeftBalance(node, shorter);
    }
    else {
        Node* removeNode = node;
        if(removeNode->right == NULL){
            Node* temp = node->left;
            success = true;
            shorter = true;
            delete removeNode;
            return temp;
        }
        else if(removeNode->left == NULL){
            Node* temp = node->right;
            success = true;
            shorter = true;
            delete removeNode;
            return temp;
        }
        else {
            Node* exchNode = node->right;
            while(exchNode->left != NULL)
                exchNode = exchNode->left;
            node->element = exchNode->element;
            node->left = removeRec(node->left, exchNode->element->addr, shorter, success);
            if(shorter)
                node = removeLeftBalance(node, shorter);
        }
    }
    return node;
}
void AVL::remove(const int &addr){
    bool shorter = false;
    bool success = false;
    this->root = removeRec(this->root, addr, shorter, success);
    this->size--;
}

void AVL::preOrderRec(Node* node){
    if(node != NULL){
        node->element->print();
        preOrderRec(node->left);
        preOrderRec(node->right);
    }
}
void AVL::preOrder(){
    this->preOrderRec(this->root);
}
void AVL::inOrderRec(Node* node){
    if(node != NULL){
        inOrderRec(node->left);
        node->element->print();
        inOrderRec(node->right);
    }
}
void AVL::inOrder(){
    this->inOrderRec(this->root);
}

Elem* AVL::searchRec(Node* node, const int &addr){
    if(node == NULL) 
        return NULL; 
    if(node->element->addr > addr) 
        return this->searchRec(node->left, addr);
    else if(node->element->addr < addr) 
        return this->searchRec(node->right, addr);
    else return node->element;
}
Elem* AVL::search(const int &addr){ 
    return this->searchRec(this->root, addr);
}





Data* Cache::read(int addr) {
    Elem* tmp = this->avlTree.search(addr);
    if(tmp) return tmp->data;
    else return NULL;
}
Elem* Cache::put(int addr, Data* cont) {
    if(this->avlTree.getSize() < this->maxSize){
        this->avlTree.insert(addr, cont);
        Elem* elemIn = this->avlTree.search(addr);
        this->arrLifetime.push(elemIn);
    }
    else {
        Elem* elemOut= this->arrLifetime.front();
        this->arrLifetime.pop();
        this->avlTree.remove(elemOut->addr);
        this->avlTree.insert(addr, cont);
        Elem* elemIn = this->avlTree.search(addr);
        this->arrLifetime.push(elemIn);
        return elemOut;
    }
    return NULL;
}
Elem* Cache::write(int addr, Data* cont) {
    Elem* tmp = this->avlTree.search(addr);
    if(tmp){
        tmp->data = cont;
        tmp->sync = false;
    }
    else{
        if(this->avlTree.getSize() < this->maxSize){
            this->avlTree.insert(addr, cont);
            Elem* elemIn = this->avlTree.search(addr);
            elemIn->sync = false;
            this->arrLifetime.push(elemIn);
        }
        else {
            Elem* elemOut= this->arrLifetime.front();
            this->arrLifetime.pop();
            this->avlTree.remove(elemOut->addr);
            this->avlTree.insert(addr, cont);
            Elem* elemIn = this->avlTree.search(addr);
            elemIn->sync = false;
            this->arrLifetime.push(elemIn);
            return elemOut;
        }
    }
    return NULL;
}
void Cache::print() {
    int queueSize = this->arrLifetime.size();
    Elem** tmpArr = new Elem*[queueSize];
    int i = 0;
    while(!(this->arrLifetime.empty())){
        tmpArr[i] = this->arrLifetime.front();
        tmpArr[i]->print();
        this->arrLifetime.pop();
        i++;
    }
    for(i = 0; i < queueSize; i++)
        this->arrLifetime.push(tmpArr[i]);
    delete[] tmpArr;
}
void Cache::preOrder() {
	this->avlTree.preOrder();
}
void Cache::inOrder() {
    this->avlTree.inOrder();
}
