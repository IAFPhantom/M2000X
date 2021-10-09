// ED_FM_Template.cpp : Defines the exported functions for the DLL application.
//#include "pch.h"
#include "include/FM/wHumanCustomPhysicsAPI_ImplementationDeclare.h"
#include <Math.h>
#include <stdio.h>
#include <string>

//#include "Log.h"
#include "aero.h"
#include "CFD.h"

#define arg_throttle 28
#define arg_elevon1L 12
#define arg_elevon2L 10
#define arg_elevon1R 11
#define arg_elevon2R 9
#define arg_rudder 17
#define arg_airbrakes 21
#define arg_nose_gear 0
#define arg_right_gear 3
#define arg_left_gear 5
#define arg_slats1 13
#define arg_slats2 14

PlaneState state;
PlaneData data;
CFD cfd;
std::string fmcfg_path;
void add_local_force_cg(const Vec3 & Force, const Vec3 & Force_pos)
{
	state.common_force.x += Force.x;
	state.common_force.y += Force.y;
	state.common_force.z += Force.z;

	Vec3 delta_pos(Force_pos.x - state.center_of_gravity.x,
				   Force_pos.y - state.center_of_gravity.y,
				   Force_pos.z - state.center_of_gravity.z);

	Vec3 delta_moment = cross(delta_pos, Force);

	state.common_moment.x += delta_moment.x;
	state.common_moment.y += delta_moment.y;
	state.common_moment.z += delta_moment.z;
}


void ed_fm_add_local_force(double & x,double &y,double &z,double & pos_x,double & pos_y,double & pos_z)
{
	x = state.common_force.x;
	y = state.common_force.y;
	z = state.common_force.z;
	pos_x = state.center_of_gravity.x;
	pos_y = state.center_of_gravity.y;
	pos_z = state.center_of_gravity.z;
}

void ed_fm_add_global_force(double & x,double &y,double &z,double & pos_x,double & pos_y,double & pos_z)
{
}

void ed_fm_add_global_moment(double & x,double &y,double &z)
{
}

bool ed_fm_add_local_force_component(double& x, double& y, double& z, double& pos_x, double& pos_y, double& pos_z)
{
	return false;
}

bool ed_fm_add_global_force_component(double& x, double& y, double& z, double& pos_x, double& pos_y, double& pos_z)
{
	return false;
}

bool ed_fm_add_local_moment_component(double& x, double& y, double& z)
{
	return false;
}

bool ed_fm_add_global_moment_component(double& x, double& y, double& z)
{
	return false;
}

/*/////////////////////////////////////////////////////////////////////////
function of force source in body axis
x,y,z			  - force components in body coordinate system
pos_x,pos_y,pos_z - position of force source in body coordinate system

body coordinate system system is always X - positive forward ,
										Y - positive up,
										Z - positive to right
*/
void ed_fm_add_local_moment(double & x,double &y,double &z)
{
	x = state.common_moment.x;
	y = state.common_moment.y;
	z = state.common_moment.z;
}

void simulate_fuel_consumption(double dt)
{
	state.fuel_consumption_since_last_time = 10 * state.throttle * dt; //10 kg persecond
	if (state.fuel_consumption_since_last_time > state.internal_fuel)
		state.fuel_consumption_since_last_time = state.internal_fuel;
	state.internal_fuel -= state.fuel_consumption_since_last_time;
}

