#pragma once
/*
This is a program that implements the queue abstract data type using a linked list.
The queue is implemented as a chain of linked nodes that has two pointers, 
a frontPtr pointer for the front of the queue and a backPtr pointer for the back of the queue.
*/

/*

				The Node: item of type T and a "next" pointer
					------------- 
					| item| next | --->
					-------------
General Queue case:

                 frontPtr																backPtr
					\											   						/		
					 \											  					   /		
					------------- 	  ------------- 	  ------------- 	  ------------- 	  	  
					| item| next |--->| item| next |--->  | item| next |--->  | item| next |---> NULL
					------------- 	  ------------- 	  ------------- 	  -------------	  
		
Empty Case:

                 frontptr	 backptr
						\	 /				
						 \	/				
					---- NULL ------


Single Node Case:
                 frontPtr	 backPtr
					\		/	
					 \	   /			
					----------- 	
					|item| next| -->NULL
					-----------	

*/

#ifndef LINKED_QUEUE_
#define LINKED_QUEUE_

template < typename T>
class Node
{
private :
	T item; // A data item
	Node<T>* next; // Pointer to next node
public :
	Node();
	Node( const T & r_Item);	
	Node( const T & r_Item, Node<T>* nextNodePtr);
	void setItem( const T & r_Item);
	void setNext(Node<T>* nextNodePtr);
	T getItem() const ;
	Node<T>* getNext() const ;
}; // end Node

template < typename T>
Node<T>::Node() 
{
	next = nullptr;
} 

template < typename T>
Node<T>::Node( const T& r_Item)
{
	item = r_Item;
	next = nullptr;
} 

template < typename T>
Node<T>::Node( const T& r_Item, Node<T>* nextNodePtr)
{
	item = r_Item;
	next = nextNodePtr;
}
template < typename T>
void Node<T>::setItem( const T& r_Item)
{
	item = r_Item;
} 

template < typename T>
void Node<T>::setNext(Node<T>* nextNodePtr)
{
	next = nextNodePtr;
} 

template < typename T>
T Node<T>::getItem() const
{
	return item;
} 

template < typename T>
Node<T>* Node<T>::getNext() const
{
	return next;
} 




template <typename T>
class LinkedQueue
{
private :
	int entries;
	Node<T>* backPtr;
	Node<T>* frontPtr;
public :
	LinkedQueue();	
	LinkedQueue(int size, T default_value);
	bool isEmpty() const ;
	bool enqueue(const T& newEntry);
	bool dequeue(T& frntEntry);  
	bool peek(T& frntEntry)  const;	
	Node<T> *get_backPtr()const {return backPtr;}
	Node<T> *get_frontPtr()const {return frontPtr;}
	void set_backptr(Node<T>* node) {backPtr = node;}
	void print();
	void decrement_entries() {entries--;}
	int get_entries() const {return entries;}
	~LinkedQueue();
	void set_backPtr(Node<T> *node){backPtr = node;}
	//copy constructor
	LinkedQueue(const LinkedQueue<T> & LQ);
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: Queue()
The constructor of the Queue class.

*/

template <typename T>
LinkedQueue<T>::LinkedQueue()
{
	backPtr=nullptr;
	frontPtr=nullptr;
	entries = 0;
}
template <typename T>
LinkedQueue<T>::LinkedQueue(int size, T default_value){
	while (size-- && enqueue(default_value)) {}
}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: isEmpty
Sees whether this queue is empty.

Input: None.
Output: True if the queue is empty; otherwise false.
*/
template <typename T>
bool LinkedQueue<T>::isEmpty() const
{
	return (frontPtr==nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

/*Function:enqueue
Adds newEntry at the back of this queue.

Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool LinkedQueue<T>::enqueue( const T& newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);
	// Insert the new node
	if (isEmpty())	//special case if this is the first node to insert
		frontPtr = newNodePtr; // The queue is empty
	else
		backPtr->setNext(newNodePtr); // The queue was not empty

	backPtr = newNodePtr; // New node is the last node now
	entries++;
	return true ;
} // end enqueue


/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Function: dequeue
Removes the front of this queue. That is, removes the item that was added
earliest.

Input: None.
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool LinkedQueue<T>:: dequeue(T& frntEntry)  
{
	if(isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
		backPtr = nullptr ;	
		
	// Free memory reserved for the dequeued node
	delete nodeToDeletePtr;
	entries--;
	return true;

}
/////////////////////////////////////////////////////////////////////////////////////////
/*
Function: peek
copies the front of this queue to the passed param. The operation does not modify the queue.

Input: None.
Output: The front of the queue.
*/
template <typename T>
bool LinkedQueue<T>:: peek(T& frntEntry) const 
{
	if(isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}
///////////////////////////////////////////////////////////////////////////////////
/*
Function: destructor
removes all nodes from the queue by dequeuing them
*/
template <typename T>
LinkedQueue<T>::~LinkedQueue()
{
	//Free all nodes in the queue
	T temp;
	while(dequeue(temp));
}
/////////////////////////////////////////////////////////////////////////////////////////
/*
Function: Copy constructor
To avoid shallow copy, 
copy constructor is provided

Input: LinkedQueue<T>: The Queue to be copied
Output: none
*/

template <typename T>
LinkedQueue<T>::LinkedQueue(const LinkedQueue<T> & LQ)
{	
	frontPtr = backPtr = nullptr;
	Node<T>* NodePtr = LQ.frontPtr;	//start at the front node in LQ
	while (NodePtr)
	{
		enqueue(NodePtr->getItem());	//get data of each node and enqueue it in this queue 
		NodePtr = NodePtr->getNext();
	}	
}
template <typename T>
std::ostream& operator<<(std::ostream& os, const LinkedQueue<T>& queue) {
    Node<T>* advance = queue.get_frontPtr(); 
    while (advance) {
        os << advance->getItem()->get_patientID() << ", ";
        advance = advance->getNext();
    }
    os << std::endl;
    return os;
}
#endif

template < typename T>
class CancelQueue :public LinkedQueue<T>
{
public:
	bool cancel_request(const int patientID);
};

template<typename T>
bool CancelQueue<T>::cancel_request(const int patientID){
	Node<T>* nodeToDeletePtr=nullptr;Node<T> *advance = this->get_frontPtr();
	Patient* request;
	if(this->isEmpty()){return false;}
	else if(advance->getItem()->get_patientID()==patientID){
		return this->dequeue(request);
	}

	while(advance){
		nodeToDeletePtr = advance->getNext();
		if(nodeToDeletePtr->getItem()->get_patientID()==patientID){
			advance->setNext(nodeToDeletePtr->getNext());
			request=nodeToDeletePtr->getItem();
			break;
		}
		else if(nodeToDeletePtr->getItem()->get_patientID()>patientID){return false;}
		advance = advance->getNext();
	}

	if (nodeToDeletePtr == this->get_backPtr())
		this->set_backPtr(nullptr);	
		
	delete nodeToDeletePtr;
	this->decrement_entries();
	return true;
}
