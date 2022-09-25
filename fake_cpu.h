#ifndef FAKE_CPU_H
#define FAKE_CPU_H
#include <map>
#include <fstream>
class FakeCPU
{
public:
    FakeCPU() : reg1{}, reg2{}, reg3{}, reg4{}, reg5{}, reg6{}, reg7{}, reg8{}, reg9{}, reg10{}
    {
        make();
    }

public:
    typedef void (FakeCPU::*Func_Point)(std::string &, std::string &);
    void make();
    void get();
    void cut(std::string &);
    bool is_register(std::string &);
    bool is_number(std::string &);
public:
    void mov(std::string &, std::string &);
    void add(std::string &, std::string &);
    void mul(std::string &, std::string &);
    void div(std::string &, std::string &);
    void sub(std::string &, std::string &);
public:
    void jmp(std::string &, std::string &);
    void jumps_impl(std::string &, std::string &);
    void jl(std::string &, std::string &);
    void je(std::string &, std::string &);
    void jg(std::string &, std::string &);
    void jle(std::string &, std::string &);
    void jge(std::string &, std::string &);
    void cmp(std::string &, std::string &);
public:
    void _and(std::string &, std::string &);
    void _or(std::string &, std::string &);
    void _xor(std::string &, std::string &);
    void _not(std::string &, std::string &);
private:
    std::ifstream file;
    std::map<std::string, Func_Point> Function;
    std::map<std::string, int *> _register;
    std::map<std::string, int> label;
    std::map<std::string, bool> Jumps;
    int reg1;
    int reg2;
    int reg3;
    int reg4;
    int reg5;
    int reg6;
    int reg7;
    int reg8;
    int reg9;
    int reg10;
};

#endif // FAKE_CPU_H