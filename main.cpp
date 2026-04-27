#include "node.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*
  Ella MacPherson Red black tree insertion 4/14/2026
  
  1. Every node is either red or black.
  2. All null nodes are considered black.
  3. A red node does not have a red child.
  4. Every path from a given node to any of its leaf nodes (that is, to any descendant null node) goes
  through the same number of black nodes.

  DELETION

 */


// Functions declerations 

// get uncle return node*
node* getUncle(node* n);
node* getSibiling(node* n);
node* getCloseNeph(node* n, node* s);
node* getDistantNeph(node* n, node* s);

// BST insertion
void bstinsert(node*& root, node* current, int value, node*& storage);

// Search
void search(int s, node* current, bool& found);

// detect case/check violations
void checkViolations(node* myNode, node* parent, node* grandparent, node* uncle, node*& root);

// function for each case:

void case2(node* n, node* p, node* g, node* u, node*& root);
// 5L+R rotations
void case5R(node* n, node* p, node* g, node* u, node*& root);
void case5L(node* n, node* p, node* g, node* u, node*& root);
// 6L+R rotations
void case6L(node* n, node* p, node* g, node* u, node*& root);
void case6R(node* n, node* p, node* g, node* u, node*& root);

// Display tree
void print(int depth, node* current);

// DELETE functions
void getNode(int input, node* current, node*& n);
void getValues(node*& n, node*& p, node*& s, node*& c, node*& d, node*& root);
void simpleDeletion(node* n, node* root);

int main(){

  string command = "";
  // Hold root of the tree, start as NULL
  node* root = NULL;
  
  // Loop for commands
  while(true){

    cout<<"Enter INSERT, FILE, PRINT, SEARCH"<<endl;
    getline(cin, command);

    if(command == "SEARCH" || command == "search"){
      //search func + output
      cout<<"What number are you searching for?"<<endl;
      int s = 0;
      cin>>s;
      cin.ignore();
      bool found = false; // keep track
      if(root != NULL){
	search(s, root, found);
	if(found == false){
	  cout<<"Your number is not in the tree.."<<endl;
	}else{
	  cout<<"Your number is in the tree"<<endl;
	}
      }
      else{
	cout<<"Tree is empty"<<endl;
      }
    }

    if(command == "delete" || command == "DELETE"){
      cout<<"Which number would you like to delete?"<<endl;
      int input = 0;

      cin>>input;
      cin.ignore();

      if(root == NULL){
	cout<<"Tree is empty"<<endl;
      }else{
	bool exists = false;
	search(input, root, exists);
	if(exists == true){

	  
	  // Get all node values relevant for checking cases
	  node* n = NULL;
	  getNode(input, root, n);

	  node* p = NULL;
	  node* s = NULL;
	  node* c = NULL;
	  node* d = NULL;
	  
	  // Test ability to get case valuse
	  getValues(n, p, s, c, d, root);

	  // pre case checking deletion func
	  simpleDeletion(n, root);
	  
	  // I have the ability to get all these values ^^^^^^^^
	  

	  // Only need to check cases once its a double black scenario

	  // OKAY CONCLUSION ASK MR G THIS NEXT TIME:
	  // 1. feed in node to delete into old bst and remove iteration through. get tothe point where its structuerd, but coloring is js messed u
	  // 2. Then save color of deleted node
	  // 3. Save successors color too and position

	  // 4. If double black or deleted node is black -> THEN check cases and fix them?

	  //what im running with:

	  // 1. Get all values and data line in delete command func
	  // Create a deletefunc for SIMPLE delete
	  // then anotha deletefunc for complex cases ->fixdelete

	  
	}else{
	  cout<<"This data is not in the tree"<<endl;
	}
      }
      
      
    }
    
    if(command == "PRINT" || command == "print"){
      if(root == NULL){
	cout<<"There is no data in the tree"<<endl;
      }else{
	print(0, root);
      }
    }

    if(command == "FILE" || command == "file"){
      string filename = ""; // Stores file name
      int input = 0; // Currently being added
      
      cout<<"Please enter name of file: "<<endl;
      getline(cin, filename);
      
      ifstream inputFile;
      inputFile.open(filename);
      
      if(!inputFile.is_open()){
	cout<<"file doesnt exist"<<endl;
      }
      else{
	while(inputFile >> input){
	  // Do insertion for THAT number
	  // Initial BSTinsert set it to RED
	  node* recent = NULL;
	  //	  cout<<"Currently inserting: "<<input<<endl;
	  bstinsert(root, root, input, recent); 
	  
	  node* uncle = NULL;
	  node* grandpa = NULL;
	  node* sibiling = NULL;

	  // Set uncle if exists
	  if(root != recent){
	    sibiling = getSibiling(recent);
	  }
	  if(recent->parent != root && recent != root){
	    uncle = getUncle(recent);
	  }
	  
	  // Set grandpa if exists
	  if(recent->parent != root && recent != root){
	    grandpa = recent->parent->parent;
	  }
	  
	  if(recent != root){ // NO violations if root just inserted
	    checkViolations(recent, recent->parent, grandpa, uncle, root);
	  }
	  
	}
      }
    }
    

  
  
    if(command == "INSERT" || command == "insert"){
      // Get inputs
      int value = 0;
      cout<<"What value would you like to add?"<<endl;
      cin>>value;
      cin.ignore();
      
      node* recent = NULL; // for storage of newly added one so it so can be used for checking conditions

      //Initial BSTinsert set it to RED
      bstinsert(root, root, value, recent); 

      node* uncle = NULL;
      node* grandpa = NULL;
      node* sibiling = NULL;
      // Set uncle if exists
      if(root != recent){
	sibiling = getSibiling(recent);
      }
      if(recent->parent != root && recent != root){
	uncle = getUncle(recent);
      }
      
      // Set grandpa if exists
      if(recent->parent != root && recent != root){
	grandpa = recent->parent->parent;
      }

      if(recent != root){ // NO violations if root just inserted
	checkViolations(recent, recent->parent, grandpa, uncle, root);
      }

    }
  }

}

