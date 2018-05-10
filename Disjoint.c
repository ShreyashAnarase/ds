#include <stdio.h>
#include <stdlib.h>

int find(int array[],int no_of_elements,int parent[] ,int k);
void cunion(int array[],int no_of_elements,int parent[],int rank[],int i,int j);
int ret_pos(int array[],int no_of_elements,int k);


void main()
{
	int no_of_elements;
	printf("Enter the number of elements\n");
	scanf("%d",&no_of_elements);

	int* array = (int*)malloc((no_of_elements)*sizeof(int));

	printf("Enter the elements\n The elements can be anything need not be 0 to 9\n");
	for (int i = 0; i < no_of_elements; ++i)
	{
		scanf("%d",&array[i]);
	}


	int *parent = (int*)malloc(no_of_elements*sizeof(int));     //array of parents of all elements. parent[i] stores the parent of the element array[i]
	int *rank = (int*)malloc(no_of_elements*sizeof(int));

	for(int i=0;i<no_of_elements;i++)    //initialised the vertices parents to themselves and set the ranks to zero
	{
		parent[i]=array[i];
		  rank[i]=0;
	}

	int stop = 0;
	int choice;

	int m,n;

	int index_m = -1;
	int index_n = -1;

	
	while(stop==0)
	{
		printf("1)Find\n");
		printf("2)Union\n");
		printf("3)Exit\n");

		scanf("%d",&choice);

		switch(choice)
		{
			case 1: printf("Enter the two elements you want to check if they are in the same set\n");
					scanf("%d  %d",&m,&n);

					for (int i = 0; i < no_of_elements; ++i)
					{
						if(array[i]== m)index_m=i;
						if(array[i]==n)index_n=i;
					}

					if(index_m == -1 || index_n == -1  )
						{	
							if(index_m == -1)
							printf("%d does not belong to any of the set.\n Enter correct input \n",m);
							if(index_n == -1)printf("%d does not belong to any of the set.\n Enter correct input \n",n);
							break;
						}

					if(find(array,no_of_elements,parent,m)==find(array,no_of_elements,parent,n))printf("The elements %d and %d are in the the same set\n",m,n );

					else printf("These elements are in different sets\n");

					break;

			case 2: 
					printf("Enter the two elements on which you want to perform UNION\n");
					printf("The sets of the two elements will be unified\n");
					scanf("%d  %d",&m,&n);

					for (int i = 0; i < no_of_elements; ++i)
					{
						if(array[i]== m)index_m=i;
						if(array[i]==n)index_n=i;
					}

					if(index_m == -1 || index_n == -1  )
						{	
							if(index_m == -1)
							printf("%d does not belong to any of the set.\n Enter correct input \n",m);
							if(index_n == -1)printf("%d does not belong to any of the set.\n Enter correct input \n",n);
							break;
						}

					if(find(array,no_of_elements,parent,m)==find(array,no_of_elements,parent,n))
						{
							printf("The elements %d and %d are already in the same set\n",m,n );
							break;
						}

					else
						{
							cunion(array,no_of_elements,parent,rank,m,n);
						}	

					break;

				case 3: free(array);
						free(parent);
						free(rank);
						stop =1;
						break;

				default : printf("Invalid input\n");
							break;

		}


	}

}

int ret_pos(int array[],int no_of_elements,int k) // returns the index of element k in the array 'array' which is same its index in parent array.
{
					for (int i = 0; i < no_of_elements; ++i)
					{
						if(array[i]== k)return i;
					}
}

int find( int array[],int no_of_elements,int parent[] ,int k)      //finds the root of tree containing element k;  by going upward from k till root
{	
	int position= ret_pos(array,no_of_elements,k); //position is index of k in parent array

	if(k!=parent[position]) 	 //checks if k = its parent		//position is index of k in parent array			
	{											
		parent[position] =	find(array,no_of_elements,parent,parent[position]);			
	}											
	return parent[position];							
}

void cunion(int array[],int no_of_elements,int parent[],int rank[],int i,int j)	// unifies the two trees by making the smaller rank tree's root point to the larger. 
{
	int i_root= find(array,no_of_elements,parent,i);  // root of i
	int j_root= find(array,no_of_elements,parent,j);  // root of j	

	int position_i = ret_pos(array,no_of_elements,i_root);  //stores index of iroot
	int position_j = ret_pos(array,no_of_elements,j_root);

	if( rank[position_i] > rank[position_j] )   //if tree with i is grater than tree with j , makes j's root point to i's  root
	{
		parent[position_j] = i_root;
	}
	else             //case 2:  both trees are equal or  tree with j is  geater tree.     When both trees are equal it doesn't matter which root we make parent
	{
		parent[position_i]=j_root;
	}

	 //rank changes(increases by 1) only when the trees being unified are of same ranks
	if(rank[position_i]==rank[position_j]) rank[position_j]=rank[position_j] + 1;

}   //USED CUNION COZ UNION IS A KEYWORD
