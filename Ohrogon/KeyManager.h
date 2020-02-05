#pragma once
#include <glfw3.h>
#include <unordered_map>

enum class KeyCode {
	UNKNOWN = GLFW_KEY_UNKNOWN,
	SPACE = GLFW_KEY_SPACE,
	APOSTROPHE = GLFW_KEY_APOSTROPHE,
	COMMA = GLFW_KEY_COMMA,
	MINUS = GLFW_KEY_MINUS,
	PERIOD = GLFW_KEY_PERIOD,
	SLASH = GLFW_KEY_SLASH,
	Number0 = GLFW_KEY_0,
	Number1 = GLFW_KEY_1,
	Number2 = GLFW_KEY_2,
	Number3 = GLFW_KEY_3,
	Number4 = GLFW_KEY_4,
	Number5 = GLFW_KEY_5,
	Number6 = GLFW_KEY_6,
	Number7 = GLFW_KEY_7,
	Number8 = GLFW_KEY_8,
	Number9 = GLFW_KEY_9,
	SEMICOLON = GLFW_KEY_SEMICOLON,
	EQUAL = GLFW_KEY_EQUAL,
	A = GLFW_KEY_A,
	B = GLFW_KEY_B,
	C = GLFW_KEY_C,
	D = GLFW_KEY_D,
	E = GLFW_KEY_E,
	F = GLFW_KEY_F,
	G = GLFW_KEY_G,
	H = GLFW_KEY_H,
	I = GLFW_KEY_I,
	J = GLFW_KEY_J,
	K = GLFW_KEY_K,
	L = GLFW_KEY_L,
	M = GLFW_KEY_M,
	N = GLFW_KEY_N,
	O = GLFW_KEY_O,
	P = GLFW_KEY_P,
	Q = GLFW_KEY_Q,
	R = GLFW_KEY_R,
	S = GLFW_KEY_S,
	T = GLFW_KEY_T,
	U = GLFW_KEY_U,
	V = GLFW_KEY_V,
	W = GLFW_KEY_W,
	X = GLFW_KEY_X,
	Y = GLFW_KEY_Y,
	Z = GLFW_KEY_Z,
	LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET,
	BACKSLASH = GLFW_KEY_BACKSLASH,
	RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET,
	GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,
	WORLD_1 = GLFW_KEY_WORLD_1,
	WORLD_2 = GLFW_KEY_WORLD_2,
	ESCAPE = GLFW_KEY_ESCAPE,
	ENTER = GLFW_KEY_ENTER,
	TAB = GLFW_KEY_TAB,
	BACKSPACE = GLFW_KEY_BACKSPACE,
	INSERT = GLFW_KEY_INSERT,
	DELETE_Key = GLFW_KEY_DELETE,
	RIGHT = GLFW_KEY_RIGHT,
	LEFT = GLFW_KEY_LEFT,
	DOWN = GLFW_KEY_DOWN,
	UP = GLFW_KEY_UP,
	PAGE_UP = GLFW_KEY_PAGE_UP,
	PAGE_DOWN = GLFW_KEY_PAGE_DOWN,
	HOME = GLFW_KEY_HOME,
	END = GLFW_KEY_END,
	CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
	SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
	NUM_LOCK = GLFW_KEY_NUM_LOCK,
	PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,
	PAUSE = GLFW_KEY_PAUSE,
	F1 = GLFW_KEY_F1,
	F2 = GLFW_KEY_F2,
	F3 = GLFW_KEY_F3,
	F4 = GLFW_KEY_F4,
	F5 = GLFW_KEY_F5,
	F6 = GLFW_KEY_F6,
	F7 = GLFW_KEY_F7,
	F8 = GLFW_KEY_F8,
	F9 = GLFW_KEY_F9,
	F10 = GLFW_KEY_F10,
	F11 = GLFW_KEY_F11,
	F12 = GLFW_KEY_F12,
	F13 = GLFW_KEY_F13,
	F14 = GLFW_KEY_F14,
	F15 = GLFW_KEY_F15,
	F16 = GLFW_KEY_F16,
	F17 = GLFW_KEY_F17,
	F18 = GLFW_KEY_F18,
	F19 = GLFW_KEY_F19,
	F20 = GLFW_KEY_F20,
	F21 = GLFW_KEY_F21,
	F22 = GLFW_KEY_F22,
	F23 = GLFW_KEY_F23,
	F24 = GLFW_KEY_F24,
	F25 = GLFW_KEY_F25,
	KP_0 = GLFW_KEY_KP_0,
	KP_1 = GLFW_KEY_KP_1,
	KP_2 = GLFW_KEY_KP_2,
	KP_3 = GLFW_KEY_KP_3,
	KP_4 = GLFW_KEY_KP_4,
	KP_5 = GLFW_KEY_KP_5,
	KP_6 = GLFW_KEY_KP_6,
	KP_7 = GLFW_KEY_KP_7,
	KP_8 = GLFW_KEY_KP_8,
	KP_9 = GLFW_KEY_KP_9,
	KP_DECIMAL = GLFW_KEY_KP_DECIMAL,
	KP_DIVIDE = GLFW_KEY_KP_DIVIDE,
	KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
	KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
	KP_ADD = GLFW_KEY_KP_ADD,
	KP_ENTER = GLFW_KEY_KP_ENTER,
	KP_EQUAL = GLFW_KEY_KP_EQUAL,
	LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
	LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,
	LEFT_ALT = GLFW_KEY_LEFT_ALT,
	LEFT_SUPER = GLFW_KEY_LEFT_SUPER,
	RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,
	RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,
	RIGHT_ALT = GLFW_KEY_RIGHT_ALT,
	RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER,
	MENU = GLFW_KEY_MENU,
	LAST = GLFW_KEY_LAST
};


