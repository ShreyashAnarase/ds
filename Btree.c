#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int degree;
struct node            //each B-tree node contains
{
	int n;        		 //no. of keys
	int* data;			// the array of keys
	bool isleaf;        //true for leaf nodes 
	struct node** child;   //array of pointers to its children (note that it is an array of pointers) if it was simply one pointer we've would hav used only one *

};                                                        //we use -> for pointer variables of struct. and . for simple variable
struct node* root = NULL;

int successor(struct node* x, int index);
int predecessor(struct node* x, int index);
void next_child_key(struct node* x,int index);
void delete (struct node* x,int k) ;
void combine(struct node* x,int index);
void fill(struct node* x,int index);
void prev_child_key (struct node* x,int index);

struct node* Create()                //creates a new Btree node 
{
	int dmax= 2*degree-1;
	struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->n=0;
    temp->isleaf=true;
    temp->data = (int*)malloc(dmax*sizeof(int));       //creates an array of integers to store the keys
    temp->child = (struct node**)malloc((dmax+1)*sizeof(int*));	//creates an array of pointers to children. since children are of (struct node) type

    for(int i=0; i<(dmax+1) ; i++)temp->child[i]=NULL;  //no. of children = no. of keys + 1

    return temp;
}

int search( struct node* root,int k)
{
	if(root==NULL)return -1;  //empty tree
			 
	
	struct node* current = root;

	int i=0;
	while(i<current->n && k> current->data[i])i=i+1;     // to find the key in the node if present else the key just greater than k
		

	if(i<current->n  && k==current->data[i])return current->data[i] ;    //if they key is found in this node

	if(current->isleaf)return 0;       // we've reached the leaf node but k still not found. therfore k not present

	else     //right now i is pointing to a key which is just greater than k
		return search(current->child[i],k);   // it recurses for the appropriate subtree. i.e. the subtree between the keys which contain k.

}

void Btree_split_child(struct node* x,int i)    // HERE the  i'th child of node x is going to be split.
{
	struct node* y = x->child[i];
	struct node* z = Create(degree);

	z->isleaf= y->isleaf;
	z->n = (degree -1);    //no of keys in z     
						   // we split a full node y  (2*degree-1 keys) into two nodes y,z of ((degree -1) keys ) each and move the middle element to the parent node x

	for(int j=0 ; j<(degree-1) ; j++)z->data[j] = y->data[j + degree];
		//above for loop splits the keys between y and z

		// below for loop splits children between y and z
		if(y->isleaf==false)  //if y is not leaf then we need to split children between the two new nodes y and z
		{
			for(int k=0 ; k< degree; k++) z->child[k] = y->child[k +degree];
		}

		y->n = (degree -1);   // reducing the size of y. freeing the other elements


	for(int j = x->n; j>=(i+1) ; j--)  //repointing the child pointers (from i+2 upto degree+1) in the parent node x 
	{
		x->child[j+1]= x->child[j];
	}

	x->child[i+1]= z;

	for(int j= (x->n -1) ; j>=i ; j--)
	{
		x->data[j+1] = x->data[j];
	}

	x->data[i] = y->data[degree-1];

	x->n = x->n +1;

}

void Btree_insert_nonfull (struct node* x, int k)  //K= KEY TO BE INSERTED
{

	int i = x->n -1;   //set i = index of last element in the data array
 
	if(x->isleaf)    //if x is a leaf node we will be inserting k in this node
	{

		while(i>=0 && x->data[i] > k ) 
		{
			x->data[i+1]= x->data[i]; // shifting the elements greater than k ahead by one place since we're going to increase the size of data array as we're inserting element
			i--;
		}

		//now (i+1) = the index where we have to insert the new element k
		x->data[i+1]=k;
		x->n = x->n +1;

	}

	else  //the node is not leaf node. so the insertion will not take place here. we will have to go to one of the subtree. 
	{
		while(i>=0 && x->data[i] > k ) //finding the child which will have the new key
			i--;

		// so the new node will have to be inserted in the (i+1)th child of node x

		if((x->child[i+1])->n == (2*degree-1)) //checking if this child is full 
		{
			Btree_split_child(x,i+1); //if full then split it
		

			if(x->data[i+1] < k) //to check which subtree we have to insert in after the split we compare k with the middle value of the child which has gone up
				i++;
		}
		//since we've split the node now it's not full so we can apply Btree_insert_nonfull now.
				Btree_insert_nonfull ( x->child[i+1],k);
	}

}

