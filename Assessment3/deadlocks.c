/*3.
* Write a C program in Linux that demonstrates preventing deadlocks
* using semaphores. A single-lane bridge connects two towns. Farmers in the two villages
* use this bridge to deliver their produce to the neighboring town. The bridge can become
* deadlocked if a northbound and a southbound farmer get on the bridge at the same time.
* Using semaphores, design an algorithm in pseudocode that prevents deadlock. Represent
* northbound and southbound farmers as separate threads. Once a farmer is on the bridge,
* the associated thread will sleep for a random period of time, representing traveling across
* the bridge.
*/