#include "node.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*
  1. Every node is either red or black.
  2. All null nodes are considered black.
  3. A red node does not have a red child.
  4. Every path from a given node to any of its leaf nodes (that is, to any descendant null node) goes
  through the same number of black nodes.

NULLS ARE BLACK!!!!!!!

 */

// Functions declerations 

// get uncle  return node*
node* getUncle(node* n);
node* getSibiling(node* n);

// BST insertion
void bstinsert(node*& root, node* current, int value, node*& storage);

// detect case/check violatiojs
void checkViolations(node* myNode, node* parent, node* grandparent, node* uncle, node*& root);

// function for each case
void case2(node* n, node* p, node* g, node* u, node*& root);


//Rrotation
void case5R(node* n, node* p, node* g, node* u, node*& root);
void case5L(node* n, node* p, node* g, node* u, node*& root);
//Lrotation
void case6L(node* n, node* p, node* g, node* u, node*& root);

void print(int depth, node* current);


int main(){

  string command = "";
  // Hold root of the tree, start as NULL
  node* root = NULL;

  // new node(parent, l, r, value);

  while(true){
    // Printing stuff 
    cout<<"Enter INSERT, PRINT"<<endl;
    getline(cin, command);

    if(command == "PRINT" || command == "print"){
      print(0, root);
    }


  
  
    if(command == "INSERT" || command == "insert"){
      // get inputs
      int value = 0;
      cout<<"What value would you like to add?"<<endl;
      cin>>value;
      cin.ignore();

      
      node* recent = NULL; // for storage of newly added one so it so can be used for checking conditions

      //Initial BSTinsert set it to RED
      bstinsert(root, root, value, recent); // structure GOOD, parents GOOD, node* of js added GOOD, uncle GOOD

      node* uncle = NULL;
      node* grandpa = NULL;
      node* sibiling = NULL;
      // Set uncle if exists
      if(root != recent){
	sibiling = getSibiling(recent);
      }
      
      if(recent->parent != root && recent != root){
	uncle = getUncle(recent);
      }else{
	cout<<"just inserted one has root before uncle"<<endl;
      }
      // Set grandpa if exists
      if(recent->parent != root && recent != root){
	grandpa = recent->parent->parent;
      }else{
	cout<<"just inserted one has root before grandpa"<<endl;
      }
      // REMEMBER: uncle is NULL if its the root or child of root but if that is NOT the case then it is VALID and needs to be checked for in cases

      //FIX  GRANDPA NOT BEING FED INTO THIS RIGHTTT!!
      if(recent != root){ // NO violations if root just inserted
	checkViolations(recent, recent->parent, grandpa, uncle, root);
      }

      // I HAVE: the UNCLE, the NODE* js added, its PARENT, its GRANDPARENT the ROOT,
      

      

      //      checkViolations();
      //Check violations WIKIPEDIA + GALBY VIDEO ->need way to look at uncles and such think about this
      /* Possible cases:
	 1. Inserting at root. -> set it to B
       
	 2. Parent is B -> DONE
       
	 3. Parent AND uncle are R -> change Parent and Unc to BLACK, change Grandparent to RED -> Recursively call on Grandparent (if grandparent is root????)
       
	 4. Uncle is B AND { (Parent is left AND node is right) || (Parent is right AND node is left) } ->
	 Execute a LEFT rotation  if it's node on RIGHT
	 Executre a RIGHT rotation if it's node on LEFT
	 then call CASE 5
	
	 5. Uncle is B AND { (Parent is left AND node is left) || (Parent is right AND node is right) } ->
	 Have parent left rotate with grandparent
	 Have parent right rotate with grandparent
	 Switch grandparent and parent colors

	 AND THEN ALL THESE WHEN GRANDPARENT OR PARENT IS ROOT IS GONNA BE DIFFERENT!!!!!!!!!!!!!!!!!!!!!!!
        
      */
      // If violations foubd, fIX THEM
    
    }
  }

}


