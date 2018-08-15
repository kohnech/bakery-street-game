#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Grid.h"

#include "json.hpp"


using json = nlohmann::json;


TEST(GridTests, Test_function_load)
{
    Assets assets;
    assets.loadFile("assets.json");

    Grid testGrid;

    Pair pos = testGrid.getPosition();
    EXPECT_EQ(pos.first, 0);
    EXPECT_EQ(pos.second, 0);

    Pair size = testGrid.getSize();
    EXPECT_EQ(size.first, 0);
    EXPECT_EQ(size.second, 0);

    testGrid.load(assets);

    size = testGrid.getSize();
    EXPECT_EQ(size.first, 10);
    EXPECT_EQ(size.second, 10);

    pos = testGrid.getPosition();
    EXPECT_EQ(pos.first, 100);
    EXPECT_EQ(pos.second, 100);

    std::vector<std::string> vec = testGrid.getAssets();
    EXPECT_EQ(vec.size(), 5u);

    Pair gridSize = testGrid.getBoardSize();
    EXPECT_EQ(gridSize.first, 5);
    EXPECT_EQ(gridSize.second, 5);
}

TEST(GridTests, Test_function_getIndexesFromPosition)
{
    Assets assets;
    assets.loadFile("assets.json");

    Grid testGrid;
    testGrid.load(assets);

    Pair size = testGrid.getSize();
    EXPECT_EQ(size.first, 10);
    EXPECT_EQ(size.second, 10);

    Index ind;
    ind = testGrid.getIndexesFromPosition(50, 50);
    EXPECT_EQ(ind.column, 0);
    EXPECT_EQ(ind.row, 0);

    ind = testGrid.getIndexesFromPosition(100, 100);
    EXPECT_EQ(ind.column, 0);
    EXPECT_EQ(ind.row, 0);

    ind = testGrid.getIndexesFromPosition(105, 103);
    EXPECT_EQ(ind.column, 0);
    EXPECT_EQ(ind.row, 0);

    ind = testGrid.getIndexesFromPosition(110, 110);
    EXPECT_EQ(ind.column, 1);
    EXPECT_EQ(ind.row, 1);

    ind = testGrid.getIndexesFromPosition(121, 122);
    EXPECT_EQ(ind.column, 2);
    EXPECT_EQ(ind.row, 2);

    ind = testGrid.getIndexesFromPosition(121, 122);
    EXPECT_EQ(ind.column, 2);
    EXPECT_EQ(ind.row, 2);

    // Test extreme point now we are outside of the board
    ind = testGrid.getIndexesFromPosition(150, 150);
    EXPECT_EQ(ind.column, 0);
    EXPECT_EQ(ind.row, 0);

    // still inside... so a valid index...
    ind = testGrid.getIndexesFromPosition(149, 149);
    EXPECT_EQ(ind.column, 4);
    EXPECT_EQ(ind.row, 4);
}

TEST(GridTests, Test_function_isAdjacent)
{
    Assets assets;
    assets.loadFile("assets.json");

    Grid testGrid;
    testGrid.load(assets);

    // Set mPrevClickedPosition with a mouse click... Now it is att Index (0,0)
    testGrid.onLButtonDown(100,100);
    EXPECT_TRUE(testGrid.isAdjacent(Index(1,0)));
    EXPECT_TRUE(testGrid.isAdjacent(Index(0,1)));

    // Click index (4,0)
    testGrid.onLButtonDown(149,100);
    EXPECT_TRUE(testGrid.isAdjacent(Index(3,0)));
    EXPECT_TRUE(testGrid.isAdjacent(Index(4,1)));

    // Click index (4,4)
    testGrid.onLButtonDown(149,149);
    EXPECT_TRUE(testGrid.isAdjacent(Index(4,3)));
    EXPECT_TRUE(testGrid.isAdjacent(Index(3,4)));

    // Click index (2,1)
    testGrid.onLButtonDown(120,110);
    EXPECT_TRUE(testGrid.isAdjacent(Index(2,0)));
    EXPECT_TRUE(testGrid.isAdjacent(Index(2,2)));
    EXPECT_TRUE(testGrid.isAdjacent(Index(1,1)));
    EXPECT_TRUE(testGrid.isAdjacent(Index(3,1)));

    // Click index (3,3)
    testGrid.onLButtonDown(130,130);
    EXPECT_TRUE(testGrid.isAdjacent(Index(3,2)));
    EXPECT_TRUE(testGrid.isAdjacent(Index(2,3)));
    EXPECT_TRUE(testGrid.isAdjacent(Index(4,3)));
    EXPECT_TRUE(testGrid.isAdjacent(Index(3,4)));
}

TEST(GridTests, Test_function_swapEntity)
{
    Assets assets;
    assets.loadFile("assets.json");

    Grid testGrid;
    testGrid.load(assets);

    Index indFrom(0,0);
    Index indTo(1,1);
    Entity* from = testGrid.getEntity(indFrom);
    Entity* to = testGrid.getEntity(indTo);

    testGrid.swapEntity(indFrom, indTo);
    Entity* newFrom = testGrid.getEntity(indFrom);
    Entity* newTo = testGrid.getEntity(indTo);

    EXPECT_EQ(from, newTo);
    EXPECT_EQ(to, newFrom);

    ////
    indFrom = Index(4,4);
    indTo = Index(3,4);
    from = testGrid.getEntity(indFrom);
    to = testGrid.getEntity(indTo);

    testGrid.swapEntity(indFrom, indTo);
    newFrom = testGrid.getEntity(indFrom);
    newTo = testGrid.getEntity(indTo);

    EXPECT_EQ(from, newTo);
    EXPECT_EQ(to, newFrom);

    // Try swap impossible case. This case will result in NO swap!!!
    indFrom = Index(4,4);
    indTo = Index(5,4);
    from = testGrid.getEntity(indFrom);
    to = testGrid.getEntity(indTo);

    testGrid.swapEntity(indFrom, indTo);
    newFrom = testGrid.getEntity(indFrom);
    newTo = testGrid.getEntity(indTo);

    EXPECT_EQ(testGrid.getEntity(Index(4, 4)), newTo);
    EXPECT_EQ(from, newFrom);
    EXPECT_EQ(testGrid.getEntity(Index(4, 4)), to);
}

TEST(GridTests, Test_function_getDistinctColumns)
{
    Assets assets;
    assets.loadFile("assets.json");
    Grid testGrid;
    testGrid.load(assets); // TODO create fixture...

    std::vector<Index> matches;
    Index ind1(1, 1);
    Index ind2(2, 1);
    Index ind3(3, 1);

    matches = {ind1, ind2, ind3};

    std::vector<int> columns = testGrid.getDistinctRows(matches);
    EXPECT_EQ(columns.at(0), 1);
    EXPECT_EQ(columns.at(1), 2);
    EXPECT_EQ(columns.at(2), 3);
}
