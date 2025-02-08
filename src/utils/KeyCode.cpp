#include "glib/utils/KeyCode.h"

using namespace glib;

std::string glib::KeyCodeToString(KeyCode code)
{
	switch (code)
	{
	case glib::KeyCode::UNKNOWN: return "UNKNOWN";
	case glib::KeyCode::SPACE: return "SPACE";
	case glib::KeyCode::APOSTROPHE: return "APOSTROPHE";
	case glib::KeyCode::COMMA: return "COMMA";
	case glib::KeyCode::MINUS: return "MINUS";
	case glib::KeyCode::PERIOD: return "PERIOD";
	case glib::KeyCode::SLASH: return "SLASH";
	case glib::KeyCode::ZERO: return "ZERO";
	case glib::KeyCode::ONE: return "ONE";
	case glib::KeyCode::TWO: return "TWO";
	case glib::KeyCode::THREE: return "THREE";
	case glib::KeyCode::FOUR: return "FOUR";
	case glib::KeyCode::FIVE: return "FIVE";
	case glib::KeyCode::SIX: return ":";
	case glib::KeyCode::SEVEN: return "SEVEN";
	case glib::KeyCode::EIGHT: return "EIGHT";
	case glib::KeyCode::NINE: return ":";
	case glib::KeyCode::SEMICOLON: return "SEMICOLON";
	case glib::KeyCode::EQUAL: return "EQUAL";
	case glib::KeyCode::A: return "A";
	case glib::KeyCode::B: return "B";
	case glib::KeyCode::C: return "C";
	case glib::KeyCode::D: return "D";
	case glib::KeyCode::E: return "E";
	case glib::KeyCode::F: return "F";
	case glib::KeyCode::G: return "G";
	case glib::KeyCode::H: return "H";
	case glib::KeyCode::I: return "I";
	case glib::KeyCode::J: return "J";
	case glib::KeyCode::K: return "K";
	case glib::KeyCode::L: return "L";
	case glib::KeyCode::M: return "M";
	case glib::KeyCode::N: return "N";
	case glib::KeyCode::O: return "O";
	case glib::KeyCode::P: return "P";
	case glib::KeyCode::Q: return "Q";
	case glib::KeyCode::R: return "R";
	case glib::KeyCode::S: return "S";
	case glib::KeyCode::T: return "T";
	case glib::KeyCode::U: return "U";
	case glib::KeyCode::V: return "V";
	case glib::KeyCode::W: return "W";
	case glib::KeyCode::X: return "X";
	case glib::KeyCode::Y: return "Y";
	case glib::KeyCode::Z: return "Z";
	case glib::KeyCode::LEFT_BRACKET: return "LEFT_BRACKET";
	case glib::KeyCode::BACKSLASH: return "BACKSLASH";
	case glib::KeyCode::RIGHT_BRACKET: return "RIGHT_BRACKET";
	case glib::KeyCode::GRAVE_ACCENT: return "GRAVE_ACCENT";
	case glib::KeyCode::WORLD_1: return "WORLD_1";
	case glib::KeyCode::WORLD_2: return "WORLD_2";
	case glib::KeyCode::ESCAPE: return "ESCAPE";
	case glib::KeyCode::ENTER: return "ENTER";
	case glib::KeyCode::TAB: return "TAB";
	case glib::KeyCode::BACKSPACE: return "BACKSPACE";
	case glib::KeyCode::INSERT: return "INSERT";
	case glib::KeyCode::DELETE: return "DELETE";
	case glib::KeyCode::RIGHT: return "RIGHT";
	case glib::KeyCode::LEFT: return "LEFT";
	case glib::KeyCode::DOWN: return "DOWN";
	case glib::KeyCode::UP: return "UP";
	case glib::KeyCode::PAGE_UP: return "PAGE_UP";
	case glib::KeyCode::PAGE_DOWN: return "PAGE_DOWN";
	case glib::KeyCode::HOME: return "HOME";
	case glib::KeyCode::END: return "END";
	case glib::KeyCode::CAPS_LOCK: return "CAPS_LOCK";
	case glib::KeyCode::SCROLL_LOCK: return "SCROLL_LOCK";
	case glib::KeyCode::NUM_LOCK: return "NUM_LOCK";
	case glib::KeyCode::PRINT_SCREEN: return "PRINT_SCREEN";
	case glib::KeyCode::PAUSE: return "PAUSE";
	case glib::KeyCode::F1: return "F1";
	case glib::KeyCode::F2: return "F2";
	case glib::KeyCode::F3: return "F3";
	case glib::KeyCode::F4: return "F4";
	case glib::KeyCode::F5: return "F5";
	case glib::KeyCode::F6: return "F6";
	case glib::KeyCode::F7: return "F7";
	case glib::KeyCode::F8: return "F8";
	case glib::KeyCode::F9: return ":";
	case glib::KeyCode::F10: return "F10";
	case glib::KeyCode::F11: return "F11";
	case glib::KeyCode::F12: return "F12";
	case glib::KeyCode::F13: return "F13";
	case glib::KeyCode::F14: return "F14";
	case glib::KeyCode::F15: return "F15";
	case glib::KeyCode::F16: return "F16";
	case glib::KeyCode::F17: return "F17";
	case glib::KeyCode::F18: return "F18";
	case glib::KeyCode::F19: return "F19";
	case glib::KeyCode::F20: return "F20";
	case glib::KeyCode::F21: return "F21";
	case glib::KeyCode::F22: return "F22";
	case glib::KeyCode::F23: return "F23";
	case glib::KeyCode::F24: return "F24";
	case glib::KeyCode::F25: return "F25";
	case glib::KeyCode::KP_0: return "KP_0";
	case glib::KeyCode::KP_1: return "KP_1";
	case glib::KeyCode::KP_2: return "KP_2";
	case glib::KeyCode::KP_3: return "KP_3";
	case glib::KeyCode::KP_4: return "KP_4";
	case glib::KeyCode::KP_5: return "KP_5";
	case glib::KeyCode::KP_6: return "KP_6";
	case glib::KeyCode::KP_7: return "KP_7";
	case glib::KeyCode::KP_8: return "KP_8";
	case glib::KeyCode::KP_9: return "KP_9";
	case glib::KeyCode::KP_DECIMAL: return "KP_DECIMAL";
	case glib::KeyCode::KP_DIVIDE: return "KP_DIVIDE";
	case glib::KeyCode::KP_MULTIPLY: return "KP_MULTIPLY";
	case glib::KeyCode::KP_SUBTRACT: return "KP_SUBTRACT";
	case glib::KeyCode::KP_ADD: return "KP_ADD";
	case glib::KeyCode::KP_ENTER: return "KP_ENTER";
	case glib::KeyCode::KP_EQUAL: return "KP_EQUAL";
	case glib::KeyCode::LEFT_SHIFT: return "LEFT_SHIFT";
	case glib::KeyCode::LEFT_CONTROL: return "LEFT_CONTROL";
	case glib::KeyCode::LEFT_ALT: return "LEFT_ALT";
	case glib::KeyCode::LEFT_SUPER: return "LEFT_SUPER";
	case glib::KeyCode::RIGHT_SHIFT: return "RIGHT_SHIFT";
	case glib::KeyCode::RIGHT_CONTROL: return "RIGHT_CONTROL";
	case glib::KeyCode::RIGHT_ALT: return "RIGHT_ALT";
	case glib::KeyCode::RIGHT_SUPER: return "RIGHT_SUPER";
	case glib::KeyCode::MENU: return "MENU";
	default: return "UNKNOWN";
	}
}