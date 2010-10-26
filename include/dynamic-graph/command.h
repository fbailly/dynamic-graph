//
// Copyright 2010 CNRS
//
// Author: Florent Lamiraux
//
// This file is part of dynamic-graph.
// dynamic-graph is free software: you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
// dynamic-graph is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.  You should
// have received a copy of the GNU Lesser General Public License along
// with dynamic-graph.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DYNAMIC_GRAPH_COMMAND_H
#define DYNAMIC_GRAPH_COMMAND_H

#include <vector>
#include "dynamic-graph/value.h"
#include "dynamic-graph/dynamic-graph-api.h"

namespace dynamicgraph {
  class Entity;
  namespace command {
    /// Abstract class for entity commands
    ///
    /// This class provide a mean to control entities from external python script.
    /// A command has several parameters (dynamicgraph::parameter) that can take
    /// various types of values (dynamicgraph::Value).

    class DYNAMICGRAPH_EXPORT Command
    {
    public:
      virtual ~Command();
      /// Store the owner entity and a vector of value types
      /// \param entity reference to Entity owning this command.
      /// \param valueTypes vector specifying the number and types of parameters
      Command(Entity& entity, const std::vector<Value::Type>& valueTypes);
      /// Return the value type of all parameters
      const std::vector<Value::Type> valueTypes() const;
      /// Set parameter values
      void setParameterValues(const std::vector<Value>& values);
      /// Get parameter values
      const std::vector<Value> getParameterValues() const;
      /// Execute the command after checking parameters
      Value execute();
      /// Get a reference to the Entity owning this command
      Entity& owner();
    protected:
      /// Specific action performed by the command
      virtual Value doExecute() = 0;
    private:
      Entity& owner_;
      std::vector<Value::Type> valueTypeVector_;
      std::vector<Value> valueVector_;
    };
  } // namespace command
} // namespace dynamicgraph

#endif //DYNAMIC_GRAPH_COMMAND_H
