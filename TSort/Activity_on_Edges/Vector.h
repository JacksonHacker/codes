#ifndef VECTOR_H
#define VECTOR_H

#include <memory>             //定义了allocator类以及与allocate配套的算法如：uninitialized_copy(b, e, b2)，这些算法可以一次性构造多个元素
#include <utility>            //定义了pair类型，move函数
#include <initializer_list>   //定义了标准库类型initializer_list
#include <iterator>           //定义了移动迭代器
#include "Algorithm.h"        //定义了for_each算法

template <typename T>
class Vector {
public:
	typedef T* iterator;
	typedef const T* const_iterator;
	Vector() : elements(nullptr), first_free(nullptr), cap(nullptr) { }  //显示地将指针初始化为nullptr
	Vector(std::initializer_list<T> il);                                 //可列表初始化的构造函数
	explicit Vector(size_t n);
	Vector(const Vector&);                                               //拷贝构造函数
	Vector(Vector&&) noexcept;                                           //移动构造函数：移动过程中不应抛出任何异常
	Vector& operator=(const Vector&);                                    //拷贝赋值运算符
	Vector& operator=(Vector&&) noexcept;                                //移动赋值运算符：移动过程中不应抛出任何异常
	~Vector();                                                           //析构函数
	
	T& operator[](size_t index) { return elements[index]; }
	const T& operator[](size_t index) const { return elements[index]; }
	
	void push_back(const T&);
	void push_back(T&&);
	void pop_back() { if(elements != first_free) alloc.destroy(--first_free); }
	const T& back() const { return *(first_free - 1); }
	
	size_t size() const { return first_free - elements; }
	size_t capacity() const { return cap - elements; }
	bool empty() const { return size() == 0; }
	
	void resize(size_t n);
	void resize(size_t n, const T& v);
	void reserve(size_t n) { if (n > capacity()) reallocate(n); }
	
	iterator begin() { return elements; }                                 //begin()和end()的返回类型由对象是否是常量决定
	const_iterator begin() const { return elements; }        
	const_iterator cbegin() const { return elements; }                    //有时候上面这种默认行为并非我们想要的。如果对象只需读而无须写的话，最好使用常量类型
	iterator end() { return first_free; }               
	const_iterator end() const { return first_free; }
	const_iterator cend() const { return first_free; }
private:
	static std::allocator<T> alloc;                                             //可以分配T的allocator对象，它可以为类型为T的对象分配内存
	//下面四个函数不允许被用户代码直接调用，仅限被类内的成员函数调用，所以声明为private
	void chk_n_alloc() { if (cap == first_free) reallocate(); }                 //被添加元素的函数使用
	std::pair<iterator, iterator> alloc_n_copy(const_iterator, const_iterator); //工具函数，被拷贝构造函数、赋值运算符和析构函数使用。用来分配内存并构造给定的一串元素的副本
	void free();                    //销毁元素并释放内存
	void reallocate();              //获得更多内存并拷贝已有元素
	void reallocate(size_t newcapacity);
	iterator elements;              //指向数组首元素的指针
	iterator first_free;            //指向数组第一个空闲元素的指针
	iterator cap;                   //指向数组尾后位置的指针
};
template <typename T> 
std::allocator<T> Vector<T>::alloc; //因为静态数据成员不属于类的任何一个对象，所以它们并不是在创建类的对象时被定义的。这意味着它们不是由类的构造函数初始化的。静态成员在类的内部仅仅是被声明了，未被定义，当然也没有被初始化
                                    //除非静态数据成员是字面值常量类型constexpr，否则不能在类的内部初始化静态成员
									//静态数据成员只能被定义一次，且一旦被定义，就会一直存在于程序的整个生命周期中（类似于全局变量）。若想确保它只被定义一次，最好将静态数据成员的定义与其他非内联函数的定义放在同一个文件中
                                    //每一个类模板Vector的实例都有其自己的static成员实例。即给定一个T，则所有Vector<T>类型的对象共享相同的Vector<T>::alloc。所以，将static数据成员也定义成模板。
                                    //这里提供了一个不带初始值的静态成员的定义
                                    //如果不在类外定义静态成员，则当模板实例化的时候，会发生类型相关的错误。由于编译器管理实例化的不同，这类错误可能在链接时才报告。例如，不定义该静态成员，会报出undefined reference to 'Vector<int>::alloc'