void simpleDeletion(node* n, node* root){

  // If deleted item is ROOT and NO CHILDREN
  if(n == root && n->right == NULL && n->left == NULL){
    delete n;
    root = NULL;
  }

  // If color of node is RED -> just remove it NO cases needa be fixed
  if(n->color == ){
    // copy bstdelete?
  }

  // If only 1 child, replace node with its child and color it black
  
  // 2 child case, swap integers and make it a 1 child delete case
  if(n->right != NULL & n->left != NULL){


    
  }
}

void getValues(node*& n, node*& p, node*& s, node*& c, node*& d, node*& root){

  p = NULL;
  if(n != root){
    p = n->parent;
  }
  
  s = getSibiling(n);
  c = NULL;
  d = NULL;
  if(s != NULL){
    c = getCloseNeph(n, s);
    d = getDistantNeph(n, s);
  }
  
  // Testing outputs
  cout<<"Deleting: "<<endl;
  
  cout<<"Node: "<< n->value;

  if(p != NULL){
	    cout<<" Parent: "<< p->value;
  }
  if(s != NULL){
    cout<<" Sibiling: "<< s->value;
  }
  if(c != NULL){
    cout<<" Close Neph: "<<c->value;
  }
  if(d != NULL){
    cout<<" Distant Neph: " <<d->value;
  }

  cout<<endl;
  
	  
}

node* getDistantNeph(node* n, node* s){
  if(s == n->parent->right){
    if(s->right != NULL){
      return s->right;
    }else{
      return NULL;
    }
  }

  if(s == n->parent->left){
    if(s->left != NULL){
      return s->left;
    }else{
      return NULL;
    }
  }

  return NULL;

}

node* getCloseNeph(node* n, node* s){
  if(s == n->parent->right){
    if(s->left != NULL){
      return s->left;
    }else{
      return NULL;
    }
  }

  if(s == n->parent->left){
    if(s->right != NULL){
      return s->right;
    }else{
      return NULL;
    }
  }

  return NULL;
}

// getNode function: locates node that is going to be deleted and returns it
void getNode(int input, node* current, node*& n){
   if(current->right != NULL){
     getNode(input, current->right, n);
   }

  if(current != NULL){
    if(current->value == input){
      n = current;
      return;
    }
  }

  // move all way to the left side of the tree
  if(current->left != NULL){
    getNode(input, current->left, n);
  }

}

// Search function
void search(int s, node* current, bool& found){
  if(current->right != NULL){
    search(s, current->right, found);
  }

  if(current != NULL){
    if(current->value == s){
      found = true;
      return;
    }
  }

  // move all way to the left side of the tree
  if(current->left != NULL){
    search(s, current->left, found);
  }
}


