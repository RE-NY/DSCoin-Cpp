#ifndef PAIR_HPP
#define PAIR_HPP

template<typename K, typename V>
class Pair {
public:
    K first;
    V second;

    Pair(K _first, V _second) : first(_first), second(_second) {}

    K get_first() const {
        return first;
    }

    V get_second() const {
        return second;
    }
};

#endif // PAIR_HPP
