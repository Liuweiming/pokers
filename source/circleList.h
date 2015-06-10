#pragma once
#include <functional>
template <typename value_type>
struct CirLstNode;
template <typename value_type>
class CircleList;
template <typename value_type>
class CirLstiterator;
template<typename value_type>
bool operator==(const CirLstNode<value_type> &lhs, const CirLstNode<value_type> &rhs){
	return (lhs.key == rhs.key);
}
template<typename value_type>
bool operator< (const CirLstNode<value_type> &lhs, const CirLstNode<value_type> &rhs){
	return (lhs.key < rhs.key);
}
template<typename value_type>
void swap(CirLstNode<value_type> &lhs, CirLstNode<value_type> &rhs){
	std::swap(lhs.right, rhs.right);
	std::swap(lhs.left, rhs.left);
	lhs.right->left = lhs;
	lhs.left->right = lhs;
	rhs.right->left = rhs;
	rhs.left->right = rhs;
}

template <typename value_type>
struct CirLstNode{
	typedef CirLstNode * ref_type;
	typedef typename std::size_t size_type;
	friend bool operator==<value_type>(const CirLstNode &lhs, const CirLstNode &rhs);
	friend bool operator< <value_type>(const CirLstNode &lhs, const CirLstNode &rhs);
	friend void swap<value_type>(CirLstNode &lhs, CirLstNode &rhs);
	value_type key;
	ref_type left, right;
	~CirLstNode(){
		left->right = nullptr;
		if (right != nullptr){
			delete right;
		}
	}
	CirLstNode() : key(key()), left(nullptr), right(nullptr){}
	CirLstNode(const value_type &val) : key(val), left(this), right(this){}
	CirLstNode(const value_type &val, ref_type left, ref_type right) :
		key(val), left(left), right(right){}
	void unionNode(ref_type rightNode){
		if (rightNode == nullptr)
			return;
		ref_type rright = rightNode->left;
		this->right->left = rright;
		rright->right = this->right;
		rightNode->left = this;
		this->right = rightNode;
	}
	void insertNode(ref_type leftNode){
		if (leftNode == nullptr)
			return;
		ref_type lleft = leftNode->left;
		lleft->right = this;
		this->left->right = leftNode;
		leftNode->left = this->left;
		this->left = lleft;
	}
	ref_type removeNode(){
		if (this->right != this){
			this->left->right = this->right;
			this->right->left = this->left;
			this->left = this->right = this;
		}
		return this;
	}
};

template<typename value_type>
void swap(CirLstiterator<value_type> &lhs, CirLstiterator<value_type> &rhs){
	std::swap(lhs.head, rhs.head);
	std::swap(lhs.nodeRef, rhs.nodeRef);
}
template<typename value_type>
bool operator==(const CirLstiterator<value_type> &lhs, const CirLstiterator<value_type> &rhs){
	if (lhs.nodeRef == rhs.nodeRef)
		return true;
	return false;
}
template<typename value_type>
bool operator!=(const CirLstiterator<value_type> &lhs, const CirLstiterator<value_type> &rhs){
	return !(lhs == rhs);
}
template<typename value_type>
CirLstiterator<value_type>pluseS(const CirLstiterator<value_type> &lhs, int n){
	auto ret = lhs;
	while (n != 0){
		ret++;
		--n;
	}
	return ret;
}
template<typename value_type>
CirLstiterator<value_type> decreS(const CirLstiterator<value_type> &lhs, int n){
	auto ret = lhs;
	while (n != 0){
		ret--;
		--n;
	}
	return ret;
}
template <typename value_type>
class CirLstiterator{
public:
	friend class CircleList < value_type >;
	typedef CirLstNode<value_type> * ref_type;
	friend void swap<value_type>(CirLstiterator &lhs, CirLstiterator &rhs);
	friend CirLstiterator pluseS<value_type>(const CirLstiterator &lhs, int n);
	friend CirLstiterator decreS<value_type>(const CirLstiterator &lhs, int n);
	friend bool operator==<value_type>(const CirLstiterator &lhs, const CirLstiterator &rhs);
	friend bool operator!=<value_type>(const CirLstiterator &lhs, const CirLstiterator &rhs);
private:
	ref_type head;
	ref_type nodeRef;
public:
	CirLstiterator() : head(nullptr), nodeRef(nullptr) {}
	CirLstiterator(ref_type head) : head(head), nodeRef(head) {}
	CirLstiterator(ref_type head, ref_type nodeR) :
		head(head), nodeRef(nodeR){}
	CirLstiterator(const CirLstiterator &old) :
		head(old.head), nodeRef(old.nodeRef){}
	~CirLstiterator(){}
	explicit operator bool() const{
		if (head == nullptr){
			return false;
		}
		return true;
	}
	CirLstiterator &operator = (CirLstiterator old){
		swap(*this, old);
		return *this;
	}
	CirLstiterator &operator++(){
		if (nodeRef->right != head){
			nodeRef = nodeRef->right;
		}
		else{
			nodeRef = head = nullptr;
		}
		return *this;
	}
	CirLstiterator &operator++(int){
		nodeRef = nodeRef->right;
		return *this;
	}
	CirLstiterator &operator--(){
		if (nodeRef != head){
			nodeRef = nodeRef->left;
		}
		else{
			nodeRef = head = nullptr;
		}
		return *this;
	}
	CirLstiterator &operator--(int){
		nodeRef = nodeRef->left;
		return *this;
	}
	value_type &operator*() const{
		return nodeRef->key;
	}
	value_type *operator->() const{
		return &this->operator*();
	}
};