void ed_fm_simulate_engine_control(double dt)
{
	// place holder for engine management
	state.throttle = state.stick_throttle;
	state.thrust = state.stick_throttle * data.EnginePwrFull;

	// TODO add engine allignment angle in relation to body axis
	CLog::cotrace("EnginePwr,%f,%f,%f\n", state.thrust, data.EnginePwrFull, state.throttle);

	simulate_fuel_consumption(dt);
}
void aminate_control(double handle, double & control, double dt)
{
	if (handle == 1)
	{
		if (control < 1)
			control += dt;
		else 
			control = 1;
	}
	else if (handle == 0)
	{
		if (control > 0)
			control -= dt;
		else 
			control = 0;
	}
}
void ed_fm_simulate_gear_control(double dt)
{
	aminate_control(state.slats_handle, state.slats1, dt);
	state.slats2 = state.slats1;
	aminate_control(state.wheelbreaks_handle, state.wheelbreaks, dt);
	aminate_control(state.airbrakes_handle, state.airbrakes, dt);
	aminate_control(state.gear_handle, state.gear, dt);
	//CLog::trace("gear,%f,%f,%f\n", state.gear_handle, state.gear, dt);
}
void ed_fm_simulate_input_control(double dt)
{
	state.rudder = -state.stick_rudder * data.max_rudder_angle;
	//stick_aliron_angle = stick_aliron * max_aliron_angle;
	//stick_elevator_angle = -stick_elevator * max_elevator_angle;
	if (state.stick_aliron >= 0)
	{
		state.elevon1L = state.stick_aliron * data.max_elevon1L_angle_up;
		state.elevon1R = -state.stick_aliron * data.max_elevon1R_angle_up;
	}
	else if (state.stick_aliron < 0)
	{
		state.elevon1L = state.stick_aliron * -data.max_elevon1L_angle_down;
		state.elevon1R = -state.stick_aliron * -data.max_elevon1R_angle_down;
	}
	if (state.stick_elevator >= 0)
	{
		state.elevon2L = state.stick_elevator * data.max_elevon1L_angle_up;
		state.elevon2R = state.stick_elevator * data.max_elevon1R_angle_up;
	}
	else if (state.stick_elevator < 0)
	{
		state.elevon2L = state.stick_elevator * -data.max_elevon1L_angle_down;
		state.elevon2R = state.stick_elevator * -data.max_elevon1R_angle_down;
	}


	CLog::cotrace("stick,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", state.stick_elevator, state.stick_aliron, state.stick_rudder, state.elevon1L, state.elevon1R, state.elevon2L, state.elevon2R, state.rudder, state.slats1, state.slats2, state.airbrakes, state.gear);
}
void ed_fm_simulate_init(double dt)
{	
	state.common_force.x = 0;
	state.common_force.y = 0;
	state.common_force.z = 0;
	state.common_moment.x = 0;
	state.common_moment.y = 0;
	state.common_moment.z = 0;

	state.airspeed.x = state.vx - state.wind_vx;
	state.airspeed.y = state.vy - state.wind_vy;
	state.airspeed.z = state.vz - state.wind_vz;
	state.V_scalar = sqrt((state.airspeed.x * state.airspeed.x) + (state.airspeed.y * state.airspeed.y) + (state.airspeed.z * state.airspeed.z));
	if (state.speed_of_sound != 0)
		state.mach = state.V_scalar / state.speed_of_sound;
	else
		state.mach = 0;

	state.alpha_dot = (state.alpha - state.alpha_last) / dt;   // find rate of change of alpha in time
	state.beta_dot = (state.beta - state.beta_last) / dt;     // find rate of change of beta in time

	state.q_bar = 0.5 * state.atmosphere_density * state.V_scalar * state.V_scalar;
	state.q_bar_S = state.q_bar * data.S_wingarea;
}

void ed_fm_simulate_cfd()
{
	double c2u = 0;
	double b2u = 0;
	if (state.V_scalar != 0)
	{
		c2u = data.chord / (2.0 * state.V_scalar);
		b2u = data.b_span / (2.0 * state.V_scalar);
	}
	double cos_alpha = (cos(state.alpha));
	double sin_alpha = (sin(state.alpha));
	double cos_beta = (cos(state.beta));
	double sin_beta = (sin(state.beta));

	/********** calculating aerodynamic forces*********************/
	cfd.calc_coeffeients(state);
	double lift = (cfd.CL() /* + (cfd.CLq() * state.q * c2u)*/) * state.q_bar_S;
	double drag = (cfd.CD() )								    * state.q_bar_S;
	double sideforce = (cfd.CY() /* + (cfd.CYp() * state.p * c2u)*//* + (cfd.CYr() * state.r * c2u)*/) * state.q_bar_S;
	CLog::cotrace("forces,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", lift , drag, sideforce, state.thrust, cfd.CL(), cfd.CD(), cfd.CY(), state.q_bar_S);

	// stability to body
	state.common_force.x = (lift * sin_alpha) - (drag * cos_alpha) + state.thrust;
	state.common_force.y = ((lift * cos_alpha) + (drag * sin_alpha));
	state.common_force.z = (sideforce);
	/********** calculating aerodynamic moments********************/
	state.common_moment.x = (cfd.Cl()+  (cfd.Clp() * state.p * b2u) + (cfd.Clr() * state.r * b2u)) * state.q_bar_S * data.b_span;
	state.common_moment.y = (cfd.Cn() + (cfd.Cnp() * state.p * b2u) + cfd.Cnr() * state.r * b2u ) * state.q_bar_S * data.b_span;
	state.common_moment.z = (cfd.Cm() + (cfd.Cmq() * state.q * c2u)  ) * state.q_bar_S * data.chord;
	CLog::cotrace("momentsM,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", cfd.Cm() , cfd.Cmq() , state.q , c2u, (cfd.Cmq() * state.q * c2u), state.q_bar_S, data.chord);
	CLog::cotrace("momentsN,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", cfd.Cn() , cfd.Cnp() , state.p , b2u, (cfd.Cnp() * state.p * b2u), cfd.Cnr() , state.r , b2u, (cfd.Cnr() * state.r * b2u), state.q_bar_S, data.b_span);
	CLog::cotrace("momentsL,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", cfd.Cl() , cfd.Clp() , state.p , b2u, (cfd.Cmq() * state.q * c2u), state.q_bar_S, data.chord);
};
void ed_fm_simulate_mass(double dt)
{

}
void ed_fm_simulate(double dt)
{
	ed_fm_simulate_init(dt);
	ed_fm_simulate_input_control(dt);
	ed_fm_simulate_engine_control(dt);
	ed_fm_simulate_gear_control(dt);
	ed_fm_simulate_mass(dt);
	ed_fm_simulate_cfd();
}

