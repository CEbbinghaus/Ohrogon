﻿#pragma once
#include <glfw3.h>
#include <stdio.h>
#include <memory>

enum class KeyCode : int {
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
	LAST = GLFW_KEY_LAST + 1
};

#define KeyArraySize GLFW_KEY_LAST + 1



class Keyboard{

	class KeyArray{
		bool data[KeyArraySize];

	  public:
		void Set(int i, bool value){
			data[i] = value;
		}

		bool operator[](int i){
			return data[i];
		}
		
		bool operator[](char key){
			if(key > 96 && key < 123)
				return data[key - 32];

			return data[key];
		}

		bool operator[](KeyCode key){
			return data[(int)key];
		}

		bool* operator&(){
			return data;
		}
	};

	static KeyArray data;
	static KeyArray altdata;

	static KeyArray pressed;
	static KeyArray released;

	static bool ctrlKey;
	static bool shiftKey;
	static bool altKey;

	static GLFWwindow* window;

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

		data.Set(key, (bool)action);

		ctrlKey = mods & GLFW_MOD_CONTROL;
		altKey = mods & GLFW_MOD_ALT;
		shiftKey = mods & GLFW_MOD_SHIFT;
	}

	static void SwapArrays(){
		memcpy(&altdata, &data, KeyArraySize * sizeof(bool));
	}

public:

	static void Update(){
		auto& Keydata = data;
		auto& Keyaltdata = altdata;
		auto& Keypressed = pressed;
		auto& Keyreleased = released;

		for(int i = 0; i < KeyArraySize; ++i){
			if(data[i] == altdata[i]){
				pressed.Set(i, false);
				released.Set(i, false);
			}else{
				if(data[i]){
					pressed.Set(i, true);
					released.Set(i, false);
				} else{
					pressed.Set(i, false);
					released.Set(i, true);
				}
			}
		}

		SwapArrays();
	}

	static void RegisterKeyboard(GLFWwindow* glWindow) {

		window = glWindow;

		glfwSetKeyCallback(window, &(Keyboard::keyCallback));
	}

	static bool CtrlKey(){
		return ctrlKey;
	}
	static bool AltKey(){
		return altKey;
	}
	static bool ShiftKey(){
		return shiftKey;
	}

	static bool GetKey(KeyCode key) {
		return data[(int)key];
	}
	static bool GetKeyDown(KeyCode key) {
		return pressed[(int)key];
	}
	static bool GetKeyUp(KeyCode key) {
		return released[(int)key];
	}
};

Keyboard::KeyArray Keyboard::data;
Keyboard::KeyArray  Keyboard::altdata;

Keyboard::KeyArray  Keyboard::pressed;
Keyboard::KeyArray  Keyboard::released;

bool Keyboard::ctrlKey = false;
bool Keyboard::shiftKey = false;
bool Keyboard::altKey = false;

GLFWwindow* Keyboard::window = nullptr;