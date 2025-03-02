#pragma once
#include<iostream>
#include <iterator>
#include <ranges>
#include<sstream>
#include<tuple>
#include<type_traits>

inline std::ostream &operator<<(std::ostream &out, __int128_t a) {
    if (!a) return out << 0;
    if (a < 0) {
        out << '-';
        a = -a;
    }
    static int b[40], bn;
    for (bn = 0; a; a /= 10)
        b[++bn] = a % 10;
    for (int i = bn; i > 0; --i)
        out << b[i];
    return out;
}

#define debug(...) suzukaze::Debug(suzukaze::Type::DEFAULT, __LINE__, #__VA_ARGS__ __VA_OPT__(,) __VA_ARGS__)

namespace suzukaze {
    inline constexpr std::size_t OUT_LEN = 1;
    inline constexpr std::size_t IN_LEN = 1;
    inline constexpr bool SPLIT = false;

    template<typename T> concept Tuple = requires (T v) { [] <typename ...Args>(const std::tuple<Args...> &tup) { }(v); };
    template<typename T> concept String = std::is_convertible_v<T, std::string_view>;

    enum class Type { DEFAULT, SEPERATE, ARRAY };

    struct Any { template<typename T> operator T(); };
    template <typename T>
        requires std::is_aggregate_v<T>
    consteval std::size_t size(auto &&...Args) {
        if constexpr (!requires{ T{ Args... }; })
            return sizeof...(Args) - 1;
        else
            return size<T>(Args..., Any{});
    }
    template<Tuple T>
    consteval std::size_t size() { return std::tuple_size_v<T>; }

    class Debug {
        inline static const std::string IN_SEP = ',' + std::string(IN_LEN, ' '), OUT_BLANK = std::string(OUT_LEN, ' '),
            OUT_SEP = OUT_BLANK + '|' + OUT_BLANK, EQUAL = OUT_BLANK + '=' + OUT_BLANK;
        std::string_view names;
        std::ostringstream sout;
        std::size_t idx = 0, cover = 0;

    public:
        template<typename T, typename ...Args>
        Debug(Type type, std::size_t line, std::string_view names, const T &arg, const Args &...args) : names(names) {
            sout << ".." << line << "..\t";
            if constexpr ((std::ranges::range<T> || std::input_iterator<T>) && !String<T> && (... && std::integral<Args>)) {
                if (type == Type::SEPERATE)
                    print(arg, false), (..., print(args, true));
                else
                    print(arg, false, args...);
            } else
                print(arg, false), (..., print(args, true));
            std::clog << sout.str() << std::endl;
        }

        Debug(Type, std::size_t, std::string_view) { std::clog << "-------------------------------" << std::endl; }

    private:
        void print_in_sep(bool flag = true) { if (flag) sout << IN_SEP; }

        template<typename T, std::integral ...Sizes>
        void print(const T &arg, bool flag, Sizes ...args) {
            if constexpr (SPLIT) {
                if (flag)
                    sout << OUT_SEP;
                for (; idx < names.size() && (cover || names[idx] != ','); idx++) {
                    cover += names[idx] == '(' || names[idx] == '<';
                    cover -= names[idx] == ')' || names[idx] == '>';
                    if (names[idx] != ' ')
                        sout << names[idx];
                }
                sout << EQUAL;
            } else {
                if (!flag)
                    sout << names << EQUAL;
                else
                    print_in_sep();
            }
            print_(arg, args...);
        }

        template<typename T, typename ...Sizes>
        void print_(const T &arg, std::size_t len = -1, Sizes ...args) {
            if constexpr (String<T>)
                sout << '"' << arg << '"';
            else if constexpr (std::input_iterator<T>) {
                sout << '[';
                auto it = arg;
                for (std::size_t idx = 0; idx < len; idx++, it++)
                    print_in_sep(idx), print_(*it, args...);
                sout << ']';
            } else if constexpr (std::ranges::range<T>) {
                sout << '[';
                auto it = std::begin(arg);
                for (std::size_t idx = 0; idx < len && it != std::end(arg); idx++, it++)
                    print_in_sep(idx), print_(*it, args...);
                sout << ']';
            } else if constexpr (requires {arg.pop();}) {
                sout << '[';
                auto tmp = arg;
                for (std::size_t idx = 0; idx < len && tmp.size(); idx++, tmp.pop())
                    if constexpr (requires {tmp.top();})
                        print_in_sep(idx), print_(tmp.top(), args...);
                    else
                        print_in_sep(idx), print_(tmp.front(), args...);
                sout << ']';
            } else if constexpr (std::is_aggregate_v<T> || Tuple<T>) {
                if constexpr (size<T>() <= 8) {
                    sout << "{("[Tuple<T>];
                    constexpr auto cnt = size<T>();
                    if constexpr (cnt == 1) {
                        const auto &[m1] = arg;
                        print_(m1);
                    } else if constexpr (cnt == 2) {
                        const auto &[m1, m2] = arg;
                        print_(m1), print_in_sep(), print_(m2);
                    } else if constexpr (cnt == 3) {
                        const auto &[m1, m2, m3] = arg;
                        print_(m1), print_in_sep(), print_(m2), print_in_sep(), print_(m3);
                    } else if constexpr (cnt == 4) {
                        const auto &[m1, m2, m3, m4] = arg;
                        print_(m1), print_in_sep(), print_(m2), print_in_sep(), print_(m3), print_in_sep(), print_(m4);
                    } else if constexpr (cnt == 5) {
                        const auto &[m1, m2, m3, m4, m5] = arg;
                        print_(m1), print_in_sep(), print_(m2), print_in_sep(), print_(m3), print_in_sep(), print_(m4), print_in_sep(), print_(m5);
                    } else if constexpr (cnt == 6) {
                        const auto &[m1, m2, m3, m4, m5, m6] = arg;
                        print_(m1), print_in_sep(), print_(m2), print_in_sep(), print_(m3), print_in_sep(), print_(m4), print_in_sep(), print_(m5), print_in_sep(), print_(m6);
                    } else if constexpr (cnt == 7) {
                        const auto &[m1, m2, m3, m4, m5, m6, m7] = arg;
                        print_(m1), print_in_sep(), print_(m2), print_in_sep(), print_(m3), print_in_sep(), print_(m4), print_in_sep(), print_(m5), print_in_sep(), print_(m6), print_in_sep(), print_(m7);
                    } else {
                        const auto &[m1, m2, m3, m4, m5, m6, m7, m8] = arg;
                        print_(m1), print_in_sep(), print_(m2), print_in_sep(), print_(m3), print_in_sep(), print_(m4), print_in_sep(), print_(m5), print_in_sep(), print_(m6), print_in_sep(), print_(m7), print_in_sep(), print_(m8);
                    }
                    sout << "})"[Tuple<T>];
                }
            } else if constexpr (requires {sout << arg;})
                sout << arg;
            else
                sout << "NoDebugType";
        }
    };
}