void ed_fm_set_atmosphere(double h,//altitude above sea level
							double t,//current atmosphere temperature , Kelwins
							double a,//speed of sound
							double ro,// atmosphere density
							double p,// atmosphere pressure
							double wind_vx,//components of velocity vector, including turbulence in world coordinate system
							double wind_vy,//components of velocity vector, including turbulence in world coordinate system
							double wind_vz //components of velocity vector, including turbulence in world coordinate system
						)
{
	//TODO: check if need to adjust the y/z axis
	//wind.x = wind_vx;
	//wind.y = wind_vy;
	//wind.z = wind_vz;

	state.atmosphere_density = ro;
	state.atmosphere_pressure = p;// kg_m^3
	state.speed_of_sound     = a;
}
/*
called before simulation to set up your environment for the next step
*/
void ed_fm_set_current_mass_state (double mass,
									double center_of_mass_x,
									double center_of_mass_y,
									double center_of_mass_z,
									double moment_of_inertia_x,
									double moment_of_inertia_y,
									double moment_of_inertia_z
									)
{
	//TODO: check if need to adjust the y/z axis
	state.currmass = mass;
	state.center_of_gravity.x  = center_of_mass_x;
	state.center_of_gravity.y  = center_of_mass_y;
	state.center_of_gravity.z  = center_of_mass_z;
	state.moment_of_inertia.x = moment_of_inertia_x;
	state.moment_of_inertia.y = moment_of_inertia_y;
	state.moment_of_inertia.z = moment_of_inertia_z;
}
/*
called before simulation to set up your environment for the next step
*/
void ed_fm_set_current_state (double ax,//linear acceleration component in world coordinate system
							double ay,//linear acceleration component in world coordinate system
							double az,//linear acceleration component in world coordinate system
							double vx,//linear velocity component in world coordinate system
							double vy,//linear velocity component in world coordinate system
							double vz,//linear velocity component in world coordinate system
							double px,//center of the body position in world coordinate system
							double py,//center of the body position in world coordinate system
							double pz,//center of the body position in world coordinate system
							double omegadotx,//angular accelearation components in world coordinate system
							double omegadoty,//angular accelearation components in world coordinate system
							double omegadotz,//angular accelearation components in world coordinate system
							double omegax,//angular velocity components in world coordinate system
							double omegay,//angular velocity components in world coordinate system
							double omegaz,//angular velocity components in world coordinate system
							double quaternion_x,//orientation quaternion components in world coordinate system
							double quaternion_y,//orientation quaternion components in world coordinate system
							double quaternion_z,//orientation quaternion components in world coordinate system
							double quaternion_w //orientation quaternion components in world coordinate system
							)
{
	//TODO: check if need to adjust the y/z axis
	//velocity_world_cs.x = vx;
	//velocity_world_cs.y = vy;
	//velocity_world_cs.z = vz;
}

