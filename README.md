# To pit or not to pit...

## Description
The purpose of this sensor manager is to provide real-time monitoring of Power Management Units and Tires in a car. This information can be used by drivers and mechanics to make informed decisions about when to pit, or change tires.

## Installation
Once the repository has been cloned, you can run the sensor manager using the following commands:
	make build
	./main <file_name>
	
The read file needs to have the following structure:

> <number_of_sensors> <br />
> <type_of_sensor_1> <br />
> <data_of_sensor_1> <br />
> <number_of_operations_1> <br />
> <operation1_1> <operation2_1> <operation3_1> ... <br />
> <type_of_sensor_2> <br />
> <data_of_sensor_2> <br />
> <number_of_operations_2> <br />
> <operation1_2> <operation2_2> <operation3_2> ... <br />
> ...

<data_of_sensor_n> can either be <tire_sensor> or <pmu_sensor>

A <tire_sensor> needs the have the following structure:

>	<pressure_value> <temperature_value> <wear_level> <performance_score>

A <pmu_sensor> needs to have the following structure:

>	<voltage_value> <current_value> <power_consumption> <energy_regen> <energy_storage> 

## Usage
The program has the following functions:

* print <index>: Prints the information of the given sensor

* analyze <index>: Executes the corresponding operations of the given sensor

* clear: deletes all sensors that do not fit between the following conditions:

>	Tire Sensor: <br />
>	pressure: between 19 and 28 psi <br />
>	temperature: between 0°C and 120°C <br />
>	wear_level: between 0% and 100% <br />

>	Power Management Unit Sensor: <br />
>	voltage: between 10V and 20V <br />
>	current: between -100A and 100A <br />
>	power_consumption: between 0kW and 1000kW <br />
>	energy_regen: between 0% and 100% <br />
>	energy_storage: between 0% and 100% <br />
	
* exit: Stops the program



The list of analyze operations is the following:
0. tire_pressure_status
1. tire_temperature_status
2. tire_wear_level_status
3. tire_performance_score
4. pmu_compute_power
5. pmu_regenerate_energy
6. pmu_get_energy_usage
7. pmu_is_battery_healthy

<tire_sensor> can have operations from 0 to 3 while <pmu_sensor> can have operations from 4 to 7.

## Implementation
The sensors are stored in a vector of pointers **sensor_list that points to them. The sensors save inside their struct their type, the data corresponding to their type, the number of operations and a vector of the operations it executes in the analyze function. We read the binary data in those sensor structs.
