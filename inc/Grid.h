#pragma once

#include "Assets.h"
#include "Define.h"
#include "Entity.h"
#include "Board.h"
#include "IUiComponent.h"

#include <SDL.h>

#include <string>

/*!
 * Helper struct to represent (row, column)
 *
 *  We use same coordinate system for indexes as for mouse SDL mouse positions:
 *
 *            ------------------> x-axis = row
 *            |
 *            |
 *            |
 *            |
 *            |
 *           \/
 *           y-axis = column
 *
 *           Hint: Don't think MS excel here...
 *           Reason: Much better to align 2 coordinate systems than creating
 *           an inversion that needs remap/conversion all the time...
 *
 */
struct Index
{
public:
    Index(int r = 0, int c = 0) {
        row = r;
        column = c;
    }
    int row, column; // row is x-axis, column y-axis aligned with SDL coordinate system
};

class Grid : public Board, public Event
{
public:
    static Grid instance;

    /*!
     * Construct a grid with left upper corner at (x,y)
     * @param x x position upper left corner
     * @param y y position upper left corner
     */
    Grid(int x = 0, int y = 0);

    /*!
     * Unload resources
     */
    ~Grid();

    /*!
     * Set grid upper left corner at position (x, y)
     * @param xpos
     * @param ypos
     */
    void setPosition(int x, int y);

    /*!
     * Loads grid assets
     * @param assets The asset manager to extract assets
     * @return
     */
    bool load(Assets& assets);

    void render(SDL_Surface* Surf_Display);

    void cleanup();

    /*!
     * Creates a new random integer for randomizing the grid...
     * @return a random int
     */
    int getRandomInt();

    /*!
     * Creates a starting point with bricks laid our randomly with max
     * 2 of the same in a set.
     */
    void initGrid();

    /*!
     * Creates a new entity in the big grid. This is where the bricks are created
     * with id that has been randomly generated according to many rules...
     * @param row
     * @param column
     * @param id
     */
    void loadEntity(int row, int column, int id);

    //// Events

    virtual void onLButtonDown(int mX, int mY);
    //virtual void onLButtonUp(int mX, int mY);
    virtual void onKeyDown(SDL_Keycode sym, Uint16 mod, SDL_Scancode unicode);

    /*!
     * Calculates the grid indexes from mouse clicked positions
     * @param x
     * @param y
     * @return
     */
    Index getIndexesFromPosition(int x, int y);

    /*!
     * Update game based on new interaction from user
     * @param ind
     */
    void update(const Index& ind);

    /*!
     * Check if this index is neighbour with previous clicked position
     * @param ind
     * @return
     */
    bool isAdjacent(const Index& ind);

    /*!
     * Swap grid entities and undo if no match
     * @param from
     * @param to
     */
    void swapEntity(Index from, Index to);

    /*!
     * Searches for vertical matches and return
     * index set with connected entities
     * @param ind
     * @return
     */
    std::vector<Index> findVerticalMatches(const Index& ind);

    /*!
     * Find matches horizontally
     * @param ind
     * @return
     */
    std::vector<Index> findHorizontalMatches(const Index& ind);

    /*!
     * Return my icon asset list
     * @return
     */
    std::vector<std::string> getAssets();

    /*!
     * Get entity at index for unit test purpose
     */
     Entity* getEntity(Index ind);

     /*!
      * Load a pre grid used for unit test purpose.
      * Then it will not use any ranomness...
      */
    void setGridMatrix(Entity*** grid);

    //////////////////
    void removeMatches(const std::vector<Index>& matches);

    void collapseColumns(const std::vector<Index>& matches);

    void printGrid();

    void fillCollapsedColumns(const std::vector<Index>& matches);

    std::vector<int> getDistinctColumns(const std::vector<Index>& matches);

    void collapse(std::vector<int> columns);

    void createNewEntitiesInColumns(std::vector<int> columns);

    std::vector<Index> getEmptyItemsOnColumn(int column);

      /*!
       * Moves current highlight position to new index
       * @param index new index to highlight
       */
      void setHighlightPosition(const Index& index);

      /*!
       * Returns the maximum Grid index
       * @return maximum grid index which naturally is the width & height
       * of the grid...
       */
        Index getMaximumGridIndex();

private:
    std::vector<std::string> mAssets;
    Entity*** mGrid;
    std::string mImagePath;

    int mTileWidth, mTileHeight;
    Index mPrevClickedIndexes;
};