/*/////////////////////////////////////////////////////////////////////////
Pointer to function of force source in body axis
x,y,z			  - force components in body coordinate system
pos_x,pos_y,pos_z - position of force source in body coordinate system

body coordinate system system is always X - positive forward ,
										Y - positive up,
										Z - positive to right
*/
void ed_fm_set_current_state_body_axis(double _ax,//linear acceleration component in body coordinate system
	double _ay,//linear acceleration component in body coordinate system
	double _az,//linear acceleration component in body coordinate system
	double _vx,//linear velocity component in body coordinate system
	double _vy,//linear velocity component in body coordinate system
	double _vz,//linear velocity component in body coordinate system
	double _wind_vx,//wind linear velocity component in body coordinate system
	double _wind_vy,//wind linear velocity component in body coordinate system
	double _wind_vz,//wind linear velocity component in body coordinate system

	double _omegadotx,//angular accelearation components in body coordinate system
	double _omegadoty,//angular accelearation components in body coordinate system
	double _omegadotz,//angular accelearation components in body coordinate system
	double _omegax,//angular velocity components in body coordinate system
	double _omegay,//angular velocity components in body coordinate system
	double _omegaz,//angular velocity components in body coordinate system
	double _yaw,  //radians
	double _pitch,//radians
	double _roll, //radians
	double common_angle_of_attack, //AoA radians
	double common_angle_of_slide   //AoS radians
	)
{
	state.ax = _ax;//linear acceleration component in body coordinate system
	state.ay = _ay;//linear acceleration component in body coordinate system
	state.az = _az;//linear acceleration component in body coordinate system

	state.vx = _vx;//linear velocity component in body coordinate system
	state.vy = _vy;//linear velocity component in body coordinate system
	state.vz = _vz;//linear velocity component in body coordinate system

	state.wind_vx = _wind_vx;//wind linear velocity component in body coordinate system
	state.wind_vy = _wind_vy;//wind linear velocity component in body coordinate system
	state.wind_vz = _wind_vz;//wind linear velocity component in body coordinate system

	state.p_dot = _omegadotx;//angular accelearation components in body coordinate system
	state.r_dot = -_omegadoty;//angular accelearation components in body coordinate system
	state.q_dot = _omegadotz;//angular accelearation components in body coordinate system
	// roll, yaw, pitch
	state.p = _omegax;//angular velocity components in body coordinate system  // p roll
	state.r = -_omegay;//angular velocity components in body coordinate system  // r yaw
	state.q = _omegaz;//angular velocity components in body coordinate system  // q pitch
//	CLog::cotrace("state,%f,%f,%f\n", _omegax, _omegay, _omegaz);

	state.yaw   = _yaw;  //radians
	state.pitch = _pitch;//radians
	state.roll  = _roll; //radians

	state.alpha_last = state.alpha; //save last alpha radians
	state.beta_last = state.beta;   //save last beta radians

	state.alpha = common_angle_of_attack; //AoA radians
	state.beta = common_angle_of_slide;   //AoS radians
}
/*
input handling
*/
void ed_fm_set_command (int command, float value)
{
	//CLog::trace(0, "Input,%d,%f\n", command, value);
	switch (command) 
	{
	case 2001://iCommandPlanePitch
		state.stick_elevator = (-value);		break;
	case 2002://iCommandPlaneRoll
		state.stick_aliron = (-value);		break;
	case 2003://iCommandPlaneRudder
		state.stick_rudder = (value);		break;
	case 2004://iCommandPlaneThrustCommon
		state.stick_throttle = (1.0 - value) * 0.5;                 break;  // so values will be between 0 and 100
	case 68: // Gear  -- toggle
		state.gear_handle == 0 ? state.gear_handle = 1 : state.gear_handle = 0; break;
	case 72: // PlaneFlaps 
		state.slats_handle == 0 ? state.slats_handle = 1 : state.slats_handle = 0; break;
	case 73: // PlaneAirBrake - toggle
		state.airbrakes_handle =0 ? state.airbrakes_handle = 1 : state.airbrakes_handle = 0;	break;
	case 74: //PlaneWheelBrakeOn
		state.wheelbreaks_handle = 1;// value;						break;
	case 75: //PlaneWheelBrakeOff 
		state.wheelbreaks = 0;						break;
	case 76: //PlaneParachute 
		state.parashute_handle == 0 ? state.parashute_handle = 1 : state.parashute_handle = 0; break;
	case 430: // GEAR_UP 
		state.gear_handle = 0;
		break;
	case 431: // GEAR DOWN /
		state.gear_handle = 1;
		break;
	}
}
/*
	Mass handling 

	will be called  after ed_fm_simulate :
	you should collect mass changes in ed_fm_simulate 

	double delta_mass = 0;
	double x = 0;
	double y = 0; 
	double z = 0;
	double piece_of_mass_MOI_x = 0;
	double piece_of_mass_MOI_y = 0; 
	double piece_of_mass_MOI_z = 0;
 
	//
	while (ed_fm_change_mass(delta_mass,x,y,z,piece_of_mass_MOI_x,piece_of_mass_MOI_y,piece_of_mass_MOI_z))
	{
	//internal DCS calculations for changing mass, center of gravity,  and moments of inertia
	}
*/
bool ed_fm_change_mass  (double & delta_mass,
						double & delta_mass_pos_x,
						double & delta_mass_pos_y,
						double & delta_mass_pos_z,
						double & delta_mass_moment_of_inertia_x,
						double & delta_mass_moment_of_inertia_y,
						double & delta_mass_moment_of_inertia_z
						)
{
	if (state.fuel_consumption_since_last_time > 0)
	{
		delta_mass		 = state.fuel_consumption_since_last_time;
		delta_mass_pos_x = 0;// -1.0;
		delta_mass_pos_y = 0;// 1.0;
		delta_mass_pos_z = 0;

		delta_mass_moment_of_inertia_x	= 0;
		delta_mass_moment_of_inertia_y	= 0;
		delta_mass_moment_of_inertia_z	= 0;

		state.fuel_consumption_since_last_time = 0; // set it 0 to avoid infinite loop, because it called in cycle 
		// better to use stack like structure for mass changing 
		return true;
	}
	else 
	{
		return false;
	}
}
/*
	set internal fuel volume , init function, called on object creation and for refueling , 
	you should distribute it inside at different fuel tanks
*/
void   ed_fm_set_internal_fuel(double fuel)
{
	state.internal_fuel = fuel;
}
/*
	get internal fuel volume 
*/
double ed_fm_get_internal_fuel()
{
	return state.internal_fuel;
}
/*
	set external fuel volume for each payload station , called for weapon init and on reload
*/
void  ed_fm_set_external_fuel (int	 station, double fuel, double x, double y, double z)
{

}
/*
	get external fuel volume 
*/
double ed_fm_get_external_fuel ()
{
	return 0;
}

