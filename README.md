# Algorithm Design Project
## Group-Series: 321CC
## Participants:
* Iudenco Ianos
* Iurciuc Denis
* Preda Victor-Andrei

## Project Implementation Description:

### To implement the functionality of this bot, we designed a search algorithm to find the best tile for the current tile as follows:

* For each move, we create a vector of length (width * height), which we populate with tiles different from the current tile (checking that the current tile does not have a strength of 0, in which case the vector will be empty) using the formula (production / strength * 100), thus representing a "score."

* We then perform another traversal of the map to find the best block in this generated vector. The best block is represented by the highest score in the vector at the current step, divided by (1.5 ^ distance (between the current map block and the block in the score vector)).

* After this iteration, we find the block with the best score and compare its strength with that of the current block (if the current block has a lower strength, it will not be moved).
  
* Additionally, we generate an expansion coefficient from the map, and if this coefficient is lower than the strength of the current block, or if the best-found block has a strength of 0 and the distance between the current tile and it is 1, we proceed with the move toward this initially searched block based on the angle-finding function (provided in the skeleton) and update the score vector accordingly.

## Testing:

* In order to test it you can use the command: "python ./run.py --cmd "./MyBot" --round 2"

* In order to check the result go in "visualizer" and check ".htm" files. It will display what happened during the game. Also, there is a file there to grant you that the bot is working: "30x30-2-3026956134.htm".