// ALL RIGHT: we have THE NODE,THE PARENT, THE ROOT, and GRANPARENT + UNCLE (Which can be NULL, if they are up by root they simply do not exist, if they are NOT then they are a black LEAF!!!!!!!) js for uncle or sibiling. if gp null then we are at left or right of ROOT
void checkViolations(node* n, node* p, node* g, node* u, node*& root){
  // check each and every case and then call a seprate functin to FIX that case
  cout<<"Node: "<<n->value<<" ";
  cout<<"Root: "<<root->value<<" ";
  cout<<"Parent: "<<p->value<<" ";
  
  if(u != NULL){
    cout<<"Uncle: "<<u->value<<" ";
  }
  if(g != NULL){
    cout<<"Grandparent: "<< g->value<<" ";
  }
  cout<<endl;

  // ALRIGHT lets check our cases -- CASE 1-4 WORKING TO THE BEST OF MY KNOWLEDGE

  // CASE 1: Current Nodes parent is BLACK
  if(p->color == false){
    cout<<"Case 1"<<endl;
    return;
  }

  // CASE 2: Parent and uncle are RED
  if(u != NULL){
    if(p->color == true && u->color == true){
      cout<<"Case 2"<<endl;
      // CASE 3: wrapped in here, and fixed with case 4 
      case2(n, p, g, u, root);
    }
  }

  // CASE 4: root is red
  if(root->color == true){
    cout<<"Case 4"<<endl;
    root->color = false;
  }

  // CASE 5: Uncle is BLACK and inserted node has a a red parent, and is on the right side of the parent if it's on the left and vice versa

  
  // Check colors, n is inhertly red so just check uncle and parent
  if((u == NULL  || u->color == false) && p->color == true){
    // Check for inner child
    if(n == p->left && p == g->right){
      cout<<"Case 5: right of g, left of p"<<endl;
      case5R(n, p, g, u, root);
    }

    if(n == p->right && p == g->left){
      cout<<"Case 5: left side of grand, right of p"<<endl;
      case5L(n, p, g, u, root);
    }
  }

  // Case 6: Uncle is BLACK and inserted node has a red parent and is directly to the right or left   // FIX ROOT IN THIS CASE: ITS POSSIBLE FOR G to BE THE ROOT, NOT for P tho!!!!!!!!!!!!!!

  if((u == NULL || u->color == false) && p->color == true){
    // Check for outter child

    if( n == p->right && p == g->right){
      cout<<"Case 6: right side"<<endl;
      case6R(n, p, g, u, root);
    }
    if(n == p->left && p == g->left){
      cout<<"Case 6: left side"<<endl;
      case6L(n, p, g, u, root);
    }
    
  }

}

// Right rotate for Case 6,
void case6R(node* n, node* p, node* g, node* u, node*& root){

}

// Left rotate for Case 6, works with root and not root
void case6L(node* n, node* p, node* g, node* u, node*& root){
  //left rotate for case 6

  //n->left + n->right remains the same
  //u->left + u->right remains the same
  //g->right remains the same

  //Rotate
  cout<<"running func"<<endl;
  node* oldpright = p->right;
  cout<<"accessing p->right"<<endl;
  p->right = g;
  cout<<"set p->right = g"<<endl;
  g->left = oldpright;
  cout<<"set g->left = to old p right"<<endl;
  

  //fix parents + check for root
  if(g->left != NULL){
    g->left->parent = g;
  }

  cout<<"set new g->left->parent to g!"<<endl;

  node* goldparent = g->parent;
  
  g->parent = p;

  if(root == g){
    root = p;
  }else{
    p->parent = goldparent;
    goldparent->right = p; // IS IT ALWAYS GONNA BE RIGHT???? IF ERROR IN FUTURE AT THIS PART IT MAY BE THIS!!
  }
  
  cout<<"rotated not recolored"<<endl;


  //Recolor
  p->changeColor();
  g->changeColor();
  cout<<"recolored"<<endl;

  // recall again with new variables DONT NEED TO BECAUSE ALL CONDITIONS SHOULD BE SATISIFED I BELIEVE?

}

