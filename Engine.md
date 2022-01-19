Generic Plane High Level Design


The generic plane is built from multi components sharing interfaces and flows.
The components follow the same concept and the ED DCS API flow: 
Step 0 - build systems and initialize to cold, hot or in air.
step 1 - Data collection: collect plane, atmosphere, weight,wind, damage and pilot action commands.
Step 2 - simulate: each system component is activated in turn starting with faster to slower, electric - fluids - mechanic - Aerodynamics.
Step 3 - Data update: propagate the updates forces, moments, weights and MOI deltas back to DCS. Updates the external and cockpit related animations and parameters for pilot and sim integration with Lua code.


Generic plane Description
The generic plane class is responsible for all the interfacing and interactions between DCS and the plane, building and loading configuration data of the gereneric plane components and initializing them to the selected type of start of cold, hot or in-air. 
The Specific plane inheriates this functionality and overrides the and creates the plane specific classes or parameteres over the generic plane and systems.
for example: the generic plane has electric and hydraulic components and when created creates a simplistic implentation. A specific plane can use this implenentation, modify parameters or create a completly new one.


Generic plane components Description
The generic plane contains a collection of elements to support a common plane components through common interfaces. 
Each element can be changed by the user has a unique ID synchronized with the Lua code to support receiving updates from DCS. Same with elements that require updating state to the such as animations, arguments or parameters. These elements are connected as a described in the specific plane schematic diagram description. The elements are created and linked during step 0 and receives interactions and updates status during the simulate stage and prepares a list of values changed in the current step for updating the UI in step 3. 
Elements can be part of more than one type, such as and electric compressor engine that is connected as part of the electric diagram and a liquid system. Since the electric is a faster system than liquid it is simulated before the liquid and hence is updated in time for the compressor to change its state as accordingly when the liquid system is simulated.
The Generic plane is a simulation of simulations of each component built from the following components and interactions:
1. Electric System: this component supplies the plane with a electric DC AC power and associated sub systems. DC battery, switchs, lights, relays and busess. It also includes higher components such as alternators, transrecivers. Relays are switch components that when the receive power they in turn open or close switchs, since the reaction time is shorter by orders of magnitude then the frame update time, this requires an immediate update of the switchs in the electric system. For this reason the elec. system simulate may activated a number of times if a relay changed state or if the elecitric system caused internal damage to itself such as shorting or overloading (* in implenetation need to exit if loop of after 10 continues calls). 
Another special case are electric controlers - these contain a logical state machine to support complex functionalies such as electric busses and compoents that support controls.
 
2. Liquid systems: Hydraulic / Phenumatic / Oxygen / Pilot Air Cooling / Fuel 
    follows the same concepts as in the elec.
    The following cross systems interactions are 
     - elec. pressure gauges will only supply data if electricy is flowing through gauge.
     - actuators project force on machanical system  
3. Mechanical systems: push rods, cables, gears, rotation bars, engines
    follows the same concepts as in the elec.
    gears, rods feel force projected in specific direction and create movement accordingly.
    elec engine compressor - will function as following, the engine is linked as part of the electric system as a electric consumer (e.g resistor) depending on the internal state of the engine different amount of current can be drawn from the system and if it exceeds its operational conditions (temperature) it can fail. When the Mechanical systems are activated the compressor checks the internal rpm state and creates updates the pressure accordingly in the pipe of the liuid systems.
    
4. Aerodynamics: lifting surfaces (wings/elevators/canards/flaps) / drag / weight / thrust / control surfaces
The system calculates the forces and moments and updated 


Generic liquid - template class to support any type of liquid flow and associated elements.
This class is used to create all of the following plane systems, 


Liquid properties
Calculate pressure
Generic tank
* Receives liquid amount
* Get liquid
* Damage , no damages, small leak, big leak


Actuator
* State 
* Pressure
* Damage
* Oxygen system


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


