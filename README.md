# isometric
Wannabe isomorphic RTS

Project scope:
- Create a custom game engine based upon an isomorphic projection. 
- Using this engine make a RTS game which mimics the controls of games like age of Empires. 
- Pathfinding using A* or biderectional A*
- Make villagers gather different resources
- Show different animations for these resources
- Use these resources to build different buildings
- Require specific buildings to house population
- Make the buildings produce units
- Make the units fight each other
- Implant a basic Rock/Paper/Scissors for damage calculations during battle
- Researchable upgrades for units
- Basic AI to play against

===============================================

Current functionality
-	The world gets drawn
-	You can place units by pressing the left mouse button while holding A
-	You can place objects by pressing the left mouse button while holding O
-	The right mouse button while holding O will cycle objects
-	You can place buildings by pressing the left mouse button while holding B
-	The right mouse button while holding B will cycle buildings
-	You can select units by left clicking on them 
-	You can select multiple units by boxdragging while holding down the left mouse button
-	You can order the selected unit to walk somewhere by right clicking
-	The unit will now use A* to calculate a route and walk along this route
-	When the unit encounters a new obstacle it will calculate a new route
-	When you order a unit to go to a collactable object it wil start gathering it
-	When it is at carrying capacity it will search the nearest reachable drop-off point
-	Once the recources have been delivered it will walk back and collect more
-	If the source is depleted it will search for the nearest reachable source of the same type
