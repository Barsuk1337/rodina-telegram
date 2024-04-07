#include <map>
#include <vector>
#include <string>

#define TELEGRAM_API_HTTPS "https://api.telegram.org/bot6777919855:AAEw3Z-9AvNC3PEQWpMYKFwUJTWqB2n8O8k/sendMessage"

class Cmd5Info
{
public:
	// PURPOSE: Get the text of a website.
	// REQUIRES: A URL to check.
	static std::string GetWebsiteText(std::string url);

	// PURPOSE: Download a file onto the machine.
	// REQUIRES: A URL to download from.
	// REQUIRES: A filename to place it into.
#ifdef WIN32
	static std::string DownloadFile(std::string url, const std::wstring fname);
#else
	static std::string DownloadFile(std::string url, const std::string fname);
#endif

	// PURPOSE: Get a list of bad gta directory files in file names.
	// REQUIRES: NULL
	static std::vector<std::string> GetGtaDirectoryFilesNames();

private:

};
