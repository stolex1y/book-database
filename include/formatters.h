#pragma once

#include <flat_map>
#include <format>

template <typename K, typename V, typename Cmp, typename VecKey, typename VecVal, typename CharT>
struct std::formatter<std::flat_map<K, V, Cmp, VecKey, VecVal>, CharT> : std::formatter<std::string, CharT> {
    template <typename FormatContext>
    auto format(const std::flat_map<K, V, Cmp, VecKey, VecVal> &map, FormatContext &ctx) const {
        std::string out;
        out += "{\n\t";

        bool first = true;
        for (auto const &[k, v] : map) {
            if (!first)
                out += ",\n\t";
            first = false;
            out += std::format("{}: {}", k, v);
        }

        out += "\n}";
        return std::formatter<std::string, CharT>::format(out, ctx);
    }
};

template <typename V, typename CharT>
struct std::formatter<std::vector<V>, CharT> : std::formatter<std::string, CharT> {
    template <typename FormatContext>
    auto format(const std::vector<V> &vec, FormatContext &ctx) const {
        std::string out;
        out += "{\n\t";

        bool first = true;
        for (auto const &v : vec) {
            if (!first)
                out += ",\n\t";
            first = false;
            out += std::format("{}", v);
        }

        out += "\n}";
        return std::formatter<std::string, CharT>::format(out, ctx);
    }
};

template <typename V, typename CharT>
struct std::formatter<std::reference_wrapper<V>, CharT> : std::formatter<std::string, CharT> {
    template <typename FormatContext>
    auto format(const std::reference_wrapper<V> &vec, FormatContext &ctx) const {
        return format_to(ctx.out(), "{}", vec.get());
    }
};
