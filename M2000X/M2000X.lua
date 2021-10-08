--dofile(current_mod_path.."/Weapons.lua")
M2000X =  {
	Name 				=   'M2000X',
	DisplayName			= _('M2000X'),
	ViewSettings		= ViewSettings,

	EmptyWeight 		= "7500",
	MaxFuelWeight 		= 2570,
	MaxHeight 			= "21336", -- 70000ft
	MaxSpeed 			= "1481.6",
	MaxTakeOffWeight 	= "21081",
	WingSpan 			= "8.22",
	
	Picture 			= "M-2000C.png",
	Rate 				= 40, -- RewardPoint in Multiplayer
	Shape 				= "M2000X",

	shape_table_data 	=
	{
		{
			file  	 = 'M2000X';
			life  	 = 18; -- lifebar
			vis   	 = 3; -- visibility gain.
			desrt    = 'M2000X-destr'; -- Name of destroyed object file name
			fire  	 = { 300, 2}; -- Fire on the ground after destoyed: 300sec 2m
			username = 'M2000X';
			index    =  WSTYPE_PLACEHOLDER;
			--classname = "lLandPlane";
			--positioning = "BYNORMAL";
		},
		{
			name  = "M2000X-destr";
			file  = "M2000X-destr";
			fire  = { 240, 2};
		},

	},


	-------------------------

	mapclasskey	= "P0091000024",
	attribute	= {wsType_Air, wsType_Airplane, wsType_Fighter,WSTYPE_PLACEHOLDER, "Multirole fighters" },  -- C.7 "Refuelable" 
	Categories 	= {"{78EFB7A2-FD52-4b57-A6A6-3BF0E1D6555F}", "Interceptor",},
	-------------------------
	M_empty	=	7500,
	M_nominal	=	9525,
	M_max	=	17000,
	M_fuel_max	=	3160,
	H_max	=	16460,
	average_fuel_consumption	=	0.07,
	CAS_min	=	52,
	V_opt	=	170,
	V_take_off	=	64,
	V_land	=	64,
	has_afteburner	=	true,
	has_speedbrake	=	true,
	main_gear_pos = 	{-1.121,	-1.748,	1.751},
	radar_can_see_ground	=	true,
	nose_gear_pos = 	{3.924,	-1.746,	0},
	AOA_take_off	=	0.14,
	stores_number	=	9,
	bank_angle_max	=	60,
	Ny_min	=	-2,
	Ny_max	=	8,
	tand_gear_max	=	0.577,
	V_max_sea_level	=	403,
	V_max_h	=	660,
	tanker_type	=	2,
	wing_area	=	41,
	wing_span	=	9.13,
	thrust_sum_max	=	6430,
	thrust_sum_ab	=	9810,
	Vy_max	=	315.3,
	length	=	14.36,
	height	=	5.2,
	flaps_maneuver	=	1,
	Mach_max	=	2.3,
	range	=	1852,
	crew_size	=	1,
	RCS	=	5,
	Ny_max_e	=	8,
	detection_range_max	=	150,
	IR_emission_coeff	=	0.8,
	IR_emission_coeff_ab	=	3,
	engines_count	=	1,
	wing_tip_pos = 	{-4.372,	-0.435,	4.583},
	nose_gear_wheel_diameter	=	0.754,
	main_gear_wheel_diameter	=	0.972,
	engines_nozzles = 
	{
		[1] = 
		{
			pos = 	{-6.63,	0.401,	0},
			elevation	=	0,
			diameter	=	0.9,
			exhaust_length_ab	=	6.8,
			exhaust_length_ab_K	=	0.76,
			smokiness_level     = 	0.1, 
		}, -- end of [1]
	}, -- end of engines_nozzles
	crew_members = 
	{
		[1] = 
		{
			ejection_seat_name	=	9,
			drop_canopy_name	=	40,
			pos = 	{3.369,	-0.146,	0},
		}, -- end of [1]
	}, -- end of crew_members
	brakeshute_name	=	0,
	is_tanker	=	false,
	air_refuel_receptacle_pos = 	{5.928,	0.885,	0.521},
	fires_pos = 
	{
		[1] = 	{-1.606,	-0.489,	0},
		[2] = 	{-2.455,	-0.079,	1.466},
		[3] = 	{-2.521,	-0.136,	-2.015},
		[4] = 	{-0.82,	0.265,	2.774},
		[5] = 	{-0.82,	0.265,	-2.774},
		[6] = 	{-0.82,	0.255,	4.274},
		[7] = 	{-0.82,	0.255,	-4.274},
		[8] = 	{-5.63,	0.401,	0},
		[9] = 	{-5.63,	0.401,	0},
		[10] = 	{-2.346,	-0.448,	0},
		[11] = 	{2.346,	-0.448,	0},
	}, -- end of fires_pos
		-----------------------------------------------------------------------
	----------------- SUSPENSION CODE BEGINS
	-----------------------------------------------------------------------

	tand_gear_max 							 = 2.050,	-- tangent on maximum yaw angle of front wheel, 65 degrees tan(64deg)

	nose_gear_pos 			   				 = { 4.680,   -1.727,   0.000},		-- nosegear coord
	nose_gear_wheel_diameter				 =  0.445,		-- in m
	nose_gear_amortizer_direct_stroke        =  1.32,  	    -- down from nose_gear_pos !!!
	nose_gear_amortizer_reversal_stroke      =  0,		-- up
	nose_gear_amortizer_normal_weight_stroke =  0,		-- down from nose_gear_pos
	
	main_gear_pos 							 = {-0.328,   -1.5919,    1.62},	-- main gear coords
	main_gear_wheel_diameter				 =  0.72345,				-- in m
	main_gear_amortizer_direct_stroke        =  1.3288,		-- down from main_gear_pos !!!
	main_gear_amortizer_reversal_stroke      =  -0.4, 		-- up
	main_gear_amortizer_normal_weight_stroke =  0,				-- down from main_gear_pos

	-----------------------------------------------------------------------
	----------------- SUSPENSION CODE ENDS
	-----------------------------------------------------------------------

	-- Countermeasures
	Countermeasures = {
--		ECM = "AN/ALQ-135"		-- has no internal ECM
	},

	passivCounterm = {
		CMDS_Edit = true,
		SingleChargeTotal = 200,
		chaff = {default = 100, increment = 3, chargeSz = 1},
		flare = {default = 96,  increment = 3, chargeSz = 1},
	},

	chaff_flare_dispenser 	= {
-- TODO: verify
		{ dir =  {-1, 0, -1.0}, pos =   {-3.827,  0.435, -1.108}, }, -- Chaff L
		{ dir =  {-1, 0, 1.0}, pos =   {-3.827,  0.435, 1.108}, },  -- Chaff R
		{ dir =  {-1, 0, -1.0}, pos =  {-5.032,  0.873, -1.2}, }, -- Flares L
		{ dir =  {-1, 0,  1.0}, pos =  {-5.032,  0.873,  1.2}, }, -- Flares R
	},

	--sensors
	detection_range_max		 = 400,
	radar_can_see_ground 	 = true,
	
	CanopyGeometry = {
		azimuth   = {-120.0, 120.0}, -- pilot view horizontal (AI)
		elevation = {-60.0, 90.0}-- pilot view vertical (AI)
	},

     Sensors = {
		RWR = "Abstract RWR", -- RWR type
		RADAR = "RDY",        -- Radar type
     },

	Guns = {-- TODO: Change to DEFA
        gun_mount("GSh_30_1", { count = 150 },{muzzle_pos_connector = "GunLeft", muzzle_pos = {6.0, 0.00, 1.1}}),
		gun_mount("GSh_30_2", { count = 150 },{muzzle_pos_connector = "GunRight", muzzle_pos = {-6.0, 0.00, 1.1}}),
	},

	-- TODO: update Pylon locations
	Pylons = {
		-- left outer pylon 
	--[[	pylon(1, 0,0,0,0,---1,1.796, -0.566, -3.638,  
            {
				use_full_connector_position=true,
            },
            {
				{ CLSID = "{9BFD8C90-F7AE-4e90-833B-BFD0CED0E536}",												Cx_gain = 0.0302},			-- AIM_9P				-- rev.177067
            },
        ),
        pylon(2,0,0,0,0,--- 1, -1.41, -0.593, -1.756,
		    {
				use_full_connector_position=true,
            },
            {
				{ CLSID = "{9BFD8C90-F7AE-4e90-833B-BFD0CED0E536}",												Cx_gain = 0.0302},			-- AIM_9P				-- rev.177067
            },
        ),
        pylon(3, 0,  2.389, -0.695, -0.956,
            {
				use_full_connector_position=true,
            },
            {
				{ CLSID = "{9BFD8C90-F7AE-4e90-833B-BFD0CED0E536}",												Cx_gain = 0.0302},			-- AIM_9P				-- rev.177067
            },
		),
		-- center line
		pylon(4, 0,0,0,0,---0, 0.283, -0.466, 0.0,
            {
				use_full_connector_position=true,
            },
            {
				{ CLSID = "{9BFD8C90-F7AE-4e90-833B-BFD0CED0E536}",												Cx_gain = 0.0302},			-- AIM_9P				-- rev.177067
            },
		),
		-- right center line
		pylon(5, 1, 2.389, -0.695, 0.956,
		    {
            },
            {
				{ CLSID = "{9BFD8C90-F7AE-4e90-833B-BFD0CED0E536}",												Cx_gain = 0.0302},			-- AIM_9P				-- rev.177067
            },
        ),
        pylon(6, 0,0,0,0,---0,  -1.412, -0.593, 1.786,
            {
				use_full_connector_position=true,
            },
            {
				{ CLSID = "{9BFD8C90-F7AE-4e90-833B-BFD0CED0E536}",												Cx_gain = 0.0302},			-- AIM_9P				-- rev.177067
            },
        ),
		pylon(7,   0,0,0,0,---0, -1.849, -0.593, 3.638,
            {
				use_full_connector_position=true,
            },
            {
				{ CLSID = "{9BFD8C90-F7AE-4e90-833B-BFD0CED0E536}",												Cx_gain = 0.0302},			-- AIM_9P				-- rev.177067
			},
        ),--]]
	},


	Tasks = {
        aircraft_task(CAP),
        aircraft_task(Intercept),
        aircraft_task(Escort),
        aircraft_task(FighterSweep),
        aircraft_task(AFAC),
        aircraft_task(GroundAttack),
        aircraft_task(RunwayAttack),
		aircraft_task(AntishipStrike),
		aircraft_task(CAS),

    },

	DefaultTask = aircraft_task(CAP),

	SFM_Data = {
		aerodynamics =
		{
		---------------------------------------------------------------------------
		----- SFM Basic Data
		---------------------------------------------------------------------------
			Cy0	=	0,			--- This makes more sense, prevents odd pitch up, that flight controller would normally handle
			Mzalfa	=	6.6,	--- 
			Mzalfadt	=	1,	--- pitch agility mine 5.23
			kjx	=	2.85,       --- roll inertia
			kjz	=	0.00125,    --- pitch inertia
			Czbe	=	-0.012, --- 
			cx_gear	=	0.02,   --- 
			cx_flap	=	0.05,   --- 
			cy_flap	=	0.6,  	--- 
			cx_brk	=	0.025, 	--- 
			-------------------- Drag Polar Coefficients:
			-- Cx = Cx_0 + Cy^2*B2 +Cy^4*B4
			-- It appears this really provides us with a good way to tune turn rate performance
			-- and provides us with how much drag we build up for a given amount of lift we make
			-- Drag polars were tuned from the flight manual and testing with correct max sustained
			-- turn rate
			-------------------- Roll rate taken directly from performance charts
			table_data =
			{
				--	M		Cx0			Cya		B		B4		Omxmax	Aldop		Cymax
				{0,	 	0.0100,		0.081,	0.1500,	0.1100,	0.500,	24.0,	1.280},
				{0.05, 	0.0100,		0.079,	0.1500,	0.1000,	1.000,	29.0,	1.140},
				{0.1,	0.0100,		0.074,	0.1500,	0.0900,	2.800,	17.0,	1.070},
				{0.2,	0.0100,		0.069,	0.1500,	0.0750,	3.280,	17.0,	0.980},
				{0.3,	0.0100,		0.067,	0.1442,	0.3897,	3.400,	17.0,	0.950},
				{0.4,	0.0110,		0.064,	0.1383,	0.7044,	4.363,	17.0,	0.950},
				{0.6,	0.0110,		0.065,	0.1265,	1.3339,	4.854,	17.0,	0.950},
				{0.7,	0.0120,		0.065,	0.1206,	1.6486,	4.829,	17.0,	0.930},
				{0.8,	0.0130,		0.066,	0.1148,	1.9633,	4.803,	17.0,	0.925},
				{0.85,	0.0130,		0.067,	0.1118,	2.1206,	4.778,	17.0,	0.920},
				{0.9,	0.0140,		0.071,	0.1089,	2.2780,	4.752,	17.0,	0.915},
				{1,		0.0150,		0.082,	0.1030,	2.5927,	4.726,	17.0,	0.910},
				{1.05,	0.0160,		0.085,	0.1000,	2.7500,	4.713,	17.0,	0.905},
				{1.1,	0.0160,		0.086,	0.1035,	2.7586,	4.701,	17.0,	0.900},
				{1.2,	0.0165,		0.083,	0.1104,	2.7759,	4.675,	17.0,	0.900},
				{1.3,	0.0170,		0.077,	0.1173,	2.7931,	4.650,	16.0,	0.900},
				{1.5,	0.0180,		0.062,	0.1311,	2.8276,	4.598,	13.0,	0.900},
				{1.7,	0.0200,		0.051,  0.1449,	2.8621,	4.547,	12.0,	0.700},
				{1.8,	0.0210,		0.046,	0.1518,	2.8793,	4.522,	11.4,	0.640},
				{2,		0.0230,		0.039,	0.1656,	2.9138,	4.470,	10.2,	0.520},
				{2.1,	0.0240,		0.034,	0.1725,	2.9310,	4.445,	 9.0,	0.400},
				{2.2,	0.0245,		0.034,	0.1794,	2.9483,	4.419,	 9.0,	0.400},
				{2.5,	0.0300,		0.033,	0.2000,	3.0000,	3.500,	 9.0,	0.400},
			}, -- end of table_data
			-- M - Mach number
			-- Cx0 - Coefficient, drag, profile, of the airplane
			-- Cya - Normal force coefficient of the wing and body of the aircraft in the normal direction to that of flight. Inversely proportional to the available G-loading at any Mach value. (lower the Cya value, higher G available) per 1 degree AOA
			-- B - Polar quad coeff
			-- B4 - Polar 4th power coeff
			-- Omxmax - roll rate, rad/s
			-- Aldop - Alfadop Max AOA at current M - departure threshold
			-- Cymax - Coefficient, lift, maximum possible (ignores other calculations if current Cy > Cymax)

		}, -- end of aerodynamics
		engine =
		{
			Nmg	    =	51.0,
			MinRUD	=	0,
			MaxRUD	=	1,
			MaksRUD	=	0.85,
			ForsRUD	=	0.91,
			typeng	=	1,
			hMaxEng	=	19.0,
			dcx_eng	=	0.0144, -- Affects drag of engine when shutdown
			-- cemax/cefor affect sponginess of elevator/inertia at slow speed
			-- affects available g load apparently
			cemax	=	1.24,
			cefor	=	2.56, 
			-- following affect thrust/max alt
			--
			dpdh_m	=	4100, --  altitude coefficient for max thrust
			dpdh_f	=	11800, --  altitude coefficient for AB thrust  -- affects thrust?? Apparently makes no diff. between 1000 and 20000
			table_data =
			{
				{0,		64300,		95000},
				{0.2,	67000,		96000},
				{0.4,	67000,		96000},
				{0.6,	67000,		102000},
				{0.7,	67000,		111000},
				{0.8,	75000,		126000},
				{0.9,	75000,		145000},
				{1,		75000,		164000},
				{1.1,	75000,		177000},
				{1.2,	80000,		187000},
				{1.3,	82000,		194000},
				{1.5,	76000,		213000},
				{1.8,	69246,		229000},
				{2,		71719,		233000},
				{2.2,	70483,		240000},
				{2.5,	68009,		237000},
				{3,		61827,		117436},
			}, -- end of table_data
			-- M - Mach number
			-- Pmax - Engine thrust at military power - kilo Newtons
			-- Pfor - Engine thrust at AFB
		}, -- end of engine
	},
	
	Failures = {-- TODO: update failures
			{ id = 'asc', 		label = _('ASC'), 		enable = false, hh = 0, mm = 0, mmint = 1, prob = 100 },
			{ id = 'autopilot', label = _('AUTOPILOT'), enable = false, hh = 0, mm = 0, mmint = 1, prob = 100 },
			{ id = 'hydro',  	label = _('HYDRO'), 	enable = false, hh = 0, mm = 0, mmint = 1, prob = 100 },
			{ id = 'l_engine',  label = _('L-ENGINE'), 	enable = false, hh = 0, mm = 0, mmint = 1, prob = 100 },
			{ id = 'r_engine',  label = _('R-ENGINE'), 	enable = false, hh = 0, mm = 0, mmint = 1, prob = 100 },
			{ id = 'radar',  	label = _('RADAR'), 	enable = false, hh = 0, mm = 0, mmint = 1, prob = 100 },
		    { id = 'eos',  		label = _('EOS'), 		enable = false, hh = 0, mm = 0, mmint = 1, prob = 100 },
			{ id = 'helmet',  	label = _('HELMET'), 	enable = false, hh = 0, mm = 0, mmint = 1, prob = 100 },
			{ id = 'mlws',  	label = _('MLWS'), 		enable = false, hh = 0, mm = 0, mmint = 1, prob = 100 },
			{ id = 'rws',  		label = _('RWS'), 		enable = false, hh = 0, mm = 0, mmint = 1, prob = 100 },
			{ id = 'ecm',   	label = _('ECM'), 		enable = false, hh = 0, mm = 0, mmint = 1, prob = 100 },
			{ id = 'hud',  		label = _('HUD'), 		enable = false, hh = 0, mm = 0, mmint = 1, prob = 100 },
			{ id = 'mfd',  		label = _('MFD'), 		enable = false, hh = 0, mm = 0, mmint = 1, prob = 100 },
	},


	Damage = {-- TODO: update damage arguments
		-- NOSE, COCKPIT & AVIONICS
		[0]	 = {critical_damage =  3, args = {82}},		-- NOSE_CENTER
		[1]	 = {critical_damage =  8, args = {150}},	-- NOSE_LEFT_SIDE
		[2]	 = {critical_damage =  8, args = {149}},	-- NOSE_RIGHT_SIDE
		[3]	 = {critical_damage =  2, args = {65}},		-- COCKPIT
		[4]	 = {critical_damage =  8, args = {298}},	-- CABIN_LEFT_SIDE
		[5]	 = {critical_damage =  8, args = {299}},	-- CABIN_RIGHT_SIDE
		[90] = {critical_damage =  5},					-- PILOT
		[86] = {critical_damage =  8, args = {300}},	-- AVIONICS BAY 1 (PYLON1)
		[87] = {critical_damage =  8, args = {301}},	-- AVIONICS BAY 2 (PYLON2)
		[88] = {critical_damage =  8, args = {302}},	-- JAMMER (PYLON3)

		-- CONTROL SURFACES
		[53] = {critical_damage =  5, args = {248}},	-- RUDDER
		[25] = {critical_damage =  5, args = {226}},	-- AILERON L
		[51] = {critical_damage =  5, args = {240}},	-- ELEVATOR L IN
		[52] = {critical_damage =  5, args = {238}},	-- ELEVATOR R IN
		[26] = {critical_damage =  5, args = {216}},	-- AILERON R
		[21] = {critical_damage =  5, args = {232}},	-- SLAT L OUT
		[33] = {critical_damage =  5, args = {230}},	-- SLAT L IN
		[22] = {critical_damage =  5, args = {222}},	-- SLAT R OUT
		[34] = {critical_damage =  5, args = {220}},	-- SLAT R IN
		[19] = {critical_damage =  5, args = {183}},	-- AIRBRAKE LEFT
		[20] = {critical_damage =  5, args = {185}},	-- AIRBRAKE RIGHT

		-- ENGINE & FUEL TANKS
		[11] = {critical_damage = 10, args = {271}},	-- ENGINE
		[61] = {critical_damage = 10, args = {224}},	-- FUEL_TANK_LEFT_SIDE
		[62] = {critical_damage = 10, args = {214}},	-- FUEL_TANK_RIGHT_SIDE
		[65] = {critical_damage = 10, args = {155}},	-- FUEL_TANK_CENTER (BLADE_1_CENTER)
--		[66] = {critical_damage = 3               },	-- BATTERY (BLADE_1_OUT)
--		[67] = {critical_damage = 3               },	-- ALTERNATOR 1 (BLADE_2_IN)
--		[68] = {critical_damage = 3               },	-- ALTERNATOR 2 (BLADE_2_CENTER)
--		[69] = {critical_damage = 3               },	-- CONVERTER 1 (BLADE_2_OUT)
--		[70] = {critical_damage = 3               },	-- CONVERTER 2 (BLADE_3_IN)
--		[71] = {critical_damage = 3               },	-- HYD PUMP 1 (BLADE_3_CENTER)
--		[72] = {critical_damage = 3               },	-- HYD PUMP 2 (BLADE_3_OUT)

		-- FUSELAGE & WINGS
		[39] = {critical_damage = 10, args = {244}},
		[41] = {critical_damage = 10, args = {245}, deps_cells = {39,53}},
		[43] = {critical_damage = 10, args = {246}, deps_cells = {41,88}},
		[23] = {critical_damage = 8,  args = {223}, deps_cells = {21}},
		[29] = {critical_damage = 9,  args = {224}, deps_cells = {19,23,84}},
		[35] = {critical_damage = 10, args = {225}, deps_cells = {23,21,29,33,61,84}},
		[36] = {critical_damage = 10, args = {215}, deps_cells = {24,22,30,34,62,85}},
		[30] = {critical_damage = 9,  args = {214}, deps_cells = {20,24,85}},
		[24] = {critical_damage = 8,  args = {213}, deps_cells = {22}},
		[9]	 = {critical_damage = 10, args = {154}},
		[82] = {critical_damage = 10, args = {152}},
		[10] = {critical_damage = 10, args = {153}},
		[55] = {critical_damage = 10, args = {159}},
		[56] = {critical_damage = 10, args = {158}},
		[57] = {critical_damage = 10, args = {157}},
		[58] = {critical_damage = 10, args = {156}}, 			-- TAIL BOTTOM

		-- LANDING GEAR
		[8]  = {critical_damage = 8, args = {265}, deps_cells = {83}},	-- FRONT_GEAR_BOX
		[15] = {critical_damage = 8, args = {267}, deps_cells = {84}},	-- LEFT_GEAR_BOX
		[16] = {critical_damage = 8, args = {266}, deps_cells = {85}},	-- RIGHT_GEAR_BOX
		[83] = {critical_damage = 3, args = {134}},						-- WHEEL F
		[84] = {critical_damage = 3, args = {135}},						-- WHEEL L
		[85] = {critical_damage = 3, args = {136}},						-- WHEEL R

		-- WEAPONS
		[7]  = {critical_damage = 5, args = {296}},						-- GUNS

	},

	--oblomok = russian for splinter
	--must create models for damaged parts
	DamageParts =
	{

	-------------UNDER CONSTRUCTION---------------------

   		[1] = "M2000X-part-wing-R", -- wing R
   	    [2] = "M2000X-part-wing-L", -- wing L
   		[3] = "M2000X-part-nose", -- nose
   		[4] = "M2000X-part-tail", -- tail
	},

	---------------------------------------------------------
	lights_data = {
		typename = "collection",
		lights = {
			[2] = { typename = "collection", -- form lights aft
				lights = {
					--{typename = "spotlight",argument = 208,dir_correction = {elevation = math.rad(3)}}, -- form front
					{typename = "spotlight",argument = 209,connector = "NOSE_TAXI_SPOT", dir_correction = {elevation = math.rad(0)}}, -- form aft
					{typename = "natostrobelight",connector = "TOP_BEACON",argument_1 = 83,period = 2.0,color = {1.0, 0.0, 0.0},phase_shift = 0.0},
					{typename = "natostrobelight",connector = "BOTTOM_BEACON",argument_1 = 802,period = 2.0,color = {1.0, 0.0, 0.0},phase_shift = 0.0},
				},
			},
			[3] = { typename = "collection", -- left nav light
				lights = {
					{typename = "omnilight",argument = 190}, -- left nav light red
					{typename = "omnilight",argument = 191}, -- tail nav light white
					{typename = "omnilight",argument = 192}, -- right nav light green
				},
			},
			[4] = { typename = "collection", -- tail nav light
				lights = {
					{typename = "argumentlight",argument = 200}, -- FORMATION LIGHTS
					{typename = "argumentlight",argument = 201}, -- FORMATION LIGHTS
				},
			},
		},
    }, -- end lights_data

}

add_aircraft(M2000X)
