#include <stdio.h>
#include <stdlib.h>
#define MAX_NAME_LEN 100


typedef struct record {

    char name [MAX_NAME_LEN];
    int price;
    int index;
    int height;
    int maxP;/*maximum price in the sub tree (including current node)*/
    struct record* left;
    struct record* right;
    struct record* parent;

} record;

typedef struct maxRecord/*contains the data that we want to return in the function Max_Price*/
{
    int index;
    int price;
    char* name;

} maxRecord;

typedef struct node
{
    struct node* next;
    record* rec;

} node;

typedef struct DataStructure {

    int i; /*index to save the last "mespar sidori" in the shop*/
    record* avl; /* avl tree that is ordered by the index ("mespar sidori") */
    record* maxPrice; /*a pointer to save the maximum price of a record-maxPrice()*/
    node* month_list; /* a list to save the indexes between i and j (Ex 2) and their sub-trees */

} DataStructure;

/* ==============HELPER FUNCTIONS=============== */
void copyString(char dest[], const char src[]); /*a function that copies the string -src- to -dst-*/
int max (int a, int b); /*return the maximum value of a and b*/
int absValue (int a, int b);/*function that returns the difference between a and b in the absolute value */
int getHeight (record* N);/*function that returns the height of a node in the tree*/
void printLevel(record* root, int level);/*a function that prints the records in the level*/
void printLevelOrder(record* root);/*a function that prints all the values of the records in the tree*/
void printNamesInOrder (record* root);/*a function that prints the names of the records in the AVL trees in order*/
void print_InOrder (record* root);/*a recursive function that prints the tree values in order*/
node* addToList (node* list, record* N);/*a function that takes a list and a record and adds the record to the list*/
node* addSubTrees (node* list, record* root, int key, int i_or_j );/*Recursively traverses the search path to the given bound (i or j), collecting valid subtrees into the monthly hits list in O(log n) time.*/



/*===============AVL TREE ENGINE===============*/
record* createRecord(char name1 [], int price1, int index1); /*a function that creates a new record with the data it takes as parameters*/
record* left_Rotate (record* a);/*the left rotation function*/
record* right_Rotate (record* a);/*the right rotation function*/
record* balance(record* t);/*function that takes an AVL tree and balance it*/
record* InsertToAvl (record* t, record* N);/*a function that adds the record N to the tree t(t represents an AVL and N represent the Node to insert)*/
record* DeleteFromAvl (record* root, int key);/*deletes key from the AVL tree*/
record* BalanceAfterEditing (record* root, int key);/*function that goes on the search route(maslol 5ipos) and checks if we need to make any rotations to make sure that the tree remains balanced*/
record* Find_Min (record* root);/*returns a pointer to the minimum node*/
record* Find_V_Split (record *t, int key1, int key2);/*function that finds the v split in an AVL tree and returns a pointer on it*/
record* FindMaxPrice (record* root);/*returns a pointer on the node with the maximum price*/

/*===============CORE RECORD STORE LOGIC===============*/
DataStructure* Init(); /*** Initializes an empty record store in O(1) worst-case time. ***/
record* Insert (DataStructure* shop, char* name, int price); /*** Inserts a new record, assigns it the next available sequence number, and maintains tree balance in O(log n) time. ***/
record* Delete (DataStructure* shop, int key); /***Removes the record matching the given sequence number from the tree in O(log n) time.***/
maxRecord* Max_Price(DataStructure* shop);/***Returns the sequence number, price, and name of the most expensive record in the store in O(1) time. ***/


/*===============MONTH HITS LOGIC===============*/
void Pick_Month_Hits (DataStructure* shop, int i, int j); /***Sets the monthly hits series to encompass records with sequence numbers between i and j in O(log n) time. ***/
void Show_Month_Hits(node* list, record* V_Split); /*** Prints the names of the currently selected monthly hits in O(k) time, where k is the number of records printed. ***/
void End_Month (DataStructure* shop); /*** Clears the monthly hits selection in O(log n) time, re-enabling standard insert and delete operations.***/

