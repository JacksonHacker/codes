#ifndef VECTOR_H
#define VECTOR_H

#include <memory>             //������allocator���Լ���allocate���׵��㷨�磺uninitialized_copy(b, e, b2)����Щ�㷨����һ���Թ�����Ԫ��
#include <utility>            //������pair���ͣ�move����
#include <initializer_list>   //�����˱�׼������initializer_list
#include <iterator>           //�������ƶ�������
#include "Algorithm.h"        //������for_each�㷨

template <typename T>
class Vector {
public:
	typedef T* iterator;
	typedef const T* const_iterator;
	Vector() : elements(nullptr), first_free(nullptr), cap(nullptr) { }  //��ʾ�ؽ�ָ���ʼ��Ϊnullptr
	Vector(std::initializer_list<T> il);                                 //���б��ʼ���Ĺ��캯��
	explicit Vector(size_t n);
	Vector(const Vector&);                                               //�������캯��
	Vector(Vector&&) noexcept;                                           //�ƶ����캯�����ƶ������в�Ӧ�׳��κ��쳣
	Vector& operator=(const Vector&);                                    //������ֵ�����
	Vector& operator=(Vector&&) noexcept;                                //�ƶ���ֵ��������ƶ������в�Ӧ�׳��κ��쳣
	~Vector();                                                           //��������
	
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
	
	iterator begin() { return elements; }                                 //begin()��end()�ķ��������ɶ����Ƿ��ǳ�������
	const_iterator begin() const { return elements; }        
	const_iterator cbegin() const { return elements; }                    //��ʱ����������Ĭ����Ϊ����������Ҫ�ġ��������ֻ���������д�Ļ������ʹ�ó�������
	iterator end() { return first_free; }               
	const_iterator end() const { return first_free; }
	const_iterator cend() const { return first_free; }
private:
	static std::allocator<T> alloc;                                             //���Է���T��allocator����������Ϊ����ΪT�Ķ�������ڴ�
	//�����ĸ������������û�����ֱ�ӵ��ã����ޱ����ڵĳ�Ա�������ã���������Ϊprivate
	void chk_n_alloc() { if (cap == first_free) reallocate(); }                 //�����Ԫ�صĺ���ʹ��
	std::pair<iterator, iterator> alloc_n_copy(const_iterator, const_iterator); //���ߺ��������������캯������ֵ���������������ʹ�á����������ڴ沢���������һ��Ԫ�صĸ���
	void free();                    //����Ԫ�ز��ͷ��ڴ�
	void reallocate();              //��ø����ڴ沢��������Ԫ��
	void reallocate(size_t newcapacity);
	iterator elements;              //ָ��������Ԫ�ص�ָ��
	iterator first_free;            //ָ�������һ������Ԫ�ص�ָ��
	iterator cap;                   //ָ������β��λ�õ�ָ��
};
template <typename T> 
std::allocator<T> Vector<T>::alloc; //��Ϊ��̬���ݳ�Ա����������κ�һ�������������ǲ������ڴ�����Ķ���ʱ������ġ�����ζ�����ǲ�������Ĺ��캯����ʼ���ġ���̬��Ա������ڲ������Ǳ������ˣ�δ�����壬��ȻҲû�б���ʼ��
                                    //���Ǿ�̬���ݳ�Ա������ֵ��������constexpr��������������ڲ���ʼ����̬��Ա
									//��̬���ݳ�Աֻ�ܱ�����һ�Σ���һ�������壬�ͻ�һֱ�����ڳ�����������������У�������ȫ�ֱ�����������ȷ����ֻ������һ�Σ���ý���̬���ݳ�Ա�Ķ��������������������Ķ������ͬһ���ļ���
                                    //ÿһ����ģ��Vector��ʵ���������Լ���static��Աʵ����������һ��T��������Vector<T>���͵Ķ�������ͬ��Vector<T>::alloc�����ԣ���static���ݳ�ԱҲ�����ģ�塣
                                    //�����ṩ��һ��������ʼֵ�ľ�̬��Ա�Ķ���
                                    //����������ⶨ�徲̬��Ա����ģ��ʵ������ʱ�򣬻ᷢ��������صĴ������ڱ���������ʵ�����Ĳ�ͬ������������������ʱ�ű��档���磬������þ�̬��Ա���ᱨ��undefined reference to 'Vector<int>::alloc'
