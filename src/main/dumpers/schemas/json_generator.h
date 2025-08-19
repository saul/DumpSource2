#pragma once
#include <sstream>
#include <string>
#include <type_traits>
#include <stack>
#include <fmt/format.h>

namespace json {
    constexpr char kTabSym = ' ';
    constexpr std::size_t kTabsPerBlock = 2; // @note: @es3n1n: how many characters shall we place per each block
    constexpr std::initializer_list<char> kBlacklistedCharacters = {':', ';', '\\', '/'};

    struct generator_t {
        using self_ref = std::add_lvalue_reference_t<generator_t>;
        
        enum class block_type_t {
            object,
            array
        };
        
        struct block_state_t {
            block_type_t type;
            bool needs_comma = false;
        };
        
    public:
        constexpr generator_t() = default;
        //constexpr ~generator_t() = default;
        constexpr self_ref operator=(self_ref v) {
            return v;
        }
    public:
        self_ref begin_json_object() {
            write_comma_if_needed();
            push_line("{");
            inc_tabs_count(kTabsPerBlock);
            _block_stack.push({block_type_t::object, false});
            return *this;
        }

        self_ref begin_json_object_value() {
            _stream << "{" << std::endl;
            inc_tabs_count(kTabsPerBlock);
            _block_stack.push({block_type_t::object, false});
            return *this;
        }

        self_ref end_json_object() {
            dec_tabs_count(kTabsPerBlock);
            push_line("}");
            if (!_block_stack.empty()) {
                _block_stack.pop();
            }
            mark_item_written();
            return *this;
        }

        self_ref begin_json_array_value() {
            _stream << "[" << std::endl;
            inc_tabs_count(kTabsPerBlock);
            _block_stack.push({block_type_t::array, false});
            return *this;
        }

        self_ref end_json_array() {
            dec_tabs_count(kTabsPerBlock);
            push_line("]");
            if (!_block_stack.empty()) {
                _block_stack.pop();
            }
            mark_item_written();
            return *this;
        }

        self_ref json_property_name(const std::string& str) {
            write_comma_if_needed();
            return push_line("\"" + escape_json_string(str) + "\": ", false);
        }

        self_ref json_string_value(const std::string& str) {
            _stream << "\"" + escape_json_string(str) + "\"" << std::endl;
            mark_item_written();
            return *this;
        }

        template <typename T>
        self_ref json_literal_value(T value) {
            _stream << fmt::format("{}", value) << std::endl;
            mark_item_written();
            return *this;
        }

    public:
        [[nodiscard]] std::string str() {
            return _stream.str();
        }

        self_ref push_line(const std::string& line, bool move_cursor_to_next_line = true) {
            for (std::size_t i = 0; i < _tabs_count; i++)
                _stream << kTabSym;
            _stream << line;
            if (move_cursor_to_next_line)
                _stream << std::endl;
            return *this;
        }
        
    private:
        void write_comma_if_needed() {
            if (!_block_stack.empty() && _block_stack.top().needs_comma) {
                _stream << ",";
            }
            _stream << std::endl;
        }
        
        void mark_item_written() {
            if (!_block_stack.empty()) {
                _block_stack.top().needs_comma = true;
            }
        }
        
        std::string escape_json_string(const std::string& input) {
            std::ostringstream ss;
            for (auto c : input) {
                switch (c) {
                case '\\':
                    ss << "\\\\";
                    break;
                case '"':
                    ss << "\\\"";
                    break;
                case '/':
                    ss << "\\/";
                    break;
                case '\b':
                    ss << "\\b";
                    break;
                case '\f':
                    ss << "\\f";
                    break;
                case '\n':
                    ss << "\\n";
                    break;
                case '\r':
                    ss << "\\r";
                    break;
                case '\t':
                    ss << "\\t";
                    break;
                default:
                    if ('\x00' <= c && c <= '\x1f') {
                        ss << "\\u" << std::hex << (int)c;
                    } else {
                        ss << c;
                    }
                }
            }
            return ss.str();
        }

    public:
        self_ref inc_tabs_count(std::size_t count = 1) {
            _tabs_count_backup = _tabs_count;
            _tabs_count += count;
            return *this;
        }

        self_ref dec_tabs_count(std::size_t count = 1) {
            _tabs_count_backup = _tabs_count;
            if (_tabs_count)
                _tabs_count -= count;
            return *this;
        }
    private:
        std::stringstream _stream = {};
        std::size_t _tabs_count = 0, _tabs_count_backup = 0;
        std::stack<block_state_t> _block_stack = {};
    };

    generator_t get() {
        return generator_t{};
    }
} // namespace codegen