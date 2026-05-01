#include <stdio.h>
#include <stdlib.h>
#define MAX_NAME_LEN 100

typedef struct record {

    char name [MAX_NAME_LEN];
    int price;
    int index;
    int height;
    int maxP;      /*maximum price in the sub tree (including current node)*/
    int rank;
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

typedef struct Series/*contains the data of the series*/
{
    int num;/*Sequence number (unique identifier)*/
    record* maxPrice;
    record* avl;

} series;

typedef struct DataStructure {

    int M; /*The length of the DynamicArray*/
    int n; /*The number of full cells in the DynamicArray (number of serieses) */
    int last_num; /*an index to save the last Sequence number (unique identifier) of a series*/
    int last_i; /*an index to save the last Sequence number (unique identifier) of a series*/
    series* DArray; /*a pointer to the dynamic array that saves the serieses*/

} DataStructure;


/* ===============HELPER FUNCTIONS===============*/
void copyString(char dest[], const char src[]);
int max (int a, int b); /*return the maximum value of a and b*/
int absValue (int a, int b);/*function that returns the difference between a and b in the absolute value */
int getHeight (record* N); /*returns the height of the record "N" */
int getRank (record* N);/*returns the rank of the node - in other words the the size of the subtree of the node*/
void print_InOrder (record* root);/*a recursive function that prints the tree values in order*/
void printDArray (DataStructure* shop);/*print the values in the dynamical array*/
series* DoubleIfNeeded (DataStructure* shop);/*a function that doubles the dynamic array size( if needed)*/


/*===============AVL TREE ENGINE===============*/
record* createRecord(char name1 [], int price1, int index1);/*a function that creates a new record with the data it takes as parameters and returns it*/
record* left_Rotate (record* a);/*left rotation function*/
record* right_Rotate (record* a);/*right rotation function*/
record* balance(record* t);/*function that takes an AVL tree and balance it*/
record* InsertToAvl (record* t, record* N);/*a function that adds the record N to the tree t(t represents an AVL and N represent the Node to insert)*/
record* DeleteFromAvl (record* root, int key);/*deletes key from the AVL tree*/
record* BalanceAfterEditing (record* root, int key);/*function that goes on the search route(maslol 5ipos) and checks if we need to make any rotations to make sure that the tree remains balanced*/
record* Find_Min (record* root);/*returns a pointer to the minimum node*/
record* FindMaxPrice (record* root);/* a function that returns a pointer to the record in the avl tree with the max price*/
record* Jth_in_order (record* root, int j);/*a function that returns a pointer to the Jth node in order in the tree*/

/*===============ADVANCED AVL SPLIT/CONCATENATE LOGIC===============*/
void Split(record* root, record** x, record** smaller, record** bigger, int Jth_key);/*a function th*/
record* concatenate(record* T1,record* T2,record* x);/*T1 is smaller and T2 is bigger*/
record* Concatenate1(record* T1,record* T2,record* x); /*T1's indexes are smaller than x's, and T2's indexes are bigger than x's*/
record* Concatenate2(record* T1,record* T2,record* x); /*T2's indexes are smaller than x's, and T1's indexes are bigger than x's*/

/*===============DYNAMIC SERIES STORE LOGIC===============*/
DataStructure* Init();/***Initializes an empty store containing a single series with ID 1 in O(1) time.***/
 record* Insert (DataStructure* shop, char* name, int price, int i);/***Inserts a record into the series with ID i, assigning a unique sequence number in O(log n) time.***/
record* Delete (DataStructure* shop, int number, int i);/***Removes the record matching the sequence number from series i in O(log n) time.***/
maxRecord* Max(DataStructure* shop, int i);/***Returns the details of the most expensive record in series i in O(1) time.***/
void split_series(int i, int j, DataStructure* shop);/***Splits series i into two separate series, dividing them at the j-th smallest record.***/


/*===============MEMORY MANAGEMENT===============*/
void freePostOrder (record* root);/*a function that takes a root to an AVL tree and frees the memory that the tree needed*/
void freeDs(DataStructure* shop);/*a function that frees the memory we allocated*/



DataStructure* Init() {

    DataStructure* ds = (DataStructure*)malloc(sizeof(DataStructure));
    record* m = (record*)malloc(sizeof(record));/*memory allocation*/
    series* a = (series*)malloc(sizeof(series));/*memory allocation*/
    a->avl=NULL;/*we initialize the AVL tree to be NULL*/
    a->num=1;/*Sequence number (unique identifier) to be 1*/
    ds->M=1;/*the array size=1*/
    ds->n=1;/*the current array index*/
    ds->last_i=0;
    ds->last_num=1;
    m->price=0;/*initialize the price to be 0*/
    m->index=0;/*initialize the index to be 0*/
    copyString(m->name,"");/*initialize the name to be ""(empty)*/
    m->right=NULL;/*initialize the right child to be NULL*/
    m->left=NULL;/*initialize the left child to be NULL*/
    m->maxP=0;/*initialize the price of the max record to be 0*/
    m->height=0;/*initialize the height to be 0*/
    a->maxPrice=m;
    ds->DArray = a;
    return ds;
}

 record* Insert (DataStructure* shop, char* name, int price, int i){/**EXE 3- B**/

   series* a = (shop->DArray+(i-1));/*find the tree in the array that we need to insert for it*/
   record* newRec = createRecord( name,price,(++shop->last_i) );/*create the record that we are willing to add*/
   a->avl = InsertToAvl(a->avl, newRec);/*insert the record using the function "InsertToAvl"*/
   a->avl = BalanceAfterEditing (a->avl,shop->last_i);/*make sure that the tree is balanced- if not balance it!*/
   a->maxPrice = FindMaxPrice(a->avl);/*update the maxPrice record*/
   return a->avl;
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
    newNode->rank= 1;/*initialize the rank to be 1*/

    return newNode;
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

   if(root==NULL) return NULL;/*the tree is empty- there is nothing to update*/

   if(key==root->index) return root;/*after finding the key that equals the index then the updating is done!- return the root*/
   if(key<root->index){/*if we have not found it yet we check if we should search for it in the right or left subtree*/

    root->left = (BalanceAfterEditing(root->left , key));/*balancing process*/
    if(root->left!=NULL) root->left->parent = root;
    return balance(root);
   }

   root->right = (BalanceAfterEditing(root->right , key));/*balancing process*/
   if(root->right!=NULL) root->right->parent = root;
   /*updating the values of the height, rank, maxP*/
   root->height = ( getHeight(root->right) > getHeight(root->left) ? getHeight(root->right)+1 : getHeight(root->left)+1 );
   root->rank = getRank(root->right)+getRank(root->left)+1;
   root->maxP = max( root->price , max(  (root->right==NULL ? 0 : root->right->maxP ) ,  (root->left==NULL ? 0 : root->left->maxP )  ) );
   return balance(root);
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
   /*updating the rank*/
   a->rank=getRank(a->right)+getRank(a->left)+1;
   right_child->rank=getRank(right_child->right)+getRank(right_child->left)+1;

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
   /*updating the rank and maxP*/
   a->rank=getRank(a->right)+getRank(a->left)+1;
   a->maxP = max( a->price , max(  (a->right==NULL ? 0 : a->right->maxP ) ,  (a->left==NULL ? 0 : a->left->maxP )  ) );
   left_child->rank=getRank(left_child->right)+getRank(left_child->left)+1;
   a->maxP = max( left_child->price , max(  (left_child->right==NULL ? 0 : left_child->right->maxP ) ,  (left_child->left==NULL ? 0 : left_child->left->maxP )  ) );

   return left_child;
}

record* FindMaxPrice (record* root){/* a function that returns a pointer to the record in the avl tree with the max price*/

    if(root==NULL) return NULL;/*if the tree is empty there is no record to return - return null*/
    if(root->price==root->maxP) return root;/*we found the record with the max price- return it!*/
    if(root->right!=NULL){
        if(root->maxP==root->right->maxP)
            return FindMaxPrice(root->right);/*searching process*/
    }
    return FindMaxPrice(root->left);/*searching process*/
}

record* Delete (DataStructure* shop, int number, int i){/**EXE 3- C**/

    series* a = (shop->DArray+(i-1));/*let a be at the right place( where the deletion will happen)*/
    if (a->avl==NULL) return NULL;/*if the tree is empty then there is nothing to delete- return null(the tree after the deletion)*/
    a->avl = DeleteFromAvl(a->avl,number);/*delete the number from the tree using the function we described earlier (DeleteFromAvl)*/
    a->avl = BalanceAfterEditing(a->avl,number); /*although we already deleted it, we want to update it's search path up to the root*/
    a->maxPrice = FindMaxPrice(a->avl);/*update the maxPrice value*/

    return a->avl;
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


    /*updating (height) and (maxP) and (rank)*/
    root->height = ( getHeight(root->right) > getHeight(root->left) ? getHeight(root->right)+1 : getHeight(root->left)+1 );
    root->maxP = max( root->price , max(  (root->right==NULL ? 0 : root->right->maxP ) ,  (root->left==NULL ? 0 : root->left->maxP )  ) );
    root->rank = getRank(root->right)+getRank(root->left)+1;

    return root;
}

maxRecord* Max(DataStructure* shop, int i)/**EXE 3- D**/
{/*a function that returns a pointer to the record with the maximum price( the record has the values of the index , price and name*/
    maxRecord* Result = (maxRecord*)malloc(sizeof(maxRecord));/*memory allocation*/
    series* a = (shop->DArray+(i-1));
    /*updating the values*/
    Result->price=a->maxPrice->price;
    Result->index=a->maxPrice->index;
    Result->name=a->maxPrice->name;

    return Result;/*return the record*/
}


void copyString(char dest[], const char src[]){ /*a function that copies the string -src- to -dst-*/
    int i;
    for (i = 0; src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}


void split_series(int i, int j, DataStructure* shop)/**EXE 3-D**/
{
    record* T1=NULL, *T2=NULL, *x=NULL, *Jth, *N; /*we will put in T1 the indexes smaller than j, and in T2 the bigger than j, and x will contain the case of equal values*/
    /*so we use the function split exactly as we learned in the lecture*/
    series* a, *b;
    a=(shop->DArray+i);

    Jth=Jth_in_order(a->avl, j);

    Split(a->avl, &x,&T1 ,&T2, Jth->index);

    N= createRecord (x->name,x->price,x->index); /*insert a node that contains the j-th data to the tree of the indexes smaller than j (T1)*/
    T1=InsertToAvl(T1,N);
    T1=BalanceAfterEditing(T1,N->index);
    a->avl = T1;
    a->maxPrice = FindMaxPrice(a->avl);
    free(x);

    /*Inserting the new series to the array:*/

    DoubleIfNeeded(shop); /*doubles the size of the array (if needed - if its already full)*/
    b = (shop->DArray+shop->n);
    b->avl=T2;
    b->num = shop->last_num+1;/* Update the number of the series in the new array */
    shop->last_num++;
    b->maxPrice = FindMaxPrice((shop->DArray+shop->n)->avl); /*updating the max price in the new series*/
    (shop->n)++;
    T1=NULL;
    T2=NULL;
    x=NULL;
}

record* Jth_in_order (record* root, int j) {/*a function that gets a root of an avl tree and an index j, and returns a pointer to the record in place j*/
    int leftRank;
    if (root == NULL)  return NULL;


    leftRank= getRank(root->left);

    if (j <= leftRank) {/*if J is <= the size of the left subtree then Jth node in order must be in the left subtree*/
        return Jth_in_order(root->left, j);
    }

    if (j == leftRank+1 ) /*we found the Jth node*/
        return root;

    else
        return Jth_in_order(root->right, j-(leftRank + 1));/*we make a recursive call to continue searching for the Jth node in the right subtree*/


}

series* DoubleIfNeeded (DataStructure* shop)/*a function that doubles the dynamic array size( if needed)*/
{
    series* newArray;
    int i;
    if ((shop->M)==(shop->n))/*we need to double the dynamical array*/
    {
         newArray=(series*)malloc(2*shop->M*sizeof(series));/*memory allocation*/
         for(i=0;i<shop->M;i++)/*copying the data from the first array to the new doubled one*/
         {
             (newArray+i)->avl=(shop->DArray+i)->avl;
             (newArray+i)->maxPrice=(shop->DArray+i)->maxPrice;
             (newArray+i)->num=(shop->DArray+i)->num;
         }
         free(shop->DArray);/*free the memory of the first array*/
         shop->DArray=newArray;
         shop->M=2*shop->M;
         return (newArray);/*return the new array - the doubled size*/
    }

    return (shop->DArray);/*no need to double- just return the dynamical array without changing anything*/
}


record* concatenate(record* T1,record* T2,record* x)/*T1's indexes are smaller than x's, and T2's indexes are bigger than x's*/
{ /*implementing as the algorithm we learned in the lecture:*/

    if(getHeight(T1) == getHeight(T2))
    {
        x->left=T1;
        T1->parent=x;
        x->right=T2;
        T2->parent=x;
        return x;

    }

    /* in case getHeight(T1) > getHeight(T2), we need to find the node in the right spine of T1 which has the same height of T2,
    and in case getHeight(T2) > getHeight(T1), we need to find the node in the left spine of T2 which has the same height of T1,
    exactly as we learned in the lecture.*/
    if(getHeight(T1) > getHeight(T2)) return Concatenate1(T1,T2, x) ;
    return Concatenate2(T1,T2, x) ;
}

record* Concatenate1(record* T1,record* T2,record* x) /*T1's indexes are smaller than x's, and T2's indexes are bigger than x's*/
{ /* in case getHeight(T1) > getHeight(T2), we need to find the node in the right spine of T1 which has the same height of T2*/

    record* N=createRecord(x->name, x->price,x->index);/*creating the record with the values of x*/
    if(T1==NULL)/*if T1 is empty then add the record  N to the tree T2 and return it*/
    {
        T2=InsertToAvl(T2,N);
        free(x);
        return T2;
    }
    if(T2==NULL)/* if T2 is empty then add the record N to the tree T1 and return it*/
    {
        T1=InsertToAvl(T1,N);
        free(x);
        return T1;
    }
    /*the difference between the heights of T1 and T2 is 1 or 0 */
    if(getHeight(T2)-getHeight(T1)==0 || getHeight(T2)-getHeight(T1)==1)
    {
        x->left=T1;
        T1->parent=x;
        x->right=T2;
        T2->parent=x;

        x->maxP = max( x->price , max(  (x->right==NULL ? 0 : x->right->maxP ) ,  (x->left==NULL ? 0 : x->left->maxP )  ) );/*update the value maxP*/
        x->rank=getRank(x->left)+getRank(x->right)+1;
        x->height = ( getHeight(x->right) > getHeight(x->left) ? getHeight(x->right)+1 : getHeight(x->left)+1 );/*update the height*/
        return (x = balance(x));

    }
    T1->right=Concatenate1(T1->right,T2,x);
    if(T1->right!=NULL)
    T1->right->parent=T1;
    T1->maxP = max( T1->price , max(  (T1->right==NULL ? 0 : T1->right->maxP ) ,  (T1->left==NULL ? 0 : T1->left->maxP )  ) );/*update the value maxP*/
    x->rank=getRank(T1->left)+getRank(T1->right)+1;
    x->height= max(getHeight(T1->left),getHeight(T1->right))+1;/*update the height*/
    return (T1=balance(T1));
}

record* Concatenate2(record* T1,record* T2,record* x) /*T1's indexes are smaller than x's, and T2's indexes are bigger than x's*/
{ /*in case getHeight(T2) > getHeight(T1), we need to find the node in the left spine of T2 which has the same height of T1*/

    record* N=createRecord(x->name, x->price,x->index);


    if(T1==NULL)/*if T1 is empty then add the record  N to the tree T2 and return it*/
    {
        T2=InsertToAvl(T2,N);
        free(x);
        return T2;
    }
    if(T2==NULL)/* if T2 is empty then add the record N to the tree T1 and return it*/
    {
        T1=InsertToAvl(T1,N);
        free(x);
        return T1;
    }
    /*the difference between the heights of T1 and T2 is 1 or 0 */
    if(getHeight(T2)-getHeight(T1)==0 || getHeight(T2)-getHeight(T1)==1)
    {
        x->left=T1;
        T1->parent=x;
        x->right=T2;
        T2->parent=x;

        x->maxP = max( x->price , max(  (x->right==NULL ? 0 : x->right->maxP ) ,  (x->left==NULL ? 0 : x->left->maxP )  ) );/*update the value maxP*/
        x->rank=getRank(x->left)+getRank(x->right)+1;/*update the rank*/
        x->height = ( getHeight(x->right) > getHeight(x->left) ? getHeight(x->right)+1 : getHeight(x->left)+1 );/*update the height*/
        return (x = balance(x));
    }
    T2->left=Concatenate2(T1,T2->left,x);
    if(T2->left!=NULL)
    T2->left->parent=T2;
    T2->maxP = max( T2->price , max((T2->right==NULL ? 0 : T2->right->maxP ),(T2->left==NULL ? 0 : T2->left->maxP )));/*update the value maxP*/
    x->rank=getRank(T2->left)+getRank(T2->right)+1;/*update the rank*/
    x->height= max(getHeight(T2->left),getHeight(T2->right))+1;/*update the height*/
    return (T2 = balance(T2));
}

void Split(record* root, record** x, record** T1, record** T2, int Jth_key)/*a function that splits the parameter root into 2 trees*/
{
    record* save;
    if (root == NULL) return;/*the tree is empty, nothing to split*/

    if (root->index > Jth_key)
    {
        Split(root->left, x, T1, T2, Jth_key); /*go split in the left sub tree because all the indexes in the right one are bigger*/

        /*separate the root from the tree */
        root->left = NULL;
        root->parent = NULL;
        /*save a pointer (save) on its right subtree*/
        save = root->right;
        root->right = NULL;
        if(save!=NULL) save->parent = NULL;

        *T2 = concatenate(*T2, save, root); /*the indexes in the left subtree that are bigger than Jth_key, and all the right sub tree plus
        the root, represent all the indexes that are bigger than Jth_key in the whole tree, so concatenate them.*/
        return;
    }

    if (root->index < Jth_key)
    {

        Split(root->right, x, T1, T2, Jth_key); /*go split in the right sub tree because all the indexes in the left one are smaller*/

        /*separate the root from the tree*/
        root->right = NULL;
        root->parent = NULL;
        /*save a pointer (save) on its left subtree*/
        save = root->left;
        root->left = NULL;
        if(save!=NULL) save->parent = NULL;

        *T1 = concatenate(save, *T1, root); /*the indexes in the right subtree that are smaller than Jth_key, and all the left sub tree plus
        the root, represent all the indexes that are smaller than Jth_key in the whole tree, so concatenate them.*/
        return;
    }

    /*in case root->index == Jth_key*/
    *T1 = root->left;
    *T2 = root->right;

    /*separate the root from the tree*/
    root->left = NULL;
    root->right = NULL;
    root->parent = NULL;
    *x = root; /*it is it*/
    /*printf("end of split");*/ /*- for debugging*/
    return;
}


int getHeight (record* N){ /*returns the height of the record "N" */
  if(N==NULL) return -1;
  return N->height;
}

int getRank (record* N)/*returns the rank of the node - in other words the the size of the subtree of the node*/
{
    if (N==NULL) return (0);
    return N->rank;
}

int absValue (int a, int b){ /*returns the absolute value of (a-b)*/

 if(a>=b) return (a-b);
 return (b-a);

}

int max (int a, int b){ /*returns the maximum value between a and b*/
  if (a>b) return a;
  return b;
}


void print_InOrder (record* root){/*a recursive function that prints the tree values in order*/

  if ((root!=NULL)){

    print_InOrder(root->left);
    printf("(%d/%d/%d/%d) , ",root->index,root->price,root->maxP,root->height);/*prints the values of the index, price, max price in the sub tree and the height of the record in the AVL tree*/
    print_InOrder(root->right);

  }

}

record* Find_Min (record* root)/*returns a pointer to the minimum node*/
{

    record* ptr = root;
    while(ptr!=NULL && ptr->left!=NULL) ptr=ptr->left;/*the minimum node is the most left so we go left until we arrive to null and then return the node before the null*/
    return ptr;
}


void freePostOrder (record* root){/*a function that takes a root to an AVL tree and frees the memory that the tree needed*/

    if(root!=NULL){
        freePostOrder(root->left);
        freePostOrder(root->right);
        free(root);
    }
}

void freeDs(DataStructure* shop){/*a function that frees the memory we allocated*/

    int i;
    series* a;
    for (i=1;i<=shop->M;i++)/*free all the array */
    {
        a = (shop->DArray+(i-1));
        freePostOrder(a->avl);/*free the avl tree in the array*/
    }
    free(shop->DArray);/*free the actual array*/
    free(shop);

}


void printDArray (DataStructure* shop){/*print the values in the dynamical array*/
    int i=0;
    for(;i<shop->n;i++){
        printf("-------------------------------------\n");
        printf("series number: %d\n",(shop->DArray+i)->num);
        print_InOrder((shop->DArray+i)->avl);
        printf("\n");
        printf("the maximum record :\n");
        
        maxRecord* currentMax = Max(shop, i+1);
        printf("the index of the maximum record is %d\n", currentMax->index);
        printf("the price of the maximum record is %d\n", currentMax->price);
        printf("the name of the maximum record is %s\n", currentMax->name);
        free(currentMax); /* Free the allocated memory! */
    }

}

int main (){

    int x=1,p1,i;
    series* a;
    char str [MAX_NAME_LEN];
    DataStructure* s = Init();

    while (x!=0){

        printf("Menu:\nFor Deleting a record - Enter 0.\nFor Inserting a record - Enter 1.\nFor Splitting a series - Enter 2.\nFor Results Display - Enter 3.\n");
        scanf("%d",&x);
        if(x==1) {
              printf("enter price: \n");
              scanf("%d",&p1);
              printf("enter name: \n");
              scanf("%99s",str);
              printf("enter i: \n");
              scanf("%d",&i);
              a = (s->DArray+(i-1));
              a->avl = Insert(s,str,p1,i); }

        else if(x==0){

            printf("What index to Delete? \n");
            scanf("%d",&p1);
            printf("enter i: \n");
            scanf("%d",&i);
            a = (s->DArray+(i-1));
            a->avl = Delete(s,p1,i);
        }
        else if (x==3){
            printDArray(s);
        }
        else {
            printf("enter i: \n");
            scanf("%d",&i);
            a = (s->DArray+(i-1));
            printf("enter j: \n");
            scanf("%d",&p1);
            split_series(i-1,p1,s);
        }


     printf("enter 0 to stop, or 1 to continue: \n");
     scanf("%d",&x);
    }

    printf("\n\n");
    printf("------------------------------------\n");
    printf("shop result: \n");
    printDArray(s);

    freeDs(s);
    return 0;
}
