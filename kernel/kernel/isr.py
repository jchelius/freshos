#!/bin/python3

NUM_ISRS = 256

with open('kernel/kernel/isr.S', 'w') as f:
    f.write("/* ISR common routine */")
    f.write("\n.extern isr_handler")
    f.write("\nisr_common:\n")
    f.write("\tpusha\n")
    f.write("\tpushw %ds\n")
    f.write("\tpushw %es\n")
    f.write("\tpushw %fs\n")
    f.write("\tpushw %gs\n")
    f.write("\tmovw $0x10, %ax\n")
    f.write("\tmovw %ax, %ds\n")
    f.write("\tmovw %ax, %es\n")
    f.write("\tmovw %ax, %fs\n")
    f.write("\tmovw %ax, %gs\n")
    f.write("\tpushl %esp\n")
    # f.write("\tmovl %esp, %eax\n")
    # f.write("\tpushl %eax\n")
    # f.write("\tmovl $isr_handler, %eax\n")
    f.write("\tcld\n")
    # f.write("\tcall %eax\n")
    f.write("\tcall isr_handler\n")
    f.write("\tpopl %eax\n")
    f.write("\tpopw %gs\n")
    f.write("\tpopw %fs\n")
    f.write("\tpopw %es\n")
    f.write("\tpopw %ds\n")
    f.write("\tpopa\n")
    f.write("\taddl $8, %esp\n")
    f.write("\tiret\n")

    f.write("\n/* ISR definitions */\n")
    for i in range(NUM_ISRS):
        f.write(f"isr{i}:\n")
        if i < 8 or i == 9 or i > 14:
            # push dummy value
            f.write("\tpushl $0\n")
        # push interrupt number
        f.write(f"\tpushl ${i}\n")
        f.write("\tjmp isr_common\n")
    f.write("\n.data\n")
    f.write(".global isrs\n")
    f.write("isrs:\n")
    for i in range(NUM_ISRS):
        f.write(f"\t.long isr{i}\n")

