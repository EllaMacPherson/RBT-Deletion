#include "node.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*
  Ella MacPherson red black tree 5/05/2026
  
  1. Every node is either red or black.
  2. All null nodes are considered black.
  3. A red node does not have a red child.
  4. Every path from a given node to any of its leaf nodes (Black height)

 */


// Functions declerations 

// Get nodes in relation to a node, n
node* getUncle(node* n);
node* getSibiling(node* n);
node* getCloseNeph(node* n, node* s);
node* getDistantNeph(node* n, node* s);

// BST insertion
void bstinsert(node*& root, node* current, int value, node*& storage);

// Search
void search(int s, node* current, bool& found);

// Check violations for insertion
void checkViolations(node* myNode, node* parent, node* grandparent, node* uncle, node*& root);

// Case functions for insertion
void case2(node* n, node* p, node* g, node* u, node*& root);
// 5L+R rotations
void case5R(node* n, node* p, node* g, node* u, node*& root);
void case5L(node* n, node* p, node* g, node* u, node*& root);
// 6L+R rotations
void case6L(node* n, node* p, node* g, node* u, node*& root);
void case6R(node* n, node* p, node* g, node* u, node*& root);

// Display tree
void print(int depth, node* current);

// Delete functions

// Get functions
void getNode(int input, node* current, node*& n);
void getValues(node*& n, node*& p, node*& s, node*& c, node*& d, node*& root);
node* getSuccessor(node* n);
// Different deletion case types
void simpleDeletion(node* n, node*& root);
void complexDeletion(node* n, node* p, node* s, node* c, node* d, node*& root);

// Delete rotations
void deleteCase3L(node* n, node* p, node* s, node* c, node* d, node*& root);
void deleteCase3R(node* n, node* p, node* s, node* c, node* d, node*& root);
void deleteCase5R(node* n, node* p, node* s, node* c, node* d, node*& root);
void deleteCase5L(node* n, node* p, node* s, node* c, node* d, node*& root);
void deleteCase6L(node* n, node* p, node* s, node* c, node* d, node*& root);
void deleteCase6R(node* n, node* p, node* s, node* c, node* d, node*& root);


