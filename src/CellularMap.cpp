#include <iostream>
#include<time.h>

/*!
* A Minimized Compressed Cellular Cave Generator.
*
* @author   Davide Aversa <thek3nger@gmail.com>
* @version  1.0
*/

/*!
 * Implementation of a Cellular Automata cave-like map generator.
 *
 * USAGE EXAMPLE:
 *      CellularMap cm(50,40,40);
 *      cm.evolveMap();
 */
class CellularMap
{
public:

    /*!
     * Specifies the rule set used by the cellular automata.
     */
    enum RuleSet {
        CM_CONSERVATIVE,    //! Corresponding to R1(p) >= 5 || R2(p) <= 1
        CM_SMOOTH           //! Corresponding to R1(p) >= 5
    };

    /*!
     * Constructor.
     *
     * @param width                 Desired map width.
     * @param height                Desired map height.
     * @param walls_probability     Initial percentage of walls.
     */
    CellularMap(int width, int height, int walls_probability)  :
            height(height), width(width), walls_probability(walls_probability)
    {
        map = new int[height*width];
        randomFill();
    }

    ~CellularMap() { delete[] map; }

    /*!
     * Execute a step of the automata algorithm creating the cave or
     * Smoothing the existing ones.
     */
    void evolveMap() { evolveMap(CellularMap::CM_CONSERVATIVE); }

    /*!
     * Execute a step of the automata algorithm creating the cave or
     * Smoothing the existing ones using a specific rule set.
     */
    void evolveMap(RuleSet rule) {
        // By initializing column in the outer loop, its only created ONCE
        for (int row = 0; row <= height - 1; row++)
        {
            for (int column = 0; column <= width - 1; column++)
            {
                map[getIndex(column, row)] = placeWallLogic(column, row, rule);
            }
        }
    }

    /*!
     * Get the element in position <x,y>.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     * @return The element in <x,y>.
     */
    int inline getElement(int x, int y) { return map[getIndex(x, y)]; }

    /*!
     * Get the map width.
     */
    int inline getWidth () { return width; }

    /*!
     * Get the map height.
     */
    int inline getHeight() { return height; }

private:

    int* map;               // Store the map.
    int width;              // The map width.
    int height;             // The map height.
    int walls_probability;  // The initial amount of walls.

    /*
    * Initialize the map with a random amount of walls.
    */
    void randomFill() {
        srand (time(NULL));
        for (int row = 0; row < this->height; row++) {
            for (int column = 0; column < this->width; column++)
            {
                // If coordinates lie on the the edge of the map (creates a border).
                // We want border on a map.
                if (column == 0 || row == 0 || (column == width - 1) || (row == height - 1))
                {
                    map[getIndex(column, row)] = 1;
                }
                    // Else, fill with a wall a random percent of the time
                else
                {
                    int mapMiddle = (height / 2);

                    // Add an empty line in the center of the map. This leads to better maps.
                    // There is no other reason.
                    if (row == mapMiddle)
                    {
                        map[getIndex(column, row)] = 0;
                    }
                    else
                    {
                        map[getIndex(column, row)] = (rand() % 101 >= walls_probability) ? 0 : 1;
                    }
                }
            }
        }
    }

    /*
    * Execute the Cellular Automata rules on every tile of the map.
    *
    * @return The new value of tile <x,y>.
    */
    int  placeWallLogic(int x, int y, RuleSet rule) {
        int numWalls = getAdjacentWalls(x, y, 1, 1);
        int numWalls2 = getAdjacentWalls(x, y, 2, 2);

        if (map[getIndex(x,y)] == 1)
        {
            return (numWalls >= 3) ? 1 : 0;
        }
        else
        {
            if (rule == CellularMap::CM_CONSERVATIVE) {
                if (numWalls >= 5 || numWalls2 <= 2) {
                    return 1;
                }
            } else {
                if (numWalls >= 5) {
                    return 1;
                }
            }
        }
        return 0;
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
    int  getAdjacentWalls(int x, int y, int scope_x, int scope_y) {
        int startX = x - scope_x;
        int startY = y - scope_y;
        int endX = x + scope_x;
        int endY = y + scope_y;

        int wallCounter = 0;

        for (int iY = startY; iY <= endY; iY++) {
            for (int iX = startX; iX <= endX; iX++)
            {
                if (!(iX == x && iY == y))
                {
                    if (isWall(iX,iY))
                    {
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
    bool isOutOfBound(int x, int y) { return (x < 0 || y<0 || x>width - 1 || y > height - 1); }

    /*
    * Get the rom-major index of the tile <x,y>
    *
    * @param x  The x coordinate in the map.
    * @param y  The y coordinate in the map.
    * @return   The row-major index of <x,y>.
    */
    int  inline getIndex(int x, int y) { return x + y*width; }

    /*
    * Chek if the tile is a Wall.
    *
    * @param x  The x coordinate in the map.
    * @param y  The y coordinate in the map.
    * @return   True iff <x,y> is a wall. False otherwise.
    */
    bool isWall(int x, int y) { return (isOutOfBound(x, y)) || (map[getIndex(x, y)] == 1); }

};

using namespace std;

string mapToString(CellularMap& cm) {
    string returnString;
    for (int column = 0, row = 0; row < cm.getHeight(); row++) {
        for (column = 0; column < cm.getWidth(); column++)
        {
            returnString += cm.getElement(column, row) == 1 ? "#" : ".";
        }
        returnString += "\n";
    }
    return returnString;
}

int main() {
    // This is an use example for the CellularMap class.
    CellularMap cm(50, 50, 40);
    for (int i=0;i<4;++i){ cm.evolveMap(CellularMap::CM_CONSERVATIVE); }
    for (int i=0;i<2;++i){ cm.evolveMap(CellularMap::CM_SMOOTH); }
    cout << mapToString(cm) << endl;
    return 0;
}