//void ed_fm_set_draw_args(EdDrawArgument* drawargs,size_t size)
//{
/*	drawargs[arg_nose_gear].f = (float)state.gear;
	drawargs[arg_right_gear].f = (float)state.gear;
	drawargs[arg_left_gear].f = (float)state.gear;

	drawargs[arg_elevon2L].f = (float)state.elevon2L;
	drawargs[arg_elevon2R].f = (float)state.elevon2R;
	drawargs[arg_elevon1R].f = (float)state.elevon1R;
	drawargs[arg_elevon1L].f = (float)-state.elevon1L;

	drawargs[arg_slats1].f = (float)state.slats1;
	drawargs[arg_slats2].f = (float)state.slats2;
	drawargs[arg_rudder].f = (float)state.stick_rudder;
	drawargs[arg_throttle].f = (float)state.throttle;*/
	//if (size > 616)
	//{	
	//	drawargs[611].f = drawargs[0].f;
	//	drawargs[614].f = drawargs[3].f;
	//	drawargs[616].f = drawargs[5].f;
	//}

//}

void ed_fm_set_draw_args_v2 (float * drawargs,size_t size)
{
	
	drawargs[arg_nose_gear]  = (float)state.gear;
	drawargs[arg_right_gear] = (float)state.gear;
	drawargs[arg_left_gear]  = (float)state.gear;

	drawargs[arg_elevon2R] = (float)state.stick_elevator;
	drawargs[arg_elevon2L] = (float)state.stick_elevator;
	drawargs[arg_elevon1R] = (float)state.stick_aliron;
	drawargs[arg_elevon1L] = (float)-state.stick_aliron;

	drawargs[arg_slats1] = (float)state.slats1;
	drawargs[arg_slats2] = (float)state.slats2;
	drawargs[arg_rudder] = (float)-state.stick_rudder;
	drawargs[arg_throttle] = (float)state.throttle;

}

void ed_fm_configure(const char * cfg_path)
{
	//CLog::cotrace("total, q, CL, CD, CY, Cl, Cm, Cn, \n");
	//CLog::cotrace("forces, common_force.x, common_force.y, common_force.z, common_moment.x, common_moment.y, common_moment.z\n");
	//CLog::cotrace("state, vx, vy, vz, aoa, beta, roll, pitch, yaw, omegax, omegay, omegaz, atmosphere_density, stick_elevator, stick_aliron, stick_rudder, _1_2V\n");

	fmcfg_path = cfg_path;
}


