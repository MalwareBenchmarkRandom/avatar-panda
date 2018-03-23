#include "panda/plugin.h"

#include <stdlib.h>
#include "panda/plog-cc.hpp"

extern "C" {
bool init_plugin(void *self);
void uninit_plugin(void *self);
}

void print_state(CPUState *cpu){
#if defined(TARGET_ARM)
    int NREGS = 16;
    CPUARMState *env = (CPUARMState *) cpu->env_ptr;

    fprintf(stdout, "regs:\n");
    for (int i=0; i<NREGS; i++){
        fprintf(stdout, "\treg[%d]: %x\n", i, env->regs[i]);
    }

#elif defined(TARGET_I386)
    printf("[print_state] not implemented yet for i386\n");
#else
    printf("[print_state] not implemented yet\n");
#endif
}




int after_block_exec(CPUState *cpu, TranslationBlock *tb){
    fprintf(stderr, "[after_block_exec] current_asid: %x\n", panda_current_asid(cpu));
    print_state(cpu);

    uninit_plugin(NULL);
    exit(1);
    return 0;
}

bool init_plugin(void *self){
    printf("[get_asid] init_plugin\n");

    panda_cb pcb;     

    pcb.after_block_exec = after_block_exec;
    panda_register_callback(self, PANDA_CB_AFTER_BLOCK_EXEC, pcb);

    return true;
}


void uninit_plugin(void *self){
    printf("[get_asid] uninit_plugin\n");
}


