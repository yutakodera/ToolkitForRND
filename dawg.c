#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SEQ_MAX_LEN 1000000
#define SOURCE 0
#define TRUE 1
#define FALSE 0

typedef struct node {
  int id;
  char *substring;
  struct node **primary,**secondary,*suffixptr;
} NODE;


static int l,k,node_id,moc; /* k and l denote the variation of digits in a sequence and the capable of the length of substring */
static char seq[SEQ_MAX_LEN];
static NODE source;

void attach_memories(NODE *node);
void detach_memories(NODE *node);
static NODE *create_node();
void init_node(NODE *node);
void clear_everynode(NODE *node);
static NODE *update(NODE *currentsink,char a);
static NODE *split(NODE *parentnode,NODE *childnode,char a);
void print(NODE *node);
void labeling(const NODE parentnode,NODE *childnode,char a);
void find_endpoints(int *endpoints,const char subseq[]);
void MOC(const NODE node,int depth);


int main(int argc,char *argv[]) {
  if(argc != 2) {
    printf("./dawg filename\n");
    exit(1);
  }
  l = 64;
  k = 3;
  node_id = 1;
  moc = 0;

  int i;
  NODE *currentsink;
  attach_memories(&source);
  init_node(&source);
  source.id = 0;

  FILE *fp;
  if((fp=fopen(argv[1],"r")) == NULL) {
    exit(1);
  }
  while(fgets(seq,SEQ_MAX_LEN,fp) != NULL) {
//    printf("%s", seq);
  }
  seq[strlen(seq)-1] = '\0';
/*
  printf("Input a seqeunce\n>");
  if(fgets(seq,sizeof(seq),stdin) == NULL) {
    printf("stdin error\n");
    exit(1);
  }
  if(strstr(seq,"\n") != NULL) {
    seq[strlen(seq)-1] = '\0';
  }
*/

  
  currentsink = &source;
  for(i = 0;i < strlen(seq);i++) {
    currentsink = update(currentsink,seq[i]);
  }

//  print(&source);
  
  MOC(source,0);
  printf("MOC(S) = %d\n",moc);

 
  clear_everynode(&source);

  return 0;
}


void attach_memories(NODE *node) {
  if(node == NULL) {
    printf("this node does not have memory space\n");
    exit(1);
  }

  node->substring = malloc(sizeof(char)*l);
  if(node->substring == NULL) {
    printf("malloc failed\n");
    exit(1);
  }

  node->primary = malloc(sizeof(NODE)*k);
  if(node->substring == NULL) {
    printf("malloc failed\n");
    exit(1);
  }

  node->secondary = malloc(sizeof(NODE)*k);
  if(node->substring == NULL) {
    printf("malloc failed\n");
    exit(1);
  }
}


void detach_memories(NODE *node) {
  free(node->substring);
  free(node->primary);
  free(node->secondary);
}


static NODE *create_node() {
  NODE *node;
  node = malloc(sizeof(NODE));
  attach_memories(node);
  init_node(node);
  node->id = node_id;
  node_id++;

  return node;
}


void init_node(NODE *node) {
  int i;
  node->id = -1;
  memset(node->substring,'\0',strlen(node->substring));
  for(i = 0;i < k;i++) {
    node->primary[i] = NULL;
    node->secondary[i] = NULL;
  }
  node->suffixptr = NULL;
}


void clear_everynode(NODE *node) {
  if(node == NULL) {
    return;
  }

  int i;
  for(i = 0;i < k;i++) {
    if(node->primary[i] != NULL) {
      clear_everynode(node->primary[i]);
    }
  }

  detach_memories(node);
  if(node->id != SOURCE) {
    free(node);
  }
}


static NODE *update(NODE *currentsink,char a) {
  int a_int = a - '0';
  NODE *newsink,*currentnode,*suffixnode;

  newsink = create_node();
  
  /*  labeling */
  labeling(*currentsink,newsink,a);

  /* algorithm proc.2 */
  currentsink->primary[a_int] = newsink;

  /* algorithm proc.3 */
  currentnode = currentsink;

  /* algorithm proc.4 */
  suffixnode = NULL;

  /* algorithm proc.5 */
  while((currentnode->id!=0) && (suffixnode==NULL)) {
    /* algorithm proc.5-a */
    currentnode = currentnode->suffixptr;

    /* algorithm proc.5-b */
    if(currentnode->primary[a_int] != NULL) {
      suffixnode = currentnode->primary[a_int];
    }

    /* algorithm proc.5-c */
    else if(currentnode->secondary[a_int] != NULL) {
      NODE *childnode;
      childnode = currentnode->secondary[a_int];
      suffixnode = split(currentnode,childnode,a);
    }

    /* algorithm proc.5-d */
    else {
      currentnode->secondary[a_int] = newsink;
    }
  }

  /* algorithm proc.6 */
  if(suffixnode == NULL) {
    suffixnode = &source;
  }
  
  /* algorithm proc.7 */
  newsink->suffixptr = suffixnode;

  return newsink;
}


