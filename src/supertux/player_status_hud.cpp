//  SuperTux
//  Copyright (C) 2018 Ingo Ruhnke <grumbel@gmail.com>
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

#include "supertux/player_status_hud.hpp"

#include "sprite/sprite_manager.hpp"
#include "supertux/game_object.hpp"
#include "supertux/player_status.hpp"
#include "supertux/resources.hpp"
#include "video/drawing_context.hpp"
#include "video/surface.hpp"
#include "editor/editor.hpp"

#include <iostream>

static const int DISPLAYED_COINS_UNSET = -1;

PlayerStatusHUD::PlayerStatusHUD(PlayerStatus& player_status) :
  m_player_status(player_status),
  displayed_coins(DISPLAYED_COINS_UNSET),
  displayed_coins_frame(0),
  coin_surface(Surface::from_file("images/engine/hud/coins-0.png")),
  fire_surface(Surface::from_file("images/engine/hud/fire-0.png")),
  ice_surface(Surface::from_file("images/engine/hud/ice-0.png")),
  m_bonus_sprites(),
  m_item_pocket_border(Surface::from_file("images/engine/hud/item_pocket.png"))
{
  m_bonus_sprites[FIRE_BONUS]  = SpriteManager::current()->create("images/powerups/fireflower/fireflower.sprite");
  m_bonus_sprites[ICE_BONUS]   = SpriteManager::current()->create("images/powerups/iceflower/iceflower.sprite");
  m_bonus_sprites[AIR_BONUS]   = SpriteManager::current()->create("images/powerups/airflower/airflower.sprite");
  m_bonus_sprites[EARTH_BONUS] = SpriteManager::current()->create("images/powerups/earthflower/earthflower.sprite");
}

void
PlayerStatusHUD::reset()
{
  displayed_coins = DISPLAYED_COINS_UNSET;
}

void
PlayerStatusHUD::update(float dt_sec)
{
}

void
PlayerStatusHUD::draw(DrawingContext& context)
{
  if (Editor::is_active())
    return;

  if ((displayed_coins == DISPLAYED_COINS_UNSET) ||
      (std::abs(displayed_coins - m_player_status.coins) > 100)) {
    displayed_coins = m_player_status.coins;
    displayed_coins_frame = 0;
  }
  if (++displayed_coins_frame > 2) {
    displayed_coins_frame = 0;
    if (displayed_coins < m_player_status.coins) displayed_coins++;
    if (displayed_coins > m_player_status.coins) displayed_coins--;
  }
  displayed_coins = std::min(std::max(displayed_coins, 0), m_player_status.get_max_coins());

  float hudpos = BORDER_Y + 1.0f;
  const std::string coins_text = std::to_string(displayed_coins);

  context.push_transform();
  context.set_translation(Vector(0, 0));
  context.transform().scale = 1.f;
  if (!Editor::is_active())
  {
    if (coin_surface)
    {
      context.color().draw_surface(coin_surface,
                                  Vector(context.get_width() - BORDER_X - static_cast<float>(coin_surface->get_width()) - Resources::fixed_font->get_text_width(coins_text),
                                         hudpos),
                                  LAYER_HUD);
    }

    context.color().draw_text(Resources::fixed_font,
                              coins_text,
                              Vector(static_cast<float>(context.get_width()) - BORDER_X - Resources::fixed_font->get_text_width(coins_text),
                                    hudpos + 13.f),
                              ALIGN_LEFT,
                              LAYER_HUD,
                              PlayerStatusHUD::text_color);
  }

  hudpos += 8.f;
  for (int target = 0; target < InputManager::current()->get_num_users(); target++)
  {
    SurfacePtr surface;
    std::string ammo_text;

    if (m_player_status.bonus[target] == FIRE_BONUS)
    {
      surface = fire_surface;
      ammo_text = std::to_string(m_player_status.max_fire_bullets[target]);
    }
    else if (m_player_status.bonus[target] == ICE_BONUS)
    {
      surface = ice_surface;
      ammo_text = std::to_string(m_player_status.max_ice_bullets[target]);
    }
    else
    {
      continue;
    }

    hudpos += static_cast<float>(surface->get_height());

    const float ammo_text_width = Resources::fixed_font->get_text_width(ammo_text);

    if (InputManager::current()->get_num_users() > 1)
    {
      const std::string player_text = std::to_string(target + 1) + ":";

      context.color().draw_text(Resources::fixed_font,
                                player_text,
                                Vector(context.get_width() - BORDER_X - ammo_text_width -
                                         static_cast<float>(surface->get_width()) -
                                         Resources::fixed_font->get_text_width(player_text) - 3.f,
                                       hudpos + 13.f),
                                ALIGN_LEFT,
                                LAYER_HUD,
                                PlayerStatusHUD::text_color);
    }

    context.color().draw_surface(surface,
                                 Vector(context.get_width() - BORDER_X - ammo_text_width -
                                          static_cast<float>(surface->get_width()),
                                        hudpos),
                                 LAYER_HUD);

    context.color().draw_text(Resources::fixed_font,
                              ammo_text,
                              Vector(context.get_width() - BORDER_X - ammo_text_width,
                                     hudpos + 13.f),
                              ALIGN_LEFT,
                              LAYER_HUD,
                              PlayerStatusHUD::text_color);
  }

  std::cout << m_player_status.m_num_players << std::endl;
  for (int i = 0; i < m_player_status.m_num_players; i++) {
    float ypos = static_cast<float>(m_item_pocket_border->get_height() * i);
    Vector pos(BORDER_X, BORDER_Y + ypos);
    context.color().draw_surface(m_item_pocket_border, pos, LAYER_HUD);

    if (m_bonus_sprites.find(m_player_status.m_item_pockets[i]) != m_bonus_sprites.end())
    {
      pos += 20;
      Sprite* sprite = m_bonus_sprites[m_player_status.m_item_pockets.front()].get();
      sprite->draw(context.color(), pos, LAYER_HUD);
    }
  }

  context.pop_transform();
}

/* EOF */
