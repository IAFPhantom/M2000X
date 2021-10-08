/***************************************************************************/
/* FILE: aero.c */
/* AUTHOR: Joe Cooke */
/* DATE: 1/9/92 */
/* DESCRIPTION: This file contains the mathematical model for all */
/* aerodynamic calculations. */
/* Input: aircraft state structure (P) */
/* aircraft specification stucture (T) */
/* Output : updated state structure (P) */
/***************************************************************************/
#include <stdio.h>
#include <math.h>
#include "aero.h"

double densaltarray[10] = { 1.225,0,0,0,0,0,0,0 ,0,0 };

void aero_model(ACFTPTR P, TYPEPTR T, double deltatime)
{
	double Q, QS, QSc, QSb, c2u, b2u, g, w, m;	/*misc. variables*/
	double tot_vel;								/*total velocity*/
	double densaltratio, densalt;				/*atmosperic data*/
	double aoa, cos_aoa, sin_aoa;				/*angle of attack data*/
	double cos_sideslip, sin_sideslip;			/*sideslip data*/
	double For_thrust_X;						/*thrust force*/
	double ailpos, rudpos, elevpos;				/*control positions*/
	double Itemp, Ltemp, Ntemp;					/*temporary terms*/
	double du, dv, dw, dp, dq, dr;				/*newly computed accels*/
	double diff;								/*temp var for rpm determination*/
	double loopdelta;							/*deltatime / loop iterations*/
//	double world[3], vwor[3];					/*vel in world inertial frame*/
//	double vaircraft[3];						/*vel in aicraft frame*/
	double hpower;								/* horsepower*/
	double max_thrust;							/*thrust*/
	double loop_iterations;						/*number of aero calc iterations*/
	int ixt, alt;								/*index and loop vars*/
//	Matrix A_MATRIX;							/* temporary matrix*/

	/**************** test for valid structure*********************/
	if (P == NULL || T == NULL)
	{
		printf("****error : null pointer sent to aero . c****\n\n");
		return;
	} /****some initial assignments****/
	m = T->mass;
	g = GRAVITY;
	//w = m * GRAVITY;
	//P->gfor = P->lift / w; /*calculate current G's */
	if (P->u < 10.0) 
		P->u = 10.0;

	tot_vel = sqrt(P->v * P->v + P->w * P->w + P->u * P->u);
	/************ atmosheric density calculations*****************/
	if (P->posy >= 0.0 && P->posy < MAX_ALT)
	{
		alt = (int)(P->posy / (ALT_INCREMENT * FT_TO_METERS)); 
		densalt = densaltarray[alt];
		densaltratio = densalt / DENSITYSL;
	}
	else {
		densalt = DENSITYSL;
		densaltratio = densalt / DENSITYSL;
	} 
	/************** *calculate new rpm*****************************/
	diff = (P->thro - P->rpm) / (2.0 / deltatime);
	P->rpm = P->rpm + diff;
	/******** ******* calcuate new thrust ********* *****************/
	if (T->planeclass == 0) { /*if a prop plane*/
		hpower = PROP_EFFIC * T->horsepwr;
		max_thrust = (hpower * HPR2THRUST) / tot_vel;
		For_thrust_X = max_thrust * P->rpm * densaltratio;
	}
	else 
	{   /*if a jet plane*/
		if (T->max_thrust > T->mil) 
			For_thrust_X = (T->max_thrust * (P->rpm)) * densaltratio;
		else 
			For_thrust_X = (T->mil * (P->rpm)) * densaltratio;
	} 
	/* ******* **determine control differentials********************/
	ailpos = (P->ail * T->def_ail) + P->ailtrim;
	rudpos = (P->rud * T->def_rud) + P->rdtrim;
	elevpos = (P->elev * T->def_elev) + P->eltrim;
	/************adujust time step for aero calculations********/
	loop_iterations = (double)((int)(tot_vel * TENTH_OF_AIRSPEED_FACTOR)); 
	loopdelta = deltatime * 1.0 / loop_iterations;
	/******** ***loop to reduce time step for dynamic model*****/
	for (ixt = 0; ixt < ((int)loop_iterations); ixt++) 
	{
		if (P->u < 10.0) 
			P->u = 10.0;
		tot_vel = sqrt((P->v * P->v) + (P->w * P->w) + (P->u * P->u));
		P->sideslip = atan((P->v / P->u));
		sin_sideslip = (sin(P->sideslip));
		cos_sideslip = (cos(P->sideslip));

		aoa = atan((P->w / P->u));/* + (5.0/RAD_TO_DEG) */
		cos_aoa = (cos(aoa)); 
		sin_aoa = (sin(aoa)); 
		P->d_ang_atk = aoa - P->ang_atk;
		P->ang_atk = aoa;

		Q = 0.5 * densalt * tot_vel * tot_vel;
		QS = Q * T->S_area;
		QSc = QS * T->cord;
		QSb = QS * T->b_span;
		c2u = T->cord / (2.0 * tot_vel);
		b2u = T->b_span / (2.0 * tot_vel); 
		/********** calculating aerodynamic forces*********************/
		P->lift = (T->CLo + (T->CLa * P->ang_atk) + (T->CLq * P->q * c2u) + (T->CLdalpha * P->d_ang_atk * c2u) + (T->CLde * elevpos)) * QS;
		P->drag = (T->CDo + (T->CDa * P->ang_atk)) * QS;
		P->sideforce =((T->CYb * P->sideslip) + (T->CYdr * rudpos)) * QS;
		P->Fax = ((P->lift * sin_aoa) - (P->drag * cos_aoa) - (P->sideforce * sin_sideslip));
		P->Fay = (P->sideforce * cos_sideslip);
		P->Faz = ((-P->lift * cos_aoa) - (P->drag * sin_aoa));
		/********** calculating aerodynamic moments********************/
		P->L = (T->CLb * P->sideslip + T->CLp * P->p * b2u + T->CLr * P->r * b2u + T->CLda * ailpos + T->CLdr * rudpos) * QSb;
		// P->L +=  LTorque
		P->M = (T->CMo + T->CMa * P->ang_atk + T->CMq * P->q * c2u + T->CMda * P->d_ang_atk * c2u + T->CMde * elevpos) * QSc;
		// P->M +=  MThrust + Mgyro
		P->N = (T->CNb * P->sideslip + T->CNp * P->p * b2u + T->CNr * P->r * b2u + T->CNda * ailpos + T->CNdr * rudpos) * QSb;
		// P->N +=  NThrust + Ngyro
		/*** *dete rmine total moments and forces***********************/
		P->Fx = P->Fax + For_thrust_X * sin(T->engine_alignment_x);
		P->Fy = P->Fay + For_thrust_X * cos(T->engine_alignment_y);
		P->Fz = P->Faz + For_thrust_X * cos(T->engine_alignment_z);
		/** ** * temporary calculations** **********/
		Ltemp = P->L + T->Ixz * P->p * P->q - (T->Iz - T->Iy) * P->r * P->q;
		Ntemp = P->N - (T->Iy - T->Ix) * P->p * P->q - T->Ixz * P->r * P->q;
		Itemp = (T->Ix * T->Iz - T->Ixz * T->Ixz); 
		/******determine Linear and Angular Accelerations****** *******************/
		du = (P->v * P->r - P->w * P->q + P->Fx / m - g * P->sptch);
		dv = (P->p * P->w - P->r * P->u + P->Fy / m + g * P->sroll * P->cptch);
		dw = (P->u * P->q - P->v * P->p + P->Fz / m + g * P->croll * P->cptch);
		dp = (Ltemp * T->Iz + Ntemp * T->Ixz) / Itemp;
		dq = (P->M - ((T->Ix - T->Iz) * P->p * P->r) - ((P->p * P->p - P->r * P->r) * T->Ixz)) / T->Iy;
		dr = (Ntemp * T->Ix + Ltemp * T->Ixz) / Itemp;
						
		/*** determine velocities with trapizoidal integration****/
		P->u = P->u + ((P->udot + du) / 2.0)* loopdelta;
		P->v = P->v + ((P->vdot + dv) / 2.0)* loopdelta;
		P->w = P->w + ((P->wdot + dw) / 2.0)* loopdelta;
		P->p = P->p + ((P->pdot + dp) / 2.0)* loopdelta;
		P->q = P->q + ((P->qdot + dq ) / 2.0)* loopdelta;
		P->r = P->r + ((P->rdot + dr ) / 2.0)* loopdelta;
						
		/****** record last acceleration********^**************/
		P->udot = du; P->vdot = dv; P->wdot = dw;
		P->pdot = dp; P->qdot = dq; P->rdot = dr;
	} /* end loop * **/

	/****incrementally rotate quaternion * **************/
	//increment_quat(P->p, P->q, P->r, P->Q, deltatime); 
	///**** ^update rotation matrix from quaternion^**************/
	//rotation_matrix_from_quat(P->Q, P->H); 
	///* extract euler angles from matrix*********************/
	//euler_angles_from_matrix(&P->sroll, &P->croll, &P->sptch, &P->cptch,
	//		&P->shdg, &P->chdg, P->h_matrix);
	//euler_ang_f rm_rot_matrix(&P->roll, &P->ptch, &P->hdg, P->H);
	//P->ptch = P->ptch * RAD_TO_DEG;
	//P->roll = P->roll * RAD_TO_DEG;
	//P->hdg = P->hdg * RAD_TO_DEG;
	///****calculate new world velocity**************************/
	//vaircraft[0] = P->u;
	//vaircraft[1] = P->v;
	//vaircraft[2] - P->w;
	//transpose_matrix(P->H, A_MATRIX);
	//post_mult_matrix_by_vector(A_MATRIX, vaircraft, vworld);
	//vwor[O] = vworld[0] * FT_TO_METERS;
	//vwor[l] = -vworld[2] * FT_TO_METERS;
	//vwor[2] = vworld[l] * FT_TO_METERS;
	///* calculate new world position * *************************/
	//P->H[3][0] = P->posx += vwor[0] * deltatime;
	//P->H[3][1] - P->posy += vworfl] * deltatime;
	//P->H[3][2] = P->posz += vwor[2] * deltatime;
	//P->H[3][3] = 1.0;
	///* calculate world reference point*********/
	//P->refx = P->posx + (P->cptch * P->chdg);
	//P->refz = P->posz + (P->cptch * P->shdg);
	//P->refy = P->posy + P->sptch;
}
			