static NODE *split(NODE *parentnode,NODE *childnode,char a) {
  int i,a_int;
  NODE *newchildnode,*currentnode;
  newchildnode = create_node();
 
  /* labeling */
  labeling(*parentnode,newchildnode,a);

  /* algorithm proc.2 */
  a_int = a - '0';
  parentnode->primary[a_int] = newchildnode;
  parentnode->secondary[a_int] = NULL;

  /* algorithm proc.3 */
  for(i = 0;i < k;i++) {
    if(childnode->primary[i] != NULL) {
      newchildnode->secondary[i] = childnode->primary[i];
    }
    if(childnode->secondary[i] != NULL) {
      newchildnode->secondary[i] = childnode->secondary[i];
    }
  }
    
  /* algorithm proc.4 */
  newchildnode->suffixptr = childnode->suffixptr;
  childnode->suffixptr = NULL;
  
  /* algorithm proc.5 */
  childnode->suffixptr = newchildnode;

  /* algorithm proc.6 */
  currentnode = parentnode;

  /* algorithm proc.7 */
  while(currentnode->id != SOURCE) {
    /* algorithm proc.7-a */
    currentnode = currentnode->suffixptr;

    /* algorithm proc.7-b */
    int hassecondary = FALSE;
    for(i = 0;i < k;i++) {
      if(currentnode->secondary[i] != NULL) {
        if(currentnode->secondary[i]->id == childnode->id) {
          currentnode->secondary[i] = newchildnode;
          hassecondary = TRUE;
        }
      }
    }

    /* algorithm proc.7-c */
    if(hassecondary == FALSE) {
      break;
    }
  } 

  return newchildnode;
}


void print(NODE *node) {
  int i;
  
  printf("==========================\n");
  printf("ID: %d\n",node->id);
  printf("Label: %s\n",node->substring);
  for(i = 0;i < k;i++) {
    if(node->primary[i] != NULL) {
      printf("primary[%d] -> %d\n",i,node->primary[i]->id);
    }
  }
  for(i = 0;i < k;i++) {
    if(node->secondary[i] != NULL) {
      printf("secondary[%d] -> %d\n",i,node->secondary[i]->id);
    }
  }
  if(node->suffixptr != NULL) {
    printf("suffixptr -> %d\n",node->suffixptr->id);
  }
  printf("\n");

  for(i = 0;i < k;i++) {
    if(node->primary[i] != NULL) {
      print(node->primary[i]);
    }
  }
}


void find_endpoints(int *endpoints,const char subseq[]) {
  int i,len_subseq = strlen(subseq);

  if(len_subseq < 1) {
    printf("length of subsequence must be larger than 1\n");
    exit(1);
  }

  for(i = 0;i < SEQ_MAX_LEN+1;i++) {
    endpoints[i] = -1;
  }
  for(i = 0;i < strlen(seq);i++) {
    if(strncmp(seq+i,subseq,len_subseq) == 0) {
      *endpoints = i + len_subseq;
      endpoints++;
    }
  }
}


void labeling(const NODE parentnode,NODE *childnode,char a) {
  if(parentnode.id == SOURCE) {
    childnode->substring[0] = a;
  }
  else {
    int i,j,label_ends[SEQ_MAX_LEN],temp_ends[SEQ_MAX_LEN];
    char label[l],temp[l],label_min[l];

    memset(label,'\0',l);
    memset(label_min,'\0',l);
    sprintf(label,"%s%c",parentnode.substring,a);

    for(i = 0;i < SEQ_MAX_LEN;i++) {
      label_ends[i] = -1;
    }

    find_endpoints(label_ends,label);

    int len_label = strlen(label);
//    printf("label: %s\n",label);
    for(i = len_label-1;len_label-i < strlen(label);i--) {
      int hasequiv = TRUE;
      strcpy(temp,label+len_label-i);
      for(j = 0;j < SEQ_MAX_LEN;j++) {
        temp_ends[j] = -1;
      }
      find_endpoints(temp_ends,temp);

      /* compare the endpoints */
      for(j = 0;temp_ends[j] != -1;j++) {
        hasequiv &= (label_ends[j] == temp_ends[j]);
      }
      if(hasequiv == TRUE) {
        memset(label_min,'\0',l);
        strcpy(label_min,temp);
      }
    }
    if(strlen(label_min) > 0) {
      sprintf(childnode->substring,"%s",label_min);
//      printf("%s:-> %s\n",label,label_min);
    }
    else {
      sprintf(childnode->substring,"%s",label);
//      printf("%s:-> %s\n",label,label);
    }
  }
//  printf("\n");

/*
  if(parentnode.id == SOURCE) {
    childnode->substring[0] = a;
  }
  else {
    sprintf(childnode->substring,"%s%c",parentnode.substring,a);
  }
*/
}


void MOC(const NODE node,int depth) {
  int i,numofedge = 0;
  for(i = 0;i < k;i++) {
    if(node.primary[i]!=NULL || node.secondary[i]!=NULL) {
      numofedge++;
    }
  }

  if(numofedge > 1) {
    for(i = 0;i < k;i++) {
      if(node.primary[i] != NULL) {
        MOC(*node.primary[i],depth+1);
      }
      else if(node.secondary[i] != NULL) {
        MOC(*node.secondary[i],depth+1);
      }
    }
  }
  else {
    return;
  }
  
  if(moc < depth+1) {
    moc = depth+1;
  } 
}




