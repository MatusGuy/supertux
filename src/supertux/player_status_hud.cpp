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

#include <iostream>

#include "sprite/sprite_manager.hpp"
#include "supertux/game_object.hpp"
#include "supertux/level.hpp"
#include "supertux/player_status.hpp"
#include "supertux/resources.hpp"
#include "supertux/title_screen.hpp"
#include "supertux/screen_manager.hpp"
#include "video/drawing_context.hpp"
#include "video/surface.hpp"
#include "editor/editor.hpp"
#include "worldmap/worldmap_sector.hpp"

static const int DISPLAYED_COINS_UNSET = -1;

PlayerStatusHUD::PlayerStatusHUD(PlayerStatus& player_status) :
  m_player_status(player_status),
  m_hud_items(),
  m_bonus_sprites(),
  m_item_pocket_border(Surface::from_file("images/engine/hud/item_pocket.png"))
{
  auto coinshud = std::make_unique<CoinHUDItem>(this, ANCHOR_TOP_RIGHT);
  coinshud->initialize();
  m_hud_items.push_back(std::move(coinshud));

  for (std::unique_ptr<HUDItem>& item : m_hud_items)
  {
    item->initialize();
  }

  m_bonus_sprites[BONUS_FIRE]  = SpriteManager::current()->create("images/powerups/fireflower/fireflower.sprite");
  m_bonus_sprites[BONUS_ICE]   = SpriteManager::current()->create("images/powerups/iceflower/iceflower.sprite");
  m_bonus_sprites[BONUS_AIR]   = SpriteManager::current()->create("images/powerups/airflower/airflower.sprite");
  m_bonus_sprites[BONUS_EARTH] = SpriteManager::current()->create("images/powerups/earthflower/earthflower.sprite");
}

PlayerStatusHUD::~PlayerStatusHUD()
{
  for (std::unique_ptr<HUDItem>& item : m_hud_items)
  {
    item.reset();
  }
}

void
PlayerStatusHUD::reset()
{

}

void
PlayerStatusHUD::update(float dt_sec)
{
  for (std::unique_ptr<HUDItem>& item : m_hud_items)
  {
    item->update(dt_sec);
  }
}

void
PlayerStatusHUD::draw(DrawingContext& context)
{
  if (Editor::is_active())
    return;

  context.push_transform();
  context.set_translation(Vector(0, 0));
  context.transform().scale = 1.f;

  for (std::unique_ptr<HUDItem>& item : m_hud_items)
  {
    item->draw(context);
  }

  context.pop_transform();

  /*
  if ((m_displayed_coins == DISPLAYED_COINS_UNSET) ||
      (std::abs(m_displayed_coins - m_player_status.coins) > 100)) {
    m_displayed_coins = m_player_status.coins;
    m_displayed_coins_frame = 0;
  }
  if (++m_displayed_coins_frame > 2) {
    m_displayed_coins_frame = 0;
    if (m_displayed_coins < m_player_status.coins) m_displayed_coins++;
    if (m_displayed_coins > m_player_status.coins) m_displayed_coins--;
  }
  m_displayed_coins = std::min(std::max(m_displayed_coins, 0), m_player_status.get_max_coins());

  float hudpos = BORDER_Y + 1.0f;
  const std::string coins_text = std::to_string(m_displayed_coins);

  context.push_transform();
  context.set_translation(Vector(0, 0));
  context.transform().scale = 1.f;
  if (m_coin_surface)
  {
    context.color().draw_surface(m_coin_surface,
                                Vector(context.get_width() - BORDER_X - static_cast<float>(m_coin_surface->get_width()) - Resources::fixed_font->get_text_width(coins_text),
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


  if (m_player_status.is_item_pocket_allowed())
  {
    for (int i = 0; i < InputManager::current()->get_num_users(); i++)
    {
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
  }

  context.pop_transform();
  */
}

PlayerStatusHUD::HUDItem::HUDItem(PlayerStatusHUD* parent, AnchorPoint anchor):
  m_parent(parent),
  m_pos(0.f, 0.f),
  m_width(0.f),
  m_state(HUD_STATE_ACTIVE),
  m_timer(),
  m_anchor(anchor)
{
}

void
PlayerStatusHUD::HUDItem::initialize()
{
  m_pos = get_active_pos();
  reset_timer();
}

void
PlayerStatusHUD::HUDItem::popup()
{
  if (m_state == HUD_STATE_POPUP || m_state == HUD_STATE_ACTIVE)
    return;

  m_timer.start(.1f);
  m_state = HUD_STATE_POPUP;
}