void insert (int k)                            //dont take struct node* root as an argument here. or it always takes root as null after every insert call. and enters the 1st if. 
{
	if(root==NULL)      //empty tree
	{
		root = Create(degree);
		root->data[0]=k;
		root->n=1;	
		
	}

	else //tree not empty
	{	
		
		if(root->n == 2*degree-1)   //root is full
		{
			struct node* s = Create();     //the new root
			s->isleaf=false;
			s->child[0]=root;

			Btree_split_child(s,0);   // => 0'th child of s will be split. i.e. root is gonna get split 
										//and middle key of root will be inserted into s.
			root= s;     //now s will be the new root with one key
			
			Btree_insert_nonfull(s,k);
		}

		else 	Btree_insert_nonfull(root,k); 
	}
	
	
}

int return_index(struct node* x,int k)        //returns the index of the first key in the node x which is >= key to be deleted.  it is the index of tbhe child node in wich x might be present 
{											// note that : the key to be deleted may not be present in this node. if there is equality in while cond. then index is index of key to be deleted else key to be deleted is not present in this node 	
	int index=0;
	while( index< x->n && x->data[index]<k  )index=index+1;

	return index;
}

void delete_from_leaf(struct node* x,int i)  //deletes the i'th key from the node x, which is a leaf node.
{

	int j= i;
	while(j< (x->n -1) )
	{
		x->data[j]= x->data[j+1];
		j++;
	
	}
	x->n = x->n -1;
	if(x==root && i==0)root=NULL;
}

void delete_from_NONleaf(struct node* x,int index) //deletes the i'th key from the node x
{
	

	//This has 3 cases.  Find the child of node x which comes before index and the child after that i.e. child[index] and child[index+1]   	
	//case 1: child[index] has atleast t keys. t=degree  
	//case 2: child[index] has less than t keys
	//case 3: Both   child[index] & child[index+ 1] have less than t keys 

	int k = x->data[index]; //the key to be deleted

	//case1
	if((x->child[index])->n  >= degree)
	{
		int p = predecessor(x,index);
		x->data[index] = p;
		delete( x->child[index], p );
	}

	//case 2 child[index] has less than t keys and child[index+1] has at least t keys
	else if ( (x->child[index+1])->n  >= degree )
	{
		int s = successor(x,index);
		x->data[index] = s;
		delete( x->child[index+1] , s);
	}

	//case 3: both have less than t keys
	else
	{
		combine(x,index);
		delete( x->child[index], k );
	}


}

void delete (struct node* x,int k) 
{

 //made changes
 int index = return_index(x,k);   

 	if( index < x->n && x->data[index] ==k)      //IF k is present in node x
 	{

 		if(x->isleaf)
 			{	
 				delete_from_leaf(x,index);
 			}

 		else delete_from_NONleaf(x,index);


 	}	

 	else            //k is not present in node x
 	{
 		if(x->isleaf)
 			{
 				printf("Key not present in the tree. FAILED DELETION\n");
 				return;
 			}

		int last = 0;
 		if(index == x->n)last=1;

 		if(x->child[index]->n < degree) fill(x, index);


 		if(last && index> x->n ) delete( x->child[index-1],k );

 		else delete( x->child[index],k );


 	}
		
}

int successor(struct node* x, int index)      //returns the successor of x->data[index]
{
	struct node* current = x->child[index+1];
	while(current->isleaf == false)current=current->child[0];

	return current->data[0];  //first key of this leaf node will be the successor

}

int predecessor(struct node* x, int index)
{
	struct node* current = x->child[index];

	while(current->isleaf == false)current=current->child[current->n]; // recursively move to right most child of node till we reach leaf

	return current->data[current->n -1]; //last key of the leaf is the predecessor
}