template <typename value_type>
class CircleList{
public:
	typedef CirLstNode<value_type> * ref_type;
	typedef std::size_t size_type;
	typedef CirLstiterator < value_type > iterator;
private:
	ref_type head;
	size_type listNum;
public:
	CircleList() : head(nullptr), listNum(0){}
	~CircleList(){
		if (head != nullptr)
			delete head;
	}
	CircleList(value_type key) : head(nullptr), listNum(0) { cirLstInsert(key); }
	template <typename iter_type>
	CircleList(const iter_type &begin, const iter_type &end) : head(nullptr), listNum(0) {
		for (auto p = begin; p != end; ++p){
			cirLstInsert(*p);
		}
	}
	size_type size(){
		return listNum;
	}
	ref_type getHead(){
		return head;
	}
	void clear(){
		if (head != nullptr){
			delete head;
		}
		head = nullptr;
		listNum = 0;
	}
	void removeNode(iterator p){
		if (p.nodeRef == head){
			if (head->right == head)
				head = nullptr;
			else
				head = head->right;
		}
		auto ref = p.nodeRef->removeNode();
		delete ref;
		--listNum;
	}
	iterator begin() const {
		return iterator(head);
	}
	value_type front() const {
		return head->key;
	}
	iterator end() const {
		iterator it = iterator();
		return it;
	}
	value_type back() const{
		return head->left->key;
	}
	iterator rbegin() const {
		ref_type ret = head->left;
		return iterator(head, ret);
	}
	iterator rend() const {
		iterator it = iterator();
		return it;
	}
	void sort(){
		auto p = head;
		auto newHead = head;
		for (p = head->right; p != head;){
			auto q = p->left;
			auto ret = p->right;
			while (q != head && *p < *q){
				q = q->left;
			}
			
			if (q == head){
				if (*p < *q){
					p->removeNode();
					q->insertNode(p);
					newHead = p;
				}
			}
			if (q != p->left){
				p->removeNode();
				q->unionNode(p);;
			}
			p = ret;
		}
		head = newHead;
	}
	void sort(std::function<bool(const value_type &lhs, const value_type &rhs)> func){
		auto p = head;
		auto newHead = head;
		for (p = head->right; p != newHead;){
			auto q = p->left;
			auto ret = p->right;
			while (q != newHead && func(p->key, q->key)){
				q = q->left;
			}

			if (q == newHead){
				if (func(p->key, q->key)){
					p->removeNode();
					q->insertNode(p);
					newHead = p;
				}
			}
			if (q != p->left){
				p->removeNode();
				q->unionNode(p);;
			}
			p = ret;
		}
		head = newHead;
	}
	void cirLstInsert(value_type key){
		ref_type newNode = new CirLstNode<value_type>(key);
		if (head == nullptr){
			head = newNode;
		}
		else{
			head->insertNode(newNode);
			head = newNode;
		}
		++listNum;
	}
	iterator find(const value_type &key) const{
		for (auto p = begin(); p != end(); ++p){
			if (*p == key){
				return p;
			}
		}
		return iterator();
	}
	iterator find_if(const iterator &beg, const iterator &end,
		std::function<bool(const value_type &hs)> func) const{
		for (auto p = beg; p != end && p != this->end(); ++p){
			if (func(*p)){
				return p;
			}
		}
		return iterator();
	}
	iterator find_if_r(const iterator &beg, const iterator &end,
		std::function<bool(const value_type &hs)> func){
		for (auto p = beg; p != end && && p != this->rend(); --p){
			if (func(*p)){
				return p;
			}
		}
		return iterator();
	}
	CircleList &cirLstUnion(CircleList &rHeap){
		if (head == nullptr){
			head = rHeap.head;
		}
		else{
			head->unionNode(rHeap.head);
			if (rHeap.head != nullptr && rHeap.head->key < head->key)
				head = rHeap.head;
		}
		listNum += rHeap.listNum;
		rHeap.head = nullptr;
		rHeap.listNum = 0;
		return *this;
	}
};