// copied from https://github.com/yalekseev/shared_ptr/blob/master/test.cpp and modified

#include "shared_ptr.h"

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


void test_copy_assignment() {
    util::SharedPtr<int> sp1(new int(111));
    util::SharedPtr<int> sp2;

    sp2 = sp1;

    assert(*sp1 == 111);
    assert(*sp1 == *sp2);
}

void test_copy_construction() {
    util::SharedPtr<int> sp1(new int(111));
    util::SharedPtr<int> sp2(sp1);

    assert(*sp1 == 111);
    assert(*sp1 == *sp2);
}

void test_get() {
    util::SharedPtr<int> sp(new int(111));

    assert(&(*sp) == sp.get());
}

void test_operator_less() {
    util::SharedPtr<int> sp1(new int(1));
    util::SharedPtr<int> sp2(new int(2));

    assert(&(*sp1) < &(*sp2) == sp1 < sp2);
}

void test_pointer_constructor() {
    util::SharedPtr<int> sp1(new int(1));
    assert(*sp1 == 1);

    util::SharedPtr<TestBased> sp2(new TestDerived);
    sp2->name() == "TestDerived";
}

void test_swap() {
    util::SharedPtr<int> sp1(new int(1));
    util::SharedPtr<int> sp2(new int(2));

    sp1.swap(sp2);

    assert(*sp1 == 2);
    assert(*sp2 == 1);
}

void test_deleter() {
    int val = 1;

    auto deleter = [&val](int *ptr) -> void { val = 2; };

    {
        util::SharedPtr<int> sp(new int(), deleter);
    }

    assert(val == 2);
}

void test_make_shared() {
    auto ptr = util::make_shared<int>(10);
    assert(*ptr == 10);
}

int main() {
    test_copy_assignment();
    test_copy_construction();
    test_get();
    test_operator_less();
    test_pointer_constructor();
    test_swap();
    test_deleter();
    test_make_shared();

    return 0;
}