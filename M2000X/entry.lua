declare_plugin("M2000X",
{
displayName     = _("M2000C_X"),
developerName   =   "IAF_Phantom",
-- ..\DCSWorld\Scripts\Database\planes\F-4E.lua original plane def

image     	 = "FC.bmp",
installed 	 = true, -- if false that will be place holder , or advertising
dirName	  	 = current_mod_path,
fileMenuName = _("M2000X"),

version		 = __DCS_VERSION__,
state		 = "installed",
info		 = _(""),

--InputProfiles =
--{
--    ["M2000X"] = current_mod_path .. '/Input/M2000X',
--},


Skins	=
	{
		{
			name	= _("M2000X"),
			dir		= "Theme"
		},
	},
	
Missions =
	{
		{
			name		    = _("M2000X"),
			dir			    = "Missions",
		},
	},		

LogBook =
	{
		{
			name		= _("M2000X"),
			type		= "M2000X",
		},
	},	
binaries 	 =
	{
		'M2000X',
	},
})

mount_vfs_texture_path(current_mod_path ..  "/Theme/ME")--for simulator loading window
mount_vfs_texture_path  (current_mod_path ..  "/Textures/")
mount_vfs_model_path    ("Bazar/World/Shapes")
mount_vfs_model_path (current_mod_path ..  "/Shapes")
mount_vfs_liveries_path (current_mod_path ..  "/Liveries")

dofile(current_mod_path.."/Views.lua")
make_view_settings('M2000X', ViewSettings, SnapViews)

dofile(current_mod_path.."/M2000X.lua")


local cfg_path = current_mod_path ..  "/FM/"
local FM = 
{
	[1] = "M2000X",-- id
	[2] = 'M2000X',  -- dll
	config_path 		= cfg_path,
	-- center of mass position relative to object 3d model center for empty aircraft 
	center_of_mass = {0, 0.0, 0.0}, -- 8.56 
	-- moment of inertia of empty (Ixx,Iyy,Izz,Ixy)	in kgm^2  Ixx - forward Iyy - up Izz - right 
	moment_of_inertia = {6262, 75686, 78802, 2141 }, 
	suspension =  
	{
		{ -- NOSE WHEEL
			damage_element = 0,

			self_attitude     		 = true,
			damper_coeff			= 100.0,
			yaw_limit				= math.rad(29.0),
			amortizer_max_length     = 0.1279,
			amortizer_basic_length   = 0.1279,
			amortizer_spring_force_factor   = 3000000, -- force = spring_force_factor * pow(reduce_length, amortizer_spring_force_factor_rate)
			amortizer_spring_force_factor_rate   = 2,
			amortizer_static_force     			 = 7000 * 9.81 * 0.13 * 1.0,
			amortizer_reduce_length     		 = 0.12,
			amortizer_direct_damper_force_factor = 20000,
			amortizer_back_damper_force_factor   = 40000,
			
			wheel_moment_of_inertia			= 0.6,
			wheel_radius					= 0.2794,
			wheel_static_friction_factor	= 0.75,
			wheel_side_friction_factor		= 0.85,
			wheel_roll_friction_factor		= 0.04,
			wheel_glide_friction_factor		= 0.65,

			arg_post     = 0,
			arg_amortizer    = 1,
			arg_wheel_rotation = 101,
		},
		{-- LEFT WHEEL
			damage_element = 5,

				amortizer_min_length					= 0.0,
				amortizer_max_length     				= 0.238, 
				amortizer_basic_length   				= 0.238,
				amortizer_spring_force_factor   		= 70000000.0, -- force = spring_force_factor * pow(reduce_length, amortizer_spring_force_factor_rate)
				amortizer_spring_force_factor_rate		= 3,
				amortizer_static_force	   				= 7000 * 9.81 * 0.435 * 1.0,
				amortizer_reduce_length    				= 0.23,
				amortizer_direct_damper_force_factor	= 20000.0,
				amortizer_back_damper_force_factor		= 10000.0,

			wheel_kz_factor					= 0.2,
			wheel_moment_of_inertia			= 2.65,
			wheel_radius					= 0.3302,
			wheel_static_friction_factor	= 0.75,
			wheel_side_friction_factor		= 0.85,
			wheel_roll_friction_factor		= 0.04,
			wheel_glide_friction_factor		= 0.65,
			--wheel_damage_force_factor		= 250.0,
			--wheel_damage_speed				= 90.0,
			wheel_brake_moment_max			= 4517.0,

			anti_skid_installed = false,

			arg_post     = 5,
				arg_amortizer    = 6,
			arg_wheel_rotation = 102,
		},
		{-- RIGHT WHEEL
			damage_element = 3,

			amortizer_min_length					= 0.0,
			amortizer_max_length     				= 0.238, 
			amortizer_basic_length   				= 0.238,
			amortizer_spring_force_factor   		= 70000000.0, -- force = spring_force_factor * pow(reduce_length, amortizer_spring_force_factor_rate)
			amortizer_spring_force_factor_rate		= 3,
			amortizer_static_force	   				= 7000 * 9.81 * 0.435 * 1.0,
			amortizer_reduce_length    				= 0.23,
			amortizer_direct_damper_force_factor	= 20000.0,
			amortizer_back_damper_force_factor		= 10000.0,

			wheel_kz_factor					= 0.2,
			wheel_moment_of_inertia			= 2.65,
			wheel_radius					= 0.3302,
			wheel_static_friction_factor	= 0.75,
			wheel_side_friction_factor		= 0.85,
			wheel_roll_friction_factor		= 0.04,
			wheel_glide_friction_factor		= 0.65,
			--wheel_damage_force_factor		= 250.0,
			--wheel_damage_speed				= 90.0,
			wheel_brake_moment_max			= 4517.0,

			anti_skid_installed = false,

			arg_post     = 3,
				arg_amortizer    = 4,
			arg_wheel_rotation = 102,-- 103
		}, -- gears
		wing_buffet_ampl = 0.4,
		tail_buffet_ampl = 0.4,
		debugLine = "{M}:%1.3f {IAS}:%4.1f {AoA}:%2.1f {ny}:%2.1f {nx}:%1.2f {AoS}:%2.1f {mass}:%2.1f {Vy}:%2.1f  {Pl}:%2.1f {Fy}:%2.1f {Fx}:%2.1f {wx}:%.1f {wy}:%.1f {wz}:%.1f",
		record_enabled = false,
		disable_built_in_oxygen_system	= true,
	}
}

make_flyable('M2000X', nil, FM, current_mod_path..'/comm.lua')

plugin_done()
