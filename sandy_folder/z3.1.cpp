#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <set>
#include <functional>

struct Printer {
    std::string str() const {
        return _ss.str();
    }

    template<typename T>
    Printer &format(const T& x) {
        _ss << x;
        return *this;
    }

    template<typename T>
    Printer &format(const std::vector<T> &x) {
        if (x.size() == 0) {
            return this->format("[]");
        }
        return this->format("[ ").formatIterable(x).format(" ]");
    }

    template<typename T>
    Printer &format(const std::set<T> &x) {
        if (x.size() == 0) {
            return this->format("{}");
        }
        return this->format("{ ").formatIterable(x).format(" }");
    }

    template<typename U, typename V>
    Printer &format(const std::pair<U, V> &x) {
        return this->format("( ").format(x.first).format(", ").format(x.second).format(" )");
    }

    template<typename ...T, std::enable_if_t<(sizeof...(T) == 0), int> = 0>
    Printer &format(const std::tuple<T...>&) {
        return this->format("()");
    }

    template<typename ...T, std::enable_if_t<(sizeof...(T) > 0), int> = 0>
    Printer &format(const std::tuple<T...> &x) {
        return this->format("( ").formatTuple<decltype(x), sizeof...(T)>(x).format(" )");
    }

private:
    template<typename Tuple, std::size_t N, std::enable_if_t<(N > 1), int> = 0>
    Printer &formatTuple(const Tuple& t) {
        return this->formatTuple<Tuple, N-1>(t).format(", ").format(std::get<N-1>(t));
    }

    template<typename Tuple, std::size_t N, std::enable_if_t<(N == 1), int> = 0>
    Printer &formatTuple(const Tuple& t) {
        return this->format(std::get<0>(t));
    }

    template<typename T>
    Printer &formatIterable(const T& x) {
        auto i = x.begin();
        while(true) {
            this->format(*i);
            ++i;
            if (i != x.end()) {
                this->format(", ");
            } else {
                break;
            }
        }
        return *this;
    }

private:
    std::stringstream _ss;
};

int main()
{
    auto a = std::make_tuple();
    std::set<std::set<int>> s = {{1}, {2}};
    std::tuple<std::tuple<>, int, int> t = {a, 1, 2};
    std::vector<std::pair<int, int>> v = {{1, 4}, {5, 6}};
    std::string s1 = Printer().format("  vector:  ").format(v).str();
    std::string s2 = Printer().format(t).format("  !  ").format(0).str();
    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;
    std::cout << Printer().format(a).str() << std::endl;
    std::cout << Printer().format(s).str() << std::endl;
    std::cout << Printer().format(std::set<int>()).str() << std::endl;
    std::cout << Printer().format(std::vector<int>()).str() << std::endl;
    std::cout << Printer().format(std::make_tuple(std::make_tuple())).str() << std::endl;
    return 0;
}
