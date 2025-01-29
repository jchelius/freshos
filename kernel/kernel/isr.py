#!/bin/python3

NUM_ISRS = 256

with open('isr.S', 'w') as f:
    f.write("/* ISR definitions */\n\n")
    # for i in range(NUM_ISRS):
    #     f.write(f".global isr{i}\n")
    # f.write("\n")
    for i in range(NUM_ISRS):
        f.write(f"isr{i}:\n")
        if i < 8 or i == 9 or i > 14:
            # push dummy value
            f.write("\tpushw $0\n")
        # push interrupt number
        f.write(f"\tpushw ${i}\n")
        f.write("\tjmp isr_common\n")
    f.write("\n.data\n")
    f.write(".global isrs\n")
    f.write("isrs:\n")
    for i in range(NUM_ISRS):
        f.write(f"\t.long isr{i}\n")

    f.write("\nisr_common:\n")
    f.write("\tpushad\n")
    f.write("\tpushw %ds\n")
    f.write("\tpushw %es\n")
    f.write("\tpushw %fs\n")
    f.write("\tpushw %gs\n")
    f.write("\tpushl %esp\n")
    f.write("\tcld\n")
    f.write("\tcall isr_handler\n")
    f.write("\tpopl $eax\n")
    f.write("\tpopw %gs\n")
    f.write("\tpopw %fs\n")
    f.write("\tpopw %es\n")
    f.write("\tpopw %ds\n")
    f.write("\taddl $esp, 8\n")
    f.write("\tiret\n")
