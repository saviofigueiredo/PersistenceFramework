#ifndef DATETIMEHELPER_H
#define DATETIMEHELPER_H

#include <ctime>
#include <string>

namespace PersistenceFramework
{
	class DateTimeHelper
	{
	public:
		static time_t ConvertDateTime(const char* buff)
		{
			int yy, mm, dd, hour = 0, min = 0, sec = 0;
			struct tm when;

			if (strchr(buff, ':') != NULL)
				sscanf_s(buff, "%d/%d/%d %d:%d:%d", &dd, &mm, &yy, &hour, &min, &sec);
			else
				sscanf_s(buff, "%d/%d/%d", &dd, &mm, &yy);
			yy -= 1900; // year 2000 compliant

			when.tm_year = yy;
			when.tm_mon = mm - 1;
			when.tm_mday = dd;

			when.tm_hour = hour;
			when.tm_min = min;
			when.tm_sec = sec;

			return mktime(&when);
		}

		static std::string ConverteDateTimeToString(time_t dateTime)
		{
			if (dateTime < 0)
				return "";

			struct tm * ts = localtime(&dateTime);
			char szTime[24] = { 0 };
			strftime(szTime, 32, "%d/%m/%Y %H:%M:%S", ts);
			std::string strData = szTime;

			return strData;
		}
	};
}

#endif