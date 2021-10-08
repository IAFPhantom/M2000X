// testCFD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "CFD.h"
#include "aero.h"
#include "ED_FM_Utility.h"

int main()
{
    std::cout << "Hello World!\n";
    Matrix a(2,4);
    a.set(0, 0, 1);
    a.set(0, 1, 1);
    a.set(0, 2, 1);
    a.set(0, 3, 1);
    a.set(1, 0, 1);
    a.set(1, 1, 1);
    a.set(1, 2, 1);
    a.set(1, 3, 1);

    Matrix b(4,1);
    b.set(0, 0, 1);
    b.set(0, 1, 1);
    b.set(0, 2, 1);

    Matrix result = a * b;

    /*  FLIGHTSPECS flight = {0};
    flight.type= 4;         // A-4
    flight.planeclass= 1; // jet

    flight.b_span = 27.5;
    flight.S_area = 260;
    flight.cord = 10.8;
    flight.mass = 546;
    flight.Ix = 8090;
    flight.Iy = 25900;
    flight.Iz = 29200;
    flight.Ixz = 1300;
    flight.max_thrust = 0; // no after burner
    flight.mil = 8000;
    flight.CDo = 0.03;
    flight.CDa = 0.3;
    flight.CLo = 0.28;
    flight.CLa = 3.45;
    flight.CLq = 0.0;
    flight.CLda = 0.72;
    flight.CLde = 0.36;
    flight.CMo = 0.0;
    flight.CMa = -0.38;
    flight.CMq = -3.6;
    flight.CMda = -1.1;
    flight.CMde = -0.5;
    flight.CYb = 0.98;
    flight.CYdr = 0.17;
    flight.CLb = -0.12;
    flight.CLp = -0.26;
    flight.CLr = 0.14;
    flight.CLda = 0.08;
    flight.CLdr = -0.105;
    flight.CNb = 0.25;
    flight.CNp = 0.022;
    flight.CNr = -0.35;
    flight.CNda = 0.06;
    flight.CNdr = 0.032;
    flight.def_rud = 0.2618;
    flight.def_ail = -0.5236;
    flight.def_elev = 0.5236;

    ACFT air = { 0 };
    air.type = 4;
    air.status = 0;
    air.Tptr = 0;
    aero_model(&air, &flight, 0.02);*/
    CFD cfd;
    if (cfd.load("../../Mirage2000-vsp.xml"))
    {
        PlaneState State = { 0 };
        State.vx = 0.2;
        State.alpha = 0.82;
        State.beta = 0;
        State.airbrakes = 0;
        State.gear = 0;
        State.elevon1L = 10;
        State.elevon2L = 10;
        State.elevon1R = 10;
        State.elevon2R = 10;
        State.rudder = 10;
        State.r = 10;
        State.p = 10;
        //State.stick_elevator = 0.1;
        //State.stick_rudder = 0.1;
        State.mach = 0.13;
        cfd.calc_coeffeients(State);
        std::cout << "CFXB: " << cfd.CD() << std::endl;
        std::cout << "CFZB: " << cfd.CL() << std::endl;
        std::cout << "CMM: " << cfd.Cm() << "  CMQ: " << cfd.Cmq() << std::endl;
        std::cout << "CFYB: " << cfd.CY() << std::endl;
        std::cout << "CML: " << cfd.Cl() << "  CLP: " << cfd.Clp() << "  CLR: " << cfd.Clr() << std::endl;
        std::cout << "CMN: " << cfd.Cn() << "  CNP: " << cfd.Cnp() << "  CNR: " << cfd.Cnr() << std::endl;
    }
    else
        std::cout << "couldn't load file!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