/*===============MEMORY MANAGEMENT===============*/
void freePostOrder (record* root);/*a function that frees all the records in a tree*/
void freeDs(DataStructure* shop);/*a function that frees the memory we allocated*/


void copyString(char dest[], const char src[]) {/*a function that copies the string -src(source)- to -dst(destination)-*/
    int i;
    for (i = 0; src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

record* createRecord(char name1 [], int price1, int index1) {/*a function that creates a new record with the data it takes as parameters and returns it*/

    record* newNode = (record*)malloc(sizeof(record));/*the allocation in the memory*/
    newNode->height=0;/*we initialize the height 0 and then update it in another functions */
    newNode->maxP=price1;/*we initialize the max price to the price that we got and then update it (if needed)*/
    newNode->price = price1;
    copyString(newNode->name,name1);/*copying the name that the function got as a parameter to the value in the record that we are creating*/
    newNode->index = index1;
    newNode->left = NULL;/*initialize the left node to NULL (update it later in the insertion)*/
    newNode->right = NULL;/*initialize the right node to NULL (update it later in the insertion)*/
    newNode->parent = NULL;/*initialize the parent to NULL (update it later in the insertion)*/

    return newNode;
}

DataStructure* Init() {/**exercise 1 -A**/ /*we initialize our data structure */

    DataStructure* ds = (DataStructure*)malloc(sizeof(DataStructure));/*memory allocation*/
    record* m = (record*)malloc(sizeof(record));/*memory allocation*/
    ds->i=0;/*we start the index to save the last "mespar sidori" in the shop 0 (because our data structure is currently empty)*/
    ds->avl = NULL;/*the AVL tree is currently empty*/
    ds->month_list = NULL;/*the month list is currently empty(ex2)*/
    m->price=0;/*initialize the price of the max record to be 0*/
    m->index=0;/*initialize the index of the max record to be 0*/
    copyString(m->name,"");/*initialize the name of the max record to be ""*/
    m->right=NULL;/*initialize the left child of the max record to be null*/
    m->left=NULL;/*initialize the right child of the max record to be null*/
    m->maxP=0;/*we initialize the maximum price to be 0 and update it later when we start inserting*/
    ds->maxPrice=m;/*update the pointer to save the maximum price of a record-maxPrice()*/
    return ds;/*return our data structure*/
}

int max (int a, int b){ /*returns the maximum value between a and b*/
  if (a>b) return a;
  return b;
}

int absValue (int a, int b){ /*returns the absolute value of (a-b)*/

 if(a>=b) return (a-b);
 return (b-a);

}

int getHeight (record* N){ /*returns the height of the record "N" */
  if(N==NULL) return -1;
  return N->height;
}

record* left_Rotate (record* a){/*the left rotation function*/

   record* left_tree_of_right_child;/*a pointer to the left tree of the right child*/
   record* right_child;/*a pointer to the right child*/

    if(a==NULL)/*first we check if the AVL tree is NULL (the tree is empty)*/
        return a;/*we return the tree because no rotations should be done*/

   right_child = a->right;/*a pointer to the right child*/
   if((a->right)==NULL) return a;/*we check if the right child is null (in that case no rotations can be done) so we return a(the AVL tree)*/

   left_tree_of_right_child = right_child->left;/*update the pointer to point at the left tree to the right child*/
   right_child->left = a;/*the rotation process */
   a->parent = right_child;
   a->right = left_tree_of_right_child;
   if (left_tree_of_right_child!=NULL)
      left_tree_of_right_child->parent = a;

    /*updating (maxP):*/
    a->maxP = max( a->price , max(  (a->right==NULL ? 0 : a->right->maxP ) ,  (a->left==NULL ? 0 : a->left->maxP )  ) );
    right_child->maxP = max ( right_child->price , max( a->maxP,  (right_child->right == NULL ? 0 : right_child->right->maxP)   ) );

   /* updating the heights of a and right_child */
   a->height = ( getHeight(a->right) > getHeight(a->left) ? getHeight(a->right)+1 : getHeight(a->left)+1 );
   right_child->height = ( getHeight(right_child->right) > getHeight(right_child->left) ? getHeight(right_child->right)+1 : getHeight(right_child->left)+1 );
   right_child->parent = NULL;

   return right_child;
}

record* right_Rotate (record* a){/*the right rotation function*/

   record* right_tree_of_left_child;/*a pointer to the right tree of the left child*/
   record* left_child;/*a pointer to the left child*/

    if(a==NULL)/*first we check if the AVL tree is NULL (the tree is empty)*/
        return a;/*we return the tree because no rotations should be done*/

   if((left_child = a->left)==NULL) return a;/*we check if the lefft child is null (in that case no rotations can be done) so we return a(the AVL tree)*/

   right_tree_of_left_child = left_child->right;/*update the pointer to actually point to the right tree of left child*/
   left_child->right = a;/*the rotation process*/
   a->parent = left_child;
   a->left = right_tree_of_left_child;
   if (right_tree_of_left_child!=NULL)
      right_tree_of_left_child->parent = a;


    /*updating (maxP):*/
    a->maxP = max( a->price , max(  (a->right==NULL ? 0 : a->right->maxP ) ,  (a->left==NULL ? 0 : a->left->maxP )  ) );
    left_child->maxP = max ( left_child->price , max( a->maxP,  (left_child->left == NULL ? 0 : left_child->left->maxP)   ) );


   /* updating the heights of a and right_child */
   a->height = ( getHeight(a->right) > getHeight(a->left) ? getHeight(a->right)+1 : getHeight(a->left)+1 );
   left_child->height = ( getHeight(left_child->right) > getHeight(left_child->left) ? getHeight(left_child->right)+1 : getHeight(left_child->left)+1 );
   left_child->parent = NULL;

   return left_child;
}

void print_InOrder (record* root){/*a recursive function that prints the tree values in order*/

  if ((root!=NULL)){
    print_InOrder(root->left);
    printf("(%d/%d/%d/%d) , ",root->index,root->price,root->maxP,root->height);/*prints the values of the index, price, max price in the sub tree and the height of the record in the AVL tree*/
    print_InOrder(root->right);

  }

}

record* FindMaxPrice (record* root){/*a recursive function that returns a pointer on the node with the maximum price*/

    if(root==NULL) return NULL;/*if the root is null then the tree is empty so we return null*/
    if(root->price==root->maxP) return root;/*the value of maxP in the root is the maximum price in
                                            all the tree so if we find that value we return a pointer to its record*/
    if(root->right!=NULL){/*we check if the right record is not NULL because if it was then we can't go to the values in it.*/
        if(root->maxP==root->right->maxP)/*we are looking for the record that its maxP value equals to the maxP value to the root*/
            return FindMaxPrice(root->right);
    }
    return FindMaxPrice(root->left);
}

record* balance(record* t)/*function that takes an AVL tree and balance it*/
{
 record* heavy;
 if (t==NULL) return t;

 if(absValue(getHeight(t->right) ,getHeight(t->left))<=1) return t;/*if the difference between the heights is less <=1 then
                                                                   no balancing should be done and the tree is balanced so we return the tree*/

 if(getHeight(t->left) > getHeight(t->right)){/*if the height of the left subtree is bigger than the height of the right tree then the tree is left heavy*/

    heavy = t->left;/*the tree is left heavy*/
    if ( getHeight(heavy->left) < getHeight(heavy->right))/*to check if we should make right or left rotation*/
        {
           t->left = left_Rotate(heavy);/*left rotation*/
           t->left->parent = t;/*updating the parent*/
        }
    return right_Rotate(t);/*right rotation*/

 }

 else{

    heavy = t->right;/*the tree is right heavy*/
    if( getHeight(heavy->right)< getHeight(heavy->left))/*to check if we should make right or left rotation*/
        {
            t->right = right_Rotate(heavy);/*right rotation*/
            t->right->parent = t;/*updating the parent*/
        }
    return left_Rotate(t);/*left rotation*/
 }

}

record* Find_V_Split (record *t, int key1, int key2){ /*returns the first node in the searching path which
                                                         key1 is smaller than it and key2 bigger*/

    if (t==NULL) return NULL;/*if the tree is empty return null because there is no records to be the V-split*/

    if (key1>t->index && key2>t->index)/*we check when the search route(maslol 5ipos) gets diverged and the node that
                                         the "maslol" gets diverged from is our V-split*/
                                         /*we check if the index in the tree is also bigger from key1 and key2 then we go right
                                         and continue with the searching the V-split)*/
        return Find_V_Split(t->right, key1, key2);

    if (key1<t->index && key2<t->index)/*if the index in the tree is also smaller from key1 and key2 then we go left
                                         and continue with the searching the V-split)*/
        return Find_V_Split(t->left, key1, key2);

    return t;/* return the V split. we will arrive to this line just when our two search routs need to be diverged  */

}

node* addToList (node* list, record* N)/*a function that takes a list and a record and adds the record to the list*/
{
    node* newNode;
    if (N==NULL) return list;
    newNode = (node*)malloc(sizeof(node));/*allocate memory*/
    newNode->next = NULL;/*put the value of the next to be NULL*/
    newNode->rec = N;/*put the values of the record that the function got as a parameter in out node that we allocated memory for*/

    newNode->next = list;/*we add the node in the bigging of the list so the adding will take O(1) time*/
    return newNode;/*return the list after insertion*/
}

node* addSubTrees (node* list, record* root, int key, int i_or_j )/**/
{

    if (root==NULL) return list;/*if the root is null then return the list(there is nothing to add)*/
    if(i_or_j == 1){/*if the control bit(i_or_j) is 1 then we are looking for the i*/

        if(key <= root->index){/*if the key is smaller than the index of the current root */
            list = addToList (list , root);/*then we add the root to our list (that contains all the values between i and j)*/
            if(key!=root->index) return (list = addSubTrees(list, root->left, key, i_or_j));/*if we did not find yet the key then we make a recursive call
                                                                                               to the left subtree to continue searching */
            else return list;/*if we found the key then return the list*/
        }
        else  return (list = addSubTrees(list, root->right, key, i_or_j));/*if our key is bigger than the index of the root then then the key will be in the right subtree*/
    }

    else{/*if the control bit(i_or_j) is 0 then we are looking for the i*/


        if(key >= root->index){/*if the key is smaller than the index in the current root*/
            list = addToList (list , root);/*then we add the root to our list (that contains all the values between i and j)*/
            if(key!=root->index) return(list = addSubTrees(list, root->right, key, i_or_j));/*if we did not find yet the key then we make a recursive call
                                                                                               to the left subtree to continue searching */
            else return list;/*if we found the key then return the list*/
        }
        else  return(list = addSubTrees(list, root->left, key, i_or_j));/*if our key is smaller than the index of the root then then the key will be in the right subtree*/
    }
}

void Pick_Month_Hits (DataStructure* shop, int i, int j) /*** Ex2-A Function ***/
{
    record* v_split;
    int temp;
    if (shop->avl==NULL) return;/*if our AVL tree is null then there is no records to check their values*/

    if(i>j){/*if i is bigger than j then change them and make i equals j and j equals i , because in our functions we need j to be bigger than i*/
        temp = i;
        i = j;
        j = temp;
    }

    v_split = Find_V_Split(shop->avl,i,j);/*we call the function that we mentioned earlier to update our variable v_split to be actually our v split in the tree */

    shop->month_list = addToList(shop->month_list , v_split);/*adding the indexes to our list*/
    if(v_split->left!=NULL) shop->month_list = addSubTrees(shop->month_list, v_split->left, i, 1);
    if(v_split->right!=NULL) shop->month_list = addSubTrees(shop->month_list, v_split->right, j, 0);

}

void printNamesInOrder (record* root)/*a function that prints the names of the records in the AVL trees in order*/
{

  if ((root!=NULL)){

    printNamesInOrder(root->left);
    printf("(%s)\n",root->name);
    printNamesInOrder(root->right);
    }
}

void Show_Month_Hits(node* list, record* V_Split) /*** Ex2-B Function ***/
{
    node* p;
    if ((p=list)==NULL || V_Split==NULL) return;/*if our list is empty or the v split is null then there is no records to show*/
    while(p->next!=NULL){

        printf("(%s)\n",  p->rec->name);/*we print the values in our list*/
        if(p->rec->index < V_Split->index)
            printNamesInOrder(p->rec->right);

        else if (p->rec->index!= V_Split-> index) printNamesInOrder(p->rec->left);
        p=p->next;
    }
    printf("(%s)\n", p->rec->name);

}

void End_Month (DataStructure* shop){ /*** Ex2-C Function ***/

    node* p = shop->month_list;
    node* q;
    while(p->next!=NULL){/*we free all the memory that we allocated earlier*/
        q=p->next;
        free(p);
        p=q;
    }
    shop->month_list=NULL;

}

record* InsertToAvl (record* t, record* N){ /* t represents an avl and N represent the Node to insert*/

  record* y = NULL;
  if(t==NULL) return N;/*if the tree is empty then just return the node that we are willing to add*/

  if(N->index <= t->index){/*if the index of the record that we want to add is smaller than the index of the current index in the tree  */
    y=InsertToAvl(t->left,N);/*then make a recursive call to the left subtree to add*/
    t->left=y;/*update the leftsubtree*/
    y->parent=t;/*update the parent*/
    t->height=max(getHeight(t->left),getHeight(t->right))+1;/*update the height*/
  }

  else{
    y=InsertToAvl(t->right,N);/*if the index of the record that we want to add is bigger than the index of the current index in the tree
                               then make a recursive call to the left subtree to add*/
    t->right=y;/*update the right subtree*/
    y->parent=t;/*update the parent*/
    t->height=max(getHeight(t->left),getHeight(t->right))+1;/*update the height*/
  }

  t->maxP = max( t->price , max(  (t->right==NULL ? 0 : t->right->maxP ) ,  (t->left==NULL ? 0 : t->left->maxP )  ) );/*update the value maxP*/
  t->height = ( getHeight(t->right) > getHeight(t->left) ? getHeight(t->right)+1 : getHeight(t->left)+1 );/*update the height*/

  return t;
}

record* BalanceAfterEditing (record* root, int key){/*function that goes on the search route(maslol 5ipos) and checks if we need to make any rotations to make sure that the tree remains balanced*/

   if(root==NULL) return NULL;/*if the root is null then the tree is empty , there is nothing to update*/

   if(key==root->index) return root;/*if we find the index then the updating is done so we return the root*/
   if(key<root->index){/*if we have not found it yet we check if we should search for it in the right or left subtree*/

    root->left = (BalanceAfterEditing(root->left , key));/*balancing process*/
    if(root->left!=NULL) root->left->parent = root;
    return balance(root);

   }

   root->right = (BalanceAfterEditing(root->right , key));/*balancing process*/
   if(root->right!=NULL) root->right->parent = root;
   return balance(root);
}

record* Insert (DataStructure* shop, char* name, int price){ /*** Ex1-B Function ***/

   record* newRec = createRecord(name,price,shop->i+1);/*we create a record with the values that the function got as a parameters*/
   shop->i ++;/*update the index(Sequence number (unique identifier))*/

   shop->avl = InsertToAvl(shop->avl , newRec);/*we call the function (InsertToAvl) that we described earlier to add the record to the AVL tree*/
   shop->avl = BalanceAfterEditing (shop->avl, newRec->index);/*make sure that the tree is balanced after adding the new record, if not the function balance it*/
   shop->maxPrice = FindMaxPrice(shop->avl);/*update the maxPrice*/
   return shop->avl;/*return the AVL tree after the adding process*/

}

maxRecord* Max_Price(DataStructure* shop)/*** Ex1-D Function ***/
{
    maxRecord* Result = (maxRecord*)malloc(sizeof(maxRecord));/*allocate memory to the result (the record with the max price)*/
    if(shop->maxPrice!=NULL)/*we check that the maxPrice is not NULL*/
    {
        Result->price=shop->maxPrice->price;/*update the price value of the result(the record that we allocated)*/
        Result->index=shop->maxPrice->index;/*update the price index of the result(the record that we allocated)*/
        Result->name=shop->maxPrice->name;/*update the price name of the result(the record that we allocated)*/
    }
    else
    {
        Result->price=-1;/*if it's null then we put -1 the price and the index*/
        Result->index=-1;
        Result->name="";
    }

    return Result;
}


record* Find_Min (record* root)/*returns a pointer to the minimum node*/
{

    record* ptr = root;
    while(ptr!=NULL && ptr->left!=NULL) ptr=ptr->left;/*the minimum node is the most left so we go left until we arrive to null and then return the node before the null*/
    return ptr;
}

record* DeleteFromAvl (record* root, int key)/*deletes key from the AVL tree*/
{
    record *temp,*temp2;
    if (root == NULL)/*if the root is null then the tree is empty and there is nothing to delete*/
        return root;/*so we return the root*/

    if (key < root->index)/*we search for the record that should be deleted- if the key of the record that should be deleted is smaller
                          than the index in the root then the key we are trying to delete must be in the left subtree*/
        root->left = DeleteFromAvl(root->left, key);

    else if (key > root->index)/*if the key of the record that should be deleted is bigger
                              than the index in the root then the key we are trying to delete must be in the right subtree*/
        root->right = DeleteFromAvl(root->right, key);

    else {/*we found the record that we want to delete*/

        if ((root->left == NULL) || (root->right == NULL)) { /*If the node has maximum one child*/
            temp = root->left ? root->left : root->right;

            if (temp == NULL) { /*If the node is a leaf*/
                temp = root;
                root = NULL;
            } else /*If it has only one child*/
                {
                    root->left = root->right = NULL;
                    temp->parent= root->parent;
                    if(root->parent!=NULL) {
                        if(root->parent->left == root) root->parent->left = temp;
                        else  root->parent->right = temp;
                        root->parent=NULL;
                    }

                    temp2=root;
                    root=temp;
                    temp=temp2;
                }

            free(temp);/*freeing the memory of the deleted node*/
        } else {
            temp = Find_Min(root->right);
            root->index = temp->index;
            root->price = temp->price;
            copyString(root->name,temp->name);

            root->right = DeleteFromAvl(root->right, temp->index );
            if(root->right!=NULL) root->right->parent = root;
        }
    }

    if (root == NULL) /*if we deleted the last record in the tree*/
        return NULL;


    /*updating (height) and (maxP)*/
    root->height = ( getHeight(root->right) > getHeight(root->left) ? getHeight(root->right)+1 : getHeight(root->left)+1 );
    root->maxP = max( root->price , max(  (root->right==NULL ? 0 : root->right->maxP ) ,  (root->left==NULL ? 0 : root->left->maxP )  ) );

    return root;
}

record* Delete (DataStructure* shop, int key){ /*** Ex1-C Function ***/

    if (shop->avl==NULL) return NULL;
    shop->avl = DeleteFromAvl(shop->avl,key);/*we call the function that we described earlier*/
    shop->avl = BalanceAfterEditing(shop->avl,key); /*although we already deleted it, we want to update it's search way up to the root*/
    shop->maxPrice = FindMaxPrice(shop->avl);/*update the maxPrice after deleting the key(maybe the key that we deleted had the maxPrice)*/

    return shop->avl;
}

void printLevel(record* root, int level) {/*a function that takes a pointer to a record tree and a level and prints all the records in that level*/

    if (root == NULL)/*there is nothing to print*/
        return;
    if (level == 1)
        printf("(%d/%d/%d/%d) , ",root->index,root->price,root->maxP,root->height);
    else if (level > 1) {
        printLevel(root->left, level - 1);
        printLevel(root->right, level - 1);
    }
}

void printLevelOrder(record* root) {/*a function that prints all the levels of the tree in order*/

    int h = getHeight(root);
    int i;
    for (i = 1; i <= h+1; i++) {
        printLevel(root, i);
        printf("\n=============================\n");
    }
}

void freePostOrder (record* root){/*a function that frees all the records in a tree*/

    if(root!=NULL){/*go on all the records in the tree and free the memory that we allocated for them earlier*/
        freePostOrder(root->left);
        freePostOrder(root->right);
        free(root);
    }
}

void freeDs(DataStructure* shop){/*a function that frees the memory we allocated*/

    node* p = shop->month_list, *q;
    while (p!=NULL){

        q=p->next;/*we add a pointer to save our next because after we free the memory we will not be able to go to the next unless we save a pointer before the freeing process*/
        free(p);
        p=q;
    }
    freePostOrder(shop->avl);
    free(shop);

}

int main() {

    int p1,p2,x=1;
    char str [MAX_NAME_LEN];

    DataStructure* s = Init();

    while (x!=0){

    printf("Menu:\nFor Deleting a record - Enter 0.\nFor Inserting a record - Enter 1.\nFor Results Display - Enter 2.\n");
    scanf("%d",&x);
    if (x==1){
        printf("enter price: \n");
        scanf("%d",&p1);
        printf("enter name: \n");
        scanf("%99s",str);
        s->avl = Insert(s,str,p1);
    }

    if(x==0){
        printf("What index to Delete? \n");
        scanf("%d",&x);
        s->avl = Delete(s,x);
    }

    if(x==2){
        printf("\n\n");
    printf("------------------------------------\n");
    printf("avl result: \n");
    print_InOrder(s->avl);
    printf("\n");
    printf("------------------------------------\n");
    printf("\n\n");
    }
    printf("enter 0 to stop, or 1 to continue: \n");
    scanf("%d",&x);
    }

    printf("\n\n");
    printf("------------------------------------\n");
    printf("avl result: \n");
    print_InOrder(s->avl);
    printf("\n");
    printf("------------------------------------\n");
    printf("\n\n");

    printf("\n\n");
    printf("------------------------------------\n");
    printf("avl result in levels: \n");
    printLevelOrder(s->avl);
    printf("\n");
    printf("------------------------------------\n");
    printf("\n\n");

    maxRecord* currentMax = Max_Price(s);
    printf("the maximum record :\n");
    printf("the index of the maximum record is %d\n", currentMax->index);
    printf("the price of the maximum record is %d\n", currentMax->price);
    printf("the name of the maximum record is %s\n", currentMax->name);
    free(currentMax); /* Free the allocated memory! */

    printf("\n");
    printf("------------------------------------\n");
    printf("\n\n");

    printf("enter i: \n");
    scanf("%d",&p1);
    printf("\n");
    printf("enter j: \n");
    scanf("%d",&p2);
    Pick_Month_Hits(s,p1,p2);
    printf("\n");
    printf("Names of Month Hits: \n");
    Show_Month_Hits(s->month_list, Find_V_Split(s->avl,p1,p2));
    printf("\n");
    End_Month(s);
    printf("\n");
    printLevelOrder(s->avl);
    printf("\n");
    freeDs(s);
    return 0;
}
