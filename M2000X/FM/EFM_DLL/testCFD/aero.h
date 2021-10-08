#pragma once

#define GRAVITY 9.81
#define FT_TO_METERS 3.281
#define DENSITYSL 1.225
#define MAX_ALT 8000
#define ALT_INCREMENT 100
#define PROP_EFFIC 1
#define HPR2THRUST 10
#define TENTH_OF_AIRSPEED_FACTOR 10
//#define LEFTS
//#define RIGHTSCOPE 59.0
//#define TOPSCOPE 14.0 BOTTOMSCOPE 3.5 SCANRATE 72.0
//#define SCANWIDTH 120.0
//#define SCOPEWIDTH(RIGHTSCOPE - LEFTSCOPE)
//#define HALFSCANWIDTH(0.5 * SCANWIDTH)
//#define SCOPERATE(SCANWIDTH / SCOPEWIDTH)
//#define CENTERSCOPE(LEFTSCOPE + (SCOPEWIDTH / 2.0))
//#define COMPASSRADIUS5.0
//#define TEN(COMPASSRADIUS - 0.5)
//#define TfflRTY(COMPASSRADIUS - 1.0)

//typedef double* Matrix;

typedef struct
{
	int type;
	int planeclass; /*] ;jet 0;prop*/
	double b_span; /*wing span */
	double S_area; /*wing area */
	double cord; /*mean aerodynamic cord*/
	double mass; /*mass */
	double Ix; /*x axis inertia! term*/
	double Iy; /*y axis inertial term*/
	double Iz; /*z axis inertial temi*/
	double Ixz; /*cross inertial term*/
	double horsepwr; /*used for props*/
	double max_thrust; /*max afterburner thrust*/
	double mil; /*max non afterburner thrust*/
	double CDo; /*preference drag coefficient*/
	double CDa; /*W-force drag coefficient due to aoa */
	double CLo; /*reference lift coefficient*/
	double CLdalpha; /*delta alpha */
	double CLq; /*lift due to pitch moment*/
	double CLa; /*lift due to angle of attack*/
	double CLde; /*lift due to elevator deflection*/
	double CMo; /*pitch moment coefficient at alpha */
	double CMq; /*pitch moment coefficient due to pitch rate */
	double CMa; /*pitch moment coefficient due to aoa */
	double CMda; /*pitch moment coefficient due to delta aoa */
	double CMde; /*pitch moment coefficient from elevator motion*/
	double CYb; /* Y - force coefficent due to side slip angle */
	double CYdr; /*Y-force coefficent due to change in yaw rate */
	double CLr; /*roll moment coefficient due to yaw rate */
	double CLp; /*roll moment coefficient due to roll rate */
	double CLda; /*roll moment coefficient due to change in ail */
	double CLb; /*roll moment coefficient due to side slip */
	double CLdr; /*roll moment coefficient due to change in yaw */
	double CNda; /*aoa change effect on yaw moment */
	double CNb; /*sideslip change effect on yaw */
	double CNp; /*roll rate effect on yaw moment */
	double CNr; /*yaw rate effect on yaw moment */
	double CNdr; /*yaw acceleration effect on yaw moment */
	double def_rud; /* rudder deflection + / -in radians */
	double def_ail; /*aileron deflection +/- in radiaas */
	double def_elev; /*elevator deflection +/- in radians */
	double engine_alignment_x, engine_alignment_y, engine_alignment_z; /* engine alignement angle rad*/
}
FLIGHTSPECS;
typedef FLIGHTSPECS* TYPEPTR;

typedef struct
{
	int totalacft; /*track total number of aircraft in siniul*/
	TYPEPTR Tptr; /*pointer to aircraft type information*/
	double fore[3]; /*lbs; Forces in X,Y,Z direction*/
	double torq[3]; /*ft-lbs; Torques around X.Y.Z axis*/
	double lin_vel[3]; /*ft/sec; linear vel(u,v,w) in X,Y.Z dir*/
	double ang_vel[3]; /*rad/sec; ang vel(p,q,r) around X.Y.Z axes*/
	double lin_accel[3]; /*fps2; linear accels in X.Y.Z direction*/
	double ang_accel[3]; /*deg/sec2;angaccel(p,q,r) around X.Y.Z axes*/
	double sideslip; /*sideslip or beta*/
	double ang_atk; /*angle of attack*/
	double d_ang_atk; /*angle of attack rate*/
	double lift; /* total lift*/
	double drag; /*total drag*/
	double quat[4]; /*quaterion*/
	double h_matrix[3][3]; /* direction cosine matrix of world position*/
	double sroll; /*sine of roll*/
	double croll; /* cosine of roll*/
	double sptch; /* sine of pitch */
	double cplch; /* cosine of pilch */
	double shdg; /*sine of heading*/
	double chdg; /*cosine of heading*/
	double euler_angles[3]; /* euler angles in radians - ro!l, pitch, yaw*/
	double vel_world[3]; /* meters / sec; velocity in SGI coords X, Y, Z*/
	double hdg; /* world heading*/
	double gfor; /*g force exerted from back stick*/
	double rpm; /* fraction; 0.0 1.0 engine rpm */
	double elev; /* position; -1.0 to 1.0 */
	double eltrim; /* elevator trim */
	double rud; /* position; -1.0 to 1.0 */
	double ail; /* position ; -1.0 to 1.0 */
	double thro; /* position; 0.0 to 1.0 */
	int flaps; /* boolean; 0 - flapsup 1 - flapsdown */
	int gear; /* boolean; 0 - gearup 1 - geardown */
} AIRVEH;

typedef struct
{
	int ID; //number assignment
	int type; //aircraft type
	int status; //piloted;0 or autonomous level turn ; 1 climbing turn 2
	int Tptr; //pointer to aircraft specification data structure
//	double Forces[3]; //forces in X, Y, Z dir
//	double Torques[3]; //torques around X, Y, Z axis
//	double linear_vel[3]; // velocity in X.Y.Z direction
//	double angular_vel[3]; //angular velocity around X, Y, Z axes
//	double linear_accel[3]; //linear accelerations
//	double angular_accel[3]; //angular accelerations
	double sideslip ; //sideslip or beta angle
	double ang_atk ; //angle of attack
	double d_ang_atk; //angle of attack rate
	double lift; //total lift 
	double drag ; //total drag
	double sideforce; //total side force
	double Fax, Fay, Faz;//total force
	double Fx, Fy, Fz;//total force
	double L, M, N;//total moments
	double Q[4]; //quaternion
	double H[3][3]; //direction cosine matrix
	double euler_angles[3] ; //euler angles angles in radians - yaw.pitch.roll
	double pos[3]; //world position in X.Y, Z
	double ref[3]; //-look direction
	double vel_world[3]; //-velocities in world position - X, Y, Z
	double gfor ; //amount of g force
	double rpm ; //engine rpm
	double elev; //flight control positions
	double eltrim; //elevator trim
	double rdtrim; //rudder trim
	double ailtrim; //aliron trim
	double rud; //rudder position
	double ail; //aileron position
	double thro; //throttle position
	int flaps; //flap position
	int gear; //landing gear position
	//// current aircraft state variables
	double u, v, w;
	double udot, vdot, wdot;
	double p, q, r;
	double pdot, qdot, rdot;
	double sptch, cptch, sroll, croll;
	double posx, posy, posz;
} ACFT;
typedef ACFT* ACFTPTR;

void aero_model(ACFTPTR P, TYPEPTR T, double deltatime);