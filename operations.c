#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stddef.h>
#include <string.h>

static void tire_pressure_status(void *data)
{
    tire_sensor *t = (tire_sensor *)data;
    if (t->pressure >= 21 && t->pressure <= 26) {
        printf("Tire has normal pressure.\n");
    } else if (t->pressure > 26 && t->pressure <= 28) {
        printf("Tire has high pressure.\n");
    } else if (t->pressure >= 19 && t->pressure < 21) {
        printf("Tire has low pressure.\n");
    } else {
        printf("Tire has abnormal pressure.\n");
    }
}

static void tire_temperature_status(void *data)
{
    tire_sensor *t = (tire_sensor *)data;
    if (t->temperature >= 0 && t->temperature <= 120) {
        printf("Tire has normal temperature.\n");
    } else {
        printf("Tire has abnormal temperature.\n");
    }
}

static void tire_wear_level_status(void *data)
{
    tire_sensor *t = (tire_sensor *)data;
    if (t->wear_level >= 0 && t->wear_level <= 20) {
        printf("Tire is new.\n");
    } else if (t->wear_level >= 20 && t->wear_level <= 40) {
        printf("Tire is slightly used.\n");
    } else if (t->wear_level >= 40 && t->wear_level <= 60) {
        printf("Tire is used. Consider a pit stop!\n");
    } else {
        printf("Tire is extremely used. Box this lap!\n");
    }
}

static void tire_performance_score(void *data)
{
    tire_sensor *t = (tire_sensor *)data;
    int score = 0;

    // Check pressure
    if (t->pressure >= 21 && t->pressure <= 26) {
        score += 4;
    } else if (t->pressure >= 19 && t->pressure < 21) {
        score += 2;
    } else if (t->pressure > 26 && t->pressure <= 28) {
        score += 3;
    } else {
        score -= 1;
    }

    // Check temperature
    if (t->temperature >= 80 && t->temperature <= 100) {
        score += 4;
    } else if (t->temperature >= 100 && t->temperature < 120) {
        score += 3;
    } else if (t->temperature > 60 && t->temperature <= 80) {
        score += 2;
    } else {
        score -= 1;
    }

    // Check wear level
    if (t->wear_level >= 0 && t->wear_level < 20) {
        score += 4;
    } else if (t->wear_level >= 20 && t->wear_level < 40) {
        score += 2;
    } else if (t->wear_level > 40 && t->wear_level < 60) {
        score -= 1;
    } else {
        score -= 2;
    }

    // Cap the score at 10
    if (score > 10) {
        score = 10;
    } else if (score < 1) {
        score = 1;
    }

    t->performace_score = score;
    printf("The tire performance score is: %d\n", score);
}

static void pmu_compute_power(void *data)
{
    power_management_unit *pmu = (power_management_unit *)data;
    float power = pmu->voltage * pmu->current;

    printf("Power output: %.2f kW.\n", power);
}

static void pmu_regenerate_energy(void *data)
{
    power_management_unit *pmu = (power_management_unit *)data;

    pmu->energy_storage += pmu->energy_regen;
    pmu->energy_regen = 0;

    // Cap the energy storage to 100%
    if (pmu->energy_storage > 100)
        pmu->energy_storage = 100;

    printf("Energy left in storage: %d\n", pmu->energy_storage);
}

static void pmu_get_energy_usage(void *data)
{
    power_management_unit *pmu = (power_management_unit *)data;
    float energy = pmu->power_consumption * pmu->current;

    printf("Energy usage: %.2f kW\n", energy);
}

static void pmu_is_battery_healthy(void *data)
{
    power_management_unit *pmu = (power_management_unit *)data;
    int ok = 0;

    if (pmu->voltage < 10 || pmu->voltage > 20) {
        printf("Battery voltage out of range: %.2fV\n", pmu->voltage);
        ok = 1;
    }

    if (pmu->current < -100 || pmu->current > 100) {
        printf("Battery current out of range: %.2fA\n", pmu->current);
        ok = 1;
    }

    if (pmu->power_consumption < 0 || pmu->power_consumption > 1000) {
        printf("Power consumption out of range: %.2f kW\n",
               pmu->power_consumption);
        ok = 1;
    }

    if (pmu->energy_regen < 0 || pmu->energy_regen > 100) {
        printf("Energy regeneration out of range: %d%%\n", pmu->energy_regen);
        ok = 1;
    }

    if (pmu->energy_storage < 0 || pmu->energy_storage > 100) {
        printf("Energy storage out of range: %d%%\n", pmu->energy_storage);
        ok = 1;
    }

    if (ok == 1)
        return;

    printf("Battery working as expected!\n");
}

