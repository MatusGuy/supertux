//  SuperTux
//  Copyright (C) 2006 Matthias Braun <matze@braunis.de>
//                2018 Ingo Ruhnke <grumbel@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_SUPERTUX_COLLISION_COLLISION_GROUP_HPP
#define HEADER_SUPERTUX_COLLISION_COLLISION_GROUP_HPP

#include <cinttypes>

enum CollisionGroup : int {
  /** Objects in DISABLED group are not tested for collisions */
  COLGROUP_DISABLED = 1 << 0,



  /** Tested against:
      - moving objects
      and it counts as an obstacle during static collision phase.

      Use for static obstacles that Tux walks on. */
  COLGROUP_STATIC = 1 << 1,

  /** Tested against:
      - tiles + attributes
      - static obstacles
      - touchables
      - other moving objects

      Use for ordinary objects. */
  COLGROUP_MOVING = 1 << 2,


  /** Tested against:
      - tiles + attributes
      - static obstacles
      - touchables
      - other moving objects
      and it counts as an obstacle during static collision phase.

      Use for kinematic moving objects like platforms and rocks. */
  COLGROUP_MOVING_STATIC = 1 << 3,

  /** Tested against:
      - tiles + attributes
      - static obstacles

      Use for interactive particles and decoration. */
  COLGROUP_MOVING_ONLY_STATIC = 1 << 4,

  /** Tested against:
      - moving objects

      Use for triggers like spikes/areas or collectibles like coins. */
  COLGROUP_TOUCHABLE = 1 << 5,

  /// Used as an input for functions. It represents all collision groups.
  COLGROUP_ALL = 0xFF
};

#endif

/* EOF */
