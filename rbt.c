 #include <stdio.h>
#include <stdlib.h>

#define red 1
#define black 0

struct node
{
	int data;
	struct node* left;
	struct node* right;	
	struct node* parent;
	int color;                        //1 for red       0 = black
};
struct node* root=NULL; //setting the root to null initially

struct node* Create(int key)                //creates a new node of red color
{
	struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->data= key;
    temp->left= NULL;
    temp->right=NULL;
    temp->parent=NULL;
    temp->color=red;

    return temp;
}

struct node* Search(struct node* root, int key)
{
	if(!root)return root;

	if(root->data == key)return root;

	if( key > root->data)return Search(root->right,key);
		
	return Search(root->left,key);
}

void Display(struct node* root)
{
	if(root==NULL)return;

	Display(root->left);
	printf("%d\t",root->data );
	Display(root->right);
}


void left_rotate(struct node* a)     // left  rotates the tree about a 
{
	struct node* b = a->right;
	a->right  = b->left;
	if(b->left)b->left->parent=a;

	b->parent = a->parent;

	if(a->parent==NULL)root=b;

	else if(a == a->parent->left) a->parent->left = b;

	else a->parent->right = b;

	b->left =a;
	a->parent =b;

}

void right_rotate(struct node* a )	// right rotates the tree about a
{
	struct node* b = a->left;
	a->left =b->right;
	if(b->right)b->right->parent = a;

	b->parent = a->parent;

	if(a->parent==NULL)root=b;

	else if(a== a->parent->left) a->parent->left = b;

	else a->parent->right = b;

	b->right =a;
	a->parent =b;

}

void recolor (struct node* a )  //makes a and it's sibling black and a's parent red
{
	
	a->parent->color=red;		       //set gp red
	a->parent->left->color=black;		/* set parent and uncle to black */
	a->parent->right->color=black;
	
}

void   swap(int* a, int* b)
{
  int temp = *a;
  *a=*b;
  *b= temp;
                            // printf("yooooooooo\n");
}


void RB_INSERT_FIXUP(struct node* temp)  //takes the newly inserted node as the input
{
	struct node* y=NULL;

	struct node* gp=NULL;

	while( (temp!=root)  && (temp->color==red) && (temp->parent->color==red)  )
	{
		y=temp->parent;           //y= parent
		gp=temp->parent->parent;



		//case 1)parent is left child of gp
		//includes cases  left-left and left-right
		if(y==gp->left)
		{
			struct node* uncle= gp->right;

			//case 1.1 uncle is also red . only recolor
			if(uncle!=NULL && uncle->color==red)
			{
			 recolor(y);
			 temp=gp;
			}
			//case 1.2 uncle is not red or no uncle but parent is red
			else
			{
				//case 1.2.2 new node is right child.  =>left right case (parent=left, new node= right)
				//left rotation      both codes in the if block and below are executed
				if(temp == y->right)
				{
					left_rotate(y);
					temp= y;
					y= temp->parent;
				}

				// case 1.2.1 new node is left child of parent  =>left-left case (parent =left child and so is the new node)
				//right rotation

				right_rotate(gp);
				swap(&y->color,&gp->color);
				temp= y;

			}
		
		}

		// case 2 parent is right child of gp 

		// includes cases  right-left and right-right
		else
		{
			struct node* uncle= gp->left;	
			 
			   //case 2.1   uncle also red , only recolor
			   if((uncle!=NULL) && uncle->color==red) 
			   {
			   		recolor(y);
			   		temp= gp;
			   }


			   else
			   {
			   	if(temp==y->left)
			   	{
			   		right_rotate(y);
			   		temp=y;
			   		y=temp->parent;
			   	}

			   	left_rotate(gp);
			   	swap(&y->color,&gp->color);
			   	temp=y;

			   }
		
		}


	} //while

	root->color=black;	
		
}

void insert(int key)        //return the root
{
	struct node* y = NULL;
	struct node* current = root;


   if(current==NULL)   //if the tree is Empty tree
   	{
   		current=Create(key);
   		current->color=black; //coz the root is always black
   		root= current;
          return;             //this is the new root
	}

	while(current!=NULL)
	{
		y= current; 

		if(key<current->data)current=current->left;
		else current=current->right;

	}

	//now we've reached the place to insert the node. new node will be y's child 

	struct node* temp= Create(key);
	temp->parent= y;                     //temp is the new node 
	
	if (y==NULL)
	{
		root=temp;
	}

	else if(temp->data < y->data)
	{
		y->left=temp;
	}

	else y->right=temp;

	if(y->color==black)return ;  //if parent is black no need for anything

RB_INSERT_FIXUP(temp);       //we call this when the parent is red
                           	//temp is the newly inserted node of red color

}

void main()
{
	int stop=0;
	int n;
	int choice=0;

	while(stop==0)
	{

		printf("1) Insert\n");
		printf("2) Search\n");
		printf("3) Display\n");
		printf("4) EXIT\n");

		scanf("%d",&choice);

		switch(choice)
		{
			case 1:  printf("Enter the number you want to insert\n");
					 scanf("%d",&n);
					insert(n);
					//break;                 //REMOVE THIS IF YOU DON'T WANT TO DISPLAY AFTER EVERY INSERTION
			case 3: if(!root)
					{
						printf("Empty Tree\n");	
						break;
					}
					Display(root);
					printf("\n");
					break;	
			case 2: if(root==NULL)
						{
							printf("Empty tree\n"); 
							break;
						}
					 printf("Enter the number you want to Search\n");
					 scanf("%d",&n);
					if(Search(root,n) )printf("Number is present in the tree\n");
					else printf("Number not present in the tree\n");
					break;

			case 4: stop=1;
					break;

			default: printf("Invalid input\n");
						break;



		}
			
	}
	
}