double ed_fm_get_param(unsigned index)
{
	if (index <= ED_FM_END_ENGINE_BLOCK)
	{
		switch (index)
		{
		case ED_FM_ENGINE_0_RPM:			
		case ED_FM_ENGINE_0_RELATED_RPM:	
		case ED_FM_ENGINE_0_THRUST:			
		case ED_FM_ENGINE_0_RELATED_THRUST:	
			return 0; // APU
		case ED_FM_ENGINE_1_RPM:
			return state.throttle * 3000;
		case ED_FM_ENGINE_1_RELATED_RPM:
			return state.throttle;
		case ED_FM_ENGINE_1_THRUST:
			return state.throttle * data.EnginePwrFull * 9.81;
		case ED_FM_ENGINE_1_RELATED_THRUST:
			return state.throttle;
		}
	}
	else if (index >= ED_FM_SUSPENSION_0_RELATIVE_BRAKE_MOMENT &&
			 index < ED_FM_OXYGEN_SUPPLY)
	{
		static const int block_size = ED_FM_SUSPENSION_1_RELATIVE_BRAKE_MOMENT - ED_FM_SUSPENSION_0_RELATIVE_BRAKE_MOMENT;
		switch (index)
		{
		case 0 * block_size + ED_FM_SUSPENSION_0_GEAR_POST_STATE:
		case 1 * block_size + ED_FM_SUSPENSION_0_GEAR_POST_STATE:
		case 2 * block_size + ED_FM_SUSPENSION_0_GEAR_POST_STATE:
			return state.gear;
		}
	}
	return 0;

}


void ed_fm_cold_start()
{
	//CLog::trace(0,"ed_fm_cold_start");
	state.throttle = 0;
	state.gear_handle = 1; 
	state.gear = 1;
	cfd.load(fmcfg_path+ "Mirage2000-vsp.xml");
}

void ed_fm_hot_start()
{
	//CLog::trace(0,"ed_fm_hot_start");
	state.throttle = 0;
	state.gear_handle = 1;
	state.gear = 1;
	cfd.load(fmcfg_path + "Mirage2000-vsp.xml");
}

void ed_fm_hot_start_in_air()
{
	//CLog::trace(0,"ed_fm_hot_start_in_air");
	state.throttle = 0;
	state.gear_handle = 0;
	state.gear = 0;
	cfd.load(fmcfg_path + "Mirage2000-vsp.xml");
}

//////////////
// extra api's not in original ED_FM_Template
// wHumanCustomPhysicsAPI.h


/*
called before simulation to set up your environment for the next step

prototype for

ed_fm_set_surface    give parameters of surface under your aircraft usefull for ground effect
*/
void ed_fm_set_surface(double		h,//surface height under the center of aircraft
	double		h_obj,//surface height with objects
	unsigned		surface_type,
	double		normal_x,//components of normal vector to surface
	double		normal_y,//components of normal vector to surface
	double		normal_z//components of normal vector to surface
)
{
	//CLog::trace(0,"ed_fm_set_surface");

}

/*
called before simulation to set up your environment for the next step

prototype for
void ed_fm_wind_vector_field_update_request(wind_vector_field & in_out);
void ed_fm_wind_vector_field_done()

DCS will  call ed_fm_wind_vector_field_update_request first,
after that it will read  in_out  structure and  fill all "field_points_count" points starting from "field"
and fill wind  array with actual atmosphere state in that point

after that ed_fm_wind_vector_field_done  will be called :

wind_vector_field wind_data;
ed_fm_wind_vector_field_update_request(wind_data);
if (wind_data.field)
{
	for (int i = 0; i < wind_data.field_points_count; ++i)
	{
		wind_vector_field_component * pnt = (wind_vector_field_component*)((BYTE*)wind_data.field + i * (wind_data.field_point_size_in_bytes));

		...DCS will request  atmosphere here for  wind in   pnt->pos with respect of wind_data.space
	}
}
ed_fm_wind_vector_field_done();
*/
//void ed_fm_wind_vector_field_update_request(wind_vector_field& in_out)
//{
///	CLog::trace(0,"ed_fm_wind_vector_field_update_request");

//}/

void ed_fm_wind_vector_field_done()
{
//	CLog::trace(0,"ed_fm_wind_vector_field_done");

}

/*
	incremental adding of fuel in case of refueling process

	prototype for ed_fm_refueling_add_fuel(double fuel);

	(optional , if function doesnt exist  ed_fm_set_internal_fuel will be called in the next manner

		ed_fm_set_internal_fuel(ed_fm_get_internal_fuel() + additional_fuel);
	)
*/
void ed_fm_refueling_add_fuel(double fuel)
{
//	CLog::trace(0,"ed_fm_refueling_add_fuel %d", fuel);
	//ed_fm_set_internal_fuel(ed_fm_get_internal_fuel() + fuel);
}


