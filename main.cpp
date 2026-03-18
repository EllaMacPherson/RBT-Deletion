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

// Functions

// get uncle  return node*
// BST insertion
// detect case/check violatiojs
// function for each case???
//Rrotation
//Lrotation
void print(int depth, node* current);


int main(){

  string command = "";
  // Hold root of the tree, start as NULL
  node* root = NULL;

  // new node(parent, l, r, value);


  // TEST CODE FOR PRINTING
  root = new node(NULL, NULL, NULL, 13);
  root->changeColor(); //change it to black

  root->right = new node(root, NULL, NULL, 17);
  root->left = new node(root, NULL, NULL, 8);

  // Printing stuff 
  cout<<"Enter INSERT, PRINT"<<endl;
  getline(cin, command);

  if(command == "PRINT" || command == "print"){
    print(0, root);
  }


  
  
  if(command == "INSERT" || command == "insert"){
    //Initial BSTinsert set it to RED

    //Check violations ->need way to look at uncles and such think about this
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
        
    */
         // If violations foubd
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