int main(){

  string command = "";
  node* root = NULL;
  
  // Loop for commands
  while(true){

    cout<<"Enter INSERT, FILE, PRINT, SEARCH, DELETE"<<endl;
    getline(cin, command);

    if(command == "SEARCH" || command == "search"){
      // Search func and output
      cout<<"What number are you searching for?"<<endl;
      int s = 0;
      cin>>s;
      cin.ignore();
      bool found = false; // keep track if its found
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
	// Check if it exists to proceed with deletion
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

	  // Start with simple deletion -> this will call complex cases if needed
	  simpleDeletion(n, root);
	  
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

// Deletion function that encapsulates the "Simple" delete cases
void simpleDeletion(node* n, node*& root){

  // If deleted item is ROOT and NO CHILDREN
  if(n == root && n->right == NULL && n->left == NULL){
    delete n;
    root = NULL;
    return; // Done
  }

  // If color of node is RED 
  else if((n->left != NULL && n->right == NULL) || (n->right != NULL && n->left == NULL)){
    // left child case
    if(n->left != NULL){
      //check for red
      if(n->left->color == true){
	if(n != root){
	  n->left->parent = n->parent;
	  n->left->color = false;
	  if(n->parent->left == n){ //set parents next pointer equal to this
	    n->parent->left = n->left;
	  }
	  if(n->parent->right == n){
	    n->parent->right = n->left;
	  }
	  delete n;
	}else{
	  root = n->left;
	  root->color = false;
	  delete n;
	}
      }
    }

    // Right child case
    if(n->right != NULL){
      //check for red
      if(n->right->color == true){
	if(n != root){
	  n->right->parent = n->parent;
	  n->right->color = false;
	  if(n->parent->left == n){ //set parents next pointer equal to this
	  n->parent->left = n->right;
	}
	  if(n->parent->right == n){
	    n->parent->right = n->right;
	  }
	  delete n;
	}else{
	  root = n->right;
	  root->color = false;
	  delete n;
	}
      }
    }
  }
  
  // 2 child case, swap integers and make it a 1 child delete case
  else if(n->right != NULL & n->left != NULL){
    // get successor value
    node* successor = getSuccessor(n);
    n->value = successor->value;
    simpleDeletion(successor, root);
  }

  // No children and is RED, remove leaf node
  else if(n->right == NULL && n->left == NULL && n->color == true){
    if(n->parent->left == n){
      n->parent->left = NULL;
    }
    if(n->parent->right == n){
      n->parent->right = NULL;
    }
    delete n;
  }

  // has no children and is BLACK (complex case, call func for that); if root being deleted and matches all those is handled earlier
  else if(n->right == NULL && n->left == NULL && n->color == false){

    node* p = NULL;
    node* s = NULL;
    node* c = NULL;
    node* d = NULL;
    
    getValues(n,p,s,c,d,root);

    // set N equal to NULL
    if(p->right == n){
      p->right = NULL;
    }else{
      p->left = NULL;
    }
    delete n;
    
    n = NULL;

    // On first iteration, n must be NULL
    complexDeletion(n,p,s,c,d,root);
  }
}

// Check all wikipedia cases with this func and fix them and call any corresponding fucntions
void complexDeletion(node* n, node* p, node* s, node* c, node* d, node*& root){
  // Check inputs-> helpful debugging code
  /*  
  if(n != NULL){
    cout<<"Node: "<< n->value;
  }else{
    cout<<"N is null"<<endl;
  }

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
  
  */
  // EXECUTE CASES:

  // Case 1: We are n is our root, we have iterated all the way up
  if(n == root){

    return;
  }

  // Case 2: P, C, S, D are all BLACK or NULL FUNCTIONAL FOR 1 ITERATION
  if(p->color == false &&  (s == NULL || s->color == false) &&
     (c == NULL || c->color == false) && 
     (d == NULL || d->color == false)){
    //    cout<<"Case 2"<<endl;
    // recolor S to red
    s->color = true;
    n = p; // reporpogate with this
    if(n == root){
      //      cout<<"n is now root ->returning"<<endl;
      return; // dont reassign values we are DONE
    }

    p = n->parent;
    s = getSibiling(n);

    if(s == p->right){
      if(s != NULL){
	if(s->left != NULL){
	c = s->left;
	}else{
	  c = NULL;
	}
	if(s->right != NULL){
	  d = s->right;
	}else{
	  d = NULL;
	}
      }
    }

    if(s == p->left){
      if(s != NULL){
	if(s->right != NULL){
	c = s->right;
	}else{
	  c = NULL;
	}
	if(s->left != NULL){
	  d = s->left;
	}else{
	  d = NULL;
	}
      }
    }
    
    // reupdate values for n as p
    //    getValues(n,p,s,c,d,root);
      
    //recall delete
    complexDeletion(n,p,s,c,d,root);
    return;
    
  }

  // Case 3: Red sibiling, black P + c + d
  if((s != NULL && s->color == true) && p->color == false &&
     (c == NULL || c->color == false) &&
     (d == NULL || d->color == false)){


    // rotate so s is parent of p
    if(p->left == n){
      deleteCase3L(n,p,s,c,d,root);
    }
    if(p->right == n){
      deleteCase3R(n,p,s,c,d,root);
    }
    //recolor p to red and s to black
    p->color = true;
    s->color = false;

    // reassign n values -- cant reuse func cause n is NULL
    s = c;
    if(p->right == s){
      c = s->left;
      d = s->right;
    }
    if(p->left == s){
      d = s->left;
      c = s->right;
    }
   
    // Re call cause case 4,5,6 could fix it next
    complexDeletion(n,p,s,c,d,root);
    return;
  }

  // Case 4: s + c + d are black, p is red
  if(p->color == true && (s == NULL || s->color == false) &&
     (c == NULL || c->color == false) && (d == NULL || d->color == false)){

       // make p black, s red
       p->color = false;
       s->color = true;
       
     }

  // Case 5: s + d, are black, c is red, p is ANY COLOR
  if((s == NULL || s->color == false) && (d == NULL || d->color == false)
     && (c != NULL && c->color == true)){

    // Rotate
    if(p->right == s){ // n is on left side
   
      deleteCase5L(n,p,s,c,d,root);
    }
    if(p->left == s){
   
      deleteCase5R(n,p,s,c,d,root);
    }

    //recolor
   
    c->color = false;
    s->color = true;

    //reassign
    node* temp = s;
    s = c;
   
    if(p->right == s){
      if(s->right != NULL){
	d = s->right;
      }else{
	d = NULL;
      }
      if(s->left != NULL){
	c = s->left;
      }else{
	c = NULL;
      }
    }
    if(p->left == s){
      if(s->left != NULL){
	d = s->left;
      }else{
	d = NULL;
      }
      if(s->right != NULL){
	c = s->right;
      }else{
	c = NULL;
      }
    }
    
   
    //recall -> d6 is next
    complexDeletion(n,p,s,c,d,root);
    return;
  }

  // Case 6: p + c doesnt matter, S is black d, is red
  if(s->color == false && d->color == true){

    //rotate
    if(p->right == s){
      deleteCase6L(n,p,s,c,d,root);
    }
    if(p->left == s){
      deleteCase6R(n,p,s,c,d,root);
    }

    //recolor
    s->color = p->color;
    p->color = false;
    d->color = false;
    
  }
  
}

// Rotate for case6 right side
void deleteCase6R(node* n, node* p, node* s, node* c, node* d, node*& root){

  if(s->right != NULL){
    p->left = s->right;
  }else{
    p->left = NULL;
  }
  s->right = p;
  s->left = d;
  


  d->parent = s;
  s->parent = p->parent;

  if(p->parent->left == p){
    p->parent->left = s;
  }
  if(p->parent->right == p){
    p->parent->right = s;
  }

  p->parent = s;
 
  if(p->left != NULL){
    p->left->parent = p;
  }


  if(p == root){
    root = s;
  }


}

// Rotate for case6 left side
void deleteCase6L(node* n, node* p, node* s, node* c, node* d, node*& root){

  if(s->left != NULL){
    p->right = s->left;
  }else{
    p->right = NULL;
  }
  
  s->left = p;
  s->right = d;



  d->parent = s;
  s->parent = p->parent;
  
  if(p->parent->left == p){
    p->parent->left = s;
  }
  if(p->parent->right == p){
    p->parent->right = s;
  }
  
  p->parent = s;
  
  if(p->right != NULL){
    p->right->parent = p;
  }


  if(p == root){
    root = s;
  }
}

// Rotate for case 5 right
void deleteCase5R(node* n, node* p, node* s, node* c, node* d, node*& root){

  if(c->left != NULL){
    s->right = c->left;
  }else{
    s->right = NULL;
  }
  if(s->right != NULL){
    s->right->parent = s;
  }
  c->left = s;
  p->left = c;

  c->parent = p;
  s->parent = c;
  
}

// Rotate for case 5 left
void deleteCase5L(node* n, node* p, node* s, node* c, node* d, node*& root){

  if(c->right != NULL){
    s->left = c->right;
  }else{
    s->left = NULL;
  }
  if(s->left != NULL){
    s->left->parent = s;
  }
  c->right = s;
  p->right = c;

  c->parent = p;
  s->parent = c;
  
}

// Rotate for case 3 right
void deleteCase3R(node* n, node* p, node* s, node* c, node* d, node*& root){

  s->right = p;
  p->left = c;

  //fix parents
  if(p->parent != NULL){
    if(p->parent->left == p){
      p->parent->left = s;
      s->parent = p->parent;
    }
    if(p->parent->right == p){
      p->parent->right = s;
      s->parent = p->parent;
    }
  }
  c->parent = p;
  p->parent = s;


  // if p didnt have parent aka it was the root
  if(root == p){
    root = s;
  }

}

// Rotate for case 3 left
void deleteCase3L(node* n, node* p, node* s, node* c, node* d, node*& root){

  s->left = p;
  p->right = c;

  //fix parents
  if(p->parent != NULL){
    if(p->parent->left == p){
      p->parent->left = s;
      s->parent = p->parent;
    }
    if(p->parent->right == p){
      p->parent->right = s;
      s->parent = p->parent;
    }
  }
  c->parent = p;
  p->parent = s;


  // if p didnt have parent aka it was the root
  if(root == p){
    root = s;
  }
}

// Gets successor in relation to n, uses in-order succesor (what wikipedia used)
node* getSuccessor(node* n){

  node* s = n->right;

  while(s->left != NULL){
    s = s->left;
  }

  return s;
}

// get p,s,c,d in relation to a node n
void getValues(node*& n, node*& p, node*& s, node*& c, node*& d, node*& root){

  p = NULL;
  if(n != root){
    p = n->parent;
  }
  if(n != root){
    s = getSibiling(n);
  }else{
    s = NULL;
  }
  c = NULL;
  d = NULL;
  if(s != NULL){
    c = getCloseNeph(n, s);
    d = getDistantNeph(n, s);
  }
	  
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
