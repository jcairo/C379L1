#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logger.h"

#define DEBUG 1
#define BUFFER_LENGTH 256


/* Returns a formatted string */
void get_formatted_time(char *buffer) {
    // Get the time
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);
    strftime(buffer, BUFFER_LENGTH, "[%a %b %d %X %Z %Y] ", info);
}

/* Clears the existing log file */
void clear_log_file() {
    // Get log file path
    char *path = getenv("PROCNANNYLOGS");
    if (path == NULL) {
        printf("Error when reading PROCNANNYLOGS variable.\n");
        exit(EXIT_FAILURE);
    }

    // Determine whether we have a relative or absolute path.
    //if (path[0] == '/') {
    //    // We have a relative path. 
    //}

    FILE *fp;
    fp = fopen(path, "w");
    fclose(fp);
}

/* Takes a string and logs it to the log file with appropriate formatting */
void log_message(char *message, int type, char *log_file_path) {
    // Set the message type
    char message_type[BUFFER_LENGTH];
    if (type == INFO) {
        strcpy(message_type, "Info: ");
    }
    if (type == ACTION){
        strcpy(message_type, "Action: ");
    }

    // Get log file path
    //char *path = getenv("PROCNANNYLOGS");
    //if (path == NULL) {
    //    printf("Error when reading path to procnanny log file.\n");
    //    exit(EXIT_FAILURE);
    //}

    // Determine whether we have a relative or absolute path.
    //if (path[0] == '/') {
    //    // We have a relative path. 
    //}
    
    // Get the time.
    char formatted_time[BUFFER_LENGTH];
    get_formatted_time(formatted_time);
    
    // Open the file to write to.
    FILE *fp;
    fp = fopen(log_file_path, "a");
    fprintf(fp, "%s %s%s\n", formatted_time, message_type, message);
    fclose(fp);
}

/* Aggregates the logfiles from each killer child process */
void aggregate_log_files(struct Process_Group process_group, char *main_log_file_path) {
    int i = 0;
    for (; i < process_group.process_count; i++) {
        FILE *fp;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        // Open the file.
        printf("Process log file id to be openend is %s\n", process_group.process[i].process_log_file_path);
        printf("Length of the process file sting is %ld\n", strlen(process_group.process[i].process_log_file_path));
        fp = fopen(process_group.process[i].process_log_file_path, "r");
        if (fp == NULL) {
            printf("Error when opening process log file.\n"); 
            exit(EXIT_FAILURE);
        }

        // Read the log message.
        read = getline(&line, &len, fp); 
        fclose(fp);

        if (read == -1) {
            // No output from process. It did not kill its target.
            remove(process_group.process[i].process_log_file_path);
            continue;
        }
    
        // If there was a line of info in the file print it to the main log file.
        fp = fopen(main_log_file_path, "a");
        printf("The line read from process log file was %s\n", line);
        fprintf(fp,"%s", line);
        fclose(fp);
        
        // Remove the process log file.
        remove(process_group.process[i].process_log_file_path);
    }
}
