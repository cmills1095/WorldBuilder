/*
  Copyright (C) 2018 - 2022 by the authors of the World Builder code.

  This file is part of the World Builder.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation, either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "world_builder/objects/distance_from_surface.h"

namespace WorldBuilder
{
  namespace Objects
  {
    double PlaneDistances::get_distance_from_surface()
    {
      return this->distance_from_surface;
    }

    double PlaneDistances::get_distance_along_surface()
    {
      return this->distance_along_surface;
    }
  }
}