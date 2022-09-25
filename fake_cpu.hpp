#include "fake_cpu.h"
#include <fstream>

void FakeCPU::get()
{
    std::string line;
    this->file.open("input.txt");
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            this->cut(line);
        }
    }
    else
    {
        std::cout << "File is not open!!!" << std::endl;
    }
    file.close();
}

void FakeCPU::cut(std::string &rhs)
{
    std::string operation = "";
    std::string reg = "";
    std::string src = "";
    if (rhs.find(':') != -1)
    {
        rhs.erase(rhs.begin() + rhs.find(':'));
        label[rhs] = this->file.tellg();
        return;
    }
    if (rhs.find(',') != -1)
    {
        rhs.erase(rhs.begin() + (rhs.find(',')));
    }
    int space = 0;
    for (int i = 0; i < rhs.size(); ++i)
    {
        if (rhs[i] == ' ')
        {
            ++space;
        }
    }
    if (space == 1)
    {
        for (int i = 0; i < rhs.size(); ++i)
        {
            if (rhs[i] == ' ')
            {
                operation += rhs.substr(0, i);
                rhs.erase(rhs.begin(), rhs.begin() + i + 1);
            }
        }
        reg += rhs;
    }
    if (space == 2)
    {
        for (int i = 0; i < rhs.size(); ++i)
        {
            if (rhs[i] == ' ')
            {
                operation += rhs.substr(0, i);
                rhs.erase(rhs.begin(), rhs.begin() + i + 1);
                reg += rhs.substr(0, rhs.find(' '));
                rhs.erase(rhs.begin(), rhs.begin() + rhs.find(' ') + 1);
            }
        }
        src += rhs;
    }
    (this->*Function[operation])(reg, src);
}

bool FakeCPU::is_register(std::string &str)
{
    for (int i = 0; i < _register.size(); ++i)
    {
        if (_register.find(str) == _register.end())
        {
            return false;
        }
    }
    return true;
}

bool FakeCPU::is_number(std::string &str)
{
    for (int i = 0; i < str.size(); ++i)
    {
        if (!(str[i] <= '9' && str[i] >= '0'))
        {
            return false;
        }
    }
    return true;
}

void FakeCPU::mov(std::string &dest, std::string &src)
{
    if (is_register(dest))
    {
        if ((!is_register(src)) && is_number(src))
        {
            *(_register[dest]) = stoi(src);
            return;
        }
        if (is_register(src))
        {
            *(_register[dest]) = *(_register[src]);
            return;
        }
    }
    throw std::invalid_argument("register not found");
}

void FakeCPU::add(std::string &dest, std::string &src)
{
    if (is_register(dest))
    {
        if ((!is_register(src)) && is_number(src))
        {
            *(_register[dest]) += stoi(src);
            return;
        }
        if (is_register(src))
        {
            *(_register[dest]) += *(_register[src]);
            return;
        }
    }
    throw std::invalid_argument("register not found");
}

void FakeCPU::mul(std::string &dest, std::string &src)
{
    if (is_register(dest))
    {
        if ((!is_register(src)) && is_number(src))
        {
            *(_register[dest]) *= stoi(src);
            return;
        }
        if (is_register(src))
        {
            *(_register[dest]) *= *(_register[src]);
            return;
        }
    }
    throw std::invalid_argument("register not found");
}

void FakeCPU::div(std::string &dest, std::string &src)
{
    if (is_register(dest))
    {
        if ((!is_register(src)) && is_number(src))
        {
            *(_register[dest]) /= stoi(src);
            return;
        }
        if (is_register(src))
        {
            *(_register[dest]) /= *(_register[src]);
            return;
        }
    }
    throw std::invalid_argument("register not found");
}

void FakeCPU::sub(std::string &dest, std::string &src)
{
    if (is_register(dest))
    {
        if ((!is_register(src)) && is_number(src))
        {
            *(_register[dest]) -= stoi(src);
            return;
        }
        if (is_register(src))
        {
            *(_register[dest]) -= *(_register[src]);
            return;
        }
    }
    throw std::invalid_argument("register not found");
}

void FakeCPU::_and(std::string &dest, std::string &src)
{
    if (is_register(dest))
    {
        if ((!is_register(src)) && is_number(src))
        {
            *(_register[dest]) &= stoi(src);
            return;
        }
        if (is_register(src))
        {
            *(_register[dest]) &= *(_register[src]);
            return;
        }
    }
    throw std::logic_error("Error");
}

void FakeCPU::_or(std::string &dest, std::string &src)
{
    if (is_register(dest))
    {
        if ((!is_register(src)) && is_number(src))
        {
            *(_register[dest]) |= stoi(src);
            return;
        }
        if (is_register(src))
        {
            *(_register[dest]) |= *(_register[src]);
            return;
        }
    }
    throw std::logic_error("Error");
}