void get_operations(void **operations)
{
    operations[0] = tire_pressure_status;
    operations[1] = tire_temperature_status;
    operations[2] = tire_wear_level_status;
    operations[3] = tire_performance_score;
    operations[4] = pmu_compute_power;
    operations[5] = pmu_regenerate_energy;
    operations[6] = pmu_get_energy_usage;
    operations[7] = pmu_is_battery_healthy;
}

sensor *read_sensor_data(FILE *in) {
    sensor *aux_sensor;
    aux_sensor = malloc(sizeof(sensor));

    /* Read the sensor type */
    fread(&aux_sensor->sensor_type, sizeof(enum sensor_type), 1, in);

    /* Read the sensor data */
    if(aux_sensor->sensor_type == TIRE) {
        tire_sensor *aux_tire_sensor;
        aux_tire_sensor = malloc(sizeof(tire_sensor));
        fread(&aux_tire_sensor->pressure, sizeof(float), 1, in);
        fread(&aux_tire_sensor->temperature, sizeof(float), 1, in);
        fread(&aux_tire_sensor->wear_level, sizeof(int), 1, in);
        fread(&aux_tire_sensor->performace_score, sizeof(int), 1, in);
        aux_sensor->sensor_data = aux_tire_sensor;
    }
    else if(aux_sensor->sensor_type == PMU) {
        power_management_unit *aux_pmu_sensor;
        aux_pmu_sensor = malloc(sizeof(power_management_unit));
        fread(&aux_pmu_sensor->voltage, sizeof(float), 1, in);
        fread(&aux_pmu_sensor->current, sizeof(float), 1, in);
        fread(&aux_pmu_sensor->power_consumption, sizeof(float), 1, in);
        fread(&aux_pmu_sensor->energy_regen, sizeof(int), 1, in);
        fread(&aux_pmu_sensor->energy_storage, sizeof(int), 1, in);
        aux_sensor->sensor_data = aux_pmu_sensor;
    }

    /* Read the number of operations */
    fread(&aux_sensor->nr_operations, sizeof(int), 1, in);
    aux_sensor->operations_idxs = malloc(aux_sensor->nr_operations * sizeof(int));
    if(aux_sensor->operations_idxs == NULL) {
        printf("ERROR: MALLOC FAILED AT OPERATIONS_IDXS");
        exit(1);
    }

    for(int i = 0; i < aux_sensor->nr_operations; i++) {
        /* Read the operations */
        fread(&aux_sensor->operations_idxs[i], sizeof(int), 1, in);
    }

    return aux_sensor;
}

sensor **read_file(FILE *in, sensor **sensor_list, int sensor_number) {
    sensor_list = malloc(sensor_number * sizeof(sensor *));
    if(sensor_list == NULL) {
        printf("ERROR: MALLOC FAILED AT SENSOR_LIST");
        exit(1);
    }

    for(uint64_t i = 0; i < sensor_number;i++) {
        /* Read the sensor information */
        sensor_list[i] = read_sensor_data(in);
    }

    return sensor_list;
}

void sort_sensor_list(sensor **sensor_list, int *sensor_number) {
    sensor **aux_pmu_list;
    aux_pmu_list = malloc(*sensor_number * sizeof(sensor *));
    sensor **aux_tire_list;
    aux_tire_list = malloc(*sensor_number * sizeof(sensor *));

    int tire_number = 0;
    int pmu_number = 0;

    for(int i = 0; i < *sensor_number; i++) {
        if(sensor_list[i]->sensor_type == TIRE) {
            aux_tire_list[tire_number] = sensor_list[i];
            tire_number++;
        }
        else if(sensor_list[i]->sensor_type == PMU) {
            aux_pmu_list[pmu_number] = sensor_list[i];
            pmu_number++;
        }
    }

    int index = 0;

    for(int i = 0; i < pmu_number; i++) {
        sensor_list[index] = aux_pmu_list[i];
        index++;
    }
    for(int i = 0; i < tire_number; i++) {
        sensor_list[index] = aux_tire_list[i];
        index++;
    }

    free(aux_tire_list);
    free(aux_pmu_list);
}

