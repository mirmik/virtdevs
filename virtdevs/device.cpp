#include "device.h"

#include <assert.h>
#include <mutex>

static std::recursive_mutex mtx;

void virtdevs::device::recursive_alarm(device * source, std::string text)
{
	std::lock_guard<std::recursive_mutex> lock(mtx);

	status_string = source->name() + ":" + text;
	status = Status::Alarm;

	for (auto s : supervisors)
	{
		s->alarmed_deps++;

		s->recursive_alarm(source, text);
	}
}

void virtdevs::device::recursive_allgood()
{
	std::lock_guard<std::recursive_mutex> lock(mtx);

	status = Status::Ok;

	for (auto s : supervisors)
	{
		s->alarmed_deps--;
		assert(s->alarmed_deps >= 0);

		if (s->alarmed_deps == 0)
			s->recursive_allgood();
	}
}

void virtdevs::device::add_depend(device * dev)
{
	std::lock_guard<std::recursive_mutex> lock(mtx);

	depends.push_back(dev);
	dev->supervisors.push_back(this);

	if (dev->status == Status::Ok)
	{}
	else
		alarmed_deps++;
}

void virtdevs::device::alarm(std::string data)
{
	std::lock_guard<std::recursive_mutex> lock(mtx);

	if (status == Status::Alarm && data == status_string)
		return;

	else
		recursive_alarm(this, data);
}

void virtdevs::device::allgood()
{
	std::lock_guard<std::recursive_mutex> lock(mtx);

	if (status == Status::Ok)
		return;

	else
		recursive_allgood();
}
