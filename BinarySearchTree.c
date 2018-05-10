#include <stdio.h>
#include <stdlib.h>


                         //PROGRAM TO SUPPORT INSERT,SEARCH AND DELETE IN BST
struct node
{
	int data;
struct node* left;
struct node* right;	
};
struct node* root=NULL;

struct node* Create(int key)                //creates a new node pointing to null (both left and right = null)
{
	struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->data= key;
    temp->left= NULL;
    temp->right=NULL;

    return temp;
}

void display(struct node* root)   //prints the BST in  Inorder
{
  if(root==NULL)return;
	if(root!=NULL)
	{
		display(root->left);
		printf("%d\n",root->data );
		display(root->right);
	}
} 

struct node* insert(int key)     //returns the new root after inserting(we get a new root only if the tree is empty initially)
{
    struct node* current = root;
   if(current==NULL)   // Empty tree
   	{
   		current=Create(key);
   		root= current;
        return root;               //this is the new root
	}
	else      //the tree is not empty
	{  
		int insertion = 0;   //Will be changed to 1 when insertion is done
		while(!insertion)
		{
			while(key<current->data && current->left!=NULL)
			{
				current=current->left;
			}


			while(key>current->data && current->right!=NULL)
			{
				current=current->right;
			}


			if(key<current->data && current->left==NULL)   //if this condition is satisfied we've reached the node where insertion is to be done
			{
				current->left= Create(key);
				insertion=1;
			}


			if(key>current->data && current->right==NULL)  //if this condition is satisfied we've reached the node where insertion is to be done
			{
				current->right= Create(key);
				insertion=1;
			}
		}
         
         	return root;

	}
}

struct node* Search(int key)           //returns the pointer to the node where the key is present
{
	struct node* current= root;
	if(current==NULL)
	{
		return NULL;
	}

	else
	{
		int found = 0;
		while(!found)
		{
			while(key<current->data && current->left!=NULL)
			{
				current=current->left;
			}


			while(key>current->data && current->right!=NULL)
			{
				current=current->right;
			}


			if(current->data == key)
			{
				found=1;
				return current;
			}
           

           if( key>current->data && current->right==NULL )
           {
           	return NULL;
           } 


           if(key<current->data && current->left==NULL) 
           	{
           		return NULL;
          	} 

		}
	}
}

struct node* FindParent(int key)           //returns the pointer to the PARENT OF THE node where the key is present. We will need this for deleting
{
	struct node* current= root;
	struct node* parent=NULL;
	if(current==NULL)
	{
		printf("The linked list is empty\n");
		return NULL;
	}

	else
	{
		int found = 0;
		while(!found)
		{
			while(key<current->data && current->left!=NULL)
			{
				parent= current;                                     //records the parent before updating
				current=current->left;
			}


			while(key>current->data && current->right!=NULL)
			{
				parent= current;									//records the parent before updating
				current=current->right;									
			}


			if(current->data == key)
			{
				found=1;
				return parent;
			}
           

           if( key>current->data && current->right==NULL )
           {
           	printf("Key not present in the tree\n");
           	return NULL;
           } 


           if(key<current->data && current->left==NULL) 
           	{
           		printf("Key not present in the tree\n");
           		return NULL;
          	} 

		}
	}
}

struct node* delete(struct node* root,int key)    //returns the new root node.New if the previous root is deleted. Otherwise same.      
{

	int isroot=0;
   	struct node* target = Search(key);         //target is the key to be deleted

   	if(!target)
      { 
        printf("The deletion can't take  place\n" ); 
        return NULL;
      }

   	if(target==root) isroot=1;
  	int deletion = 0;          //we will change it's value after deletion is done