//����ֻ�е�����ʵ������ģ���һ���ض��汾ʱ(�����Ƕ���ģ��ʱ)���������Ż����ɴ��롣��ˣ�Ϊ���������ʵ�����汾����������������Ҫ���պ������������ඨ�壬����Ҫ���պ���ģ�����ģ���Ա�����Ķ��塣���ԣ�����ģ�����ģ���Ա�����Ķ���ͨ������ͷ�ļ���
template <typename T>               
inline void Vector<T>::push_back(const T& e) {
	chk_n_alloc();                                     //ȷ���пռ�������Ԫ�أ������Ҫ�������reallocate����
	alloc.construct(first_free++, e);                  //��first_freeָ���Ԫ���й���e�ĸ���;�����T�Ŀ������캯��
}
template <typename T>
inline void Vector<T>::push_back(T &&e) {
	chk_n_alloc();
	alloc.construct(first_free++, std::move(e));       //construct����ʹ�õڶ���������ʵ��������ȷ��ʹ���ĸ����캯����
	                                                   //����move����һ����ֵ���ã����ݸ�construct��ʵ��������T&&����ˣ������T���ƶ����캯��
}
template <typename T>
inline std::pair<T*, T*> Vector<T>::alloc_n_copy(const_iterator b, const_iterator e) {
	auto data = alloc.allocate(e - b);                 //����һ��ԭʼ�ġ�δ������ڴ棬���Ա���e-b������ΪT�Ķ���δ������ڴ治��ʹ��
	return {data, std::uninitialized_copy(b, e, data)};//C++11����Է���ֵ�����б��ʼ���������ػ����Ű�Χ��ֵ���б��������������ؽ�����˴����б�Ҳ�����Ա�ʾ�������ص���ʱ�����г�ʼ����
                                                       //uninitialized_copy�����ӵ�����b��eָ�������뷶Χ�п���Ԫ�ص�������data��ָ����δ�����ԭʼ�ڴ��С�uninitialized_copy����һ��ָ�����һ�������Ԫ��֮���λ�õ�ָ��
}
template <typename T>
inline void Vector<T>::free() {
	if (elements) {                  /*                //���ܴ��ݸ�deallocate����һ����ָ�룬���Ա�����elements�Ƿ�Ϊnullptr�������ж�
		for (auto p = first_free; p != elements; )     //��������Ԫ��
			alloc.destroy(--p);      */                //��ָ����pǰ���ָ����ָ�Ķ���ִ����������
		for_each(elements, first_free, [](T &x){ alloc.destroy(&x); });
													   //��for_each��lambda������forѭ��������汾ֻ��Ҫָ����Χ���Է�Χ�е�Ԫ��ִ��ʲô�������ɣ���for�汾�������ԱС�Ŀ���ָ�������
		alloc.deallocate(elements, cap - elements);    //�ͷ��ڴ�ռ䣻���ݸ�deallocate��ָ�벻��Ϊ�գ�����ָ��allocate������ڴ�(��allocate�������ص�ָ��)��
		                                               //���ң����ݸ�deallocate�Ĵ�С�������������allocate�����ڴ�ʱ�ṩ�Ĵ�С��������һ����ֵ�������ڵ���deallocate֮ǰ�������ȶ�ÿ���ڴ���д����Ķ������destroy
	}
}
template <typename T>
inline Vector<T>::Vector(std::initializer_list<T> il) {
	auto newdata = alloc_n_copy(il.begin(), il.end()); //����alloc_n_copy����ռ䲢����Ԫ��
	elements = newdata.first;
	first_free = cap = newdata.second;
}
template <typename T> 
inline Vector<T>::Vector(size_t n) {                   //explicit�ؼ���ֻ������������ڵĹ��캯��������
	elements = alloc.allocate(n);                  
	std::uninitialized_fill_n(elements, n, T());      
	first_free = cap = elements + n;
}
template <typename T> 
inline Vector<T>::Vector(const Vector &v) {
	auto newdata = alloc_n_copy(v.begin(), v.end());   //����alloc_n_copy����ռ䲢����Ԫ��
	elements = newdata.first;
	first_free = cap = newdata.second;
}
template <typename T>
inline Vector<T>::Vector(Vector &&v) noexcept : elements(v.elements), first_free(v.first_free), cap(v.cap) {
	v.elements = v.first_free = v.cap = nullptr;       //���������Դ�ƶ����ƶ����캯��������ȷ���ƺ�Դ��������һ��״̬(���������޺���)��һ����Դ����ƶ���Դ������벻��ָ���ƶ�����Դ����Ϊ��Դ������Ȩ�Ѿ������´����Ķ����ˡ�
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
	if (this != &rhs) {                                       //����Ը�ֵ
		free();
		elements = rhs.elements;
		first_free = rhs.first_free;
		cap = rhs.cap;
		rhs.elements = rhs.first_free = rhs.cap = nullptr;    //��v���ڿ�������״̬
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
		alloc.construct(dest++, std::move(*elem++));  //����move���صĽ������constructʹ��T���ƶ����캯�����������ǹ����ÿһ��T���͵�Ԫ�ض����elemָ���T���͵�Ԫ������ӹ��ڴ������Ȩ��
	                                                  //����һ���ƺ�Դ������в�ȷ����״̬���������std::move��Σ�յġ����ԣ����ƶ����캯�����ƶ���ֵ�������Щ��ʵ�ִ���֮��ĵط���ֻ�е���ȷ����Ҫ�����ƶ������Ҳ����ǰ�ȫ�ģ��ſ���ʹ��std::move
	free();                                           //���پ�Ԫ�ز��ͷ�Vectorԭ��ʹ�õ��ڴ�
	//����ָ��
	elements = newdata;
	first_free = dest;
	cap = elements + newcapacity;
	*/
	//��������ͨ��ʹ���ƶ���������uninitializer_copy������ʡȥ��forѭ�����鷳
	auto first = alloc.allocate(newcapacity);
	auto last = std::uninitialized_copy(std::make_move_iterator(begin()), std::make_move_iterator(end()), first);  
													  //make_move_iterator������һ����ͨ������ת��Ϊһ���ƶ���������
													  //uninitializer_copyʹ�õ������Ľ������������������������ȡԪ�أ����ƶ��������Ľ��������������һ����ֵ���ã�����ζ��construct��ʹ���ƶ����캯��������Ԫ�ء�
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
