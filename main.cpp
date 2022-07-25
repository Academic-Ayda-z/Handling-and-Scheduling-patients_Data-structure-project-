#include <iostream>
#define MAX 1000
using namespace std;
class Node{
public:
    int key;
    Node* idsTree;
    Node *left;
    Node *right;
    int height;
};
int max(int a, int b);
int height(Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
int max(int a, int b)
{
    return (a > b)? a : b;
}
Node* newNode(int key,int id)
{
    Node* node = new Node();
    node->idsTree= new Node();

    node->idsTree->key=id;
    node->idsTree->left = NULL;
    node->idsTree->right = NULL;
    node->idsTree->height = 1; // new node is initially


    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return(node);
}
Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left),
                    height(y->right)) + 1;
    x->height = max(height(x->left),
                    height(x->right)) + 1;
    return x;
}
Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(height(x->left),
                    height(x->right)) + 1;
    y->height = max(height(y->left),
                    height(y->right)) + 1;

    // Return new root
    return y;
}
int getBalance(Node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}
Node* insert(Node* node, int key,int id) {
    if (node == NULL) {
        return newNode(key, id);
    }
    if (key < node->key)
        node->left = insert(node->left, key, id);
    else if (key > node->key)
        node->right = insert(node->right, key, id);
    else {
        //equal
        node->idsTree=insert(node->idsTree, id, id);
        return node;
        }
    node->height = 1 + max(height(node->left),
                           height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}
void preOrder(Node *root)
{
    if(root != NULL)
    {
        cout << root->key << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}
Node * minValueNode(Node* node)
{
    Node* current = node;
    while (current->left != NULL)
        current = current->left;

    return current;
}
Node* deleteNode(Node* root, int key,int x)
{
    if (root == NULL)
        return root;

    if ( key < root->key )
        root->left = deleteNode(root->left, key,x);

    else if( key > root->key )
        root->right = deleteNode(root->right, key,x);

    else {//equal
        if(root->idsTree && root->idsTree->height>1){
            root->idsTree=deleteNode(root->idsTree,x,x);
            return root;
        }
        else{
        if ((root->left == NULL) ||
            (root->right == NULL)) {
            Node *temp = root->left ?
                         root->left :
                         root->right;

            // No child case
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else // One child case
                *root = *temp;
            free(temp);
        } else {
            Node *temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right,
                                     temp->key, x);
        }
    }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left),
                           height(root->right));
    int balance = getBalance(root);
    if (balance > 1 &&
        getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 &&
        getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 &&
        getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 &&
        getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}
class info {
  //  protected:
        static int ID;
        int patientsX[MAX];
        int patientsID[MAX];
        int healthMeasure[MAX];
        Node* healthMeasureTree=NULL;
        Node* firstsTree=NULL;

    public:
    void add(int x,int y){
        patientsID[ID]=x;
        patientsX[x]=ID;
        healthMeasure[ID]=y;
        healthMeasureTree=insert(healthMeasureTree, y, x);//key
        firstsTree= insert(firstsTree, ID, x);//id
        ID++;
    }
    void serveFirst(){
        if(firstsTree == NULL)
            return;
        auto first= minValueNode(firstsTree);
        cout << patientsID[first->key] << " " << healthMeasure[first->key] << endl;
        healthMeasureTree=deleteNode(healthMeasureTree, healthMeasure[patientsX[first->idsTree->key]],first->idsTree->key);
        firstsTree=deleteNode(firstsTree, first->key,first->idsTree->key);
    }
    void serveSickest(){
        if(healthMeasureTree == NULL)
            return;
        auto sickest=minValueNode(healthMeasureTree);
        cout << sickest->idsTree->key << " " << healthMeasure[patientsX[sickest->idsTree->key]] << endl;
        firstsTree=deleteNode(firstsTree, patientsX[sickest->idsTree->key],sickest->idsTree->key);
        healthMeasureTree=deleteNode(healthMeasureTree, healthMeasure[patientsX[sickest->idsTree->key]],sickest->idsTree->key);

    }
    void update(int x,int y){
        int id=patientsX[x];
        healthMeasureTree=deleteNode(healthMeasureTree,healthMeasure[id],x);
        healthMeasureTree=insert(healthMeasureTree,y,x);
        firstsTree=deleteNode(firstsTree,id,x);
        firstsTree=insert(firstsTree,id,x);
        healthMeasure[id]=y;
    }
};
int info::ID = 0;

int main() {
    info a;
    int x,y,i=0;
    string command;
    cout<<"To exit the program enter Exit\n";
    cout<<"Please enter the inputs:"<<endl;
    while(1){
        cin>>command;
        if(command=="Exit")
            break;
        if(command=="Add")
        {
            cin>>x>>y;
            a.add(x ,y);
        }
        else if(command=="Serve"){
            cin>>command;
            if(command=="First")
                a.serveFirst();
            else if(command=="Sickest")
                a.serveSickest();
        }
        else if(command=="Update"){
            cin>>x>>y;
            a.update(x,y);
        }
    }
    return 0;
}