// Left rotate through parent CASE 5
void case5L(node* n, node* p, node* g, node* u, node*& root){
  // Node->right remains the same
  // Parent->left remains the same

  p->right = n->left;
  if(p->right != NULL){
    p->right->parent = p;
  }

  n->left = p;
  g->left = n;

  p->parent = n;
  n->parent = g;

  checkViolations(p, n, g, u, root);
  
}


// Right Rotate through parent CASE 5
void case5R(node* n, node* p, node* g, node* u, node*& root){

  // Node->left remains the same
  // P->right remains the same
  p->left = n->right;
  if(p->left != NULL){
    p->left->parent = p;
  }
  
  n->right = p;
  g->right = n;

  // update parents
  p->parent = n;
  n->parent = g;


  // Check violations again with swapping n and p, should call case 6
  checkViolations(p, n, g, u, root);

  // Dotnt needa do this seperatley right?? vvvvvvvvvv
  
  // Reassign
  // G stays g
  //n becomes p
  // p becmoes n

  // Check violations again with new assignments
}



void case2(node* n, node* p, node* g, node* u, node*& root){
  // fix  case 2, recursively?
  // Set grandparent to red
  g->color = true;

  // Set parent + uncle to black
  p->color = false;
  u->color = false;
  // current node remains red

  /*  // If the grandparent has a grandparent
  if(g->parent != NULL){
    
    if(g->parent->parent != NULL){
      
    }
    else{
      return;
    }
    // g has a parent, but not a grandparent, IT HAS TO BE THE ROOT, meaning its okay that the g is red cause the root is black hurray
  }
  else{

    return;
  }*/

  // Repeat if the grandparent has a red parent and is NOT the root CASE 3
  if(g->parent != NULL && g->parent != root){
    if(g->parent->color == true){
      
      //void checkViolations(node* n, node* p, node* g, node* u, node*& root)
      // Update values 
      node* gg = NULL;
      node* gu = NULL;
      if(g->parent->parent != NULL){
	gg = g->parent->parent;
      }

      gu = getUncle(g);

      // re-do with grandparent now as the node
      checkViolations(g, g->parent, gg, gu, root);
    }
  }
  



  // g becomes n if the case must continue

}

// not needed for insertion oopsie daisie i scrolled too far
node* getSibiling(node* n){
  node* p = n->parent;
  if(n == p->left){
    return p->right;
  }
  if(n == p->right){
    return p->left;
  }

  return NULL;
}

// Get uncle of one js inserted
node* getUncle(node* n){
  node* grandpa = n->parent->parent;
  if(n->parent == grandpa->left){
    return grandpa->right;
  }
  if(n->parent == grandpa->right){
    return grandpa->left;
  }

  return NULL;
}


// Same as bst but adds parent link by storing prev
void bstinsert(node*& root, node* current, int value, node*& storage){

  // If there is no root
  if(root == NULL){
    root = new node(NULL, NULL, NULL, value);
    storage = root;
    root->changeColor(); // Make black
    return;
  }

  if(current->value < value){
    // Recursive down right till NULL
    if(current->right != NULL){
      bstinsert(root, current->right, value, storage);
    }
    else{
      current->right = new node(current, NULL, NULL, value);
      storage = current->right;
      return;
    }
  }

  if(current->value > value){
    //Recursive down left till NULL
    if(current->left != NULL){
      bstinsert(root, current->left, value, storage);
    }
    else{
      current->left = new node(current, NULL, NULL, value);
      storage = current->left;
      return;
    }

  }

}

// Print function the same as binary search tree but add printing of R or B
void print(int depth, node* current){

  //move all the way to the right of the tree

  if(current->right != NULL){
    print((depth + 1), current->right);
  }

  //tab over depth times!
  for(int i = 0; i < depth; i++){
    cout<<"\t";
  }

  if(current != NULL){
    cout<<current->value;
    if(current->color == true){
      cout<<" R"<<endl;
    }
    if(current->color == false){
      cout<<" B"<<endl;
    }
  }

  // move all way to the left side of the tree
  if(current->left != NULL){
    print((depth + 1), current->left);
  }



}
