/*
MST INPUT TEST CASE 
VERTEX1 VERTEX2 WEIGHT         TEST CASE FORM CLRS
0 1 4
0 7 8
1 7 11
1 2 8
2 8 2
7 8 7
8 6 6
7 6 1
2 5 4
6 5 2
2 3 7
3 5 14
3  4 9
5 4 10 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct node            //node of a linked list
{

    int data;               //data here means vertex adjacent 
    int weight;                       
    struct node* next;
          
};

struct node* Create(int data, int wt)                                                //creates new node of linked list
{
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->data = data;
    temp->weight = wt;
    temp->next = NULL;
    return temp;
}

struct koala          // a structure to store the vertex no. and key of a vertex.
{
    int v;
    int key;    
};

void build_Heap( struct koala array[],int* heapsize);
struct koala delete_min(struct koala array[], int* heapsize);
void min_heapify( struct koala array[],int* n,int i );

void input_graph(int no_of_edges,int no_of_vertices, struct node** array)
{
    printf("Enter the graph in following format \n");
    printf("vertex1 vertex2 weight of the connecting edge  \n");                 //weight = weight of the edge coNNECTING  vertex1 and vertex2

    int vertex1,vertex2,weight;


    for(int i=0;i<no_of_edges;i++)      //TO INPUT THE GRAPH      
    {
        vertex1,vertex2,weight = -1;

        scanf("%d",&vertex1);
        scanf("%d",&vertex2);
        scanf("%d",&weight);

                                            
                                           // WE INSERT NODES(ADJACENT VERTICES) AT THE FRONT OF THE LINKED LIST  AND THEN UPDATE THE FRONT. NO need to traverse till the end of the linked list           
            struct node* temp = Create(vertex2,weight);     //makes a node of ll
            temp->next= array[vertex1];
            array[vertex1] = temp;



            struct node* camp = Create(vertex1,weight);
            camp->next = array[vertex2];
            array[vertex2]= camp;
            

    }
}


void main()
{

    int no_of_edges,no_of_vertices;

    printf("Enter the number of edges and vertices\n (MIND THE ORDER)\n");

    scanf("%d  %d",&no_of_edges,&no_of_vertices);

    int y =no_of_vertices;

    struct node** array = (struct node**)malloc(sizeof(struct node*)*no_of_vertices ) ;          //this is any array of pointers to first node of linked list.

    for(int i=0;i<no_of_vertices;i++)
    {
        array[i] = NULL;                   //INITIALISED THE BEGINNING OF EACH LINKED LIST TO NULL.
    }


    input_graph(no_of_edges,no_of_vertices,array);


    int* parent = (int*)malloc(no_of_vertices*sizeof(int));         


    struct koala* arr = (struct koala*)malloc(no_of_vertices*sizeof(struct koala) );         /*an array whose each node contains vertex no. and they key of that vertex 
    which we will feed to minheap*/                                                                             

    int* is_in_minheap = (int*)malloc(no_of_vertices*sizeof(int));
                                            

    for (int i = 0; i < no_of_vertices; ++i)
    {
        parent[i] = -1;         //an array for parents of vertices
        arr[i].key = INT_MAX;                    //KEY OF VERTEX I 
        arr[i].v = i;
        is_in_minheap[i] = 1;     //will update to 0 when we extract the vertex from the heap
    }

    arr[0].key = 0;      //this will be the first vertex to be picked
    parent[0] = 0;

    struct koala k;

    while(no_of_vertices>0)
    {

         k =delete_min(arr,&no_of_vertices);;
         int m =k.v; // m = the extraCTED VERTEX NO. 
         is_in_minheap[m]=0;      //remove vertex m from minheap

         //now we will update the parents and keys of all the adjacent vertices(still in heap) of the extracted vertex if the weights are less than keys 

         struct node* x = array[m];

         while(x!=NULL)      //browses through all the adjacent vertices of vertex 'm'. We have to consider only those adjacent vertices which are present in minheap.
        {

                int adj_v = x->data;      //the adjacent vertex
                int wajan = x->weight;    //weight of the edge formed by vertex m and vertex adj_v
                
                //HOW DO WE ACCESS KEY OF ADJ_VERTEX?
                //SINCE WE DON'T KNOW AT WHAT INDEX (ADJ_VERTEX) IT IS STORED in the array "arr" AFTER  heapify
                //BROWSE THROUGH THE ENTIRE ARR 

                int count=0;
                while(count<no_of_vertices)          
                {
                    if(arr[count].v!=adj_v)count++;

                    if(arr[count].v==adj_v)break;
                    
                }                                         //now count stores the index of adj_v  in hte array 'arr'                             

                //NOW arr[count].v = adj_v    and     arr[count].key = the key we've to update

                if( is_in_minheap[adj_v] && wajan<arr[count].key )      //key of adj_vertex updated if .....
                {

                    arr[count].key= wajan;
                    parent[adj_v]= m;

                }
            
             x=x->next;
        }
                                     // NOW AFTER UPDATING THE KEYS WE NEED TO MIN HEAPIFY THE KEYS ARRAY SO THAT THE INDEX 0 Has minimum key
         build_Heap(arr,&no_of_vertices);
  
    }


     printf("The MST contains the following edges\n vertex1\tvertex2   \n");
    for (int i = 1; i < y; ++i)
    {
        printf("  %d\t          %d   \n",parent[i],i );
    }

    free(arr);
    free(parent);
    free(is_in_minheap);

}


void min_heapify( struct koala array[],int* n,int i )  //checks if a node with index i is heapified(satisfies heap property).
                                            //if not makes it  heapified.and also the affected sub-tress  heapified.
                                            //n=size   i=node to be heapified.    meanheapify => least element at root. parent< children
{
    
    int l = 2*i +1;    //left child
    int r = 2*i +2;
    int smallest = i;

    if( array[l].key < array[smallest].key && l< *n ) smallest = l;
    
    if( array[r].key <array[smallest].key && r< *n ) smallest = r;

    if(smallest!=i ) //if the node already satisfies heap property then nothing will happen
    {
        //swap(&array[i],&array[smallest]);
            struct koala temp = array[i];
            array[i]= array[smallest];
            array[smallest]= temp;  


        min_heapify(array, n,smallest); // to heapify the affected(if any ) sub trees.

    }

}


void build_Heap( struct koala array[],int* heapsize)           //heapsize = no of vertices 
{
    //building heap for 1st time. from bottom up.  starting from last node which HAS CHILDREN 
        for(int j=(*heapsize)/2-1;j>=0;j--)
            {
                min_heapify(array,heapsize,j);
            }
}

struct koala delete_min(struct koala array[], int* heapsize)
{
                // we exchange the first element (minimum) with the last . delete the last.(reduce heapsize by 1) 
                //and call heapify on 1st element coz heap property is violated
    struct koala min = array[0];
    array[0]= array[*heapsize-1];
    *heapsize = *heapsize -1;
    min_heapify(array,heapsize,0);
    
    return min;

}