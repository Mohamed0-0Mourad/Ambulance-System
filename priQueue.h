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
    int entries;
public:
    priQueue() : head(nullptr), entries(0) {}

    ~priQueue() {
        T tmp;
        int p;
        while (dequeue(tmp,p));
    }

    //insert the new node in its correct position according to its priority
    virtual void enqueue(const T& data, int priority) {
        priNode<T>* newNode = new priNode<T>(data, priority);

        if (head == nullptr || ((!head) &&priority > head->getPri())) {
            
            newNode->setNext(head);
            head = newNode;
            return;
        }
       
        priNode<T>* current = head;        
        while (current && current->getNext() && priority <= current->getNext()->getPri()) {
            current = current->getNext();
        }
        newNode->setNext( current->getNext());
        current->setNext( newNode);  
        entries++;      
    }

    bool dequeue(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        priNode<T>* temp = head;
        head = head->getNext();
        delete temp;
        entries--;
        return true;
    }

    bool peek(T& topEntry, int& pri) const{
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
    void increment_entries() {entries++;}
    void decrement_entries(){entries--;}
    void print_patients(){
	    priNode<T>* advance = head;
        int pr;
	    while (advance){
	    	cout << advance->getItem(pr)->get_patientID() << ", ";
	    	advance= advance->getNext();
	    } cout << endl;
    }

    void print_cars(){
        priNode<T>* advance = head;
        int pr;
	    while (advance){
            T car = advance->getItem(pr);
		    cout << 'H' << car->get_owning_hospital()<<"_P"<<car->get_carried_patient() << ", ";
		    advance= advance->getNext();
	    } cout << endl;
    }

    int get_entries()const {return entries;}
};

template<typename T>
class Min_priQueue : public priQueue<T>
{
public:
    void enqueue(const T& data, int priority);
    bool cancel_car(int patientID, T& car);
};

template <typename T>
void Min_priQueue<T>::enqueue(const T& data, int priority){
    priNode<T>* newNode = new priNode<T>(data, priority);
    priNode<T>* head = this->get_head();
    if (head == nullptr || ((!head)&&priority > head->getPri())) {
        
        newNode->setNext(head);
        head = newNode;
        return;
    }
   
    priNode<T>* current = head;        
    while (current && current->getNext() && priority >= current->getNext()->getPri()) {
        current = current->getNext();
    }
    newNode->setNext( current->getNext());
    current->setNext( newNode); 
    this->increment_entries();       
}

template <typename T>
bool Min_priQueue<T>::cancel_car(int patientID, T& car){
    priNode<T>* nodeToDeletePtr=nullptr, advance = this->get_head();
	int pri;
    if(this->isEmpty()){return false;}
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
    this->decrement_entries();
	return true;
}
