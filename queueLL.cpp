#include <iostream>
#include <stdexcept> // For exception handling
using namespace std;

// IMPROVEMENT: Make Node class templated to work with any data type
// WHY: Allows the queue to store any type, not just int
template<typename T>
class Node{
public:
    T val;           // FIXED: Changed from int to template type T
    Node* next;

    // IMPROVEMENT: Initialize next pointer to nullptr in constructor
    // WHY: Prevents undefined behavior from uninitialized pointer
    // ***const T& data EXPLANATION:***
    // • const T& = Pass by reference without copying + cannot modify original
    // • Much faster than copying entire object (especially for strings/large objects)
    Node(const T& data) : val(data), next(nullptr) {}  // FIXED: Use const T& for parameter
};

// IMPROVEMENT: Use template for generic queue
// WHY: Makes the queue reusable for any data type, not just int
template<typename T>
class Queue{
private:
    Node<T>* head;  // FIXED: Use templated Node<T> instead of Node
    Node<T>* tail;  // FIXED: Use templated Node<T> instead of Node
    size_t queueSize; // IMPROVEMENT: Track queue size for O(1) size() operation

public:
    // IMPROVEMENT: Use nullptr instead of NULL (C++11 standard)
    // WHY: nullptr is type-safe and more explicit than NULL
    Queue() : head(nullptr), tail(nullptr), queueSize(0) {}
    
    // IMPROVEMENT: Add destructor to prevent memory leaks
    // WHY: Automatically cleans up all nodes when queue is destroyed
    ~Queue(){
        while(!empty()){
            pop();
        }
    }

    // IMPROVEMENT: Use const parameter and better naming
    // WHY: Parameter shouldn't be modified, and 'enqueue' is standard terminology
    // ***IMPORTANT NOTE ABOUT const T& data:***
    // • const: Prevents modification of the parameter inside function
    // • T&: Pass by REFERENCE (alias) - NO copying during function call
    // • Benefits: Faster performance, memory efficient, type-safe
    // • Example: string s="hello"; push(s); - 's' is NOT copied, just referenced
    void push(const T& data){
        Node<T>* newNode = new Node<T>(data);  // FIXED: Use Node<T>* and Node<T>()
        
        if(empty()){
            head = newNode;
            tail = newNode;
        }
        else{
            tail->next = newNode;
            tail = newNode;
        }
        queueSize++; // IMPROVEMENT: Update size counter
    }

    // IMPROVEMENT: Add exception handling and better error management
    // WHY: Prevents undefined behavior when popping from empty queue
    void pop(){
        if(empty()){
            throw runtime_error("Cannot pop from empty queue");
        }
        
        Node<T>* temp = head;  // FIXED: Use Node<T>*
        head = head->next;
        
        // IMPROVEMENT: Handle case when queue becomes empty after pop
        // WHY: Ensures tail pointer is properly managed
        if(head == nullptr){
            tail = nullptr;
        }
        
        delete temp;
        queueSize--; // IMPROVEMENT: Update size counter
    }

    // IMPROVEMENT: Return const reference and add exception handling
    // WHY: Avoids unnecessary copying and provides clear error handling
    // 
    // ***EXPLANATION of const T& front() const:***
    // 
    // ┌─────────────────┬────────────────────────────────────────────────┐
    // │   const T&      │ Return Type: const reference to the data      │
    // │   (return)      │ • No copying of data (fast!)                  │
    // │                 │ • Caller cannot modify returned value         │
    // │                 │ • Direct access to actual node data           │
    // ├─────────────────┼────────────────────────────────────────────────┤
    // │   const         │ Function Qualifier: this function won't       │
    // │   (function)    │ modify the queue object                       │
    // │                 │ • Can be called on const queue objects        │
    // │                 │ • Compiler enforces no modifications          │
    // └─────────────────┴────────────────────────────────────────────────┘
    const T& front() const {
        if(empty()){
            throw runtime_error("Queue is empty");
        }
        return head->val;  // Returns reference to actual data, not copy
    }

    // IMPROVEMENT: Return const reference and add exception handling
    const T& back() const {
        if(empty()){
            throw runtime_error("Queue is empty");
        }
        return tail->val;
    }

    // IMPROVEMENT: Make function const and return bool
    // WHY: Function doesn't modify object, and bool is more semantic than int
    bool empty() const {
        return head == nullptr;
    }

    // IMPROVEMENT: Add size() function
    // WHY: Provides O(1) access to queue size without traversing
    size_t size() const {
        return queueSize;
    }

};

int main() {
        // IMPROVEMENT: Use improved Queue class with exception handling
        // 
        // ╔══════════════════════════════════════════════════════════════╗
        // ║                    📝 const T& EXPLANATION                    ║
        // ║                                                              ║
        // ║  const T& data means:                                        ║
        // ║  • const: Cannot modify the parameter inside function        ║
        // ║  • T&: Pass by REFERENCE (not by value)                     ║
        // ║  • No copying happens during function call = FASTER!        ║
        // ║                                                              ║
        // ║  Example: string s = "hello world";                         ║
        // ║           queue.push(s);  ← 's' is referenced, NOT copied   ║
        // ║                                                              ║
        // ║  Benefits: ⚡ Performance  💾 Memory Efficient  🔒 Safe      ║
        // ╚══════════════════════════════════════════════════════════════╝
        
        Queue<int> q;
        
        cout << "=== Testing Queue Operations ===" << endl;
        
        // Test empty queue
        cout << "Is queue empty? " << (q.empty() ? "Yes" : "No") << endl;
        cout << "Queue size: " << q.size() << endl;
        
        // Add elements
        cout << "\nAdding elements 1, 2, 3..." << endl;
        q.push(1);
        q.push(2);
        q.push(3);
        
        // Display queue state
        cout << "Front: " << q.front() << ", Back: " << q.back() << endl;
        cout << "Queue size: " << q.size() << endl;
        
    
    return 0;
}




//   1 2 3 4
//front     back
// head     tail
 //  1->2->3
//pop()   push()