/*
	update draw arguments for your aircraft

	prototype for ed_fm_set_draw_args
	also same prototype is used for ed_fm_set_fc3_cockpit_draw_args  : direct control over cockpit arguments , it can be use full for FC3 cockpits reintegration with new flight model
*/
void ed_fm_set_fc3_cockpit_draw_args_v2(float* array, size_t size)
{
//	CLog::trace(0,"ed_fm_set_fc3_cockpit_draw_args_v2");

}


/*
shake level amplitude for head simulation ,
prototype for ed_fm_get_shake_amplitude
*/
double ed_fm_get_shake_amplitude()
{
//	CLog::trace(0,"ed_fm_get_shake_amplitude");

	return 0.0;
}

/*
will be called for your internal configuration
void ed_fm_release   called when fm not needed anymore : aircraft death etc.
*/
void ed_fm_release()
{
//	CLog::trace(0,"ed_fm_release");

}


/*
ed_fm_make_balance
for experts only : called  after ed_fm_hot_start_in_air for balance FM at actual speed and height , it is directly force aircraft dynamic data in case of success
*/
bool ed_fm_make_balance(double& ax,//linear acceleration component in world coordinate system);
	double& ay,//linear acceleration component in world coordinate system
	double& az,//linear acceleration component in world coordinate system
	double& vx,//linear velocity component in world coordinate system
	double& vy,//linear velocity component in world coordinate system
	double& vz,//linear velocity component in world coordinate system
	double& omegadotx,//angular accelearation components in world coordinate system
	double& omegadoty,//angular accelearation components in world coordinate system
	double& omegadotz,//angular accelearation components in world coordinate system
	double& omegax,//angular velocity components in world coordinate system
	double& omegay,//angular velocity components in world coordinate system
	double& omegaz,//angular velocity components in world coordinate system
	double& yaw,  //radians
	double& pitch,//radians
	double& roll)//radians
{
//	CLog::trace(0,"ed_fm_make_balance");

	return false;
}


//bool ed_fm_enable_debug_info()
/*
enable additional information like force vector visualization , etc.
*/
bool   ed_fm_enable_debug_info()
{
//	CLog::trace(0,"ed_fm_enable_debug_info");

	return true;
}


/*debuf watch output for topl left corner DCS window info  (Ctrl + Pause to show)
ed_fm_debug_watch(int level, char *buffer,char *buf,size_t maxlen)
level - Watch verbosity level.
ED_WATCH_BRIEF   = 0,
ED_WATCH_NORMAL  = 1,
ED_WATCH_FULL	 = 2,

return value  - amount of written bytes*/
size_t ed_fm_debug_watch(int level, char* buffer, size_t maxlen)
{
//	CLog::trace(0,"ed_fm_debug_watch");

	return 0;
}



// void ed_fm_set_plugin_data_install_path(const char * )  path to your plugin installed
void ed_fm_set_plugin_data_install_path(const char* path)
{
//	CLog::trace(0,"ed_fm_set_plugin_data_install_path %s", path);

}

// damages and failures
// void ed_fm_on_planned_failure(const char * ) callback when preplaneed failure triggered from mission 
void ed_fm_on_planned_failure(const char*)
{
//	CLog::trace(0,"ed_fm_on_planned_failure");

}

// void ed_fm_on_damage(int Element, double element_integrity_factor) callback when damage occurs for airframe element 
void ed_fm_on_damage(int Element, double element_integrity_factor)
{
//	CLog::trace(0,"ed_fm_on_damage");
}

// void ed_fm_repair()  called in case of repair routine 
void ed_fm_repair()
{
//	CLog::trace(0,"ed_fm_repair");
}

// bool ed_fm_need_to_be_repaired()  in case of some internal damages or system failures this function return true , to switch on repair process
bool ed_fm_need_to_be_repaired()
{
//	CLog::trace(0,"ed_fm_need_to_be_repaired");

	return false;
}

// void ed_fm_set_immortal(bool value)  inform about  invulnerability settings
void ed_fm_set_immortal(bool value)
{
//	CLog::trace(0,"ed_fm_set_immortal");
}
// void ed_fm_unlimited_fuel(bool value)  inform about  unlimited fuel
void ed_fm_unlimited_fuel(bool value)
{
//	CLog::trace(0,"ed_fm_unlimited_fuel");
}

// void ed_fm_set_easy_flight(bool value)  inform about simplified flight model request 
void ed_fm_set_easy_flight(bool value)
{
//	CLog::trace(0,"ed_fm_set_easy_flight");
}