void FakeCPU::_xor(std::string &dest, std::string &src)
{
    if (is_register(dest))
    {
        if ((!is_register(src)) && is_number(src))
        {
            *(_register[dest]) ^= stoi(src);
            return;
        }
        if (is_register(src))
        {
            *(_register[dest]) ^= *(_register[src]);
            return;
        }
    }
    throw std::logic_error("Error");
}

void FakeCPU::_not(std::string &dest, std::string &src)
{
    if (is_register(dest))
    {
        *(_register[dest]) = (~*(_register[dest]));
    }
}

void FakeCPU::jumps_impl(std::string &dest, std::string &src)
{
    if (is_number(src) && (src != "0"))
    {
        if (*(_register[dest]) == (stoi(src)))
        {
            Jumps["je"] = true;
            Jumps["jle"] = true;
            Jumps["jge"] = true;
            Jumps["jl"] = false;
            Jumps["jg"] = false;
            return;
        }
        else if (*(_register[dest]) < (stoi(src)))
        {
            Jumps["jl"] = true;
            Jumps["jle"] = true;
            Jumps["je"] = false;
            Jumps["jge"] = false;
            Jumps["jg"] = false;
            return;
        }
        else if (*(_register[dest]) > (stoi(src)))
        {
            Jumps["jg"] = true;
            Jumps["jge"] = true;
            Jumps["je"] = false;
            Jumps["jle"] = false;
            Jumps["jl"] = false;
            return;
        }
    }
    if (is_register(src))
    {
        if (*(_register[dest]) == *(_register[src]))
        {
            Jumps["je"] = true;
            Jumps["jle"] = true;
            Jumps["jge"] = true;
            Jumps["je"] = false;
            Jumps["jle"] = false;
            Jumps["jge"] = false;
            Jumps["jl"] = false;
            Jumps["jg"] = false;
            return;
        }
        else if (*(_register[dest]) < *(_register[src]))
        {
            Jumps["jl"] = true;
            Jumps["jle"] = true;
            Jumps["jne"] = true;
            Jumps["je"] = false;
            Jumps["jg"] = false;
            return;
        }
        else if (*(_register[dest]) > *(_register[src]))
        {
            Jumps["jg"] = true;
            Jumps["jge"] = true;
            Jumps["jne"] = true;
            Jumps["je"] = false;
            Jumps["jle"] = false;
            Jumps["jl"] = false;
            return;
        }
    }
}

void FakeCPU::jmp(std::string &dest, std::string &src)
{
    this->file.seekg(this->label[dest]);
}

void FakeCPU::cmp(std::string &dest, std::string &src)
{
    if ((!is_register(dest)) && is_number(dest) && is_register(src))
    {
        jumps_impl(src, dest);
    }
    else if (is_register(dest) && (is_number(src) || is_register(src)))
    {
        jumps_impl(dest, src);
    }
}

void FakeCPU::jl(std::string &dest, std::string &src)
{
    if (Jumps["jl"])
    {
        this->file.seekg(this->label[dest]);
    }
}

void FakeCPU::jle(std::string &dest, std::string &src)
{
    if (Jumps["jle"])
    {
        this->file.seekg(this->label[dest]);
    }
}

void FakeCPU::jg(std::string &dest, std::string &src)
{
    if (Jumps["jg"])
    {
        this->file.seekg(this->label[dest]);
    }
}

void FakeCPU::jge(std::string &dest, std::string &src)
{
    if (Jumps["jge"])
    {
        this->file.seekg(this->label[dest]);
    }
}

void FakeCPU::je(std::string &dest, std::string &src)
{
    if (Jumps["je"])
    {
        this->file.seekg(this->label[dest]);
    }
}

void FakeCPU::make()
{
   
    _register["reg1"] = &reg1;
    _register["reg2"] = &reg2;
    _register["reg3"] = &reg3;
    _register["reg4"] = &reg4;
    _register["reg5"] = &reg5;
    _register["reg6"] = &reg6;
    _register["reg7"] = &reg7;
    _register["reg8"] = &reg8;
    _register["reg9"] = &reg9;
    _register["reg10"] = &reg10;
    
    Function["mov"] = &FakeCPU::mov;
    Function["add"] = &FakeCPU::add;
    Function["mul"] = &FakeCPU::mul;
    Function["sub"] = &FakeCPU::sub;
    Function["div"] = &FakeCPU::div;
    Function["and"] = &FakeCPU::_and;
    Function["or"] = &FakeCPU::_or;
    Function["xor"] = &FakeCPU::_xor;
    Function["not"] = &FakeCPU::_not;
    Function["jmp"] = &FakeCPU::jmp;
    Function["je"] = &FakeCPU::je;
    Function["jl"] = &FakeCPU::jl;
    Function["jg"] = &FakeCPU::jg;
    Function["jle"] = &FakeCPU::jle;
    Function["jge"] = &FakeCPU::jge;
    Function["cmp"] = &FakeCPU::cmp;
   
    Jumps["je"] = false;
    Jumps["jle"] = false;
    Jumps["jge"] = false;
    Jumps["jl"] = false;
    Jumps["jg"] = false;
}