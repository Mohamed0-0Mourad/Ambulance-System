#pragma once


template < typename T>
class priNode
{
private :
	T item;		// A data item
	int pri;	//priority of the item
	priNode<T>* next; // Pointer to next node
public :
	priNode(const T& r_Item, int PRI)
	{
		setItem(r_Item, PRI);
		next = nullptr;
	}
	void setItem(const T& r_Item, int PRI)
	{
		item = r_Item;
		pri = PRI;
	}
	void setNext(priNode<T>* nextNodePtr)
	{		next = nextNodePtr; 	}
	
	T getItem(int& PRI) const
	{
		PRI = pri;
		return item;
	}
	
	priNode<T>* getNext() const 
	{		return next;	}
	
	int getPri() const
	{		return pri; 	}
}; // end Node

//This class impelements the priority queue as a sorted list (Linked List)
//The item with highest priority is at the front of the queue
template <typename T>
class priQueue
{
    priNode<T>* head;
public:
    priQueue() : head(nullptr) {}

    ~priQueue() {
        T tmp;
        int p;
        while (dequeue(tmp,p));
    }

    //insert the new node in its correct position according to its priority
    virtual void enqueue(const T& data, int priority) {
        priNode<T>* newNode = new priNode<T>(data, priority);

        if (head == nullptr || priority > head->getPri()) {
            
            newNode->setNext(head);
            head = newNode;
            return;
        }
       
        priNode<T>* current = head;        
        while (current->getNext() && priority <= current->getNext()->getPri()) {
            current = current->getNext();
        }
        newNode->setNext( current->getNext());
        current->setNext( newNode);        
    }

    bool dequeue(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        priNode<T>* temp = head;
        head = head->getNext();
        delete temp;
        return true;
    }

    bool peek(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        // pri = head->getPri();
        return true;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    priNode<T> *get_head(){return head;}
};

template<typename T>
class Min_priQueue : public priQueue<T>
{
public:
    void enqueue(const T& data, int priority);
    bool get_car_back(int patientID, T& car);
};

template <typename T>
void Min_priQueue<T>::enqueue(const T& data, int priority){
    priNode<T>* newNode = new priNode<T>(data, priority);
    priNode<T>* head = get_head();
    if (head == nullptr || priority > head->getPri()) {
        
        newNode->setNext(head);
        head = newNode;
        return;
    }
   
    priNode<T>* current = head;        
    while (current->getNext() && priority >= current->getNext()->getPri()) {
        current = current->getNext();
    }
    newNode->setNext( current->getNext());
    current->setNext( newNode);        
}

template <typename T>
bool Min_priQueue<T>::get_car_back(int patientID, T& car){
    Node<T>* nodeToDeletePtr=nullptr, advance = get_head();
	int pri;
    if(isEmpty()){return false;}
	else if(advance->getItem()->get_patientID()==patientID){
		return dequeue(car, pri);
	}

	while(advance){
		nodeToDeletePtr = advance->getNext();
		if(nodeToDeletePtr->getItem()->get_carried_patient()->get_patientID()==patientID){
			advance->setNext(nodeToDeletePtr->getNext());
			car=nodeToDeletePtr->getItem();
			break;
		}
		advance = advance->getNext();
	}

	delete nodeToDeletePtr;
	return true;
}
