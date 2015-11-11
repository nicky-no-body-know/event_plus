#ifndef EVENT_PLUS_INCLUDE_EVENT_PLUS_INTERNAL_H
#define EVENT_PLUS_INCLUDE_EVENT_PLUS_INTERNAL_H

#include <cstdlib> 

namespace event_plus //namespace event_plus start
{ 
namespace internal  //namespace internal start
{ 

template <class T>
class ListNode {
	typedef ListNode<T> __Myt;
	typedef T value_type;
	typedef T& reference_type;
public:
	ListNode(reference_type t) : data_(t), prev_(NULL), next_(NULL) {
	}
	~ListNode() { prev_= next_ = NULL; }

	__Myt* next() { return next_; }
	void set_next(__Myt *next) { next_ = next; }

	__Myt* prev() { return prev_; }
	void set_prev(__Myt *prev) { prev_ = prev; }

	reference_type data() { return data_; }
	void set_data(reference_type data) { data_ = data;}
	void clear_data() { delete &data_; }
private:
	reference_type data_;	
	__Myt *prev_;
	__Myt *next_;
};

template <class T>
class ListNode<T*> {
	typedef ListNode<T*> __Myt;
	typedef T value_type;
	typedef T* pointer_type;
public:
	ListNode(pointer_type t) : prev_(NULL), next_(NULL) {
		data_ = t;
	}
	~ListNode() { prev_ = next_ = NULL; }

	__Myt* next() { return next_; }
	void set_next(__Myt *next) { next_ = next; }

	__Myt* prev() { return prev_; }
	void set_prev(__Myt *prev) { prev_ = prev; }

	pointer_type data() { return data_; }
	void set_data(pointer_type data) { data_ = data;}
	void clear_data() { delete data_; }
private:
	pointer_type data_;
	__Myt* next_;
	__Myt* prev_;
};

template<class T>
class List {
	typedef T value_type;
	typedef T& reference_type;
	typedef ListNode<T> Node;
	typedef void (*InsertFunctor)(Node* node);

public:
	List() : count_(0), head_(NULL), tail_(NULL) {}
	~List() { ClearAll(); }

	bool empty() { return head_ == NULL; }
	int count() { return count_; }

	// 在List尾端插入元素，同时返回t的容器指针
	Node* InsertTail(reference_type t);
	// 从头端取出一个元素，需要调用者释放该数据
	reference_type PopFront();
	// 删除一个元素，数据自动释放
	int Remove(Node *node);
	void ClearAll() { 
		while (head_->next()) {
			Remove(head_->next());
		}
	}
private:
	int count_;
	Node *head_;
	Node *tail_;
};

template <class T>
class List<T*> {
	typedef T value_type;
	typedef T* pointer_type;
	typedef ListNode<T> Node;
	typedef void (*InsertFunctor)(Node* node);
public:
	List() : count_(0), head_(NULL), tail_(NULL) {}
	~List() { ClearAll(); }

	bool empty() { return head_ == NULL; }
	int count() { return count_; }

	// 在List尾端插入元素，同时返回t的容器指针
	Node* InsertTail(pointer_type t);
	// 从头端取出一个元素，需要调用者释放该数据
	pointer_type PopFront();
	// 删除一个元素，数据自动释放
	int Remove(Node *node);
	void ClearAll()	{
		while(head_->next()) {
			Remove(head_->next());
		}
	}
private:
	int count_;
	Node *head_;
	Node *tail_;
};

} // namespace internal end
} // namespace event_plus start
#endif

