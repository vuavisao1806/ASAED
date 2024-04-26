#include "object/portal.hpp"

#include "asaed/game_manager.hpp"
#include "asaed/game_session.hpp"
#include "asaed/level.hpp"
#include "asaed/resources.hpp"
#include "asaed/room.hpp"
#include "object/camera.hpp"
#include "object/player.hpp"
#include "gui/colorscheme.hpp"

namespace {
	const float WAITING_TIME = 2.0f;
	const float BAR_WIDTH = 200.0f; // options
	const float BAR_HEIGHT = 20.0f; // options
}

Portal::Portal(const Vector& position) :
	MovingSprite(Vector(0.0f, 0.0f), "images/object/portal_big/portal.json")
{
	set_pos(position - get_bounding_box().get_middle());
}

void Portal::update(float /* dt_sec */) {
	if (Room::get().get_bounding_box().contains(get_bounding_box())) {
		if (get_bounding_box().contains(Room::get().get_nearest_player(get_pos())->get_bounding_box())) {
			if (m_timer.get_period() == 0.0f) {
				m_timer.start(WAITING_TIME, false);
			}
			else if (m_timer.check()) {
				GameManager::current()->start_level(GameSession::current()->get_current_level().get_next_level(), true);
			}
		}
		else {
			m_timer.stop();
		}
	}
	else {
		m_timer.stop();
	}
}

void Portal::draw(DrawingContext& drawing_context) {
	MovingSprite::draw(drawing_context);

	if (Room::get().get_bounding_box().contains(get_bounding_box())) {
		// draw hud
		if (get_bounding_box().contains(Room::get().get_nearest_player(get_pos())->get_bounding_box())) {
			drawing_context.get_canvas().draw_text(Resources::small_font, get_class_name(),
			                                       Vector(get_bounding_box().get_middle().x, get_bounding_box().p1().y),
			                                       FontAlignment::ALIGN_CENTER, LAYER_HUD, ColorScheme::Text::small_color);
		
		
			// draw % to transition to the next level
			{

				Vector position = Room::get().get_camera().get_translation() + 
				                  Vector(Room::get().get_camera().get_screen_size().width / 2.0f, 3.0f) -
				                  Vector(BAR_WIDTH / 2.0f, 0.0f);

				Sizef size = Sizef(BAR_WIDTH, BAR_HEIGHT);
				
				drawing_context.get_canvas().draw_filled_rect(Rectf(position, size), ColorScheme::HUD::frame_back, LAYER_HUD);
				position += Vector(1.0f, 1.0f);

				size -= Sizef(2.0f, 2.0f);
				drawing_context.get_canvas().draw_filled_rect(Rectf(position, size), ColorScheme::HUD::frame_front, LAYER_HUD);

				position += Vector(1.0f, 1.0f);
				size -= Sizef(2.0f, 2.0f);
				
				drawing_context.get_canvas().draw_filled_rect(Rectf(position, size), ColorScheme::HUD::space_back, LAYER_HUD);
				
				position += Vector(1.0f, 1.0f);
				size -= Sizef(2.0f, 2.0f);

				float add = size.width / WAITING_TIME;
				drawing_context.get_canvas().draw_filled_rect(Rectf(position, Sizef(add * (WAITING_TIME - m_timer.get_timeleft()), size.height)), ColorScheme::HUD::normal_color, LAYER_HUD);
			}
		}
	}
}


HitResponse Portal::collision(CollisionObject& /* other */, const CollisionHit& /* hit */) {
	return ABORT_MOVE;
}

bool Portal::is_singleton() const { return true; }

std::string Portal::class_name() { return "portal"; }
std::string Portal::get_class_name() const { return class_name(); }