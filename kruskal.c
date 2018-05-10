#include <stdio.h>
#include <stdlib.h> 
struct edge
{
	int v1;
	int v2;
	int weight;
};

int find( int parent[] ,int k);
void cunion(int parent[],int rank[],int i,int j);
void heapsort( struct edge array[],int n) ;
void min_heapify( struct edge array[],int n,int i );



void main()
{
  printf("Enter the number of edges and vertices\n(MIND THE ORDER)\n");
  int no_of_edges,no_of_vertices;

  scanf("%d  %d",&no_of_edges,&no_of_vertices);

 // struct edge array[no_of_edges];       REPLACE THE NEXT LINE WITH THIS LINE IF ANY  ERROR . THIS IS THE LATEST CHANGE

  struct edge* array = (struct edge*)malloc((no_of_edges)*sizeof(struct edge));

printf("Enter the graph in following format \n");
printf("vertex1 vertex2 weight \n");                 //weight = weight of the edge coNNECTING  vertex1 and vertex2

for(int i=0;i<no_of_edges;i++)
{
	scanf("%d",&array[i].v1);
	scanf("%d",&array[i].v2);
	scanf("%d",&array[i].weight);
} // now we have input the graph

//initially there are as many subsets as the number of vertices

int *parent = (int*)malloc(no_of_vertices*sizeof(int));     //array of parents of all vertices

int *rank = (int*)malloc(no_of_vertices*sizeof(int));

for(int i=0;i<no_of_vertices;i++)    //initialised the vertices parents to themselves and set the ranks to zero
{
	parent[i]=i;
	  rank[i]=0;
}

//call heapsort on the array[i].weight i=0 to no_of_vertices
	heapsort(array,no_of_edges);  //sorts the array of edges in DESCENDING order - COMPILE HEAPSORT AND SEE FOR URSELF

	/*now take an edge starting from the smallest  
	  check if a cycle is being formed if we take that edge 
	  if - YES don't take that edge 
	  		NO include that edge in the MST	*/

/* note that the edges are sorted in descending order so we are starting from the last */

	printf(" THE MST CONTAINS\n");
	printf(" VERTEX1    VERTEX2   WEIGHT \n");
	for (int i = no_of_edges-1; i >=0; i--)  //taking edges starting from smallest
	{												// we check if this edge can be included in the MST 
													//BY doing find and cunion on the 2 vertices of this edge																			

		if(	find(parent,array[i].v1) != find(parent,array[i].v2) )    // if they are not in the same set then unify them and inlcude in MST
		{
			cunion(parent,rank, find(parent,array[i].v1), find(parent,array[i].v2) );
			printf(" %d 	     %d 	         %d  \n",array[i].v1,array[i].v2,array[i].weight );

		}

	}

	free(array);
	free(rank);
	free(parent);


}

void min_heapify( struct edge array[],int n,int i )  //checks if a node with index i is heapified(satisfies heap property).
											//if not makes it  heapified.and also the affected sub-tress  heapified.
											//n=size   i=node to be heapified.    meanheapify => least element at root. parent< children
{
	int l = 2*i +1;    //left child
	int r = 2*i +2;
	int smallest = i;

	if( array[l].weight < array[smallest].weight && l<n ) smallest = l;
	
	if( array[r].weight <array[smallest].weight && r<n ) smallest = r;

	if(smallest!=i ) //if the node already satisfies heap property then nothing will happen
	{
		//swap(&array[i],&array[smallest]);
			struct edge temp = array[i];
			array[i]= array[smallest];
			array[smallest]= temp;	


		min_heapify(array,n,smallest); // to heapify the affected(if any ) sub trees.

	}

}

void heapsort( struct edge array[],int n)  //n= size equal to no of edges
{

	//building heap for 1st time. from bottom up.  starting from last node which HAS CHILDREN 
	for(int j=n/2-1;j>=0;j--)
		{
			min_heapify(array,n,j);
		}

	//swapping of root with the last element (n-1). then calling min heapify. and so on.
		for(int j=n-1;j>=0;j--)     
		{
			//swap (&array[0],&array[j]);
			struct edge temp = array[0];
			array[0]= array[j];
			array[j]= temp;

			

			min_heapify(array,j,0);  //we write size of array in function call as j. So it automaticALLY REDUCES HEAP SIZE BY 1 AFTER 
										// EVERY ITERATION
		}


}  
int find( int parent[] ,int k)      //finds the root of tree containing vertice k;  by going upward from k till root
{												/* ALTERNATIVE FIND FUNCTION */ 
	if(k!=parent[k]) 							//    while(parent[k]!=k){
	{											//		parent[k]= parent[parent[k]]; //makes k point to its grandpa
		parent[k] =	find(parent,parent[k]);			//      k=parent[k];
	}											//   }
	return parent[k];							//		return k;  //the root
}

void cunion(int parent[],int rank[],int i,int j)	// unifies the two trees by making the smaller rank tree's root point to the larger. 
{
	int i_root= find(parent,i);  // root of i
	int j_root= find(parent,j);  // root of j	

	if( rank[i_root] > rank[j_root] )   //if tree with i is grater than tree with j , makes j's root point to i's  root
	{
		parent[j_root] = i_root;
	}
	else             //case 2:  both trees are equal or  tree with j is  geater tree.     When both trees are equal it doesn't matter which root we make parent
	{
		parent[i_root]=j_root;
	}

	 //rank changes(increases by 1) only when the trees being unified are of same ranks
	if(rank[i_root]==rank[j_root]) rank[j_root]=rank[j_root] + 1;

}   //USED CUNION COZ UNION IS A KEYWORD