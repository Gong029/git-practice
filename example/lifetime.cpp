#include <iostream>
#include <string>
#include <utility>

struct Trace {
    std::string name;

    explicit Trace(std::string n) : name(std::move(n)) {
        std::cout << "+ " << name << '\n';
    }

    ~Trace() {
        std::cout << "- " << name << '\n';
    }
};

int main() {
    Trace a("A");

    {
        Trace b("B");
    }

    std::cout << "end of main body\n";
}
