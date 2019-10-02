#ifndef VIRTDEVS_DEVICE_H
#define VIRTDEVS_DEVICE_H

#include <vector>
#include <string>

namespace virtdevs 
{
	enum class Status 
	{
		Ok,
		Alarm
	};

	class device 
	{
		Status status = Status::Alarm;
		
		std::string status_string;
		std::vector<device *> supervisors;
		std::vector<device *> depends;
		
		int alarmed_deps;

		std::string m_name;
		
	protected:
		void recursive_alarm(device * source, std::string text);
		void recursive_allgood();

	public:
		device(){}

		std::string& name() { return m_name; }
		const std::string& name() const { return m_name; }

		void add_depend(device * dev);
		void alarm(std::string data);
		void allgood();
	};
}

#endif