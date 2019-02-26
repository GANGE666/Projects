# 基于FPGA的MIPS的Verilog实现 ———— 计算机组成原理实验课大作业项目

### 基本介绍
此项目是2018年大二下学期开设的数字逻辑实验课的大作业项目，我该门课程成绩97，绩点4.0。

此项目为实践计算机组成原理课上学到的CPU单周期、多周期、流水线的工作方式，通过前期熟悉储存器、ALU等结构，到自己实现单周期CPU，最终完成流水线CPU的设计、调试与仿真，具体介绍在[MIPS_CPU设计实验报告](./MIPS_CPU设计实验报告.md)中有介绍。

***
### 完成情况
流水线的源代码已上传至 https://github.com/GANGE666/MIPS-Cup-Verilog

***

Single Cycle CPU can execute add, sub, and, or, slt, addi, lw, sw, beq, j, sll, lui, slti, bne, ori, andi, srl，jal, jr, jalr, addu and subu.

***

A classic 5-stage pipeline MIPS 32-bit processor.

- 256K RAM
- 128K Cache
- 5 stage: Fetch, Decode, Execute, Memory and Write back
- The processor support follow insrtuction
  
  - TYPE-R: AND, OR, ADD, SUB, SLT, NOR, SRL
  - TYPE-I: ADDI, ANDI, ORI, XORI, LUI, SLTI
  - MEM: LW, SW
  - BRANCH: BEQ, BNE
  - JUMP: J, JAR, JR, JALR

- If you want a single cycle process, you can use the code of first commit.
***
详见[MIPS_CPU设计实验报告](./MIPS_CPU设计实验报告.md)。

***
### 相关文件
- [MIPS_CPU设计实验报告.md](./MIPS_CPU设计实验报告.md) 
- [MIPS_CPU设计实验报告.pdf](./MIPS_CPU设计实验报告.pdf)
- [单周期源代码](./SingleCycle/)
- [流水线源代码](./Pipeline/)

