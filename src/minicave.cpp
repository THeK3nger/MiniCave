#include <iostream>

/*!
* A Minimized Compressed Cellular Cave Generator.
*
* @author   Davide Aversa <thek3nger@gmail.com>
* @version  1.0
*/

using namespace std;

/*!
 * Implementation of a Cellular Automata cave-like map generator.
 */
template<int W, int H>
struct minicave {

    bool map[H * W];  // Store the map.

    /*!
     * Constructor.
     */
    minicave() {
        /*
        * Initialize the map with a random amount of walls.
        */
        for (int r = 0; r < H; r++) {
            for (int c = 0; c < W; c++) {
                // If coordinates lie on the the edge of the map (creates a border).
                // Else, fill with a wall a random percent of the time add an empty line in the center of the map.
                // This leads to better maps.
                map[k(c, r)] = c == 0 || r == 0 || (c == W - 1) || (r == H - 1) || r != H / 2 && rand() % 10 < 4;
            }
        }
    }

    /*!
     * Execute a step of the automata algorithm creating the cave or
     * Smoothing the existing ones using a specific rule set.
     *
     * If clean true RULE = Corresponding to R1(p) >= 5
     * If clean false RULE = Corresponding to R1(p) >= 5 || R2(p) <= 1
     */
    void evolveMap(bool clean) {
        // By initializing column in the outer loop, its only created ONCE
        for (int row = 0; row <= H - 1; row++) {
            for (int column = 0; column <= W - 1; column++) {
                int w1 = getAdjacentWalls(column, row, 1);
                int w2 = getAdjacentWalls(column, row, 2);
                /*
                * Execute the Cellular Automata rules on every tile of the map.
                */
                map[k(column, row)] = map[k(column, row)] ? w1 >= 3
                                                          : clean ?
                                                            w1 >= 5
                                                                  : w1 >= 5 || w2 <= 2;
                /* Original
                if (map[k(x, y)] == 1) {
                    return (numWalls >= 3) ? 1 : 0;
                } else {
                    if (!clean) {
                        if (numWalls >= 5 || numWalls2 <= 2) {
                            return 1;
                        }
                    } else if (numWalls >= 5) {
                        return 1;
                    }
                }
                return 0; */
            }
        }
    }

    /*
    * Get the sum of adjacent walls to a given tile <x,y>
    *
    * @param x          The x coordinate in the map.
    * @param y          The y coordinate in the map.
    * @param scope_x    The search radius in x and y direction.
    * @return           The number of walls in the scope of <x,y>.
    */
    int getAdjacentWalls(int x, int y, int r) {
        int w = 0;

        for (int iY = y - r; iY <= y + r; iY++) {
            for (int iX = x - r; iX <= x + r; iX++) {
                // bool isWall = iX < 0 || iY < 0 || iX > W - 1 || iY > H - 1 || map[k(iX, iY)];
                if ((iX != x || iY != y) && (iX < 0 || iY < 0 || iX > W - 1 || iY > H - 1 || map[k(iX, iY)])) w += 1;
            }
        }
        return w;
    }

    /*
    * Get the rom-major index of the tile <x,y>
    *
    * @param x  The x coordinate in the map.
    * @param y  The y coordinate in the map.
    * @return   The row-major index of <x,y>.
    */
    int k(int x, int y) { return x + y * W; }

    auto toString() {
        string r;
        for (int row = 0; row < H; row++) {
            for (int column = 0; column < W; column++) {
                r += map[k(column, row)] ? "#" : ".";
            }
            r += "\n";
        }
        return r;
    }
};

int main() {
    srand(getpid()); // Not the best, but I can remove include for time and save one line.
    // This is an use example for the minicave class.
    minicave<80, 80> cm;
    for (int i = 0; i < 4; ++i) { cm.evolveMap(false); }
    cm.evolveMap(true);
    cout << cm.toString() << endl;
    return 0;
}