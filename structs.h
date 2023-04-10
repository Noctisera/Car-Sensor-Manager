#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#define NMAX 100
#define NUM_OPERATIONS 8
#include <stdio.h>

enum sensor_type {
    TIRE,
    PMU
};

typedef struct {
    enum sensor_type sensor_type;
    void *sensor_data;
    int nr_operations;
    int *operations_idxs;
} sensor;

typedef struct __attribute__((__packed__)) {
    float voltage;
    float current;
    float power_consumption;
    int energy_regen;
    int energy_storage;
} power_management_unit;

typedef struct __attribute__((__packed__)) {
    float pressure;
    float temperature;
    int wear_level;
    int performace_score;
} tire_sensor;

void get_operations(void **operations);
sensor *read_sensor_data(FILE *in);
sensor **read_file(FILE *in, sensor **sensor_list, int sensor_number);
void sort_sensor_list(sensor **sensor_list, int *sensor_number);
void print_sensor(sensor *sensor);
void analyze_sensor(sensor **sensor_list, int index);
void free_sensor(sensor **sensor_list, int index, int *sensor_number);
void clear_sensor(sensor **sensor_list, int *sensor_number);
void free_all(sensor **sensor_list, int *sensor_number);
void execute_command(char *command, sensor **sensor_list, int *sensor_number);

#endif // _STRUCTS_H_
