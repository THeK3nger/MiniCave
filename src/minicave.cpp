#include <iostream>

/*!
* A Minimized Compressed Cellular Cave Generator.
*
* @author   Davide Aversa <thek3nger@gmail.com>
* @version  1.0
*/

using i = int;

/*!
 * Implementation of a Cellular Automata cave-like map generator.
 */
template<i W, i H>
struct minicave {

    bool map[H * W];  // Store the map.

    /*!
     * Constructor.
     */
    minicave() {
        /*
        * Initialize the map with a random amount of walls.
        */
        for (i r = 0; r < H; r++) {
            for (i c = 0; c < W; c++) {
                // If coordinates lie on the the edge of the map (creates a border).
                // Else, fill with a wall a random percent of the time add an empty line in the center of the map.
                // This leads to better maps.
                k(c, r) = c == 0 || r == 0 || (c == W - 1) || (r == H - 1) || r != H / 2 && rand() % 10 < 4;
            }
        }
    }

    /*!
     * Execute a step of the automata algorithm creating the cave or
     * Smoothing the existing ones using a specific rule set.
     *
     * C = Clean, a more smoothing set of rules.
     * If C true RULE = Corresponding to R1(p) >= 5
     * If C false RULE = Corresponding to R1(p) >= 5 || R2(p) <= 1
     */
    i evolve(bool C) { // Return int to save chars of the void type.
        // By initializing column in the outer loop, its only created ONCE
        for (i r = 0; r <= H - 1; r++) {
            for (i c = 0; c <= W - 1; c++) {
                i w1 = walls(c, r, 1);
                /*
                * Execute the Cellular Automata rules on every tile of the map.
                */
                k(c, r) = k(c, r) ? w1 >= 3
                                                          : C ?
                                                            w1 >= 5
                                                                  : w1 >= 5 || walls(c, r, 2) <= 2;
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
    i walls(i x, i y, i r) {
        i w = 0;

        for (i iY = y - r; iY <= y + r; iY++) {
            for (i iX = x - r; iX <= x + r; iX++) {
                // bool isWall = iX < 0 || iY < 0 || iX > W - 1 || iY > H - 1 || k(iX, iY);
                if ((iX != x || iY != y) && (iX < 0 || iY < 0 || iX > W - 1 || iY > H - 1 || k(iX, iY))) w += 1;
            }
        }
        return w;
    }

    // Accessor to the <x,y> position in the map.
    bool& k(i x, i y) { return map[x + y * W]; }

    auto str() {
        std::string s;
        for (i r = 0; r < H; r++) {
            for (i c = 0; c < W; c++) {
                s += k(c, r) ? "#" : ".";
            }
            s += "\n";
        }
        return s;
    }
};

i main() {
    srand(getpid()); // Not the best, but I can remove include for time and save one line.
    // This is an use example for the minicave class.
    minicave<80, 80> m;
    for (i j = 0; j < 4; ++j) { m.evolve(false); }
    m.evolve(true);
    std::cout << m.str();
}