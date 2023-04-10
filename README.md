# To pit or not to pit...

## Description
The purpose of this sensor manager is to provide real-time monitoring of Power Management Units and Tires in a car. This information can be used by drivers and mechanics to make informed decisions about when to pit, or change tires.

## Installation
Once the repository has been cloned, you can run the sensor manager using the following commands:
	make build
	./main <file_name>
	
The read file needs to have the following structure:
<number_of_sensors>
<type_of_sensor_1>
<data_of_sensor_1>
<number_of_operations_1>
<operation1_1> <operation2_1> <operation3_1> ...
<type_of_sensor_2>
<data_of_sensor_2>
<number_of_operations_2>
<operation1_2> <operation2_2> <operation3_2> ...
...

<data_of_sensor_n> can either be <tire_sensor> or <pmu_sensor>
A <tire_sensor> needs the have the following structure:
<pressure> <temperature> <wear_level> <performance_score>
A <pmu_sensor> needs to have the following structure:
<voltage> <current> <power_consumption> <energy_regen> <energy_storage> 

## Usage
The program has the following functions:

1. print <index>: Prints the information of the given sensor

2. analyze <index>: Executes the corresponding operations of the given sensor

3. clear: deletes all sensors that do not fit between the following conditions:

	Tire Sensor:
	pressure: between 19 and 28 psi
	temperature: between 0°C and 120°C
	wear_level: between 0% and 100%
	
	Power Management Unit Sensor:
	voltage: between 10V and 20V
	current: between -100A and 100A
	power_consumption: between 0kW and 1000kW
	energy_regen: between 0% and 100%
	energy_storage: between 0% and 100%
	
4. exit: Stops the program

The list of operations is the following:
0. tire_pressure_status
1. tire_temperature_status
2. tire_wear_level_status
3. tire_performance_score
4. pmu_compute_power
5. pmu_regenerate_energy
6. pmu_get_energy_usage
7. pmu_is_battery_healthy
<tire_sensor> can have from 0 to 3 while <pmu_sensor> can have from 5 to 7.

## Implementation
The sensors are stored in a vector of pointers **sensor_list that points to them. The sensors save inside their struct their type, the data corresponding to their type, the number of operations and a vector of the operations it executes in the analyze function. We read the binary data in those sensor structs.
