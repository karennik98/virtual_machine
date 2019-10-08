#pragma once

#include "token.hpp"

#include <vector>
#include <unordered_map>
#include <optional>

namespace vm
{
    enum class register_opcode : uint64_t;
    enum class instruction_opcode : uint64_t;

    class parser
    {
    public:
        using opcodes_t = std::vector<uint16_t>;

        explicit parser(const std::vector<token>& tokens);
        opcodes_t parse();

    private:
        bool is_at_end() const;
        bool is_current_register() const;

        opcodes_t get_next_opcodes();
        opcodes_t get_mem_reg_instruction(instruction_opcode reg_reg,
                                          instruction_opcode reg_mem,
                                          instruction_opcode mem_reg);
        opcodes_t get_one_operand_instruction(instruction_opcode reg,
                                              instruction_opcode mem);
        opcodes_t get_jmp_instruction(instruction_opcode instruction);
        opcodes_t get_mov();
        opcodes_t get_add();
        opcodes_t get_sub();
        opcodes_t get_mul();
        opcodes_t get_div();
        opcodes_t get_cmp();
        opcodes_t get_inc();
        opcodes_t get_dec();
        opcodes_t get_jmp();
        opcodes_t get_je();
        opcodes_t get_jl();
        opcodes_t get_jg();

        token current() const;

        void eat();

        uint16_t parse_number();
        uint16_t parse_memory_access();
        std::optional<register_opcode> parse_register();
        std::optional<register_opcode> str_to_reg(std::string_view str) const;

        void raise_error(const std::string& msg);

        template<typename... Opcodes>
        opcodes_t make_opcodes(Opcodes... opcodes) const;
    private:
        static const std::unordered_map<std::string_view, register_opcode> m_registers;

        std::vector<token>::const_iterator m_current;
        std::vector<token>::const_iterator m_end;
    };
} // namespace vm
