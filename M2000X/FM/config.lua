-- from SFM
--mach_table = {0,0.2,0.4,0.6,0.7,0.8,0.9,1,1.05,1.1,1.2,1.3,1.5,1.7,1.8,2,2.2,2.5,3.9};  --#19
--cx0 = {0.0165,0.0165,0.0165,0.0165,0.0170,0.0178,0.0215,0.0310,0.0422,0.0440,0.0432,0.0423,0.0416,0.0416,0.0416,0.0410,0.0395,0.0395,0.0395 };
--Cya = {0.077,0.077,0.077,0.080,0.083,0.087,0.091,0.094,0.094,0.091,0.085,0.068,0.051,0.043,0.037,0.036,0.033,0.033,0.033	 };
--B = {0.1,0.1,0.1,0.094,0.094,0.094,0.11,0.15,0.15,0.14,0.17,0.23,0.23,0.08,0.16,0.25,0.35,0.35,0.35	 };
--B4 = {0.032,0.032,0.032,0.043,0.045,0.048,0.050,0.1,0.1,0.1,0.096,0.09,0.38,2.5,3.2,4.5,6.0,6.0,6.0	 };
--CyMax = {1.6,1.6,1.6,1.5,1.45,1.4,1.3,1.2,1.1,1.05,1.0,0.9,0.7,0.55,0.4,0.4,0.4,0.4,0.4 };

-- plane data
wingSpan= 11.79576;			-- //  wing-span (m)
wingArea = 49.2386;			--//  wing area (m^2)
wingMeanChord = 4.8768;			--//  wing mean chord (m^2)
Xref = 0.89;
Xcg = 0.29;

Ixx = 48.508008; // 25000 slug ft^2
Iyy = 237.107144; // 122200 slug ft^2
Izz = 271.256781; // 139800 slug ft^2
Ixz = 4.268705; // 2200 slug ft^2

engineThrustN = 170000;--Newtons
maxalironangle = 0.523599; --// rad
maxelevatorangle = 0.785398; --// rad
maxrudderangle = 0.523599; --// rad

// LONGITUDUNAL 
CDl = 0.03; // 
CDA = 0.3; //
CDAD = 0;
CDQ = 0;
CDU = 0.027;
CDDE = -0.1;
CD =1;

CLl = 0.26;
CLU = 0.27;
CLA = 3.75;
CLAD = 0.86;
CLQ = 1.80;
CLDE = 0.4;
CL = 1;

CM = 1;
CMl = 0;
CMTl = 0;
CMU = -0.117;
CMA = -0.4;
CMAD = -1.30;
CMQ = -2.70;
CMTA = 0;
CMTU = 0;
CMDE = -0.58;

CTXU = -0.064;      // thrust on x by velocity
CTXl = 0.03;   // thrust on X lift
CTZU = 0;      // thrust on side Z by velocity 
CTZl = 0;      // thrust on side Z

// LONGITUDUNAL 
CY = 1;
CYB = -0.680;  // Cn beta
CYBD = 0;  // Cn beta dot
CYP = 0;  // Cn p
CYR = 0;  // Cn r
CYDA = -0.016; // Cn delta Aliron
CYDR = 0.095; // Cn delta Rudder

Cl = 1;
ClB = -0.080;  // Cl beta
ClBD = 0;  // Cl beta dot
ClP = -0.240;  // Cl p
ClR = 0.07;  // Cl r
ClDA = 0.042; // Cl delta Aliron
ClDR = 0.0060; // Cl delta Rudder

Cn = 1;
CnB = 0.125;  // Cn beta
CnBD = 0;  // Cn beta dot
CnP = -0.036;  // Cn p
CnR = -0.27;  // Cn r
CnDA = -0.001; // Cn delta Aliron
CnDR = -0.066; // Cn delta Rudder

