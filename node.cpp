#include "node.h"

node::node(node* p, node* l, node* r, int v){

  parent = p;
  left = l;
  right = r;
  value = v;

}

void node::changeColor(){
  // If black make RED
  if(color == false){
    color = true;
  }
  // If red make BLACK
  else{
    color = false;
  }
}

node::~node(){

  parent = NULL;
  right = NULL;
  left = NULL;

  delete parent;
  delete right;
  delete left;

}
