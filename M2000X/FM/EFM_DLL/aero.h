#pragma once
#include "ED_FM_Utility.h"

struct PlaneState {
	double ax;//linear acceleration component in body coordinate system
	double ay;//linear acceleration component in body coordinate system
	double az;//linear acceleration component in body coordinate system

	double vx;//linear velocity component in body coordinate system
	double vy;//linear velocity component in body coordinate system
	double vz;//linear velocity component in body coordinate system

	double wind_vx;//wind linear velocity component in body coordinate system
	double wind_vy;//wind linear velocity component in body coordinate system
	double wind_vz;//wind linear velocity component in body coordinate system

	double p_dot;//angular accelearation components in body coordinate system
	double q_dot;//angular accelearation components in body coordinate system
	double r_dot;//angular accelearation components in body coordinate system

	double p;//angular velocity components in body coordinate system - roll 
	double q;//angular velocity components in body coordinate system - pitch
	double r;//angular velocity components in body coordinate system - yaw

	double yaw;  //radians
	double pitch;//radians
	double roll; //radians

	double alpha; //AoA radians
	double beta;   //AoS radians

	double alpha_last; //AoA radians
	double beta_last;   //AoS radians

	double alpha_dot;   // find rate of change of alpha in time
	double beta_dot;     // find rate of change of beta in time

	double q_bar;
	double q_bar_S;

	double V_scalar;

	Vec3	common_moment = { 0.0, 0, 0 }; //s
	Vec3	common_force = { 0.0, 0, 0 }; //
	Vec3    airspeed = { 0.0, 0, 0 }; //

	double currmass = 1270.586; // 28000 lbs

	Vec3    moment_of_inertia = { 0.0, 0, 0 }; //0.25;
	Vec3    curr_moment_of_inertia = { 0.0, 0, 0 }; //0.25;
	Vec3    center_of_gravity = { 0.0, 0, 0 }; //0.25;
	Vec3    center_of_pressure = { 0.0, 0, 0 }; //0.25

	double  stick_throttle = 0;
	double  elevon1L = 0;  // deg 
	double  elevon2L = 0;  // deg 
	double  elevon1R = 0;  // deg 
	double  elevon2R = 0;  // deg 
	double  rudder = 0;		 // deg


	double  stick_rudder = 0;
	double  stick_aliron = 0;
	double  stick_elevator = 0;

	double  stick_ruddertrim = 0;
	double  stick_alirontrim = 0;
	double  stick_elevatortrim = 0;


	double  airbrakes_handle = 0;
	double  airbrakes = 0;

	double  wheelbreaks_handle = 0;
	double  wheelbreaks = 0;

	double  slats_handle = 0;
	double  slats1 = 0;
	double  slats2 = 0;

	double  parashute = 0;
	double  parashute_handle = 0;

	double  gear_handle = 0;
	double  gear = 0;

	double  atmosphere_density = 0;
	double  atmosphere_pressure = 0;
	double  mach = 0;
	double  speed_of_sound = 0;

	double currEnginePwr = 21400; // max thrust
	double throttle = 0;
	double thrust = 0;

	double  internal_fuel = 0;
	double  fuel_consumption_since_last_time = 0;
} ;
struct PlaneData {
	const double g = 9.81;

	const double S_wingarea = 28.17; //wingarea_ 530ft^2
	const double b_span = 7.87; // wingspan 38.7ft
	const double chord = 3.58; //mean chard 16 ft
	const double c_bar = 3.58; //mean chard 16 ft

	const double AR = b_span * b_span / S_wingarea; // aspect ratio
	const double Xref = 8.56;  // Xcp
	const double Xcg = 8.56;

	const double weight = 28000; // 28000 lbs

	const double Ixx = 6262;  // KG/ m^2 
	const double Iyy = 75686; // KG/ m^2 
	const double Izz = 78802; // KG/ m^2
	const double Ixz = 2141;  // KG/ m^2

	const double  max_elevon1L_angle_up = 25;     // deg 
	const double  max_elevon2L_angle_up = 25;     // deg 
	const double  max_elevon1L_angle_down = -16;  // deg 
	const double  max_elevon2L_angle_down = -16;  // deg 
	const double  max_elevon1R_angle_up = 25;     // deg 
	const double  max_elevon2R_angle_up = 25;     // deg 
	const double  max_elevon1R_angle_down = -16;  // deg 
	const double  max_elevon2R_angle_down = -16;  // deg 
	const double  max_rudder_angle = 25;          // deg 
	const double  max_slats_angle = 25;			  // deg 	

	const double EnginePwrFull = 21400; // max thrust
	const double EnginePwrMil = 14500; // max thrust
	// NONDIMENSIONAL DERIVATIES
};

