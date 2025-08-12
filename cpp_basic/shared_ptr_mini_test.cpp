#include "shared_ptr_mini.h"

#include <cassert>
#include <string>

class TestBased {
public:
    virtual std::string name() const {
        return "TestBased";
    }

    virtual ~TestBased() { }
};

class TestDerived : public TestBased {
public:
    virtual std::string name() const {
        return "TestDerived";
    }
};

class Person {
public:
    Person(int id, std::string&& name, int age) : id_(id), name_(name), age_(age) {}

    int id() const { return id_; }
    std::string name() const { return name_; }
    int age() const { return age_; }
private:
    int id_;
    std::string name_;
    int age_;
};

void test_copy_assignment() {
    SharedPtr<int> sp1(new int(111));
    SharedPtr<int> sp2;

    sp2 = sp1;

    assert(*sp1 == 111);
    assert(*sp1 == *sp2);
}

void test_copy_construction() {
    SharedPtr<int> sp1(new int(111));
    SharedPtr<int> sp2(sp1);

    assert(*sp1 == 111);
    assert(*sp1 == *sp2);
    assert(sp1.use_count() == (uint) 2);
    assert(sp2.use_count() == (uint) 2);
    sp1.reset(nullptr);
    assert(sp2.use_count() == (uint) 1);
}

void test_get() {
    SharedPtr<int> sp(new int(111));

    assert(&(*sp) == sp.get());
}

void test_pointer_constructor() {
    SharedPtr<int> sp1(new int(1));
    assert(*sp1 == 1);

    SharedPtr<TestDerived> sp2(new TestDerived);
    assert(sp2->name() == "TestDerived");
}

void test_swap() {
    SharedPtr<int> sp1(new int(1));
    SharedPtr<int> sp2(new int(2));

    sp1.swap(sp2);

    assert(*sp1 == 2);
    assert(*sp2 == 1);
}

void test_person() {
    SharedPtr<Person> p1(new Person(1, std::string("Alice"), 30));
    SharedPtr<Person> p2( new Person(2, std::string("Bob"), 25));
    assert(p1->id() == 1);
    assert(p2->name() == "Bob");
}

int main() {
    test_copy_assignment();
    test_copy_construction();
    test_get();
    test_pointer_constructor();
    test_swap();
    test_person();

    return 0;
}