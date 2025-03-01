#include <cstdlib>
#include <ctime>
#include <set>
#include <iostream>
#include "hlt.hpp"
#include "networking.hpp"
#include <climits>
#include <vector>

using namespace std;

struct Tile
{
    hlt::Site site;
    hlt::Location location;
    double score;
} typedef Tile;

int main()
{
    srand(time(NULL));
    std::cout.sync_with_stdio(0);

    unsigned char myID;
    hlt::GameMap presentMap;
    getInit(myID, presentMap);
    sendInit("P.O.H.U.I.");

    std::set<hlt::Move> moves;

    // Coefficient to determine the minimum strength of tile to start the movement
    double minStrengthToMove = 40 - (max(presentMap.height, presentMap.width) / 2);
    while (true)
    {
        moves.clear();
        getFrame(presentMap);

        vector<Tile>
            foreignTiles(presentMap.width * presentMap.height);
        int foreignTilesLength = 0;

        // Iterate through all tiles
        for (unsigned short y = 0; y < presentMap.height; y++)
        {
            for (unsigned short x = 0; x < presentMap.width; x++)
            {
                auto site = presentMap.getSite({x, y});
                // If the tile is not mine
                if (site.owner != myID)
                {
                    // If its strength is not 0, add it to the vector of foreign tiles
                    if (site.strength != 0)
                    {
                        Tile tile;
                        tile.location = {x, y};
                        // Calculate the score of the tile = production / strength and normalize it by 100
                        tile.score = (double)(site.production) / (site.strength) * 100;
                        tile.site = site;
                        foreignTiles[foreignTilesLength] = tile;
                        foreignTilesLength++;
                    }
                }
            }
        }

        // Iterate again through all tiles
        for (unsigned short y = 0; y < presentMap.height; y++)
        {
            for (unsigned short x = 0; x < presentMap.width; x++)
            {
                auto site = presentMap.getSite({x, y});
                // If the tile is mine, calculate best tile to move to
                if (site.owner == myID)
                {
                    Tile bestTile;
                    bestTile.score = -1;
                    int bestTileIndex = -1;
                    double bestScore = INT_MIN;
                    double bestDistance = INT_MAX;
                    // Iterate through all foreign tiles
                    for (int i = 0; i < foreignTilesLength; i++)
                    {
                        // If the tile is already taken, skip it
                        if (foreignTiles[i].score == -1)
                            continue;

                        double score = 0;
                        // Calculate distance to this tile
                        double distance = presentMap.getDistance({x, y}, foreignTiles[i].location);
                        // Calculate its score based on the distance
                        score = foreignTiles[i].score / pow(1.5, distance);
                        // If the score is better than the best score, update the best score
                        if (score > bestScore)
                        {
                            bestScore = score;
                            bestTile = foreignTiles[i];
                            bestTileIndex = i;
                            bestDistance = distance;
                        }
                    }
                    // If my site strength is greater than the tile we want to move to
                    if (site.strength > bestTile.site.strength)
                    {
                        //  If my site strength is greater than the minimum strength
                        // Or the foreign tile strength is 0, or it is very close to us
                        if ((site.strength > minStrengthToMove || (bestTile.site.strength == 0 && bestDistance == 1)))
                        {
                            // Calculate the direction to move to
                            unsigned char direction;
                            float angle = presentMap.getAngle({x, y}, bestTile.location);
                            if (angle > -M_PI / 4 && angle < M_PI / 4)
                                direction = EAST;
                            else if (angle >= M_PI / 4 && angle < 3 * M_PI / 4)
                                direction = SOUTH;
                            else if (angle >= 3 * M_PI / 4 || angle < -3 * M_PI / 4)
                                direction = WEST;
                            else
                                direction = NORTH;
                            // Add the move to the set of moves
                            moves.insert({{x, y}, direction});
                            // Update the strength of the tile we are moving to
                            bestTile.site.strength -= site.strength;
                            // If the tile strength is less than 0, mark it as taken
                            if (bestTile.site.strength < 0)
                                foreignTiles[bestTileIndex].score = -1;
                            continue;
                        }
                    }
                    // If we are not moving, stay still
                    moves.insert({{x, y}, STILL});
                }
            }
        }
        sendFrame(moves);
    }
    return 0;
}
