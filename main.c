#include <stdio.h>
#include "user_interaction.h"
#include "process_manager.h"
#include "memwatch.h"

int main() {
    // Check whether an existing procnanny process is running.
    // If so prompt the user for instructions on whether to kill the
    // existing process and relaunch a new one or continue with 
    // the existing process.
    if (proc_nanny_running()) {
        struct Process_Group *process_group;
        get_current_processes(process_group);
        // char response[1] = prompt_user_for_instructions();
    }

    return (0);
}

