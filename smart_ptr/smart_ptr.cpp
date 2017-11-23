/*************************************************************************
	> File Name: boost.cpp
	> Author: 
	> Description: The use of the boost
	> Created Time: Sun 22 Oct 2017 10:48:21 PM EDT
 ************************************************************************/

#include<boost/shared_ptr.hpp>
#include<boost/scoped_ptr.hpp>
#include<boost/weak_ptr.hpp>        //shared_ptr 循环使用!
#include<boost/intrusive_ptr.hpp>

#include<string.h>
#include<iostream>
using namespace std;
using namespace boost;

class Human
{
    string m_name;
    //shared_ptr<Human> m_lover;
    weak_ptr<Human> m_lover;
public:
    Human(string name = "Human"):m_name(name) { cout << "Construct: " << m_name << endl; }
    ~Human()    { cout << "Deconstruct: " << m_name << endl;  }

    void loves(shared_ptr<Human> other)
    {
        m_lover = other;
        cout<< m_name << " loves " <<other->getName() 
            << " whose use_count = " << other.use_count() << endl;
    }

    string getName() const {    return m_name; }
};


int main(int argc, char *argv[])
{
    cout<<"int: "<<sizeof(int)<<" char*: "<<sizeof(char*)
        <<"\nscoped_ptr<int> :"<<sizeof(scoped_ptr<int>)
        <<"\nshared_ptr<int>: "<<sizeof(shared_ptr<int>)
        <<"\nweak_ptr<int>:   "<<sizeof(weak_ptr<int>)
        <<"\nintrusive_ptr<int>: "<<sizeof(intrusive_ptr<int>)
        <<"\nchar: "<<sizeof(shared_ptr<char>)<<endl;

    //shared_ptr<char> str0((char*)"hello world");    //析构不行，智能指针只能处理堆变量。将其它变量传给他不会有编译错误，但是运行会产生未知结果。
    shared_ptr<string> str(new string("hello world"));

    cout<<"Output instance: "<<*str<<endl;

    cout<<"\n cycle reference : "<<endl;
    
    shared_ptr<Human> I(new Human("jacket"));
    shared_ptr<Human> you(new Human("Angela"));
    shared_ptr<Human> he(new Human("ivany"));

    cout<< "First, counts: "
        << I.use_count() << ' '
        << you.use_count() << ' '
        << he.use_count() << ' ' <<endl;

    I->loves(you);  //智能指针是指针又是对象!
    you->loves(he);
    he->loves(I); //循环引用会导致不析构的!

    cout<< "Later, counts:"
        << I.use_count() << ' '
        << you.use_count() << ' '
        << he.use_count() << ' ' << endl;


    scoped_ptr<char> chSmart((char*)malloc(100));
    strncpy(chSmart.get(), "I'm a scoped_ptr!", 100);

    cout<<chSmart.get()<<endl;


    return 0;
    




    
}
