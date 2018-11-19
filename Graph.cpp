/*
 * graph.cpp
 *
 *  Created on: Feb 14, 2017
 *     
 */


#include <iostream>


using namespace std;

#include "Graph.h"

// Graph class

Graph::Graph(int n){

  if(n <= 0){
    throw out_of_range("Graph size can not be zero or negative");
  }
  else{
   m_size = n;
   m_adjLists = new AdjListNode*[n];
   for(int j = 0; j < n; j++){
     m_adjLists[j] = NULL;
   }
  }
}

int Graph::size(){
  return m_size;
}

// Graph copy constructor
Graph::Graph(const Graph& G){
  m_size = G.m_size;
  m_adjLists = new AdjListNode*[m_size];
  AdjListNode* tempOld = NULL;
  AdjListNode* tempNew = NULL;
  AdjListNode* temp = NULL;
  for(int j = 0; j < this->m_size; j++){
    tempOld = G.m_adjLists[j];
    if(G.m_adjLists[j] == NULL){
      m_adjLists[j]= NULL;
    }
    else{
      tempNew = new AdjListNode();
      m_adjLists[j]= tempNew;
      while(tempOld!= NULL){
	tempNew->m_vertex = tempOld->m_vertex;
	tempOld = tempOld->next;
	if(tempOld != NULL){
	  temp = new AdjListNode();
	  tempNew->next = temp;
	  tempNew = temp;
	}
	else{
	  tempNew->next = NULL;
	}
	
      }
    }
  }
}

// Graph destructor
Graph::~Graph(){
  AdjListNode* temp = NULL;
  for(int j=0;j < m_size;j++){
    if(m_adjLists[j] == NULL){
      continue;
    }
    else{
      temp = m_adjLists[j];
      while(temp!= NULL){
	temp = temp->next;
	delete m_adjLists[j];
	m_adjLists[j] = temp;
      }
    }
  }
  delete[] m_adjLists;
}

const Graph& Graph::operator= (const Graph& rhs){

  if(this != &rhs){
    if(m_adjLists != NULL){
      AdjListNode* temp = NULL;
      for(int j=0;j<m_size;j++){
	if(m_adjLists[j] == NULL){
	  continue;
	}
	else{
	  temp = m_adjLists[j];
	  while(temp!= NULL){
	    temp = temp->next;
	    delete m_adjLists[j];
	    m_adjLists[j] = temp;
	  }
	}
      }

      temp = NULL;
      delete [] m_adjLists;
    }
    m_size = rhs.m_size;
    m_adjLists = new AdjListNode*[m_size];
    AdjListNode* tempOld = NULL;
    AdjListNode* tempNew = NULL;
    AdjListNode* temp = NULL;
    for(int j = 0; j<m_size;j++){
      tempOld = rhs.m_adjLists[j];
      if(tempOld == NULL){
	tempNew = NULL;
	m_adjLists[j]= tempNew;
      }
      else{
	tempNew = new AdjListNode();
	m_adjLists[j]= tempNew;
	while(tempOld!= NULL){
	  tempNew->m_vertex = tempOld->m_vertex;
	  tempOld = tempOld->next;
	  if(tempOld != NULL){
	    temp = new AdjListNode();
	    tempNew->next = temp;
	    tempNew = temp;
	  }
	  else{
	    tempNew->next = NULL;
	  }
	  
	}
	
      }
    }
  }
  return *this;
}

void Graph::addEdge(int u,int v){
  if( m_size < u || m_size < v){
    throw out_of_range("vertex is out of bounds") ;
  }
  AdjListNode* temp= m_adjLists[u];
  m_adjLists[u]= new AdjListNode();
  m_adjLists[u]->m_vertex = v;
  m_adjLists[u]->next = temp;
 
  temp= m_adjLists[v];
  m_adjLists[v]= new AdjListNode();
  m_adjLists[v]->m_vertex = u;
  m_adjLists[v]->next = temp;
 
}

void Graph::dump(){
  for(int j=0;j<m_size;j++){
    AdjListNode* temp = m_adjLists[j];
    cout<< "["<<j<<"]"<<"\t";
    while(temp!= NULL){
      cout << temp->m_vertex<<"\t";
      temp = temp->next;
    }
    cout<<endl;
  }
  cout<<endl;
}


//Node constructor
Graph::AdjListNode::AdjListNode(int v, AdjListNode* ptr){
  m_vertex = v;
  next = ptr;
}

// EgIterator class

//
Graph::EgIterator::EgIterator(Graph *Gptr, bool isEnd){

  m_Gptr= Gptr;
  if(m_Gptr != NULL){
    if(isEnd){
      m_source = m_Gptr->size();
      m_where = NULL;
    }
    else{
      int j = 0;
      AdjListNode* temp = NULL;
      while (temp == NULL && j < m_Gptr->size()){
	temp = m_Gptr->m_adjLists[j];
	if(temp == NULL){
	  j++;
	}
      }
      m_source = j;
      m_where = temp;
      temp = NULL;
    }
  }
  else{
    m_source = -1;
    m_where = NULL;
  }
}

bool Graph::EgIterator::operator!= (const EgIterator& rhs) {
  if(this->m_where != rhs.m_where && this->m_source != rhs.m_source){
    return true;
  }
  return false;
}
///
void Graph::EgIterator::operator++(int dummy){
  AdjListNode* temp;
  
  if(this->m_where != NULL){
    temp = this->m_where;
    temp = temp->next;
    while(temp != NULL && temp->m_vertex < this->m_source){
      temp = temp->next;
    }
    this->m_where = temp;
  }
  
  if(this->m_where == NULL){
    while(this->m_Gptr->size() > this->m_source && this->m_where== NULL){
      this->m_source++;
      if(this->m_source == this->m_Gptr->size()){
	this->m_where = NULL;
      }
      else{
	this->m_where = this->m_Gptr->m_adjLists[m_source];
	temp = this->m_where;
	while(temp != NULL && temp->m_vertex < this->m_source){
	  temp = temp->next;
	}
	this->m_where = temp;
      }
    }
  }
}

     

///
std::pair<int,int> Graph::EgIterator::operator*(){
  if (this->m_where == NULL) {
    throw out_of_range("EgIterator dereference error.") ;
  }
  std::pair<int,int>edge;
  edge.first= m_source;
  edge.second = m_where->m_vertex;
  return edge;
}

Graph::EgIterator Graph::egBegin(){
  return Graph::EgIterator(this,false);
}
Graph::EgIterator Graph::egEnd(){
  return Graph::EgIterator(this,true);
}


// NbIterator class

Graph::NbIterator::NbIterator(Graph *Gptr, int v, bool isEnd){

  m_Gptr = Gptr;
  m_source = v;
  m_where = NULL;
  if((!isEnd) && v != -1){
    m_where = m_Gptr->m_adjLists[m_source];
  }
}
bool Graph::NbIterator::operator!=(const NbIterator& rhs){
  if(this->m_where != rhs.m_where){
    return true;
  }
  return false;
}

void Graph::NbIterator::operator++(int dummy){
  AdjListNode* temp = this->m_where;
  temp = temp->next;
  this->m_where = temp;
}

int Graph::NbIterator::operator *(){
  if (this->m_where == NULL) {
    throw out_of_range("NbIterator dereference error.") ;
  }
  return this->m_where->m_vertex;
}

Graph::NbIterator Graph::nbBegin(int v){
  
  return Graph::NbIterator(this,v,false);
}
Graph::NbIterator Graph::nbEnd(int v){
  return Graph::NbIterator(this,v,true);
}
