//  SuperTux
//  Copyright (C) 2006 Matthias Braun <matze@braunis.de>
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

#include "supertux/moving_object.hpp"

#include "editor/resize_marker.hpp"
#include "supertux/debug.hpp"
#include "supertux/sector.hpp"
#include "util/reader_mapping.hpp"
#include "util/writer.hpp"
#include "video/drawing_context.hpp"

MovingObject::MovingObject() :
  m_col(COLGROUP_MOVING, *this),
  m_parent_dispenser(),
  m_detection_ranges(),
  m_curr_detection_range(0)
{
}

MovingObject::MovingObject(const ReaderMapping& reader) :
  GameObject(reader),
  m_col(COLGROUP_MOVING, *this),
  m_parent_dispenser(),
  m_detection_ranges(),
  m_curr_detection_range(0)
{
  float height, width;

  if (reader.get("width", width))
    m_col.m_bbox.set_width(width);

  if (reader.get("height", height))
    m_col.m_bbox.set_height(height);

  reader.get("x", m_col.m_bbox.get_left());
  reader.get("y", m_col.m_bbox.get_top());
}

MovingObject::~MovingObject()
{
}

void MovingObject::draw(DrawingContext& context)
{
  if (!g_debug.show_detection_ranges)
    return;

  for (DetectionRange drange : m_detection_ranges) {
    auto& range = drange.range;
    drange.color.alpha = 0.75f;
    if (std::holds_alternative<Vector>(range))
    {
      auto point = std::get<Vector>(range);
      Rectf rect(point.x - 3, point.y - 3, point.x + 3, point.y + 3);
      context.color().draw_filled_rect(rect, drange.color, 3, LAYER_HUD);
    }
    else if (std::holds_alternative<Rectf>(range))
    {
      auto rect = std::get<Rectf>(range);
      context.color().draw_filled_rect(rect, drange.color, LAYER_HUD);
    }
    else if (std::holds_alternative<DetectionRange::Line>(range))
    {
      auto line = std::get<DetectionRange::Line>(range);
      context.color().draw_line(line.first, line.second, drange.color, LAYER_HUD);
    }
    else if (std::holds_alternative<DetectionRange::Circle>(range))
    {
      auto circle = std::get<DetectionRange::Circle>(range);
      Vector origin = circle.first;
      float r = circle.second;
      Rectf rect(origin.x - r, origin.y - r, origin.x + r, origin.y + r);
      context.color().draw_filled_rect(rect, drange.color, r, LAYER_HUD);
    }
  }

  m_curr_detection_range = 0;
}

ObjectSettings
MovingObject::get_settings()
{
  ObjectSettings result = GameObject::get_settings();

  if (m_parent_dispenser)
  {
    result.remove("name");
    return result;
  }

  if (has_variable_size())
    result.add_rectf(_("Region"), &m_col.m_bbox, "region", OPTION_HIDDEN);

  result.add_float(_("X"), &m_col.m_bbox.get_left(), "x", {}, OPTION_HIDDEN);
  result.add_float(_("Y"), &m_col.m_bbox.get_top(), "y", {}, OPTION_HIDDEN);

  return result;
}

void
MovingObject::set_parent_dispenser(Dispenser* dispenser)
{
  m_parent_dispenser = dispenser;
  if (dispenser)
  {
    m_name.clear();
  }
}

void
MovingObject::editor_select()
{
  Sector::get().add<ResizeMarker>(this, ResizeMarker::Side::LEFT_UP, ResizeMarker::Side::LEFT_UP);
  Sector::get().add<ResizeMarker>(this, ResizeMarker::Side::LEFT_UP, ResizeMarker::Side::NONE);
  Sector::get().add<ResizeMarker>(this, ResizeMarker::Side::LEFT_UP, ResizeMarker::Side::RIGHT_DOWN);
  Sector::get().add<ResizeMarker>(this, ResizeMarker::Side::NONE, ResizeMarker::Side::LEFT_UP);
  Sector::get().add<ResizeMarker>(this, ResizeMarker::Side::NONE, ResizeMarker::Side::RIGHT_DOWN);
  Sector::get().add<ResizeMarker>(this, ResizeMarker::Side::RIGHT_DOWN, ResizeMarker::Side::LEFT_UP);
  Sector::get().add<ResizeMarker>(this, ResizeMarker::Side::RIGHT_DOWN, ResizeMarker::Side::NONE);
  Sector::get().add<ResizeMarker>(this, ResizeMarker::Side::RIGHT_DOWN, ResizeMarker::Side::RIGHT_DOWN);
}

void
MovingObject::on_flip(float height)
{
  Vector pos = get_pos();
  pos.y = height - pos.y - get_bbox().get_height();
  set_pos(pos);
}

void MovingObject::add_debug_range(DetectionRange range)
{
  if (!g_debug.show_detection_ranges)
    return;

  if (m_curr_detection_range + 1 >= m_detection_ranges.size())
    m_detection_ranges.push_back(range);
  else
    m_detection_ranges[m_curr_detection_range] = range;

  m_curr_detection_range++;
}

/* EOF */
