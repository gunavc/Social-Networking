#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<stdbool.h>
#define MAX 100

typedef struct node 
{
    int id; //ID of user
    int numfren; //number of friends of user
    char name[MAX]; //name of user
    int* friends; //friends of user as an array
    struct node* right;  //user to the right
    struct node* left; //user to the left
} node;

struct node* retUser(char str[MAX])
{
    char name[MAX];
    char ID[MAX];
    char strfriends[MAX];

    //copied ID
    char*token=strtok(str,",");
    strcpy(ID,token);

    //copied Name
    token=strtok(NULL,",");
    strcpy(name,token);

    //copied friends' ID
    token=strtok(NULL,",");
    strcpy(strfriends,token);
    //printf("%s\n",(strfriends));

    //creating user nodes
    int id=atoi(ID);
    struct node *user = malloc(sizeof(struct node));
    user->id=id;
    user->friends=(int*)malloc(MAX * sizeof(int));
    strcpy(user->name,name);


    //adding user's friend's IDs
    token = strtok(strfriends,"|");  
    int i=0;
    while( token != NULL ) 
    {

        int temp=atoi(token);
        user->friends[i]=temp;
        i++;
        token = strtok(NULL,"|");
    }
    user->numfren=i;
    if(i==0){user->friends[i]=-1;}
    return user;
}

//search for user with id=key
struct node* search(int key, struct node *users)
{
    //CODE HERE
    node *temp = users;
    while(temp!=NULL)
    {
        if(key==temp->id)
        {
            break;
        }
        else if(key<temp->id)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }
    if(temp==NULL)
    {
        return NULL;
    }
    else
    {
        return temp;
    }
}

//see document for explanattion
struct node*refineUser(struct node*user, struct node *users)
{
    //CODE HERE
    node *x=NULL;
    int count = 0;
    while(1)
    {
        x = search(user->id,users);
        if(x!=NULL)
        {
            user->id++;
        }
        else
        {
            break;
        }
    }
    for(int i=0; i<user->numfren; i++)
    {
        node *temp;
        temp = search(user->friends[i],users);
        if(temp!=NULL)
        {
            temp->numfren++;
            temp->friends[temp->numfren-1] = user->id;
        }
    }

    return user;
}

//insert user with id
struct node* insertUser(struct node*root,int id,struct node*user)
{
   //CODE HERE
   node *temp;
   temp = root;
   if(temp==NULL)
   {
        root = user;
   }
   else
   {
        node *prev;
        while((temp!=NULL))
        {
            prev = temp;
            if(temp->id==user->id)
            {
                break;
            }
            else if(temp->id>user->id)
            {
                temp = temp->left;
            }
            else
            {
                temp = temp->right;
            }
        }
        if(prev!=NULL)
        {
            if(prev->id>user->id)
            {
                prev->left = user;
            }
            else
            {
                prev->right = user;
            }
        }
   }
   return root;
}

//Adds -1 is friend is not present in the tree
void default_friend(struct node *user, struct node *users)
{
    if(users!=NULL)
    {
        for(int i=0;i<user->numfren;i++)
        {
            node *check;
            check = search(user->friends[i],users);
            if(check==NULL || check==user)
            {
                user->friends[i] = -1;
            }
        }
    }
}
//prints friends list
void friends(int id, struct node *users) 
{
   //CODE HERE
   node *temp;
    temp = search(id,users);
    if(temp!=NULL)
    {
        if(temp->numfren==0)
        {    
            printf("-1\n");
        }
        else
        {
            int count = 0;
            default_friend(temp,users);
            for(int i=0;i<temp->numfren;i++)
            {
                if(temp->friends[i]!=-1)
                {
                    printf("%d\n",temp->friends[i]);
                }
                else
                {
                    count++;
                }
            }
            if(count==temp->numfren)
            {
                printf("-1\n");
            }
        }
    }
}

//find child node with minimum value (inorder successor) - helper to delete node
struct node *minValueNode(struct node *node1)//Changed the variable name 
{
    //CODE HERE
    node *temp = node1;
    while (temp && temp->left != NULL)
        temp = temp->left;
  
    return temp;
}  

//deletes itself from its friend's nodes
struct node*deleteFriends(int key, struct node*users)
{
    //CODE HERE
    if(users!=NULL)
    {
        node *temp;
        temp = search(key,users);
        if(temp!=NULL)
        {
            for(int i=0;i<temp->numfren;i++)
            {
                int count = 0;
                node *temp_inner;
                temp_inner = search(temp->friends[i],users);
                if(temp_inner!=NULL)
                {
                    for(int k=0;k<temp_inner->numfren;k++)
                    {
                        if(temp_inner->friends[k]==key)
                        {
                            temp_inner->friends[k+1] = temp_inner->friends[k];
                            count++;
                        }
                    }
                    if(count!=0)
                    {
                        temp_inner->numfren--;
                    }
                }
            }
        }
    }
    return NULL;
}

// Deleting a node
struct node *deleteNode(struct node *root, int key)
{
    //CODE HERE
    if(root==NULL)
    {
        return NULL;
    }
    else 
    {
        if(key<root->id)
        {
            root->left = deleteNode(root->left,key);
        }
        else if(key>root->id)
        {
            root->right = deleteNode(root->right,key);
        }
        else
        {
            if(root->left==NULL)
            {
                node *temp = root->right;
                free(root);
                return temp;
            }
            else if(root->right==NULL)
            {
                node *temp = root->left;
                free(root);
                return temp;
            }

            node *temp = minValueNode(root->right);
            root->id = temp->id;
            strcpy(root->name,temp->name);
            root->right = deleteNode(root->right, temp->id);
        }
        return root;
    }
    
}

//Print USER's IDs in ascending order
void printInOrder(node* myusers) 
{
    //CODE HERE
    if(myusers!=NULL)
    {
        printInOrder(myusers->left);
        printf("%d %s\n",myusers->id,myusers->name);
        printInOrder(myusers->right);
    }
}


int main(int argc, char **argv)
{
    node *users=NULL;   
    while(1)
    {

        int opt, id;
        fflush(stdin);
        scanf("%d",&opt);
        char str[MAX];
        switch (opt)
        {
            case 1:
      
                scanf("%s",str);
                struct node*tbins=retUser(str);
                tbins=refineUser(tbins, users);
                users=insertUser(users,tbins->id,tbins);
                break;

            case 2:
           
                scanf("%d", &id);
                deleteFriends(id, users);
                users=deleteNode(users, id);
                break;

            case 3:
        
                scanf("%d", &id);
                node* result=search(id,users);
                if(result==NULL) 
                    printf("USER NOT IN TREE.\n");
                else{
                    printf("%d\n",result->id);
                }
                break;

            case 4:
                scanf("%d", &id);
                friends(id, users);
                break;

            case 5:
                printInOrder(users);
                break;

            case 6:
                exit(0);
                break;

            default: 
                printf("Wrong input! \n");
                break;
        }

    }
    return 0;
}