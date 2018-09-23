#include "BusDevice.h"

namespace exploringBB {

BusDevice::BusDevice(unsigned int bus, unsigned int device) {
	this->bus = bus;
	this->device = device;
	this->file=-1;
}

BusDevice::~BusDevice() {}

} /* namespace exploringBB */