class KeyManager{
	//std::unordered_map<int, bool> data;

	static KeyManager* Instance;

	KeyManager() {
		/*this->data = &std::unordered_map<int, bool>({
			{GLFW_KEY_UNKNOWN, false},
			{GLFW_KEY_SPACE, false},
			{GLFW_KEY_APOSTROPHE, false},
			{GLFW_KEY_COMMA, false},
			{GLFW_KEY_MINUS, false},
			{GLFW_KEY_PERIOD, false},
			{GLFW_KEY_SLASH, false},
			{GLFW_KEY_0, false},
			{GLFW_KEY_1, false},
			{GLFW_KEY_2, false},
			{GLFW_KEY_3, false},
			{GLFW_KEY_4, false},
			{GLFW_KEY_5, false},
			{GLFW_KEY_6, false},
			{GLFW_KEY_7, false},
			{GLFW_KEY_8, false},
			{GLFW_KEY_9, false},
			{GLFW_KEY_SEMICOLON, false},
			{GLFW_KEY_EQUAL, false},
			{GLFW_KEY_A, false},
			{GLFW_KEY_B, false},
			{GLFW_KEY_C, false},
			{GLFW_KEY_D, false},
			{GLFW_KEY_E, false},
			{GLFW_KEY_F, false},
			{GLFW_KEY_G, false},
			{GLFW_KEY_H, false},
			{GLFW_KEY_I, false},
			{GLFW_KEY_J, false},
			{GLFW_KEY_K, false},
			{GLFW_KEY_L, false},
			{GLFW_KEY_M, false},
			{GLFW_KEY_N, false},
			{GLFW_KEY_O, false},
			{GLFW_KEY_P, false},
			{GLFW_KEY_Q, false},
			{GLFW_KEY_R, false},
			{GLFW_KEY_S, false},
			{GLFW_KEY_T, false},
			{GLFW_KEY_U, false},
			{GLFW_KEY_V, false},
			{GLFW_KEY_W, false},
			{GLFW_KEY_X, false},
			{GLFW_KEY_Y, false},
			{GLFW_KEY_Z, false},
			{GLFW_KEY_LEFT_BRACKET, false},
			{GLFW_KEY_BACKSLASH, false},
			{GLFW_KEY_RIGHT_BRACKET, false},
			{GLFW_KEY_GRAVE_ACCENT, false},
			{GLFW_KEY_WORLD_1, false},
			{GLFW_KEY_WORLD_2, false},
			{GLFW_KEY_ESCAPE, false},
			{GLFW_KEY_ENTER, false},
			{GLFW_KEY_TAB, false},
			{GLFW_KEY_BACKSPACE, false},
			{GLFW_KEY_INSERT, false},
			{GLFW_KEY_DELETE, false},
			{GLFW_KEY_RIGHT, false},
			{GLFW_KEY_LEFT, false},
			{GLFW_KEY_DOWN, false},
			{GLFW_KEY_UP, false},
			{GLFW_KEY_PAGE_UP, false},
			{GLFW_KEY_PAGE_DOWN, false},
			{GLFW_KEY_HOME, false},
			{GLFW_KEY_END, false},
			{GLFW_KEY_CAPS_LOCK, false},
			{GLFW_KEY_SCROLL_LOCK, false},
			{GLFW_KEY_NUM_LOCK, false},
			{GLFW_KEY_PRINT_SCREEN, false},
			{GLFW_KEY_PAUSE, false},
			{GLFW_KEY_F1, false},
			{GLFW_KEY_F2, false},
			{GLFW_KEY_F3, false},
			{GLFW_KEY_F4, false},
			{GLFW_KEY_F5, false},
			{GLFW_KEY_F6, false},
			{GLFW_KEY_F7, false},
			{GLFW_KEY_F8, false},
			{GLFW_KEY_F9, false},
			{GLFW_KEY_F10, false},
			{GLFW_KEY_F11, false},
			{GLFW_KEY_F12, false},
			{GLFW_KEY_F13, false},
			{GLFW_KEY_F14, false},
			{GLFW_KEY_F15, false},
			{GLFW_KEY_F16, false},
			{GLFW_KEY_F17, false},
			{GLFW_KEY_F18, false},
			{GLFW_KEY_F19, false},
			{GLFW_KEY_F20, false},
			{GLFW_KEY_F21, false},
			{GLFW_KEY_F22, false},
			{GLFW_KEY_F23, false},
			{GLFW_KEY_F24, false},
			{GLFW_KEY_F25, false},
			{GLFW_KEY_KP_0, false},
			{GLFW_KEY_KP_1, false},
			{GLFW_KEY_KP_2, false},
			{GLFW_KEY_KP_3, false},
			{GLFW_KEY_KP_4, false},
			{GLFW_KEY_KP_5, false},
			{GLFW_KEY_KP_6, false},
			{GLFW_KEY_KP_7, false},
			{GLFW_KEY_KP_8, false},
			{GLFW_KEY_KP_9, false},
			{GLFW_KEY_KP_DECIMAL, false},
			{GLFW_KEY_KP_DIVIDE, false},
			{GLFW_KEY_KP_MULTIPLY, false},
			{GLFW_KEY_KP_SUBTRACT, false},
			{GLFW_KEY_KP_ADD, false},
			{GLFW_KEY_KP_ENTER, false},
			{GLFW_KEY_KP_EQUAL, false},
			{GLFW_KEY_LEFT_SHIFT, false},
			{GLFW_KEY_LEFT_CONTROL, false},
			{GLFW_KEY_LEFT_ALT, false},
			{GLFW_KEY_LEFT_SUPER, false},
			{GLFW_KEY_RIGHT_SHIFT, false},
			{GLFW_KEY_RIGHT_CONTROL, false},
			{GLFW_KEY_RIGHT_ALT, false},
			{GLFW_KEY_RIGHT_SUPER, false},
			{GLFW_KEY_MENU, false},
			{GLFW_KEY_LAST, false}
		});
		*/
		Instance = this;
	}

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if(Instance == nullptr)return;

		//Instance->data[key] = (action == GLFW_PRESS ? true : false);
	}

public:

	static KeyManager& CreateKeyManager(GLFWwindow* window) {
		if (Instance != nullptr) {
			return *Instance;
		}

		Instance = &KeyManager();

		glfwSetKeyCallback(window, &KeyManager::keyCallback);
	}

	bool operator[](char key) {
		return false;
		//if (!data.at((KeyCode)key))
		//	throw std::exception("Could Not Find Key Code");

		//return data[(KeyCode)key];
	}
	
	bool operator[](KeyCode key) {
		return false;
		//return data[key];
	}

	static bool Get(char key) {
		if (Instance == nullptr)return false;

		return false;
		//if (!Instance->data.at((KeyCode)key))
		//	throw std::exception("Could Not Find Key Code");

		//return Instance->data[(KeyCode)key];
	}

	static bool Get(KeyCode key) {
		if (Instance == nullptr)return false;

		return false;
		//return Instance->data[key];
	}
};

