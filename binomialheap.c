#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


struct node
{
	int key;
	struct node* parent;
	struct node* child;     //pointer to the leftmost child (if any)
	struct node* sibling;	//pointer to its sibling immediately to its right(if any)
	int n;        //degree =  no. of children
};
//struct node* root =NULL;

struct node* Create(int key)                                                //creates new node of linked list
{
	struct node* temp = (struct node*)malloc(sizeof(struct node));
	temp->key = key;
	temp->n = 0;            //no. of childern of a node
	temp->parent= NULL;
	temp->child= NULL;
	temp->sibling= NULL;
	return temp;	
}

struct node* H = NULL;       //HEAD OF THE LINKED LIST  OF ROOTS OF THE BINOMIAL TREES. IT NEED NOT BE MINIMUM FOR  THE ENTIRE HEAP. 
//struct node* U = NULL;       // ROOT OF EACH BINOMIAL TREE IN THE BINOMIAL-HEAP IS INDIVIDUALLY MINIMUM OF THAT BINOMIAL TREE

////THE LINKED LIST OF ROOTS OF THE BINOMIAL TREES IS IN INCREASING ORDER OF THE DEGREES OF THE BINOMIAL TREES
												
struct node* Make_Binomialheap()       
{
 	struct node* t = NULL;
	return t;
}

struct node* Binomialheap_Minimum()    //returns a pointer to node with min. key.       NOTE THAT MIN KEY NODE IS ONE OF THE ROOT NODES IN OF THE BINOMIAL TREES. SINCE FOR EACH BINOMIAL TREE ITS ROOT HAS THE MIN. KEY
{    
	//WE  will traverse through the linked list of roots since the minimum is stored at the root
	// since sibling of root is pointer to next root
	struct node* y = NULL;
	struct node* x = H;           //x POINTS TO THE HEAD OF THE BINOMIAL HEAP
	int min = INT_MAX;

	if(x==NULL)printf("Empty Binomial Heap. Can't Find minimum\n");

	while(x!= NULL)
	{
		if(x->key < min)
			{
				min=x->key;
				y = x;          //y  WILL STORE THE MINIMUM KEY ROOT
			}	
		x= x->sibling;	    //WE WILL MOVE TO THE NEXT ROOT IN THE LINKED LIST
	}
	return y;	
}

void Binomial_Link(struct node* y, struct node* z)   //links two binomial trees(rooted at y and z) of same degree.        into a tree with one degree higher
{									// makes z the root of the unified tree
	y->parent = z;
	y->sibling = z->child;
	z->child = y;
	z->n ++;
}

struct node* Binomial_Heap_Merge(struct node* H1,struct node* H2) //MERGES the root lists of Binomial heaps H1, H2  into a single Linked list H sorted by degree in increasing order.
{
	struct node* H = Make_Binomialheap();        //H IS LOCAL VARIABLE NOT RELATED TO THE GLOBAL
	struct node* y = H1;
	struct node* z = H2;
	struct node* a;                                    // SINCE IN A BINOMIAL HEAP HEAD HAS THE MINIMUM DEGREE
	struct node* b;

	if(y!=NULL && z!=NULL && y->n <= z->n) H = y;     //setting head as the head of min degree heap         

	else if(y!=NULL && z!=NULL && y->n > z->n) H = z;

	else if(y!=NULL && z==NULL) H = y;

	else H = z;



	while( y!=NULL && z!=NULL)                   
	{
		if( y->n < z->n ) y= y->sibling;   			//for root nodes sibling is the pointer to next root node
 
		else if( y->n == z->n)
		{
			a = y->sibling;
			y->sibling = z;
			y = a;
		}

		else
		{
			b= z->sibling;
			z->sibling = y;
			 z= b;
		}
	}

  return H;	
}

struct node* Binomial_Heap_Union(struct node* H1,struct node* H2 ) // Unites the two binomial heaps H1 and H2 and returns the head of resluting Binomial Heap
{
	struct node* H  = Make_Binomialheap();   //NOTE THAT THIS H IS LOCAL VARIABLE IT HAS NO RELATION WITH THE GLOBAL. ITS SCOPE IS LIMITED TO THIS FUNCTION ONLY
	H = Binomial_Heap_Merge(H1,H2);

	if(H==NULL)return H;

	struct node* prev_x;
	struct node* next_x;
	struct node* x;

	prev_x = NULL;
	x = H;
	next_x = x->sibling;

	while(next_x != NULL)
	{
		if( (x->n != next_x->n ) || ((next_x->sibling !=NULL)  &&  (next_x->sibling)->n == x->n) )
		{
			prev_x = x;
			x = next_x;
		}


		else 
		{
			if (x->key  <= next_x->key )
			{
				x->sibling = next_x->sibling;
				Binomial_Link(next_x,x);
			}
			else 
				{
					if(prev_x == NULL)H= next_x;

					else prev_x->sibling = next_x;

					Binomial_Link(x,next_x);
					x= next_x;
				}		
		}	
		next_x = x->sibling;	
	}

