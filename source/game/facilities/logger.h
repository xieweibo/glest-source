// This file is part of Glest <https://github.com/Glest>
//
// Copyright (C) 2018  The Glest team
//
// Glest is a fork of MegaGlest <https://megaglest.org/>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>

#ifndef _SHARED_UTIL_LOGGER_H_
#define _SHARED_UTIL_LOGGER_H_

#ifdef WIN32
#include <winsock2.h>
#include <winsock.h>
#endif

#include <string>
#include <deque>

#include "texture.h"
#include "properties.h"
#include "components.h"
#include "leak_dumper.h"

using std::string;
using std::deque;
using Shared::Graphics::Texture2D;
using Shared::Util::Properties;

namespace Game {
	// =====================================================
	//	class Logger
	//
	/// Interface to write log files
	// =====================================================

	class Logger {
	private:
		static const int logLineCount;

	private:
		typedef deque<string> Strings;

	private:
		string fileName;
		string state;
		string subtitle;
		string current;
		Texture2D *loadingTexture;
		Properties gameHints;
		Properties gameHintsTranslation;
		string gameHintToShow;
		int progress;
		bool showProgressBar;

		string statusText;
		bool cancelSelected;
		GraphicButton buttonCancel;
		Vec4f displayColor;
		GraphicButton buttonNextHint;

	private:
		Logger();
		~Logger();

	public:
		static Logger & getInstance();

		//void setMasterserverMode(bool value) { masterserverMode = value; }

		void setFile(const string &fileName) {
			this->fileName = fileName;
		}
		void setState(const string &state) {
			this->state = state;
		}
		void setSubtitle(const string &subtitle) {
			this->subtitle = subtitle;
		}
		void setProgress(int value) {
			this->progress = value;
		}
		int getProgress() const {
			return progress;
		}
		void showProgress() {
			showProgressBar = true;
		}
		void hideProgress() {
			showProgressBar = false;
		}

		void add(const string str, bool renderScreen = false, const string statusText = "");
		void loadLoadingScreen(string filepath);
		void loadGameHints(string filePathEnglish, string filePathTranslation, bool clearList);
		void renderLoadingScreen();

		bool getCancelLoading() const {
			return cancelSelected;
		}
		void setCancelLoading(bool value) {
			cancelSelected = value;
		}
		void handleMouseClick(int x, int y);
		void clearHints();

		void clear();

	private:
		void showNextHint();

	};

} //end namespace

#endif
