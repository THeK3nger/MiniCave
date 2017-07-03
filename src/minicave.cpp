#include <iostream>

/*!
* A Minimized Compressed Cellular Cave Generator.
*
* @author   Davide Aversa <thek3nger@gmail.com>
* @version  1.0
*/

/*!
 * Implementation of a Cellular Automata cave-like map generator.
 */
template<int W, int H>
struct minicave {

    int map[H * W];  // Store the map.
    int w, h;      // The map width and height.

    /*!
     * Constructor.
     */
    minicave() : w(W), h(H) {
        /*
        * Initialize the map with a random amount of walls.
        */
        for (int r = 0; r < H; r++) {
            for (int c = 0; c < W; c++) {
                map[k(c, r)] = (c == 0 || r == 0 || (c == W - 1) || (r == H - 1))
                               ? // If coordinates lie on the the edge of the map (creates a border).
                               1 : // Wall
                               (r == (H / 2))
                               ? //  Else, fill with a wall a random percent of the time add an empty line in the center of the map. This leads to better maps.
                               0 :
                               (rand() % 10 >= 4 ? 0 : 1); // ELSE
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
                map[k(column, row)] = placeWallLogic(column, row, clean);
            }
        }
    }

    /*
    * Execute the Cellular Automata rules on every tile of the map.
    *
    * @return The new value of tile <x,y>.
    */
    int placeWallLogic(int x, int y, bool clean) {
        int numWalls = getAdjacentWalls(x, y, 1, 1);
        int numWalls2 = getAdjacentWalls(x, y, 2, 2);

        return map[k(x, y)] == 1 ? // If
               // ( Then
               (numWalls >= 3) ? 1 : 0
               // ) Else
                                 : clean ?
                                   numWalls >= 5 ? 1 : 0
                                         : numWalls >= 5 || numWalls2 <= 2;

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

    /*
    * Get the sum of adjacent walls to a given tile <x,y>
    *
    * @param x          The x coordinate in the map.
    * @param y          The y coordinate in the map.
    * @param scope_x    The search scope radius in x direction.
    * @param scope_y    The search scope radius in y direction.
    * @return           The number of walls in the scope of <x,y>.
    */
    int getAdjacentWalls(int x, int y, int scope_x, int scope_y) {
        int startX = x - scope_x;
        int startY = y - scope_y;
        int endX = x + scope_x;
        int endY = y + scope_y;

        int wallCounter = 0;

        for (int iY = startY; iY <= endY; iY++) {
            for (int iX = startX; iX <= endX; iX++) {
                if (!(iX == x && iY == y)) {
                    if (isWall(iX, iY)) {
                        wallCounter += 1;
                    }
                }
            }
        }
        return wallCounter;
    }

    /*
    * Check if a tile is out of the map.
    *
    * @param x  The x coordinate in the map.
    * @param y  The y coordinate in the map.
    * @return   True iff <x,y> is out of the map. False otherwise.
    */
    bool isOutOfBound(int x, int y) { return x < 0 || y < 0 || x > W - 1 || y > H - 1; }

    /*
    * Get the rom-major index of the tile <x,y>
    *
    * @param x  The x coordinate in the map.
    * @param y  The y coordinate in the map.
    * @return   The row-major index of <x,y>.
    */
    int k(int x, int y) { return x + y * W; }

    /*
    * Chek if the tile is a Wall.
    *
    * @param x  The x coordinate in the map.
    * @param y  The y coordinate in the map.
    * @return   True iff <x,y> is a wall. False otherwise.
    */
    bool isWall(int x, int y) { return (isOutOfBound(x, y)) || (map[k(x, y)] == 1); }

};

using namespace std;

template<int H, int W>
string mapToString(minicave<H, W> &cm) {
    string r;
    for (int row = 0; row < cm.h; row++) {
        for (int column = 0; column < cm.w; column++) {
            r += cm.map[cm.k(column, row)] == 1 ? "#" : ".";
        }
        r += "\n";
    }
    return r;
}

int main() {
    srand(getpid()); // Not the best, but I can remove include for time and save one line.
    // This is an use example for the minicave class.
    minicave<80, 80> cm;
    for (int i = 0; i < 4; ++i) { cm.evolveMap(false); }
    cm.evolveMap(true);
    cout << mapToString(cm) << endl;
    return 0;
}