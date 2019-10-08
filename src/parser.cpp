#include "../include/parser.h"
#include "../include/instruction_opcode.hpp"
#include "../include/register_opcode.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <charconv>
#include <cassert>

namespace vm
{
    const std::unordered_map<std::string_view, register_opcode> parser::m_registers = {
        { "r0", register_opcode::r0 },
        { "r1", register_opcode::r1 },
        { "r2", register_opcode::r2 },
        { "r3", register_opcode::r3 },
        { "r4", register_opcode::r4 },
        { "r5", register_opcode::r5 },
        { "r6", register_opcode::r6 },
        { "r7", register_opcode::r7 },
        { "r8", register_opcode::r8 },
        { "r9", register_opcode::r9 },
        { "r10", register_opcode::r10 },
        { "r11", register_opcode::r11 }, 
        { "r12", register_opcode::r12 },
        { "r13", register_opcode::r13 },
        { "r14", register_opcode::r14 },
        { "r15", register_opcode::r15 }
    };

    parser::parser(const std::vector<token>& tokens)
        : m_current{ std::cbegin(tokens) }
        , m_end{ std::cend(tokens)}
    {}

    parser::opcodes_t parser::parse()
    {
        opcodes_t result;

        while(!is_at_end())
        {
            const auto opcodes = get_next_opcodes();
            if(opcodes.empty())
            {
                return {};
            }

            result.insert(std::end(result), std::cbegin(opcodes), std::cend(opcodes)); //TODO learning
        }

        return result;
    }

    parser::opcodes_t parser::get_next_opcodes()
    {
        const auto curr = current();

        if(curr.str == "mov") { return get_mov(); }
        if(curr.str == "add") { return get_add(); }
        if(curr.str == "sub") { return get_sub(); }
        if(curr.str == "mul") { return get_mul(); }
        if(curr.str == "div") { return get_div(); }
        if(curr.str == "cmp") { return get_cmp(); }

        if(curr.str == "inc") { return get_inc(); }
        if(curr.str == "dec") { return get_dec(); }
        if(curr.str == "jmp") { return get_jmp(); }
        if(curr.str == "je") { return get_je(); }
        if(curr.str == "jl") { return get_jl(); }
        if(curr.str == "jg") { return get_jg(); }

        raise_error("Unknown instruction: " + std::string{ curr.str });
        return {};
    }

    parser::opcodes_t parser::get_mov()
    {
        return get_mem_reg_instruction(instruction_opcode::mov_reg_reg,
                                       instruction_opcode::mov_reg_mem,
                                       instruction_opcode::mov_mem_reg);
    }

    parser::opcodes_t parser::get_add()
    {
        return get_mem_reg_instruction(instruction_opcode::add_reg_reg,
                                       instruction_opcode::add_reg_mem,
                                       instruction_opcode::add_mem_reg);
    }

    parser::opcodes_t parser::get_sub()
    {
        return get_mem_reg_instruction(instruction_opcode::sub_reg_reg,
                                       instruction_opcode::sub_reg_mem,
                                       instruction_opcode::sub_mem_reg);
    }

    parser::opcodes_t parser::get_mul()
    {
        return get_mem_reg_instruction(instruction_opcode::mul_reg_reg,
                                       instruction_opcode::mul_reg_mem,
                                       instruction_opcode::mul_mem_reg);
    }

    parser::opcodes_t parser::get_div()
    {
        return get_mem_reg_instruction(instruction_opcode::div_reg_reg,
                                       instruction_opcode::div_reg_mem,
                                       instruction_opcode::div_mem_reg);
    }
 
    parser::opcodes_t parser::get_cmp()
    {
        return get_mem_reg_instruction(instruction_opcode::cmp_reg_reg,
                                       instruction_opcode::cmp_reg_mem,
                                       instruction_opcode::cmp_mem_reg);
    }

    parser::opcodes_t parser::get_inc()
    {
        return get_one_operand_instruction(instruction_opcode::inc_reg,
                                           instruction_opcode::inc_mem);
    }