void combine(struct node* x,int index)
{
	struct node* ch = x->child[index];
	struct node* sibling = x->child[index+1];

	ch->data[degree-1] = x->data[index];

	for(int i=0; i< sibling->n ;i++)
		ch->data[i+degree] = sibling->data[i];

	if(ch->isleaf==false)
	{
		for(int i=0; i< sibling->n ;i++)
			ch->child[i+degree] = sibling->child[i];
	}

	for(int i= index+1; i<x->n;i++)
		x->data[i-1] = x->data[i];

	for(int i= index+2; i<=x->n ; i++)
		x->child[i-1] = x->child[i];

	ch->n =ch->n + sibling->n +1;
	x->n --;
	free(sibling);

}


void fill(struct node* x,int index)            //fill child[index] if less than t-1 keys
{
	if(index!=0 && x->child[index-1]->n >=degree)
	prev_child_key( x,index);

	else if (index!=x->n && x->child[index+1]->n >=degree)
	next_child_key( x,index);

	else
	{
		if(index != x->n)combine(x,index);

		else combine(x,index-1);
	}  
}
													
void prev_child_key (struct node* x,int index)		//takes key from child[index-1 ] and puts it into child[index]
{
	struct node* ch = x->child[index];
	struct node* sibling = x->child[index-1];

	for(int i = ch->n-1; i>=0 ; i--)
		ch->data[i+1] = ch->data[i];

	if(ch->isleaf== false)
	{
		for(int i =ch->n ; i>=0; --i)ch->child[i+1] = ch->child[i];
	}

	ch->data[0] = x->data[index-1];

	/* ambiguous*/ if(ch->isleaf==false)  ch->child[0] = sibling->child[sibling->n];

 x->data[index-1] = sibling->data[sibling->n -1];

 ch-> n ++;
 sibling->n --;
 

}

void next_child_key(struct node* x,int index)
{
	
	struct node* ch = x->child[index];
	struct node* sibling = x->child[index+1];	

	ch->data[ch->n] = x->data[index];

	if(ch->isleaf==false)
		ch->child[ch->n+1] = sibling->child[0];

	 x->data[index] = sibling->data[0];

	 for(int i=1; i<sibling->n ; i++)
	 sibling->data[i-1] = sibling->data[i];

	 if(sibling->isleaf==false)
	 {
	 	for(int i=1;i<sibling->n; i++)
	 		sibling->child[i-1] = sibling->child[i];
	 }	

	 ch->n ++;
	 sibling->n --;

}

void main ()
{
	
printf("max numbers in a node = (2*degree-1)\n");
printf("Enter the minimum degree of the B-tree\n");       //Each node can have at max (2*degree- 1) nodes
scanf("%d",&degree);

int choice=0;
	int ram = 1;
	int num;


	int n;
	printf("Enter the number of elements you want\n to insert together\n");
	scanf("%d",&n);
	printf("Enter %d number(s) with a space\n",n );
	for (int i = 0; i < n; ++i)
	{
		scanf("%d",&num);
		insert(num);	
	}

	while(ram==1)
	{

    	printf("1: Insert\n");
    	printf("2: Search \n");
    	printf("3: Delete \n");
    	printf("4: EXIT\n");

    	scanf("%d",&choice);
      
      	switch(choice)
       {
      		case 1: printf(" Enter the number you want to insert\n");
        	
        		scanf("%d",&num);
        		insert(num);
        		break;

     	 	case 2: 	

      				printf(" Enter the number you want to search\n");
      			
        			scanf("%d",&num);
        				if( search(root,num)==-1)
        					{
        						printf("Empty tree\n");
        								
        					}

      					else if( search(root,num)== num)printf("Key is present in the B tree\n");

      					else printf("Key not present \n");
				     	break;
	 		case 3:  if(!root)
	 					{
	 						printf("Empty B-tree. can't delete\n");
	 						break;
	 					}
	  			  	printf(" Enter the number you want to delete\n"); 		
	  			
        			scanf("%d",&num);
        			delete(root,num);

        			break; 

        	case 4: free(root);
        			ram=0;
        			break;    

     				default : printf("Wrong input.\n");;
        }        
    
} 


	
}