
#include <stdio.h>
#include <stdlib.h>  
// Program to support INSERT, FIND MIN AND DELETE MIN

void min_heapify( int array[],int* n,int i );
void build_Heap( int array[],int* n);
int find_min(int array[]);
void insert(int array[],int num, int* heapsize);
int delete_min(int array[], int* heapsize);

void   swap(int* a, int* b)
{
  int temp = *a;
  *a=*b;
  *b= temp;
}

void Display(int array[],int* n)  //n =heapsize
{
	for (int i = 0; i < *n; ++i)
	{
		printf("%d   ",array[i] );
	}
	printf("\n");
}

void main()
{
	
	int n;
	printf("Enter the maximum number of elements you will enter \n");
	scanf("%d",&n);

	int* array = (int*)malloc( n*sizeof(int) );

	int heapsize=0;       // coz currently the heap is empty 	                             //we will store the heapsize and increment it every time the
							//user inserts an element in the heap. and store any random values in the rest of the array
	for(int i=0;i<n;i++) 
	{
		array[i] = -1000;           //some random value stored in array[i]. when heap is empty
	}

	printf("Enter the number of elements you want to input\n together at the beginning\n");

	scanf("%d",&heapsize);
	printf("Enter the elements with space\n");
	
	for (int i = 0; i < heapsize; i++)
	{
		scanf("%d",&array[i]);
	}

	build_Heap(array,&heapsize);
	

	int choice=0;
	int ram = 1;
	int num;

	while(ram==1)
	{
		 printf("1: Insert\n");
		 printf("2:  Find minimum\n");
		 printf("3:  delete minimum \n");
		 printf("4: Display Heap\n");
		 printf("5: EXIT\n");


		 scanf("%d",&choice);

		 switch(choice)
       {
      		case 1: printf(" Enter the number you want to insert\n");
        	
        		scanf("%d",&num);
        		insert(array,num,&heapsize);
        		Display(array,&heapsize);
        		break;


        	case 2: printf("The minimum element is %d\n",find_min(array) ); 
        		break;

        	case 3: printf("The Deleted element is %d\n",delete_min(array,&heapsize) );
        	
        			Display(array,&heapsize);
        			break;

        	case 4: Display(array,&heapsize);
        			break;	

        	case 5: free(array);	
        			ram=0;
        			break;

       	 	default : printf("Wrong input.\n");
       	 				break;
        }        


	}


}



void min_heapify( int array[],int *heapsize,int i )  //checks if a node with index i is heapified(satisfies heap property).
											//if not makes it  heapified.and also the affected sub-tress  heapified.
											//n=size   i=node to be heapified.    meanheapify => least element at root. parent< children
{
	int l = 2*i +1;    //left child
	int r = 2*i +2;
	int smallest = i;

	if( array[l]<array[smallest] && l< *heapsize ) smallest = l;
	
	if( array[r]<array[smallest] && r< *heapsize ) smallest = r;

	if(smallest!=i ) //if the node already satisfies heap property then nothing will happen
	{
		swap(&array[i],&array[smallest]);

		
		min_heapify(array,heapsize,smallest); // to heapify the affected(if any ) sub trees.

	}

}
 
 void build_Heap( int array[],int* heapsize)
{
	//building heap for 1st time. from bottom up.  starting from last node which HAS CHILDREN 
		for(int j=(*heapsize)/2-1;j>=0;j--)
			{
				min_heapify(array,heapsize,j);
			}
}

int find_min(int array[])          // minimum element is stored at the root in the min heap. after the build heap
{
	return array[0];
}

void insert(int array[],int num,int* heapsize)
{
	*heapsize = *heapsize +1;    //increased the size of the heap

	array[*heapsize-1] = num;  //inserted the number at the last position

	build_Heap(array,heapsize); //to restore the heap property which may have been violated after insert.
}

int delete_min(int array[], int* heapsize)
{
	if(*heapsize==0)
	{
		printf("cant delete min as the heap is empty\n");
		return -1;
	}
				// we exchange the first element (minimum) with the last . delete the last.(reduce heapsize by 1) 
				//and call heapify on 1st element coz heap property is violated
	int min = array[0];
	array[0]= array[*heapsize-1];

	*heapsize = *heapsize -1;
	min_heapify(array,heapsize,0);
	return min;

}