//由于只有当我们实例化出模板的一个特定版本时(而不是定义模板时)，编译器才会生成代码。因此，为了生成这个实例化版本，编译器不仅仅需要掌握函数的声明和类定义，还需要掌握函数模板或类模板成员函数的定义。所以，函数模板和类模板成员函数的定义通常放在头文件中
template <typename T>               
inline void Vector<T>::push_back(const T& e) {
	chk_n_alloc();                                     //确保有空间容纳新元素，如果需要，会调用reallocate函数
	alloc.construct(first_free++, e);                  //在first_free指向的元素中构造e的副本;会调用T的拷贝构造函数
}
template <typename T>
inline void Vector<T>::push_back(T &&e) {
	chk_n_alloc();
	alloc.construct(first_free++, std::move(e));       //construct函数使用第二个及随后的实参类型来确定使用哪个构造函数。
	                                                   //由于move返回一个右值引用，传递给construct的实参类型是T&&，因此，会调用T的移动构造函数
}
template <typename T>
inline std::pair<T*, T*> Vector<T>::alloc_n_copy(const_iterator b, const_iterator e) {
	auto data = alloc.allocate(e - b);                 //分配一段原始的、未构造的内存，可以保存e-b个类型为T的对象。未构造的内存不能使用
	return {data, std::uninitialized_copy(b, e, data)};//C++11允许对返回值进行列表初始化，即返回花括号包围的值的列表，类似于其他返回结果，此处的列表也用来对表示函数返回的临时量进行初始化；
                                                       //uninitialized_copy函数从迭代器b和e指出的输入范围中拷贝元素到迭代器data所指定的未构造的原始内存中。uninitialized_copy返回一个指向最后一个构造的元素之后的位置的指针
}
template <typename T>
inline void Vector<T>::free() {
	if (elements) {                  /*                //不能传递给deallocate函数一个空指针，所以必须有elements是否为nullptr的条件判断
		for (auto p = first_free; p != elements; )     //逆序销毁元素
			alloc.destroy(--p);      */                //该指令会对p前面的指针所指的对象执行析构函数
		for_each(elements, first_free, [](T &x){ alloc.destroy(&x); });
													   //用for_each和lambda来代替for循环，这个版本只需要指出范围及对范围中的元素执行什么操作即可，而for版本则需程序员小心控制指针的增减
		alloc.deallocate(elements, cap - elements);    //释放内存空间；传递给deallocate的指针不能为空，必须指向allocate分配的内存(由allocate函数返回的指针)。
		                                               //而且，传递给deallocate的大小参数必须与调用allocate分配内存时提供的大小参数具有一样的值。而且在调用deallocate之前，必须先对每个内存块中创建的对象调用destroy
	}
}
template <typename T>
inline Vector<T>::Vector(std::initializer_list<T> il) {
	auto newdata = alloc_n_copy(il.begin(), il.end()); //调用alloc_n_copy分配空间并构造元素
	elements = newdata.first;
	first_free = cap = newdata.second;
}
template <typename T> 
inline Vector<T>::Vector(size_t n) {                   //explicit关键字只允许出现在类内的构造函数声明处
	elements = alloc.allocate(n);                  
	std::uninitialized_fill_n(elements, n, T());      
	first_free = cap = elements + n;
}
template <typename T> 
inline Vector<T>::Vector(const Vector &v) {
	auto newdata = alloc_n_copy(v.begin(), v.end());   //调用alloc_n_copy分配空间并构造元素
	elements = newdata.first;
	first_free = cap = newdata.second;
}
template <typename T>
inline Vector<T>::Vector(Vector &&v) noexcept : elements(v.elements), first_free(v.first_free), cap(v.cap) {
	v.elements = v.first_free = v.cap = nullptr;       //除了完成资源移动，移动构造函数还必须确保移后源处于这样一个状态(销毁它是无害的)。一旦资源完成移动，源对象必须不再指向被移动的资源，因为资源的所有权已经归属新创建的对象了。
}
template <typename T>
inline Vector<T>::~Vector() { free(); }
template <typename T>
inline Vector<T>& Vector<T>::operator=(const Vector &rhs) {
	auto data = alloc_n_copy(rhs.begin(), rhs.end());
	free();
	elements = data.first;
	first_free = cap = data.second;
	return *this;
}
template <typename T>
inline Vector<T>& Vector<T>::operator=(Vector &&rhs) noexcept {
	if (this != &rhs) {                                       //检测自赋值
		free();
		elements = rhs.elements;
		first_free = rhs.first_free;
		cap = rhs.cap;
		rhs.elements = rhs.first_free = rhs.cap = nullptr;    //将v置于可析构的状态
	}
	return *this;
}
template <typename T>
inline void Vector<T>::reallocate() {
	auto newcapacity = size() ? 2 * size() : 1;
	reallocate(newcapacity);
}
template <typename T>
inline void Vector<T>::reallocate(size_t newcapacity) {
	/*
	auto newdata = alloc.allocate(newcapacity);
	auto dest = newdata;
	auto elem = elements;
	for (size_t i = 0; i != size(); ++i) 
		alloc.construct(dest++, std::move(*elem++));  //调用move返回的结果会令construct使用T的移动构造函数。这样我们构造的每一个T类型的元素都会从elem指向的T类型的元素那里接管内存的所有权。
	                                                  //由于一个移后源对象具有不确定的状态，对其调用std::move是危险的。所以，在移动构造函数和移动赋值运算符这些类实现代码之外的地方，只有当你确信需要进行移动操作且操作是安全的，才可以使用std::move
	free();                                           //销毁旧元素并释放Vector原来使用的内存
	//更新指针
	elements = newdata;
	first_free = dest;
	cap = elements + newcapacity;
	*/
	//下面这种通过使用移动迭代器和uninitializer_copy函数，省去了for循环的麻烦
	auto first = alloc.allocate(newcapacity);
	auto last = std::uninitialized_copy(std::make_move_iterator(begin()), std::make_move_iterator(end()), first);  
													  //make_move_iterator函数将一个普通迭代器转换为一个移动迭代器。
													  //uninitializer_copy使用迭代器的解引用运算符从输入序列中提取元素，而移动迭代器的解引用运算符生成一个右值引用，这意味着construct将使用移动构造函数来构造元素。
	free();
	elements = first;
	first_free = last;
	cap = elements + newcapacity;
}
template <typename T>
inline void Vector<T>::resize(size_t n) {
	if (n > size()) {
		while (size() < n) 
			push_back(T());	
	}
	else if (n < size()) {
		while (size() > n)
			alloc.destroy(--first_free);
	}
}
template <typename T>
inline void Vector<T>::resize(size_t n, const T& v) {
	if (n > size()) {
		while (size() < n)
			push_back(v);
	}
}
	

#endif
