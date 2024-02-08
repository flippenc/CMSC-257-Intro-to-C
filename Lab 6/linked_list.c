#include<stdio.h>
#include<stdlib.h>

struct node {
	int data;
	struct node *next;
	struct node *prev;
}*head;

void append(int num) {
	struct node *temp,*right;
	temp= (struct node *)malloc(sizeof(struct node));
	temp->data=num;
	right=(struct node *)head;

	while(right->next != NULL)
	{
		right=right->next;
	}
	//temp is the rightmost element
	//right is the previously rightmost element
	right->next=temp;
	temp->prev=right;
	right=temp;
	right->next=NULL;
}

void add( int num ) {
	struct node *temp;
	temp=(struct node *)malloc(sizeof(struct node));
	temp->data=num;
	
	if (head== NULL) 
	{
		head=temp;
		head->next=NULL;
		head->prev=NULL;
	}
	else 
	{
		temp->next=head;
		temp->prev=NULL;
		head->prev=temp;
		head=temp;
	}
}

void addafter(int num, int loc) {
	int i;
	struct node *temp,*left,*right;
	right=head;
	
	for(i=1;i<loc;i++) 
	{
		left=right;
		right=right->next;
	}
	struct node *previous = right->prev;
	temp=(struct node *)malloc(sizeof(struct node));
	temp->data=num;
	left->next=temp;
	left=temp;
	right->prev=left;
	left->prev=previous;
	left->next=right;
	return;
}

void insert(int num) {
	int c=0;
	struct node *temp;
	temp=head;
	
	if(temp==NULL) {
		add(num);
	}
	else {
		while(temp!=NULL) {
			if(temp->data<num)
				c++;
			temp=temp->next;
		}
		
		if(c==0)
			add(num);
		else if(c<count())
			addafter(num,++c);
		else
			append(num);
    }
}

int delete(int num) {
	struct node *temp;
	temp=head;
	while(temp!=NULL) {
		if(temp->data==num)
		{
			//If deleting the first element
			if(temp==head) 
			{
				//If head is not only element
				if (temp->next!=NULL)
				{
					head=temp->next;
					head->prev==NULL;
					free(temp);
					return 1;
				}
				//If head is only element
				else
				{
					free(temp);
					return 1;
				}
			}
			//If deleting in middle of list
			else if (temp->next!=NULL)
			{
				temp->prev->next=temp->next;
				temp->next->prev=temp->prev;
				free(temp);
				return 1;
			}
			//If deleting last element
			else
			{
				temp->prev->next=NULL;
                                free(temp);
                                return 1;
			}
		}
		else {
			temp= temp->next;
		}
	}
	return 0;
}

int deleteIndex(int index)
{
	int count = 0;
	struct node *current = head;
	struct node *previous;
	struct node *nextOne;
	if (count == index )
	{
		head=current->next;
		free(current);
		return 1;
	}
	while (current!=NULL&&count<index)
	{
		previous=current;
		current=current->next;
		count++;
	}
	if (current->next!=NULL)
	{
		nextOne = current->next;
	}
	if (current == NULL)
	{
		return 0;
	}
	else
	{
		previous->next=current->next;
		if (current->next!=NULL)
		{
			nextOne->prev=current->prev;
		}
		free(current);
		return 1;
	}
	
}

void  display(struct node *r) {
	r=head;
	
	if(r==NULL) {
		return;
	}
	
	while(r!=NULL) {
		printf("%d ",r->data);
		r=r->next;
	}
	printf("\n");
}

int count() {
	struct node *n;
	int c=0;
	n=head;
	
	while(n!=NULL) {
		n=n->next;
		c++;
	}
	return c;
}

int findLast(int num)
{
	struct node *n;
	int c = 0;
	int locationOfLast = -1;
	n = head;
	while (n!=NULL)
	{
		if (n->data==num)
		{
			locationOfLast = c;
		}
		n=n->next;
		c++;
	}
	return locationOfLast;
}


int removeLast(int n)
{
	struct node *temp, *prev;
        temp=head;
	if (head == NULL)
	{
		return 0;
	}
	int replace;
	int c = 0;
	replace = findLast(n);
	if (replace == -1)
	{
		return 0;
	}
	while(temp!=NULL) 
	{
                if(c==replace) 
		{
                        if(temp==head) 
			{
                                head=temp->next;
                                free(temp);
                                return 1;
                        }
                        else 
			{
                                prev->next=temp->next;
                                free(temp);
                                return 1;
                        }
                }
		else {
                	prev=temp;
                	temp= temp->next;
                }
		c++;
	}

}

int appendNoRepeat(int num) {
        struct node *temp,*right;
        temp= (struct node *)malloc(sizeof(struct node));
        if (findLast(num) == -1)
	{
		temp->data=num;
        	right=(struct node *)head;
        	while(right->next != NULL)
        	{
                	right=right->next;
        	}
        	right->next =temp;
        	right=temp;
        	right->next=NULL;
		return 1;
	}
	else
	return 0;
}


int  main() {
	int i,num;
	struct node *n;
	head=NULL;
	
	while(1) {
		printf("\nList Operations\n");
		printf("===============\n");
		printf("1.Insert\n");
		printf("2.Display\n");
		printf("3.Size\n");
		printf("4.Delete\n");
		printf("5.Exit\n");
		printf("Enter your choice : ");
		if(scanf("%d",&i)<=0) {
			printf("Enter only an Integer\n");
			exit(0);
		} else {
			switch(i) {
				case 1:
					printf("Enter the number to insert : ");
					scanf("%d",&num);
					if (head == NULL)
					{
						add(num);
					}
					else
					{
						insert(num);
					}
					//insert(num);
					//append(num);
					break;
				case 2:
					if(head==NULL) {
						printf("List is Empty\n");
					}
					else {
					printf("Element(s) in the list are : ");
					}
					display(n);
					break;
				case 3:
					printf("Size of the list is %d\n",count());
					break;
				case 4:
					if(head==NULL)
						printf("List is Empty\n");
					else {
						printf("Enter the index of the number to delete : ");
						scanf("%d",&num);
						if(deleteIndex(num))
							printf("%d deleted successfully\n",num);
						else
							printf("%d not found in the list\n",num);
					}
					break;
				case 5:
					return 0;
				default:
					printf("Invalid option\n");
			}
		}
	}
	return 0;
}
