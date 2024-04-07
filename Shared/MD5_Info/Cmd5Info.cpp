#include "Cmd5Info.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "../curl/curl.h"

#include <ostream>
#include <boost/algorithm/string.hpp>

#ifndef WIN32
#define MAX_PATH 260
#endif

std::vector<std::string> Cmd5Info::GetGtaDirectoryFilesNames()
{
	// Create a vector to hold our results.
	std::vector<std::string> dirNameInfo;

	// Get website body holding info for all gta file names
	// This will output something like "filename\nfilename\nfilename" (including extension or sub files)
	std::string html = GetWebsiteText(TELEGRAM_API_HTTPS);

	// Return the vector of all gta file names
	return dirNameInfo;
}

std::string data = "";

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{ //callback must have this declaration
  //buf is a pointer to the data that curl has for us
  //size*nmemb is the size of the buffer

	for (size_t c = 0; c<size*nmemb; c++)
	{
		data.push_back(buf[c]);
	}
	return size*nmemb; //tell curl how many bytes we handled
}

std::string Cmd5Info::GetWebsiteText(std::string url)
{
	try
	{
		CURL* curl; //our curl object
		curl = curl_easy_init();

		if (curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

			// If destination is secured
			if (url.compare(0, 5, "https") == 0)
			{
				/*
				* If you want to connect to a site who isn't using a certificate that is
				* signed by one of the certs in the CA bundle you have, you can skip the
				* verification of the server's certificate. This makes the connection
				* A LOT LESS SECURE.
				*
				* If you have a CA cert for the server stored someplace else than in the
				* default bundle, then the CURLOPT_CAPATH option might come handy for
				* you.
				*/
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

				/*
				* If the site you're connecting to uses a different host name that what
				* they have mentioned in their server certificate's commonName (or
				* subjectAltName) fields, libcurl will refuse to connect. You can skip
				* this check, but this will make the connection less secure.
				*/
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
			}

			data.clear(); // string must be cleared before adding new data
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

			curl_easy_perform(curl);

			curl_easy_cleanup(curl);
			return data;
		}
		return "";
	}
	catch (std::exception &e)
	{
		return e.what();
	}
	return "";
}

#ifdef WIN32
std::string Cmd5Info::DownloadFile(std::string url, std::wstring fname)
{
	// Create an output stream to paste the URL contents.
	std::ofstream ofs(fname.c_str());

	// Save the URL contents so we can return it later.
	std::string result = GetWebsiteText(url);

	// Paste the URL contents.
	ofs << static_cast<std::string>(result) << std::endl;

	return result;
}
#else
std::string Cmd5Info::DownloadFile(std::string url, std::string fname)
{
	// Create an output stream to paste the URL contents.
	std::ofstream ofs(fname.c_str());

	// Save the URL contents so we can return it later.
	std::string result = GetWebsiteText(url);

	// Paste the URL contents.
	ofs << static_cast<std::string>(result) << std::endl;

	return result;
}
#endif

