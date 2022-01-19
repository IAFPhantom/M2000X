## Generic Engine Model
The generic turbojet engine is a state machine that uses states to control the operation of the pilot interations with the engine in different states, RPM, core temperatures and thrust generated. The engine requires input from the following sub systems: Electrical for power and Fuel, and in turn creates output to the Hydraulic, Phenumatic, Electric sub systems and Aerodynamics in the form of Thrust. The Engine igniters draw power from the electrical system on the AC and DC busses and has access to the fuel tank. 
In addition, the engine connects mechanical gears powering hydraulic and phenumatic compressors.
![This is an image](https://github.com/KfirMod/IAF_KFIR/blob/master/assests/iai-kfir_engine_diagram.jpg)

**Engine state Machine**

![This is an image](https://github.com/KfirMod/IAF_KFIR/blob/master/assests/iai-kfir_engine.jpg)

**State OFF** - The engine is off and not generating thrust. RPM and temperature will decrease slowly, depending on external factors such as air speed and external temperature.
The engine is transitioned to the starting stage by pressing the Starter button.

**State Starting** - The engine is in the process of starting, if successful it will transition automatically to the **Running** state.
The **Strarting** state uses the following logic:
1. The engine is receiving electrical power DC/AC from at one bus for the engine ignitors. 
2. external air pressure (from ground equipment or air) turns the compressor to between RPM 5% - 25% , external air push RPM (check?)
3. Throttle from OFF to IDLE (by moving the throttle forward pass the 0.2 mark and bring it back to 0) - for air-start (?), moving the throttle to IDLE causes the engine to consume fuel the starting process causing the RPM (RPM starting spool time) and core temperature (need chart) to increase.
4. the engine will "catch" by stay in the **Starting** state until the RPM covonor stabalizes the RPM to the 65% RPM_IDLE value (5000)

**Failures**
1. if no electric power, there is no ignition flow is injected but not burnt , no temperature rise or RPM engine "catching".
2. The engine will not catch if there is no fuel will "ventilate" the engine cooling it
3. engine can damage the combustion chambers if the temerature raises above 765C
4. engine will not start if RPM < 5%  and will not catch if RPM < 20%
?Thrust my be generated at RPM above 4000 ?

**State Running** - In this state the engine will react to the pilot throttle inputs, moving the throttle from min to max settings. min setting is equivelent to IDLE settings and max settings is full afterburnner. any position above 80% throttle is considered AB ignition and up to 90% partial AB above 90%-100% is FULL AB. AB ignition may take a random time between 0.5 and 2 sec.
The engine covonor receives the pilots throttle value and will adjust the engine RPM by spooling the RPM up or down accordingly. The covonor is not accurate and will spool in different speeds depending on the difference between the current RPM and requested throttle setting. It will spool up faster than spooling down (check?). The covonor will not hold the RPM stable there may be a 5% error in the requested throttle setting and the covonor will cycle the RPM up until passing a threshold and then spool down +-5% of the rquested throttle setting.
Thrust is generated see attached tables, depending on the RPM, Mach, external Temperature, (AoA - check?) , AB.
Fuel flow and temperatures are updated continously 
Temperature rises fast during starting but will level off slowly in the running state because of the flow of air in direct relation to RPM - high RPM more cooling low RPM less cooling. Cooling is also directly related to speed, high speed - more cooling, low speed less cooling. Temperature is also directly related to RPM state and AB State. (need to create graphs)
Intake cones "mouses" must move (time?) to limit the flow of air into the intake, they start at the fully open position and start closing at 0.8 up to 1.2 Mach (check?)

**Failures**
1. AB ignition - may fail and not ignite the AB- engine stays in the **Running** state, igniting of the AB is done continously until success
2. Exceeding Speed limits (see graph) can cause the following ,
   3. above a limit of alt and min speed AB ignition will go out automatically- engine stays in the **Running** state
   4. if above the recommended speed at alt the compressor may stall and flame out randomly 25% above the limit and change the state to **Fail**
   5. Intake cones "mouses" are not in correct location for optimal flow and plane is above 0.8 Mach a compressor stall may occour and change the state to **Fail**
   6. cones moving under high G may cause them lock in place (check?) - engine stays in the **Running** state
   7. Temperature limits - passing the temp. limits by 25% can cause damage to the combustion chambers / AB ignition tourch RPM covonor (?) - engine stays in the **Running** state



**State Fail** - The engine is in a compressor stall and flame out
If the engine is in a compressor stall the RPM will start to roll back (spool time?) and will stop in relation to the external air flow from the planes velocity. 
Thrust will become negative - reflecting the rsistance of the air to push the engine blades to generate RPM
Engine will not react to the throttle inputs to change the RPM
A pilot may recover from the compressor stall and flame out by throttling to IDLE min state and pressing the starter button (check?)
if the plane is in the recommended speed-alt (see graph) and no premanent damage the engine the engine will  move to the **starting** state