void
PlayerStatusHUD::HUDItem::hide()
{
  std::cout << "hide" << std::endl;

  if (m_state == HUD_STATE_HIDING || m_state == HUD_STATE_HIDDEN)
    return;

  m_timer.start(.1f);
  m_state = HUD_STATE_HIDING;
}

Rectf
PlayerStatusHUD::HUDItem::get_screen_rect() const
{
  std::cout << "width: " << SCREEN_WIDTH << std::endl;
  return Rectf(BORDER_X, BORDER_Y,
               static_cast<float>(SCREEN_WIDTH) - BORDER_X,
               static_cast<float>(SCREEN_HEIGHT) - BORDER_Y);
}

Vector
PlayerStatusHUD::HUDItem::get_active_pos() const
{
  return get_anchor_pos(get_screen_rect(), m_width, 0.f, m_anchor);
}

Vector
PlayerStatusHUD::HUDItem::get_hidden_pos() const
{
  return get_anchor_pos(get_screen_rect(), -(m_width + BORDER_X), 0.f, m_anchor);
}

void
PlayerStatusHUD::HUDItem::update(float dt_sec)
{
  switch (m_state)
  {
    case HUD_STATE_POPUP:
    {
      if (m_timer.check())
      {
        m_state = HUD_STATE_ACTIVE;
        reset_timer();
        break;
      }

      const float target = get_active_pos().x;
      const float origin = get_hidden_pos().x;

      const float dist = origin - target;
      const double progress = QuarticEaseOut(static_cast<double>(m_timer.get_progress()));
      const float value = origin - (static_cast<float>(progress) * dist);
      m_pos.x = value;

      break;
    }

    case HUD_STATE_ACTIVE:
      if (m_timer.check())
      {
        hide();
      }

      break;

    case HUD_STATE_HIDING:
    {
      if (m_timer.check())
      {
        m_state = HUD_STATE_HIDDEN;
        std::cout << "hidden" << std::endl;
        break;
      }

      const float target = get_hidden_pos().x;
      const float origin = get_active_pos().x;

      const float dist = target - origin;
      const double progress = QuarticEaseOut(static_cast<double>(1.f - m_timer.get_progress()));
      const float value = origin + (static_cast<float>(progress) * dist);
      m_pos.x = value;



      break;
    }

    default:
      break;
  }
}

void
PlayerStatusHUD::HUDItem::reset_timer()
{
  m_timer.start(5.f);
}

PlayerStatusHUD::CoinHUDItem::CoinHUDItem(PlayerStatusHUD* parent, AnchorPoint anchor):
  HUDItem(parent, anchor),
  m_coin_surface(Surface::from_file("images/engine/hud/coins-0.png")),
  m_coins(DISPLAYED_COINS_UNSET),
  m_coins_frame(0)
{
  m_width = static_cast<float>(m_coin_surface->get_width()) + 100.f;
  m_pos = get_active_pos();
}

void
PlayerStatusHUD::CoinHUDItem::update(float dt_sec)
{
  if (std::abs(m_coins - get_player_status().coins) != 0)
    popup();

  if ((m_coins == DISPLAYED_COINS_UNSET) ||
      (std::abs(m_coins - get_player_status().coins) > 100))
  {
    m_coins = get_player_status().coins;
    m_coins_frame = 0;
  }

  if (++m_coins_frame > 2)
  {
    m_coins_frame = 0;
    if (m_coins != get_player_status().coins)
    {
      reset_timer();

      if (m_coins < get_player_status().coins)
        m_coins++;

      if (m_coins > get_player_status().coins)
        m_coins--;
    }
  }

  m_coins = std::clamp(m_coins, 0, get_player_status().get_max_coins());

  HUDItem::update(dt_sec);
}

void
PlayerStatusHUD::CoinHUDItem::draw(DrawingContext& context)
{
  if (m_state == HUD_STATE_HIDDEN)
    return;

  std::cout << "pos: " << m_pos.x << std::endl;

  const std::string coins_text = std::to_string(m_coins);

  if (m_coin_surface)
  {
    context.color().draw_surface(m_coin_surface,
                                 m_pos,
                                 LAYER_HUD);
  }

  context.color().draw_text(Resources::fixed_font,
                            coins_text,
                            Vector(m_pos.x + static_cast<float>(m_coin_surface->get_width()),
                                   m_pos.y + 13.f),
                            ALIGN_LEFT,
                            LAYER_HUD,
                            PlayerStatusHUD::text_color);
}

/* EOF */
