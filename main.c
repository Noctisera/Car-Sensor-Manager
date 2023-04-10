#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "structs.h"

int main(int argc, char const *argv[])
{
    FILE *in;
    in = fopen(argv[1], "rb");

    sensor **sensor_list = NULL;
    int sensor_number;

    if(in == NULL) {
        printf("ERROR: NO FILE NAME INTRODUCED");
        exit(1);
    }

    /* Read the number of sensors to read and sort them */
    fread(&sensor_number, sizeof(int), 1, in);
    sensor_list = read_file(in, sensor_list, sensor_number);
    sort_sensor_list(sensor_list, &sensor_number);
    fclose(in);

    char command[NMAX];
    while(scanf("%s", command) != EOF) {
        execute_command(command, sensor_list, &sensor_number);
    }

    return 0;
}