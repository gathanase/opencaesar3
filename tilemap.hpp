// This file is part of openCaesar3.
//
// openCaesar3 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// openCaesar3 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with openCaesar3.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright 2012-2013 Gregoire Athanase, gathanase@gmail.com


#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <serializer.hpp>
#include <picture.hpp>
#include <vector>
#include <list>


class LandOverlay;

class TerrainTile : public Serializable
{
public:
   TerrainTile();

   // reset all fields
   void reset();

   bool isConstructible() const;
   bool isDestructible() const;
//   bool isWalkable();
//   bool isBuilding();
   // isBurnable

   bool isWater() const;
   bool isRock() const;
   bool isTree() const;
   bool isBuilding() const;
   bool isRoad() const;

   void setWater(const bool isWater);
   void setRock(const bool isRock);
   void setTree(const bool isTree);
   void setBuilding(const bool isBuilding);
   void setRoad(const bool isRoad);

   void setOverlay(LandOverlay *overlay);
   LandOverlay *getOverlay() const;

   // encode/decode to bitset
   int encode() const;
   void decode(const int bitset);

   void serialize(OutputSerialStream &stream);
   void unserialize(InputSerialStream &stream);
private:
   bool _isWater;
   bool _isRock;
   bool _isTree;
   bool _isBuilding;
   bool _isRoad;
   LandOverlay *_overlay;
};



// a Tile in the Tilemap
class Tile
{
public:
   Tile(const int i, const int j);
   Tile(const Tile& clone);

   // tile coordinates
   int getI() const;
   int getJ() const;

   // displayed picture
   void set_picture(Picture *picture);
   Picture& get_picture() const;

   // used for multi-tile graphics: current displayed picture
   // background of constructible tiles is 1x1 => master used for foreground
   // non-constructible tiles have no foreground => master used for background
   Tile* get_master_tile() const;
   void set_master_tile(Tile* master);

   const TerrainTile& get_terrain() const;
   TerrainTile& get_terrain();
   bool is_flat() const;  // returns true if the tile is walkable/boatable (for display purpose)

private:
   int _i; // coordinates of the tile
   int _j; // coordinates of the tile
   Tile* _master_tile;  // left-most tile if multi-tile, or "this" if single-tile
   TerrainTile _terrain;    // infos about the tile (building, tree, road, water, rock...)

   Picture *_picture; // displayed picture
};


// Square Map of the Tiles.
class Tilemap : public Serializable
{
public:
   Tilemap();
   void init(const int size);

   bool is_inside(const int i, const int j) const;
   Tile& at(const int i, const int j);
   const Tile& at(const int i, const int j) const;

   // returns all tiles on a rectangular perimeter
   // (i1, j1) : left corner of the rectangle (minI, minJ)
   // (i2, j2) : right corner of the rectangle (maxI, maxJ)
   // corners  : if false, don't return corner tiles
   std::list<Tile*> getRectangle(const int i1, const int j1, const int i2, const int j2, const bool corners = true);

   // returns all tiles in a rectangular area
   // (i1, j1) : left corner of the rectangle (minI, minJ)
   // (i2, j2) : right corner of the rectangle (maxI, maxJ)
   std::list<Tile*> getFilledRectangle(const int i1, const int j1, const int i2, const int j2);

   int getSize() const;

   void serialize(OutputSerialStream &stream);
   void unserialize(InputSerialStream &stream);

private:
   std::vector<std::vector<Tile> > _tile_array;
   int _size;
};


#endif