  	 	if(target->right!=NULL && (target->right)->left!=NULL)
  	 	{
  	 		struct node* current = target;

  	 			current=current->right;
  	 			while(current->left!=NULL)
  	 			{
  	 				current= current ->left;
  	 			}
/*After this loop we reach the successor.Current is pointing to the successor of the target */


  	 			 if(current->right!=NULL)       //lod exists
  	 			 {
  	 			 	struct node* lod = current ->right;
  	 			 	target->data= current->data;      // we copy the contents of successor to target
  	 			 	current->data= lod->data;			//we copy contents of lod into successor and delete lod
  	 			 	current->right= lod->right;
  	 			 	current->left= lod->left;
  	 			 	free(lod);
  	 			 	deletion=1;
            printf("Successfully deleted the key\n");
  	 			 	return root;                
  	 			 	  /*the root is changed when (the target is root &&  WE FREE THE TARGET), in which case we've to update the root.In all other 
  	 			 cases we are simply copying the other values to target value so even if the target is root the pointer to root is staying intact.*/
  	 			 }

  	 			 else   //lod doesn't exist
  	 			 {
  	 			 	struct node* temp= FindParent(current->data);
  	 			 	temp->left= NULL;
  	 			 	free(current);
  	 			 	deletion = 1;
            printf("Successfully deleted the key\n");
  	 			 	return root;
  	 			 }

  	 	}

  	 	if(target->right!=NULL && (target->right)->left==NULL)
  	 	{
  	 		struct node* current = target;
  	 		current = current ->right;
  	 		target->data = current ->data;
  	 		target -> right = current->right;
  	 		free (current);
  	 		deletion = 1;
        printf("Successfully deleted the key\n");
  	 		return root;
  	 	}

  	 	if(target->right == NULL)
  	 	{
  	 		if(target->left==NULL)
  	 		{
  	 			if(isroot){ 
  	 				free(target);
            deletion=1;
            printf("Successfully deleted the key\n");
  	 				return NULL;
  	 			 }

  	 			struct node* current = FindParent(key);
  	 			if(target== current->left)
  	 			{
  	 				current->left=NULL;
  	 				free(target);
  	 				deletion=1;   
            printf("Successfully deleted the key\n");                               //Deletion done
  	 				return root;                             //target can't be root as we're finding parent 
  	 			}
  	 			if(target= current->right)
  	 			{
  	 				current->right= NULL;
  	 				free(target);
  	 				deletion=1;	
            printf("Successfully deleted the key\n");								///Deletion done
  	 				return root;
  	 			}
  	 		}


  	 		else
  	 		{
  	 			struct node* current= target->left;				//this is the predecessor
  	 			target->data = current ->data;
  	 			target -> right = current->right;
  	 			target->left = current ->left;
  	 			free(current);
  	 			deletion = 1;   
          printf("Successfully deleted the key\n");                              //Deletion done
  	 			return root;
  	 		}

  	 	}	 	

}

void main()
{
	int choice=0;
	int ram = 1;
	int num;

	while(ram)
{

    printf("1: Insert\n");
    printf("2: Search \n");
    printf("3: Delete \n");
    printf("4: Display \n");
    printf("5: EXIT\n");
    scanf("%d",&choice);
      
      	switch(choice)
       {
      	case 1: printf(" Enter the number you want to insert\n");
        	
        		scanf("%d",&num);
            if(Search(num)!=NULL)
            {
              printf("Key is ALREADY present in the tree.\n You can't insert it again\n");
              break;
            }
        		insert(num);
        		break;

     	  case 2: 	if(root==NULL)
                  {
                    printf("Linked list is empty\n");
                    break;
                  }
     	        
      			printf(" Enter the number you want to search\n");
      			
        		scanf("%d",&num);
      			if(Search(num)!=NULL)printf("Key is present in the tree\n");

            else if(Search(num)==NULL)printf("Key not present in the tree\n");
				      break;
	 	    case 3:  if(root==NULL)
                  {
                      printf("Tree is empty");
                      break;
                  }
	  			  printf(" Enter the number you want to delete\n"); 		
	  			
        		scanf("%d",&num);
        		delete(root,num);
        		break;
        case 4: if(root==NULL)
                  {
                    printf("Empty tree\n");
                    break;
                  }
                printf("\n");
                display(root);
                printf("\n");
                break;    
        case 5: ram=0;
                break;


         default : printf("Wrong input.\n");;
        }        
  
} 

}




