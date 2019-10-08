#pragma once

#include <cstdint>

namespace vm
{
    enum class instruction_opcode : std::uint64_t
    {
        mov_reg_reg = 0,
        mov_reg_mem,
        mov_mem_reg,
        movv_adasd,

        add_reg_reg = 10,
        add_reg_mem,
        add_mem_reg,

        sub_reg_reg = 20,
        sub_reg_mem,
        sub_mem_reg,

        mul_reg_reg = 30,
        mul_reg_mem,
        mul_mem_reg,

        div_reg_reg = 40,
        div_reg_mem,
        div_mem_reg,

        inc_reg = 50,
        inc_mem,

        dec_reg = 60,
        dec_mem,

        cmp_reg_reg = 70,
        cmp_reg_mem,
        cmp_mem_reg,

        jmp = 80,
        je = 90,
        jl = 100,
        jg = 110

    };
}