void print_sensor(sensor *sensor) {
    if(sensor == NULL) {
        printf("ERROR: NO SENSOR TO PRINT");
        return;
    }

    if(sensor->sensor_type == TIRE) {
        tire_sensor *aux_tire_sensor;
        aux_tire_sensor = (tire_sensor*)sensor->sensor_data;
        printf("Tire Sensor\n");
        printf("Pressure: %.2f\n", aux_tire_sensor->pressure);
        printf("Temperature: %.2f\n", aux_tire_sensor->temperature);
        printf("Wear Level: %d%%\n", aux_tire_sensor->wear_level);
        if(aux_tire_sensor->performace_score == 0){
            printf("Performance Score: Not Calculated\n");
        }
        else{
            printf("Performance Score: %d\n", aux_tire_sensor->performace_score);
        }
    }
    else if(sensor->sensor_type == PMU) {
        power_management_unit *aux_pmu_sensor;
        aux_pmu_sensor = (power_management_unit*)sensor->sensor_data;
        printf("Power Management Unit\n");
        printf("Voltage: %.2f\n", aux_pmu_sensor->voltage);
        printf("Current: %.2f\n", aux_pmu_sensor->current);
        printf("Power Consumption: %.2f\n", aux_pmu_sensor->power_consumption);
        printf("Energy Regen: %d%%\n", aux_pmu_sensor->energy_regen);
        printf("Energy Storage: %d%%\n", aux_pmu_sensor->energy_storage);
    }
}

void analyze_sensor(sensor **sensor_list, int index) {
    if(sensor_list[index] == NULL) {
        printf("ERROR: NO SENSOR TO ANALYZE");
        return;
    }

    void **operations;
    operations = malloc(NUM_OPERATIONS * sizeof(void *));
    if(operations == NULL) {
        printf("ERROR: NO MEMORY FOR OPERATIONS");
        exit(1);
    }
    get_operations(operations);

    for(int i = 0; i < sensor_list[index]->nr_operations; i++) {
        int command_index = sensor_list[index]->operations_idxs[i];
        ((void (*)(void *)) operations[command_index])(sensor_list[index]->sensor_data);
    }

    free(operations);
}

void free_sensor(sensor **sensor_list, int index, int *sensor_number) {
    free(sensor_list[index]->sensor_data);
    free(sensor_list[index]->operations_idxs);
    free(sensor_list[index]);

    for(int i = index; i < *sensor_number - 1; i++) {
        sensor_list[i] = sensor_list[i + 1];
    }

    sensor_list[*sensor_number - 1] = NULL;
    *sensor_number -= 1;
}

void clear_sensor(sensor **sensor_list, int *sensor_number) {
    /* Go through the sensor list */
    for(int i = 0; i < *sensor_number; i++) {
        /* Check if the sensor has wrong data in the same if() */
        if(sensor_list[i]->sensor_type == 0){
            tire_sensor *aux_sensor = (tire_sensor*)sensor_list[i]->sensor_data;
            if(aux_sensor->pressure < 19 || aux_sensor->pressure > 28
               || aux_sensor->temperature < 0 || aux_sensor->temperature > 120
               || aux_sensor->wear_level < 0 || aux_sensor->wear_level > 100) {
                free_sensor(sensor_list, i, sensor_number);
                i--;
                continue;
            }
        }
        else if(sensor_list[i]->sensor_type == 1) {
            power_management_unit *aux_sensor = (power_management_unit*)sensor_list[i]->sensor_data;
            if(aux_sensor->voltage < 10 || aux_sensor->voltage > 20
               || aux_sensor->current < -100 || aux_sensor->current > 100
               || aux_sensor->power_consumption < 0 || aux_sensor->power_consumption > 1000
               || aux_sensor->energy_regen < 0 || aux_sensor->energy_regen > 100
               || aux_sensor->energy_storage < 0 || aux_sensor->energy_storage > 100) {
                free_sensor(sensor_list, i, sensor_number);
                i--;
                continue;
            }
        }
    }
}

void free_all(sensor **sensor_list, int *sensor_number) {
    while(*sensor_number > 0) {
        free_sensor(sensor_list, 0, sensor_number);
    }
    free(sensor_list);
}

void execute_command(char *command, sensor **sensor_list, int *sensor_number) {
    if(sensor_list == NULL) {
        printf("ERROR: NO SENSOR LIST FOR EXECUTING COMMANDS");
        exit(1);
    }

    if(strcmp(command, "print") == 0) {
        int index;
        scanf("%d", &index);
        if(index <0 || index >= *sensor_number) {
            printf("Index not in range!\n");
            return;
        }
        print_sensor(sensor_list[index]);
    }
    else if(strcmp(command, "analyze") == 0) {
        int index;
        scanf("%d", &index);
        if(index <0 || index >= *sensor_number) {
            printf("Index not in range!\n");
            return;
        }
        analyze_sensor(sensor_list, index);
    }
    else if(strcmp(command, "clear") == 0) {
        clear_sensor(sensor_list, sensor_number);
    }
    else if(strcmp(command, "exit") == 0) {
        free_all(sensor_list, sensor_number);
        exit(0);
    }
    else {
        printf("ERROR: INVALID COMMAND");
        return;
    }
}
