#include <stdio.h>
#include <stdlib.h>
#include "config_reader.h"
#include <string.h>

/* Takes absolute path to config file. Returns config struct */
struct Config read_config(char *path) {
    // Open the file
    // File reading code taken from http://stackoverflow.com/questions/3501338/c-read-file-line-by-line
    // September 21, 2015
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    struct Config config;

    // Open the file.
    fp = fopen(path, "r");
    if (fp == NULL) {
        perror("Error when opening config file"); 
        exit(EXIT_FAILURE);
    }

    // Read the time delay.
    read = getline(&line, &len, fp); 
    if (read == -1) {
        perror("Error reading first line of config file.");
        exit(EXIT_FAILURE);
    }

    // Parse time delay.
    int time = atoi(line);
    // printf("Time delay is %d\n", time);
    config.time = time;

    // Read and parse programs to be monitored.
    int i = 0;
    for (; i < MAX_CONFIG_PROGRAMS;) {
        read = getline(&line, &len, fp);

        // Make sure read returns proper value.
        if (read == -1) {
            perror("Error while reading program list in config file or end of lines reached.");
            break;
            exit(EXIT_FAILURE);
        }
        config.application_names[i] = line;

        // Copy string remove newline, ensure null terminated.
        int j = 0;
        for (;;j++) {
            // Make sure we don't have an empty line.
            if (strlen(line) == 1) {
                printf("Line length was zero");
                break; 
            }

            // Copy chars from string and deal with new line and null chars.
            if (line[j] == '\n' || line[j] == '\0') {
                config.application_names[i][j] = '\0';  
                printf("Constructed a word %s", config.application_names[i]);
                i++;
                break;
            } 
            config.application_names[i][j] = line[j];
        }
        
    }

    // Set total application count.
    config.application_count = i;
    fclose(fp);
    return config;
};