	return H;
}

struct node* Binomial_Heap_insert(struct node* Q, struct node* x )
{
	struct node* H1 = Make_Binomialheap();

	H1 = x;
	Q = Binomial_Heap_Union (H, H1);
	return Q;

}

struct node* Binomial_Heap_DeleteMin() 
{
	//FIND the Root x with min key from root list of H  and remove it from that linked list
	//also reverse the order of linked list of x's children
  struct node* H2 = Make_Binomialheap();

	int min;
	struct node* x = H;
	struct node* t = NULL;
	struct node* p;

	if(x == NULL)       //HEAD IS NULL
	{
		printf(" Binomial Heap is empty can't delete\n");
		return NULL;
	}
	p = x;      //SETTING p to head
	min = x->key; //initialising min as the head's key

	while(p->sibling !=NULL)        //traversing through the linked list of roots finding the min. 
	{
		if((p->sibling)->key < min)
		{
			min = (p->sibling)->key;
			t = p;
			x = p->sibling;
		}

		p = p->sibling;
	}
				//now x  is the root with the min. key
   		if( t==NULL && x->sibling ==NULL)H = NULL;

   		else if(t == NULL) H = x->sibling;

		else if( t->sibling ==NULL)t =NULL;

		else t->sibling = x->sibling;
// X IS THE MIN. KEY root we have to delete
// BUT BEFORE THAT WE HAVE TO REVERSE THE ORDER OF LINKED LIST OF X'S CHILDREN
	
	if(x->child != NULL)          
	{
		
		//we need to reverse linked list
		struct node* a = x->child;
		struct node* b;
		struct node* c;
		
		a=b=c= x->child;

		if(a->sibling!=NULL && a->sibling->sibling==NULL)   //only two nodes in linked list    this is how we reverse it
		{
			b= b->sibling;
			b->sibling= a;
			a->sibling =NULL;
		}
		if(a->sibling!=NULL && a->sibling->sibling!=NULL)      //linked list has more than 2 nodes
		{
			a= a->sibling->sibling;
			b= b->sibling;
			c->sibling=NULL;
		
			b->sibling = c;
		
			while(a!= NULL)
			{
				c=b;
				b=a;
				a= a->sibling;
				b->sibling = c;
			}
		}
		// now b is the new head        and x is the least node.
		H2 =b;    //H2 is the new head of heap formed by  the children of the deleted node x
		x->child->sibling =NULL;
	}
	
	H = Binomial_Heap_Union(H, H2);

	return x; //the deleted node with min. key
}
 
void printBinomialHEAP(struct node* H)
{
	while(H)
	{
		printf("%d\t",H->key );
		printBinomialHEAP(H->child);
			H = H->sibling;
	}

} 

void main()
{
	int stop=0;
	int n,l;
	int choice=0;
	struct node* p;
	struct node* q;
	int no;


	printf("Enter the number of elements you want to insert\n");
	scanf("%d",&no);
	



	printf("Enter the elements to insert\n");
	for (int i = 0; i < no; ++i)
	{
		scanf("%d",&l);
		q = Create(l);
		H = Binomial_Heap_insert(H,q);
	}

	

	while(stop==0)
	{		
		 printf("1: Insert\n");
		 printf("2: Delete Min. \n");
		 printf("3: Find Min.\n");
		 printf("4: PRINT THE BINOMIAL-HEAP\n");
		 printf("5: EXIT\n");
		 scanf("%d",&choice);

		 switch(choice)
		 {
		 	case 1: printf(" Enter the number you want to insert\n");
        	
        		scanf("%d",&n);
        		p = Create(n);
        		H = Binomial_Heap_insert(H,p);
        		break;

        	case 2: printf("Deleting the min. key node\n");
        			p= Binomial_Heap_DeleteMin();
        			if(p!=NULL)printf("Deleted node has the key = %d\n",p->key);
        			break;

        	case 3: printf("Finding the min. key node\n");

        			p = Binomialheap_Minimum();
        			if(p!=NULL)printf("The min key in the heap is %d\n",p->key);

        			break;
        	case 4:	
        		    if(H==NULL)
        		    	{
        		    		printf("The Binomial Heap is empty\n");
        					break;
        				}
        			else
        		   {
        			printf("Printing the Binomial Heap\n");        			
        			printBinomialHEAP(H);
        			printf("\n");
        			break;
        		   }

        	case 5: stop=1;
        			break;

        		default: printf("Invalid input\n");
        					break;

		 }	


	}
}