// Check for violations
void checkViolations(node* n, node* p, node* g, node* u, node*& root){

  // Helpful test cout's that inform of current node that needs to be fixed respective values
  /*    cout<<"Node: "<<n->value<<" ";
    cout<<"Root: "<<root->value<<" ";
    cout<<"Parent: "<<p->value<<" ";
  
    if(u != NULL){
      cout<<"Uncle: "<<u->value<<" ";
    }
    if(g != NULL){
      cout<<"Grandparent: "<< g->value<<" ";
    }
   */
  //  cout<<endl;

  // CASE 1: Current Nodes parent is BLACK
  if(p->color == false){
    //    cout<<"Case 1"<<endl;
    return;
  }

  // CASE 2: Parent and uncle are RED
  if(u != NULL){
    if(p->color == true && u->color == true){
      //cout<<"Case 2"<<endl;
      // CASE 3: wrapped in here, and fixed with case 4 
      case2(n, p, g, u, root);
    }
  }

  // CASE 4: root is red
  if(root->color == true){
    //    cout<<"Case 4"<<endl;
    root->color = false;
  }

  // CASE 5: Uncle is BLACK and inserted node has a a red parent, and is on the right side of the parent if it's on the left and vice versa

  
  // Check colors, n is inhertly red so just check uncle and parent
  if((u == NULL  || u->color == false) && p->color == true){
    // Check for inner child
    if(n == p->left && p == g->right){
      //      cout<<"Case 5: right of g, left of p"<<endl;
      case5R(n, p, g, u, root);
    }

    if(n == p->right && p == g->left){
      //      cout<<"Case 5: left side of grand, right of p"<<endl;
      case5L(n, p, g, u, root);
    }
  }

  // Case 6: Uncle is BLACK and inserted node has a red parent and is directly to the right or left

  if((u == NULL || u->color == false) && p->color == true){
    // Check for outter child

    if( n == p->right && p == g->right){
      //      cout<<"Case 6: right side"<<endl;
      case6R(n, p, g, u, root);
    }
    if(n == p->left && p == g->left){
      //      cout<<"Case 6: left side"<<endl;
      case6L(n, p, g, u, root);
    }
    
  }

}

// Right rotate for Case 6,
void case6R(node* n, node* p, node* g, node* u, node*& root){
  //Rotate
  node* oldpleft = p->left;

  p->left = g;


  g->right = oldpleft;
 
  // Fix parents + check for root
  if(g->right != NULL){
    g->right->parent = g;
  }

  node* goldparent = g->parent;
  
  g->parent = p;

  if(root == g){
    root = p;
  }else{
    p->parent = goldparent;
    if(goldparent-> right == g){
      goldparent->right = p;
    }else{
      goldparent->left = p;
    }
   
  }


  //Recolor
  p->changeColor();
  g->changeColor();
}

// Left rotate for Case 6
void case6L(node* n, node* p, node* g, node* u, node*& root){

  //Rotate

  node* oldpright = p->right;

  p->right = g;

  g->left = oldpright;

  //fix parents + check for root
  if(g->left != NULL){
    g->left->parent = g;
  }

  node* goldparent = g->parent;
  
  g->parent = p;

  if(root == g){
    root = p;
  }else{
    p->parent = goldparent;
    if(goldparent->left == g){
      goldparent->left = p;
    }else{
      goldparent->right = p;
    }

  }


  //Recolor
  p->changeColor();
  g->changeColor();

}

// Left rotate through parent CASE 5
void case5L(node* n, node* p, node* g, node* u, node*& root){

  // Rotate
  p->right = n->left;
  if(p->right != NULL){
    p->right->parent = p;
  }

  n->left = p;
  g->left = n;

  // Update parents
  p->parent = n;
  n->parent = g;

  // Check for more violations
  checkViolations(p, n, g, u, root);
  
}


// Right Rotate through parent CASE 5
void case5R(node* n, node* p, node* g, node* u, node*& root){


  // Rotate
  p->left = n->right;
  if(p->left != NULL){
    p->left->parent = p;
  }
  
  n->right = p;
  g->right = n;

  // Update parents
  p->parent = n;
  n->parent = g;


  // Check violations again with reassignments
  checkViolations(p, n, g, u, root);
}



void case2(node* n, node* p, node* g, node* u, node*& root){

  // Set grandparent to red
  g->color = true;

  // Set parent + uncle to black
  p->color = false;
  u->color = false;

  // Repeat if the grandparent has a red parent and is NOT the root CASE 3
  if(g->parent != NULL && g->parent != root){
    if(g->parent->color == true){
      
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
 

}

// Not needed for insertion oopsie daisie i scrolled too far on wikipedia
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

// Print function the same as binary search tree but added printing of R or B
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