// void ed_fm_set_property_numeric(const char * property_name,float value)   custom properties sheet 
void ed_fm_set_property_numeric(const char* property_name, float value)
{
//	CLog::trace(0,"ed_fm_set_property_numeric");
}
// void ed_fm_set_property_string(const char * property_name,const char * value)   custom properties sheet 
void ed_fm_set_property_string(const char* property_name, const char* value)
{
//	CLog::trace(0,"ed_fm_set_property_string");
}


//inform DCS about internal simulation event, like structure damage , failure , or effect
//
//struct ed_fm_simulation_event
//{
//	unsigned     event_type;
//	char         event_message[512];    // optional 
//	float 		 event_params[16]; // type specific data  , like coordinates , scales etc 
//};
//
// bool ed_fm_pop_simulation_event(ed_fm_simulation_event & out)  called on each sim step 
/*
	ed_fm_simulation_event event;
	while (ed_fm_pop_simulation_event(event))
	{
		do some with event data;
	}
*/
//bool ed_fm_pop_simulation_event(ed_fm_simulation_event& out)
//{
	//CLog::trace(0,"ed_fm_pop_simulation_event");

//	return false;
//}


// bool ed_fm_push_simulation_event(const ed_fm_simulation_event & in) // same as pop . but function direction is reversed -> DCS will call it for your FM when ingame event occurs
//bool ed_fm_push_simulation_event(const ed_fm_simulation_event& in)
//{
	//CLog::trace(0,"ed_fm_push_simulation_event");

//	return false;
//}



/*
	void ed_fm_suspension_feedback(int idx,const ed_fm_suspension_info * info)
	feedback to your fm about suspension state
*/
void ed_fm_suspension_feedback(int idx, const ed_fm_suspension_info* info)
{
	//CLog::trace(0,"ed_fm_suspension_feedback");
}



/*
	bool ed_fm_LERX_vortex_update(unsigned idx,LERX_vortex & out)
	idx - index of vortex ; 0 - left side , 1 - right side , others is not specified
	control animation of lerx vortex effects of your airframe

	LERX_vortex out;

	while (ed_fm_LERX_vortex_update(idx,out))
	{
		if (!out.spline || !out.spline_points_count)
		{
			if (vortex_exist(idx))
				vortex_detach(idx);
		}
		else
		{
			if (vortex_exist(idx))
				vortex_update(idx,out);
			else
				vortex_create(idx,out);
		}
		++idx;
	}
*/
bool ed_fm_LERX_vortex_update(unsigned idx, LERX_vortex& out)
{
	//CLog::trace(0,"ed_fm_LERX_vortex_update");

	return false;
}


/*
	Vec3 thrust_pos(0.0,0,0);
	Vec3 thrust(throttle * EnginePwr, 0 , 0);

	double Mach		= V_scalar/ speed_of_sound;
	double CyAlpha_ = lerp(mach_table,Cya  ,sizeof(mach_table)/sizeof(double),Mach);
	double Cx0_     = lerp(mach_table,cx0  ,sizeof(mach_table)/sizeof(double),Mach);
	double CyMax_   = lerp(mach_table,CyMax,sizeof(mach_table)/sizeof(double),Mach);
	double B_	    = lerp(mach_table,B    ,sizeof(mach_table)/sizeof(double),Mach);
	double B4_	    = lerp(mach_table,B4   ,sizeof(mach_table)/sizeof(double),Mach);


	double Cy  = (CyAlpha_ * 57.3) * aoa;
	if (fabs(aoa) > (90/57.3) )
		Cy = 0;
	if (Cy > CyMax_)
		Cy = CyMax_;

	double Cx  = Cx0_ + B_ * Cy * Cy + B4_ * Cy * Cy * Cy * Cy;

	Vec3 aerodynamic_force((throttle * EnginePwr)-Drag , -Lift , 0 );
	Vec3 aerodynamic_force_pos(0.0,0,0);

	add_local_force(aerodynamic_force, center_of_gravity);
	//add_local_force(thrust			 , center_of_gravity);

	Vec3 aileron_left (0 , 0.05 * Cy * (stick_aliron) * q * S , 0 );
	Vec3 aileron_right(0 ,-0.05 * Cy * (stick_aliron) * q * S , 0 );

	Vec3 aileron_left_pos(0,0,-1.0);
	Vec3 aileron_right_pos(0,0, 1.0);

//	add_local_force(aileron_left ,center_of_gravity);
//	add_local_force(aileron_right,center_of_gravity);

	Vec3 elevator(0, -0.05 * Cy * (stick_elevator)*q * S, 0);
	//add_local_force(elevator, center_of_gravity);
	*/
