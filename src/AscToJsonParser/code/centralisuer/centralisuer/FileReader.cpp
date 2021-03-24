#include "stdafx.h"
#include "FileReader.h"

#include <ShlObj_core.h>
#include <streambuf>
#include <fstream>

#include <iostream>

FileReader::FileReader(Tube::TubeMultiplexe* tube) : Reader(tube)
{
	wchar_t path[2048];
	int error = SHGetFolderPathW(0, CSIDL_PERSONAL, nullptr, 0, path);
	int position = 0;
	int c_position = 0;
	const char* folder = "\\Santoline\\Santoline_Communication";
	while (path[position] != '\0') ++position;
	while (folder[c_position] != '\0') path[position + c_position] = folder[c_position++];
	path[position + c_position] = '\0';
	std::wstring tampon(path);
	m_filename = std::string(tampon.begin(), tampon.end());
}


FileReader::~FileReader()
{
}

std::string FileReader::read() 
{
	std::string tube_read = m_tube->read_str();
	if ( tube_read.size() > 0 && tube_read[0] == 'f') 
	{
		std::ifstream file(m_filename);
		std::string file_content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
		file.close();
		std::ofstream emptyer;
		emptyer.open(m_filename, std::ofstream::out | std::ofstream::trunc);
		emptyer.close();
		m_tube->write("lecture finis");
		return file_content;
	}
	else 
	{
		return read();
	}
}

void FileReader::write(std::string message)
{
	std::ofstream file(m_filename);
	file << message;
	file.close();
	m_tube->write("fichier ecrit ");
	m_tube->read();
}