    parser::opcodes_t parser::get_dec()
    {
        return get_one_operand_instruction(instruction_opcode::dec_reg,
                                           instruction_opcode::dec_mem);
    }

    parser::opcodes_t parser::get_jmp()
    {
        return get_jmp_instruction(instruction_opcode::jmp);
    }

    parser::opcodes_t parser::get_je()
    {
        return get_jmp_instruction(instruction_opcode::je);
    }

    parser::opcodes_t parser::get_jl()
    {
        return get_jmp_instruction(instruction_opcode::jl);
    }

    parser::opcodes_t parser::get_jg()
    {
        return get_jmp_instruction(instruction_opcode::jg);
    }

    parser::opcodes_t parser::get_jmp_instruction(instruction_opcode instruction)
    {
        const auto ip = parse_number();
        return make_opcodes(instruction, ip);
    }

    parser::opcodes_t parser::get_one_operand_instruction(instruction_opcode reg,
                                                          instruction_opcode mem)
    {
        eat(); // instruction
        if(is_current_register())
        {
            const auto reg_opcode = parse_register();
            if(!reg_opcode)
            {
                return {};
            }

            return make_opcodes(reg, *reg_opcode);
        }
        else
        {
            const auto mem_ptr = parse_memory_access();
            return make_opcodes(mem, mem_ptr);
        }
    }

    parser::opcodes_t parser::get_mem_reg_instruction(instruction_opcode reg_reg, instruction_opcode reg_mem, instruction_opcode mem_reg)
    {
        eat(); // instruction
        if(is_current_register())
        {
            const auto reg_opcode = parse_register();
            if(!reg_opcode)
            {
                return {};
            }
            eat(); // comma
                
            if(is_current_register())
            {
                const auto reg2_opcode = parse_register();
                if(!reg2_opcode)
                {
                    return {};
                }

                return make_opcodes(reg_reg, *reg_opcode, *reg2_opcode);
            }
            else
            {
                const auto mem_ptr = parse_memory_access();
                return make_opcodes(reg_mem, *reg_opcode, mem_ptr);
            }
        }
        else
        {
            const auto mem_ptr = parse_memory_access();
            eat(); //comma

            const auto reg_opcode = parse_register();
            if(!reg_opcode)
            {
                return {};
            }
            return make_opcodes(mem_reg, mem_ptr, *reg_opcode);
        }
    }

    template<typename... Opcodes>
    parser::opcodes_t parser::make_opcodes(Opcodes... opcodes) const //TODO learn
    {
        return opcodes_t{static_cast<uint64_t>(opcodes)... };
    }

    token parser::current() const
    {
        return *m_current;
    }

    std::optional<register_opcode> parser::parse_register()
    {
        const auto reg = current();
        const auto reg_opcode = str_to_reg(reg.str);
        if(!reg_opcode)
        {
            raise_error("Used uknown register: " + std::string{ reg.str });
            return {};
        }
        eat();  // register

        return reg_opcode;
    }

    uint16_t parser::parse_memory_access()
    {
        eat(); // [
        uint16_t mem_ptr = parse_number();
        eat(); // ]
        return mem_ptr;       
    }

    uint16_t parser::parse_number()
    {
        const auto mem_ptr_str = current().str;
        eat();
        return std::stoul(std::string{ mem_ptr_str });
    }

    void parser::eat()
    {
        std::advance(m_current, 1u);
    }

    std::optional<register_opcode> parser::str_to_reg(std::string_view str) const
    {
        const auto found = m_registers.find(str);
        if(found == std::cend(m_registers))
        {
            return std::nullopt;
        }

        return found->second;
    }

    void parser::raise_error(const std::string& msg)
    {
        std::cout<<msg<<std::endl;
    }

    bool parser::is_at_end() const
    {
        return m_current == m_end;
    }

    bool parser::is_current_register() const
    {
        return m_registers.find(current().str) != std::cend(m_registers);
    }
}