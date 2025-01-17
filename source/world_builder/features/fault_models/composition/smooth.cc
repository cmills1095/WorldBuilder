/*
  Copyright (C) 2020 - 2022 by the authors of the World Builder code.

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

#include "world_builder/features/fault_models/composition/smooth.h"

#include <world_builder/utilities.h>
#include <world_builder/assert.h>
#include <world_builder/nan.h>
#include <world_builder/parameters.h>

#include <world_builder/types/double.h>
#include <world_builder/types/unsigned_int.h>
#include <world_builder/types/array.h>
#include <world_builder/types/string.h>
#include <world_builder/types/object.h>


namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    namespace FaultModels
    {
      namespace Composition
      {
        Smooth::Smooth(WorldBuilder::World *world_)
          :
          min_distance(NaN::DSNAN),
          side_distance(NaN::DSNAN),
          operation(Utilities::Operations::REPLACE)
        {
          this->world = world_;
          this->name = "smooth";
        }

        Smooth::~Smooth()
        { }

        void
        Smooth::declare_entries(Parameters &prm, const std::string &)
        {
          // Add compositions to the required parameters.
          prm.declare_entry("", Types::Object({"compositions"}), "Compositional model object");

          prm.declare_entry("min distance fault center", Types::Double(0),
                            "The distance in meters from which the composition of this feature is present.");
          prm.declare_entry("side distance fault center", Types::Double(std::numeric_limits<double>::max()),
                            "The distance over which the composition is reduced from 1 to 0.");
          prm.declare_entry("center fractions",  Types::Array(Types::Double(1.0),1),
                            "The composition fraction at the center of the fault.");
          prm.declare_entry("side fractions",  Types::Array(Types::Double(0.0),1),
                            "The composition fraction at the sides of this feature.");
          prm.declare_entry("compositions", Types::Array(Types::UnsignedInt(),0),
                            "A list with the labels of the composition which are present there.");
          prm.declare_entry("operation", Types::String("replace", std::vector<std::string> {"replace"}),
                            "Whether the value should replace any value previously defined at this location (replace) or "
                            "add the value to the previously define value (add, not implemented). Replacing implies that all values not "
                            "explicitly defined are set to zero.");
        }

        void
        Smooth::parse_entries(Parameters &prm)
        {
          min_distance = prm.get<double>("min distance fault center");
          side_distance = prm.get<double>("side distance fault center");
          WBAssert(side_distance >= min_distance, "distance at the side needs to be larger or equal than the min distance.");
          operation = Utilities::string_operations_to_enum(prm.get<std::string>("operation"));
          center_fraction = prm.get_vector<double>("center fractions");
          side_fraction = prm.get_vector<double>("side fractions");
          compositions = prm.get_vector<unsigned int>("compositions");
        }


        double
        Smooth::get_composition( const Point<3> &,
                                 const double ,
                                 const unsigned int composition_number,
                                 double composition_,
                                 const double ,
                                 const double ,
                                 const WorldBuilder::Utilities::PointDistanceFromCurvedPlanes &distance_from_planes,
                                 const Utilities::AdditionalParameters &) const
        {
          double composition = composition_;

          for (unsigned int i =0; i < compositions.size(); ++i)
            {
              if (compositions[i] == composition_number)
                {

                  // Hyperbolic tangent goes from 0 to 1 over approximately x=(0, 2) without any arguements. The function is written
                  // so that the composition returned 1 to 0 over the side_distance on either sides.
                  composition = (center_fraction[i] - side_fraction[i]) * ( 1 - std::tanh(10 * (distance_from_planes.distance_from_plane
                                                                                                - side_distance/2)/side_distance ) )/2 ;

                  return Utilities::apply_operation(operation,composition_,composition);
                }
            }
          return composition;
        }

        WB_REGISTER_FEATURE_FAULT_COMPOSITION_MODEL (Smooth, smooth)
      }
    }
  }
}

