#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstdint>
#include <iostream>
#include <vector>
#include <cstring>
#include <optional>
#include <thread>
#include <chrono>
#include <ApplicationServices/ApplicationServices.h>
#include "playback.h"
namespace py = pybind11;

constexpr uint8_t MAJOR_FMT_VERSION = 1; // CHANGE IF YOU ALSO CHANGE THIS VARIABLE IN recorder.py
constexpr uint8_t EARLIEST_SUPPORTED_FMT_VERSION = 1; // LEAVE THIS ALONE UNLESS YOU MAKE BREAKING CHANGES AND CANT READ OLDER STUFF SOMEHOW
constexpr char FILE_HEADER_ID[4] = {'N','P','R','M'}; // DONT CHANGE THIS ONE

constexpr size_t sizeof_uint8_t = sizeof(uint8_t);
constexpr size_t sizeof_uint16_t = sizeof(uint16_t);
constexpr size_t sizeof_int16_t = sizeof(int16_t);
constexpr size_t sizeof_uint64_t = sizeof(uint64_t);

void keyStatus(uint16_t vk_code, bool status) {
	CGEventRef keyStroke = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)vk_code, status);
	CGEventPost(kCGHIDEventTap, keyStroke);
	CFRelease(keyStroke);
}

void moveMouseAbsolute(uint16_t x, uint16_t y) {
	CGPoint destination = CGPointMake(x, y);
	CGEventRef motion = CGEventCreateMouseEvent(NULL,kCGEventMouseMoved,destination,kCGMouseButtonLeft);
	CGEventPost(kCGHIDEventTap, motion);
	CFRelease(motion);
}

std::pair<bool, uint8_t> ensureValidHeaders(std::vector<uint8_t>& e_bytearray) {
	uint8_t version = 0;
	if (e_bytearray.size() < 5 || std::memcmp(e_bytearray.data(),FILE_HEADER_ID,4) != 0) {
		return {false, version};
	} else {
		std::memcpy(&version,e_bytearray.data()+4,sizeof(version));
		if (version < EARLIEST_SUPPORTED_FMT_VERSION || version > MAJOR_FMT_VERSION) {
			return {false, version};
		} else {
			return {true,version};
		}
	}
}

std::pair<std::vector<EventPacket>, str> CompileEventArray(std::vector<uint8_t>& e_bytearray) {
	std::pair<bool, uint8_t> headerInfo = ensureValidHeaders(e_bytearray);
	std::vector<EventPacket> eventList;
	if (!headerInfo.first) {
		std::cerr << "nprisma: bad fileheader, found version " << int(headerInfo.second) << std::endl;
		return {eventList,"Failed to parse event array: Bad file header or incompatible version."}; 
	}
	for (size_t cur = 5; cur < e_bytearray.size();) {
		EventPacket e;
		std::memcpy(&e.timestamp, e_bytearray.data()+cur, sizeof_uint64_t);
		cur += sizeof_uint64_t;
		std::memcpy(&e.event, e_bytearray.data()+cur, sizeof_uint8_t);
		cur += sizeof_uint8_t;
		switch (e.event) {
			case Events::KEY_DOWN:
			case Events::KEY_UP:
				uint16_t vk;
				std::memcpy(&vk, e_bytearray.data()+cur, sizeof_uint16_t);
				cur += sizeof_uint16_t;
				e.payload.push_back(static_cast<int>(vk));
				break;

			case Events::MOUSE_DOWN:
			case Events::MOUSE_UP:
				uint8_t button;
				uint16_t x;
				uint16_t y;
				std::memcpy(&button, e_bytearray.data()+cur, sizeof_uint8_t);
				cur += sizeof_uint8_t;
				std::memcpy(&x, e_bytearray.data()+cur, sizeof_uint16_t);
				cur += sizeof_uint16_t;
				std::memcpy(&y, e_bytearray.data()+cur, sizeof_uint16_t);
				cur += sizeof_uint16_t;
				e.payload.push_back(static_cast<int>(button));
				e.payload.push_back(static_cast<int>(x));
				e.payload.push_back(static_cast<int>(y));
				break;

			case Events::MOUSE_MOVE_ABSOLUTE:
				uint16_t x;
				uint16_t y;
				std::memcpy(&x, e_bytearray.data()+cur, sizeof_uint16_t);
				cur += sizeof_uint16_t;
				std::memcpy(&y, e_bytearray.data()+cur, sizeof_uint16_t);
				cur += sizeof_uint16_t;
				e.payload.push_back(static_cast<int>(x));
				e.payload.push_back(static_cast<int>(y));
				break;

			case Events::MOUSE_MOVE_RELATIVE:
				break; // TO BE IMPLEMENTED LATER

			case Events::MOUSE_SCROLL:
				uint16_t x;
				uint16_t y;
				int16_t dx;
				int16_t dy;
				std::memcpy(&x, e_bytearray.data()+cur, sizeof_uint16_t);
				cur += sizeof_uint16_t;
				std::memcpy(&y, e_bytearray.data()+cur, sizeof_uint16_t);
				cur += sizeof_uint16_t;
				std::memcpy(&dx, e_bytearray.data()+cur, sizeof_int16_t);
				cur += sizeof_int16_t;
				std::memcpy(&dy, e_bytearray.data()+cur, sizeof_int16_t);
				cur += sizeof_int16_t;
				e.payload.push_back(static_cast<int>(x));
				e.payload.push_back(static_cast<int>(y));
				e.payload.push_back(static_cast<int>(dx));
				e.payload.push_back(static_cast<int>(dy));
				break;

			default:
				break;
		}
		eventList.push_back(EventPacket);
	}
	return {eventList,""};
}

void PlayEventList(std::vector<EventPacket> eventList) {
	auto start = std::chrono::high_resolution_clock::now();
	uint64_t lastTimestamp = 0;
	for (EventPacket e : eventList) {
		uint64_t deltaNs = e.timestamp - lastTimestamp;
		auto insertTime = start + std::chrono::nanoseconds(e.timestamp);
		lastTimestamp = e.timestamp;
		std::this_thread::sleep_for(std::chrono::nanoseconds(deltaNs)-std::chrono::nanoseconds(200));
		while (std::chrono::high_resolution_clock::now() < insertTime) {
			// intentionally do nothing
		}
		
	}
	
}

PYBIND11_MODULE(playback, m) {
    m.def("hello", &hello, "says hi lol");
}