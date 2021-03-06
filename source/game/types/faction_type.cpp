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

#include "faction_type.h"

#include "logger.h"
#include "util.h"
#include "xml_parser.h"
#include "tech_tree.h"
#include "resource.h"
#include "renderer.h"
#include "platform_util.h"
#include "game_util.h"
#include "conversion.h"
#include "leak_dumper.h"

using namespace Shared::Util;
using namespace Shared::Xml;

namespace Game {
	// ======================================================
	//          Class FactionType
	// ======================================================

	FactionType::FactionType() {
		music = NULL;
		personalityType = fpt_Normal;
		isLinked = false;
		healthbarheight = -100.0f;
		healthbarthickness = 0.11f;
		healthbarVisible = hbvUndefined;
		healthbarBorderTextureEnabled = false;
		healthbarBackgroundTextureEnabled = false;
		healthbarLineBorder = true;
		healthbarTexture = NULL;
		healthbarBackgroundTexture = NULL;
		flatParticlePositions = false;
	}

	//load a faction, given a directory
	void FactionType::load(const string & factionName,
		const TechTree * techTree, Checksum * checksum,
		Checksum * techtreeChecksum, std::map < string,
		vector < pair < string,
		string > > >&loadedFileList, bool validationMode) {

		if (SystemFlags::getSystemSettingType(SystemFlags::debugSystem).
			enabled)
			SystemFlags::OutputDebug(SystemFlags::debugSystem,
				"In [%s::%s Line: %d]\n",
				extractFileFromDirectoryPath(__FILE__).
				c_str(), __FUNCTION__, __LINE__);

		string techTreePath = techTree->getPath();
		string techTreeName = techTree->getNameUntranslated();
		string currentPath = "";
		Logger & logger = Logger::getInstance();

		//open xml file
		string path = "";
		XmlTree xmlTree;
		const XmlNode *factionNode;

		//printf("\n>>> factionname=%s\n",factionName.c_str());
		for (bool realFactionPathFound = false; realFactionPathFound == false;) {
			if (logger.getCancelLoading()) {
				return;
			}
			currentPath = techTreePath + "factions/" + factionName;
			endPathWithSlash(currentPath);

			name = lastDir(currentPath);

			// Add special Observer Faction
			//Lang &lang= Lang::getInstance();
			if (name == formatString(GameConstants::OBSERVER_SLOTNAME)) {
				personalityType = fpt_Observer;
			}

			if (personalityType == fpt_Normal) {
				string tmppath = currentPath + factionName + ".xml";
				std::map < string, string > mapExtraTagReplacementValues;
				//              mapExtraTagReplacementValues["$COMMONDATAPATH"] = techTreePath + "/commondata/";
				//printf("current $COMMONDATAPATH = %s\n",mapExtraTagReplacementValues["$COMMONDATAPATH"].c_str());
				XmlTree xmlTree;
				xmlTree.load(tmppath,
					Properties::
					getTagReplacementValues
					(&mapExtraTagReplacementValues));


				const XmlNode *rootNode = xmlTree.getRootNode();

				if (rootNode->getName() == "link") {
					if (SystemFlags::getSystemSettingType(SystemFlags::debugSystem).
						enabled)
						SystemFlags::OutputDebug(SystemFlags::debugSystem,
							"Faction [%s] is a linked faction\n",
							name.c_str());

					isLinked = true;
					const XmlNode *techTreeNode = rootNode->getChild("techtree");
					const string linkedTechTreeName =
						techTreeNode->getAttribute("name")->getRestrictedValue();
					//                      const XmlNode *factionLinkNode= rootNode->getChild("faction");
					//                      string linkedFactionName=factionLinkNode->getAttribute("name")->getRestrictedValue();
					string linkedTechTreePath =
						techTree->findPath(linkedTechTreeName);
					techTreePath = linkedTechTreePath;
					endPathWithSlash(techTreePath);
					techTreeName = linkedTechTreeName;

					string linkedCurrentPath =
						techTreePath + "factions/" + factionName;
					endPathWithSlash(linkedCurrentPath);
					string linkedTmppath = linkedCurrentPath + factionName + ".xml";

					//printf("linkedTmppath [%s] linkedCurrentPath [%s]\n",linkedTmppath.c_str(),linkedCurrentPath.c_str());

					loadedFileList[linkedTmppath].
						push_back(make_pair(linkedCurrentPath, linkedCurrentPath));
					loadedFileList[tmppath].
						push_back(make_pair(currentPath, currentPath));
					if (SystemFlags::getSystemSettingType(SystemFlags::debugSystem).
						enabled)
						SystemFlags::OutputDebug(SystemFlags::debugSystem,
							"techTreePath [%s] techTreeName [%s]\n",
							techTreePath.c_str(),
							techTreeName.c_str());
				} else {
					// stop looking for new path, no more links ...
					//xmlTree.load(tmppath, Properties::getTagReplacementValues(&mapExtraTagReplacementValues));

					loadedFileList[tmppath].
						push_back(make_pair(currentPath, currentPath));

					realFactionPathFound = true;
					//printf("techPath found! %s\n",tmppath.c_str());

					path = tmppath;
				}
			} else {
				break;
			}
		}
		SDL_PumpEvents();
		if (logger.getCancelLoading())
			return;

		char szBuf[8096] = "";
		snprintf(szBuf, 8096,
			Lang::getInstance().
			getString("LogScreenGameLoadingFactionType", "").c_str(),
			formatString(this->getName()).c_str());
		logger.add(szBuf, true);

		if (personalityType == fpt_Normal) {
			if (SystemFlags::getSystemSettingType(SystemFlags::debugSystem).
				enabled)
				SystemFlags::OutputDebug(SystemFlags::debugSystem,
					"Loading faction [%s] currentPath [%s]\n",
					path.c_str(), currentPath.c_str());

			checksum->addFile(path);
			techtreeChecksum->addFile(path);

			// a1) preload units
			//string unitsPath= currentPath + "units/*.";
			string unitsPath = currentPath + "units/";
			vector < string > unitFilenames;
			//findAll(unitsPath, unitFilenames);
			findDirs(unitsPath, unitFilenames, false, false);

			unitTypes.resize(unitFilenames.size());

			for (int i = 0; i < (int) unitTypes.size(); ++i) {
				if (logger.getCancelLoading())
					return;
				string str = currentPath + "units/" + unitFilenames[i];
				unitTypes[i].preLoad(str);

				SDL_PumpEvents();
			}

			// a2) preload upgrades
			//string upgradesPath= currentPath + "upgrades/*.";
			string upgradesPath = currentPath + "upgrades/";
			vector < string > upgradeFilenames;
			//findAll(upgradesPath, upgradeFilenames, false, false);
			findDirs(upgradesPath, upgradeFilenames, false, false);

			upgradeTypes.resize(upgradeFilenames.size());
			for (int i = 0; i < (int) upgradeTypes.size(); ++i) {
				if (logger.getCancelLoading())
					return;
				string str = currentPath + "upgrades/" + upgradeFilenames[i];
				upgradeTypes[i].preLoad(str);

				SDL_PumpEvents();
			}

			// b1) load units
			try {
				int progressBaseValue = logger.getProgress();
				for (int i = 0; i < (int) unitTypes.size(); ++i) {
					if (logger.getCancelLoading())
						return;
					string str = currentPath + "units/" + unitTypes[i].getName();
					try {
						unitTypes[i].loadUnit(i, str, techTree, techTreePath, this,
							checksum, techtreeChecksum, loadedFileList,
							validationMode);
						logger.setProgress(progressBaseValue +
							(int) ((((double) i +
								1.0) /
								(double) unitTypes.size()) *
								100.0 / techTree->getTypeCount()));
						SDL_PumpEvents();
					} catch (game_runtime_error & ex) {
						if (validationMode == false) {
							throw;
						} else {
							SystemFlags::OutputDebug(SystemFlags::debugError,
								"In [%s::%s Line: %d] Error [%s]\n",
								extractFileFromDirectoryPath
								(__FILE__).c_str(), __FUNCTION__,
								__LINE__, ex.what());
						}
					}
				}
			} catch (game_runtime_error & ex) {
				SystemFlags::OutputDebug(SystemFlags::debugError,
					"In [%s::%s Line: %d] Error [%s]\n",
					extractFileFromDirectoryPath(__FILE__).
					c_str(), __FUNCTION__, __LINE__,
					ex.what());
				throw game_runtime_error("Error loading units: " +
					currentPath + "\nMessage: " +
					ex.what(), !ex.wantStackTrace());
			} catch (const exception & e) {
				SystemFlags::OutputDebug(SystemFlags::debugError,
					"In [%s::%s Line: %d] Error [%s]\n",
					extractFileFromDirectoryPath(__FILE__).
					c_str(), __FUNCTION__, __LINE__,
					e.what());
				throw game_runtime_error("Error loading units: " +
					currentPath + "\nMessage: " +
					e.what());
			}

			// b2) load upgrades
			try {
				for (int i = 0; i < (int) upgradeTypes.size(); ++i) {
					if (logger.getCancelLoading())
						return;
					string str =
						currentPath + "upgrades/" + upgradeTypes[i].getName();
					try {
						upgradeTypes[i].load(str, techTree, this, checksum,
							techtreeChecksum, loadedFileList,
							validationMode);
					} catch (game_runtime_error & ex) {
						if (validationMode == false) {
							throw;
						} else {
							SystemFlags::OutputDebug(SystemFlags::debugError,
								"In [%s::%s Line: %d] Error [%s]\n",
								extractFileFromDirectoryPath
								(__FILE__).c_str(), __FUNCTION__,
								__LINE__, ex.what());
						}
					}

					SDL_PumpEvents();
				}
			} catch (const exception & e) {
				SystemFlags::OutputDebug(SystemFlags::debugError,
					"In [%s::%s Line: %d] Error [%s]\n",
					extractFileFromDirectoryPath(__FILE__).
					c_str(), __FUNCTION__, __LINE__,
					e.what());
				throw game_runtime_error("Error loading upgrades: " +
					currentPath + "\n" + e.what());
			}

			string tmppath = currentPath + factionName + ".xml";

			if (SystemFlags::getSystemSettingType(SystemFlags::debugSystem).
				enabled)
				SystemFlags::OutputDebug(SystemFlags::debugSystem,
					"Loading faction xml [%s]\n",
					tmppath.c_str());

			std::map < string, string > mapExtraTagReplacementValues;
			mapExtraTagReplacementValues["$COMMONDATAPATH"] =
				techTreePath + "/commondata/";
			//printf("current $COMMONDATAPATH = %s\n",mapExtraTagReplacementValues["$COMMONDATAPATH"].c_str());
			xmlTree.load(tmppath,
				Properties::
				getTagReplacementValues
				(&mapExtraTagReplacementValues));


			factionNode = xmlTree.getRootNode();
			//read starting resources
			//printf("factionNode->getName()=%s",factionNode->getName().c_str());
			const XmlNode *startingResourcesNode =
				factionNode->getChild("starting-resources");

			startingResources.resize(startingResourcesNode->getChildCount());
			for (int i = 0; i < (int) startingResources.size(); ++i) {
				if (logger.getCancelLoading())
					return;
				const XmlNode *resourceNode =
					startingResourcesNode->getChild("resource", i);
				string name =
					resourceNode->getAttribute("name")->getRestrictedValue();
				int amount = resourceNode->getAttribute("amount")->getIntValue();

				try {
					startingResources[i].init(techTree->getResourceType(name),
						amount);
				} catch (game_runtime_error & ex) {
					if (validationMode == false) {
						throw;
					} else {
						SystemFlags::OutputDebug(SystemFlags::debugError,
							"In [%s::%s Line: %d] Error [%s]\nFor FactionType: %s for StartResource: %s\n",
							extractFileFromDirectoryPath
							(__FILE__).c_str(), __FUNCTION__,
							__LINE__, ex.what(),
							this->name.c_str(), name.c_str());
					}
				}

				SDL_PumpEvents();
			}

			//read starting units
			const XmlNode *startingUnitsNode =
				factionNode->getChild("starting-units");
			for (int i = 0; i < (int) startingUnitsNode->getChildCount(); ++i) {
				if (logger.getCancelLoading())
					return;
				const XmlNode *unitNode = startingUnitsNode->getChild("unit", i);
				string name =
					unitNode->getAttribute("name")->getRestrictedValue();
				int amount = unitNode->getAttribute("amount")->getIntValue();
				startingUnits.
					push_back(PairPUnitTypeInt(getUnitType(name), amount));

				SDL_PumpEvents();
			}

			if (logger.getCancelLoading())
				return;

			//read music
			const XmlNode *musicNode = factionNode->getChild("music");
			bool value = musicNode->getAttribute("value")->getBoolValue();
			if (value) {
				music = new StrSound();
				music->open(musicNode->getAttribute("path")->
					getRestrictedValue(currentPath));
				loadedFileList[musicNode->getAttribute("path")->
					getRestrictedValue(currentPath)].
					push_back(make_pair
					(path,
						musicNode->getAttribute("path")->
						getRestrictedValue()));
			}

			if (factionNode->hasChild("flat-particle-positions")) {
				const XmlNode *node =
					factionNode->getChild("flat-particle-positions");
				flatParticlePositions =
					node->getAttribute("value")->getBoolValue();
			}

			//healthbar
			if (factionNode->hasChild("healthbar")) {
				const XmlNode *healthbarNode = factionNode->getChild("healthbar");
				if (healthbarNode->hasChild("height")) {
					healthbarheight =
						healthbarNode->getChild("height")->getAttribute("value")->
						getFloatValue();
				}
				if (healthbarNode->hasChild("thickness")) {
					healthbarthickness =
						healthbarNode->getChild("thickness")->getAttribute("value")->
						getFloatValue(0.f, 1.f);
				}
				if (healthbarNode->hasChild("visible")) {
					string healthbarVisibleString =
						healthbarNode->getChild("visible")->getAttribute("value")->
						getValue();
					vector < string > v = split(healthbarVisibleString, "|");
					for (int i = 0; i < (int) v.size(); ++i) {
						if (logger.getCancelLoading())
							return;
						string current = trim(v[i]);
						if (current == "always") {
							healthbarVisible = healthbarVisible | hbvAlways;
						} else if (current == "selected") {
							healthbarVisible = healthbarVisible | hbvSelected;
						} else if (current == "ifNeeded") {
							healthbarVisible = healthbarVisible | hbvIfNeeded;
						} else if (current == "off") {
							healthbarVisible = healthbarVisible | hbvOff;
						} else {
							throw game_runtime_error("Unknown Healthbar Visible Option: " + current, true);
						}
					}
				}
				if (healthbarNode->hasChild("borderTexture")) {
					healthbarBorderTextureEnabled =
						healthbarNode->getChild("borderTexture")->
						getAttribute("enabled")->getBoolValue();
					if (healthbarBorderTextureEnabled
						&& healthbarNode->getChild("borderTexture")->
						hasAttribute("path")) {
						healthbarTexture =
							Renderer::getInstance().newTexture2D(rsGame);
						if (healthbarTexture) {
							healthbarTexture->load(healthbarNode->
								getChild("borderTexture")->
								getAttribute("path")->
								getRestrictedValue(currentPath));
						}
						loadedFileList[healthbarNode->getChild("borderTexture")->
							getAttribute("path")->
							getRestrictedValue(currentPath)].
							push_back(make_pair
							(path,
								healthbarNode->getChild("borderTexture")->
								getAttribute("path")->getRestrictedValue()));
					}
				}
				if (healthbarNode->hasChild("backgroundTexture")) {
					healthbarBackgroundTextureEnabled =
						healthbarNode->getChild("backgroundTexture")->
						getAttribute("enabled")->getBoolValue();
					if (healthbarBackgroundTextureEnabled
						&& healthbarNode->getChild("backgroundTexture")->
						hasAttribute("path")) {
						healthbarBackgroundTexture =
							Renderer::getInstance().newTexture2D(rsGame);
						if (healthbarBackgroundTexture) {
							healthbarBackgroundTexture->load(healthbarNode->
								getChild
								("backgroundTexture")->
								getAttribute("path")->
								getRestrictedValue
								(currentPath));
						}
						loadedFileList[healthbarNode->getChild("backgroundTexture")->
							getAttribute("path")->
							getRestrictedValue(currentPath)].
							push_back(make_pair
							(path,
								healthbarNode->getChild("backgroundTexture")->
								getAttribute("path")->getRestrictedValue()));
					}
				}
				if (healthbarNode->hasChild("lineBorder")) {
					healthbarLineBorder =
						healthbarNode->getChild("lineBorder")->
						getAttribute("enabled")->getBoolValue();
				}

			}

			if (factionNode->hasChild("scripts")) {
				const XmlNode *scriptsNode = factionNode->getChild("scripts");

				for (int i = 0; i < (int) scriptsNode->getChildCount(); ++i) {
					if (logger.getCancelLoading())
						return;
					const XmlNode *scriptNode = scriptsNode->getChild(i);
					scripts.push_back(Script(getFunctionName(scriptNode), scriptNode->getText()));
				}
			}

			//read ai behavior
			if (factionNode->hasChild("ai-behavior") == true) {
				const XmlNode *aiNode = factionNode->getChild("ai-behavior");
				if (aiNode->hasAttribute("min-static-resource-count") == true) {
					mapAIBehaviorStaticOverrideValues[aibsvcMinStaticResourceCount] =
						aiNode->getAttribute("min-static-resource-count")->
						getIntValue();
				}

				if (aiNode->hasChild("static-values") == true) {
					const XmlNode *aiNodeUnits = aiNode->getChild("static-values");
					for (int i = 0; i < (int) aiNodeUnits->getChildCount(); ++i) {
						if (logger.getCancelLoading())
							return;
						const XmlNode *unitNode = aiNodeUnits->getChild("static", i);
						AIBehaviorStaticValueCategory type = aibsvcMaxBuildRadius;
						if (unitNode->hasAttribute("type") == true) {
							type =
								static_cast <AIBehaviorStaticValueCategory>
								(unitNode->getAttribute("type")->getIntValue());
						} else {
							type =
								EnumParser <
								AIBehaviorStaticValueCategory >::getEnum(unitNode->
									getAttribute
									("type-name")->
									getValue());
							//printf("Discovered overriden static value for AI, type = %d, value = %d\n",type,value);
						}

						int value = unitNode->getAttribute("value")->getIntValue();
						mapAIBehaviorStaticOverrideValues[type] = value;
						//printf("Discovered overriden static value for AI, type = %d, value = %d\n",type,value);
					}
				}

				if (aiNode->hasChild("worker-units") == true) {
					const XmlNode *aiNodeUnits = aiNode->getChild("worker-units");
					for (int i = 0; i < (int) aiNodeUnits->getChildCount(); ++i) {
						if (logger.getCancelLoading())
							return;
						const XmlNode *unitNode = aiNodeUnits->getChild("unit", i);
						string name =
							unitNode->getAttribute("name")->getRestrictedValue();
						int minimum =
							unitNode->getAttribute("minimum")->getIntValue();

						mapAIBehaviorUnitCategories[aibcWorkerUnits].
							push_back(PairPUnitTypeInt(getUnitType(name), minimum));
					}
				}
				if (aiNode->hasChild("warrior-units") == true) {
					const XmlNode *aiNodeUnits = aiNode->getChild("warrior-units");
					for (int i = 0; i < (int) aiNodeUnits->getChildCount(); ++i) {
						if (logger.getCancelLoading())
							return;
						const XmlNode *unitNode = aiNodeUnits->getChild("unit", i);
						string name =
							unitNode->getAttribute("name")->getRestrictedValue();
						int minimum =
							unitNode->getAttribute("minimum")->getIntValue();

						mapAIBehaviorUnitCategories[aibcWarriorUnits].
							push_back(PairPUnitTypeInt(getUnitType(name), minimum));
					}
				}
				if (aiNode->hasChild("resource-producer-units") == true) {
					const XmlNode *aiNodeUnits =
						aiNode->getChild("resource-producer-units");
					for (int i = 0; i < (int) aiNodeUnits->getChildCount(); ++i) {
						if (logger.getCancelLoading())
							return;
						const XmlNode *unitNode = aiNodeUnits->getChild("unit", i);
						string name =
							unitNode->getAttribute("name")->getRestrictedValue();
						int minimum =
							unitNode->getAttribute("minimum")->getIntValue();

						mapAIBehaviorUnitCategories[aibcResourceProducerUnits].
							push_back(PairPUnitTypeInt(getUnitType(name), minimum));
					}
				}
				if (aiNode->hasChild("building-units") == true) {
					const XmlNode *aiNodeUnits = aiNode->getChild("building-units");
					for (int i = 0; i < (int) aiNodeUnits->getChildCount(); ++i) {
						if (logger.getCancelLoading())
							return;
						const XmlNode *unitNode = aiNodeUnits->getChild("unit", i);
						string name =
							unitNode->getAttribute("name")->getRestrictedValue();
						int minimum =
							unitNode->getAttribute("minimum")->getIntValue();

						mapAIBehaviorUnitCategories[aibcBuildingUnits].
							push_back(PairPUnitTypeInt(getUnitType(name), minimum));
					}
				}

				if (aiNode->hasChild("upgrades") == true) {
					const XmlNode *aiNodeUpgrades = aiNode->getChild("upgrades");
					for (int i = 0; i < (int) aiNodeUpgrades->getChildCount(); ++i) {
						if (logger.getCancelLoading())
							return;
						const XmlNode *upgradeNode =
							aiNodeUpgrades->getChild("upgrade", i);
						string name =
							upgradeNode->getAttribute("name")->getRestrictedValue();

						vctAIBehaviorUpgrades.push_back(getUpgradeType(name));
					}
				}
			}
		}
		if (SystemFlags::getSystemSettingType(SystemFlags::debugSystem).
			enabled)
			SystemFlags::OutputDebug(SystemFlags::debugSystem,
				"In [%s::%s Line: %d]\n",
				extractFileFromDirectoryPath(__FILE__).
				c_str(), __FUNCTION__, __LINE__);
	}

	string FactionType::getFunctionName(const XmlNode * scriptNode) {
		string name = scriptNode->getName();

		for (int i = 0; i < (int) scriptNode->getAttributeCount(); ++i) {
			name += "_" + scriptNode->getAttribute(i)->getValue();
		}
		return name;
	}

	int FactionType::
		getAIBehaviorStaticOverideValue(AIBehaviorStaticValueCategory type)
		const {
		int result = INT_MAX;
		std::map < AIBehaviorStaticValueCategory,
			int >::const_iterator iterFind =
			mapAIBehaviorStaticOverrideValues.find(type);
		if (iterFind != mapAIBehaviorStaticOverrideValues.end()) {
			result = iterFind->second;
		}
		return result;
	}

	const std::vector < FactionType::PairPUnitTypeInt >
		FactionType::getAIBehaviorUnits(AIBehaviorUnitCategory category) const {
		std::map < AIBehaviorUnitCategory,
			std::vector < PairPUnitTypeInt > >::const_iterator iterFind =
			mapAIBehaviorUnitCategories.find(category);
		if (iterFind != mapAIBehaviorUnitCategories.end()) {
			return iterFind->second;
		}
		return std::vector < FactionType::PairPUnitTypeInt >();
	}

	FactionType::~FactionType() {
		delete music;
		music = NULL;
	}

	std::vector < std::string > FactionType::validateFactionType() {
		std::vector < std::string > results;

		const uint32 MAX_BITRATE_WARNING = 200000;
		StrSound *factionMusic = getMusic();
		if (factionMusic != NULL
			&& factionMusic->getInfo()->getBitRate() > MAX_BITRATE_WARNING) {
			char szBuf[8096] = "";
			snprintf(szBuf, 8096,
				"The Faction [%s] has the music [%s]\nwhich has a bitrate of [%u] which may cause some sound drivers to crash, please use a bitrate of %d or less!",
				this->getName().c_str(),
				factionMusic->getFileName().c_str(),
				factionMusic->getInfo()->getBitRate(),
				MAX_BITRATE_WARNING);
			results.push_back(szBuf);
		}

		for (int i = 0; i < (int) unitTypes.size(); ++i) {
			UnitType & unitType = unitTypes[i];

			for (int i = 0;
				i < (int) unitType.getSelectionSounds().getSounds().size();
				++i) {
				StaticSound *sound = unitType.getSelectionSounds().getSounds()[i];
				if (sound != NULL
					&& sound->getInfo()->getBitRate() > MAX_BITRATE_WARNING) {
					char szBuf[8096] = "";
					snprintf(szBuf, 8096,
						"The Unit [%s] in Faction [%s] has the sound [%s]\nwhich has a bitrate of [%u] which may cause some sound drivers to crash, please use a bitrate of %d or less!",
						unitType.getName().c_str(), this->getName().c_str(),
						sound->getFileName().c_str(),
						sound->getInfo()->getBitRate(), MAX_BITRATE_WARNING);
					results.push_back(szBuf);
				}
			}
			for (int i = 0;
				i < (int) unitType.getCommandSounds().getSounds().size(); ++i) {
				StaticSound *sound = unitType.getCommandSounds().getSounds()[i];
				if (sound != NULL
					&& sound->getInfo()->getBitRate() > MAX_BITRATE_WARNING) {
					char szBuf[8096] = "";
					snprintf(szBuf, 8096,
						"The Unit [%s] in Faction [%s] has the sound [%s]\nwhich has a bitrate of [%u] which may cause some sound drivers to crash, please use a bitrate of %d or less!",
						unitType.getName().c_str(), this->getName().c_str(),
						sound->getFileName().c_str(),
						sound->getInfo()->getBitRate(), MAX_BITRATE_WARNING);
					results.push_back(szBuf);
				}
			}

			int morphCommandCount = 0;
			for (int j = 0; j < (int) unitType.getCommandTypeCount(); ++j) {
				const CommandType *cmdType = unitType.getCommandType(j);
				if (cmdType != NULL) {
					// Check every unit's commands to validate that for every upgrade-requirements
					// upgrade we have a unit that can do the upgrade in the faction.
					for (int k = 0; k < cmdType->getUpgradeReqCount(); ++k) {
						const UpgradeType *upgradeType = cmdType->getUpgradeReq(k);

						if (upgradeType != NULL) {
							// Now lets find a unit that can produced-upgrade this upgrade
							bool foundUpgraderUnit = false;
							for (int l = 0;
								l < (int) unitTypes.size()
								&& foundUpgraderUnit == false; ++l) {
								UnitType & unitType2 = unitTypes[l];
								for (int m = 0;
									m < unitType2.getCommandTypeCount()
									&& foundUpgraderUnit == false; ++m) {
									const CommandType *cmdType2 =
										unitType2.getCommandType(m);
									if (cmdType2 != NULL
										&& dynamic_cast <
										const UpgradeCommandType *>(cmdType2) != NULL) {
										const UpgradeCommandType *uct =
											dynamic_cast <const UpgradeCommandType *>(cmdType2);
										if (uct != NULL) {
											const UpgradeType *upgradeType2 =
												uct->getProducedUpgrade();
											if (upgradeType2 != NULL
												&& upgradeType2->getName() ==
												upgradeType->getName()) {
												foundUpgraderUnit = true;
												break;
											}
										}
									}
								}
							}

							if (foundUpgraderUnit == false) {
								char szBuf[8096] = "";
								snprintf(szBuf, 8096,
									"The Unit [%s] in Faction [%s] has the command [%s]\nwhich has upgrade requirement [%s] but there are no units able to perform the upgrade!",
									unitType.getName().c_str(),
									this->getName().c_str(),
									cmdType->getName().c_str(),
									upgradeType->getName().c_str());
								results.push_back(szBuf);
							}
						}
					}

					// Ensure for each build type command that the build units
					// exist in this faction
					if (cmdType->getClass() == ccBuild) {
						const BuildCommandType *build =
							dynamic_cast <const BuildCommandType *>(cmdType);
						if (build == NULL) {
							throw game_runtime_error("build == NULL");
						}
						for (int k = 0; k < build->getBuildingCount(); ++k) {
							const UnitType *buildUnit = build->getBuilding(k);

							// Now lets find the unit that we should be able to build
							bool foundUnit = false;
							for (int l = 0;
								l < (int) unitTypes.size() && foundUnit == false; ++l) {
								UnitType & unitType2 = unitTypes[l];
								if (unitType2.getName() == buildUnit->getName()) {
									foundUnit = true;

									// Now also validate the the unit to be built
									// has a be_built_skill
									if (buildUnit->hasSkillClass(scBeBuilt) == false) {
										char szBuf[8096] = "";
										snprintf(szBuf, 8096,
											"The Unit [%s] in Faction [%s] has the command [%s]\nwhich can build the Unit [%s] but the Unit to be built\ndoes not have the skill class [be_built_skill] in this faction!",
											unitType.getName().c_str(),
											this->getName().c_str(),
											cmdType->getName().c_str(),
											buildUnit->getName().c_str());
										results.push_back(szBuf);
									}

									break;
								}
							}

							if (foundUnit == false) {
								char szBuf[8096] = "";
								snprintf(szBuf, 8096,
									"The Unit [%s] in Faction [%s] has the command [%s]\nwhich can build the Unit [%s] but the Unit to be built does not exist in this faction!",
									unitType.getName().c_str(),
									this->getName().c_str(),
									cmdType->getName().c_str(),
									buildUnit->getName().c_str());
								results.push_back(szBuf);
							}
						}
					}
					// Ensure for each repair type command that the repair units
					// exist in this faction
					if (cmdType->getClass() == ccRepair) {
						const RepairCommandType *repair =
							dynamic_cast <const RepairCommandType *>(cmdType);
						if (repair == NULL) {
							throw game_runtime_error("repair == NULL");
						}
						for (int k = 0; k < repair->getRepairCount(); ++k) {
							const UnitType *repairUnit = repair->getRepair(k);

							// Now lets find the unit that we should be able to repair
							bool foundUnit = false;
							for (int l = 0;
								l < (int) unitTypes.size() && foundUnit == false; ++l) {
								UnitType & unitType2 = unitTypes[l];
								if (unitType2.getName() == repairUnit->getName()) {
									foundUnit = true;
									break;
								}
							}

							if (foundUnit == false) {
								char szBuf[8096] = "";
								snprintf(szBuf, 8096,
									"The Unit [%s] in Faction [%s] has the command [%s]\nwhich can repair the Unit [%s] but the Unit to be repaired does not exist in this faction!",
									unitType.getName().c_str(),
									this->getName().c_str(),
									cmdType->getName().c_str(),
									repairUnit->getName().c_str());
								results.push_back(szBuf);
							}
						}
					}
					// Ensure for each morph type command that the morph units
					// exist in this faction
					if (cmdType->getClass() == ccMorph) {
						const MorphCommandType *morph =
							dynamic_cast <const MorphCommandType *>(cmdType);
						if (morph != NULL) {
							morphCommandCount++;
							const UnitType *morphUnit = morph->getMorphUnit();

							// Now lets find the unit that we should be able to morph
							// to
							bool foundUnit = false;
							for (int l = 0;
								l < (int) unitTypes.size() && foundUnit == false; ++l) {
								UnitType & unitType2 = unitTypes[l];
								if (unitType2.getName() == morphUnit->getName()) {
									foundUnit = true;
									break;
								}
							}

							if (foundUnit == false) {
								char szBuf[8096] = "";
								snprintf(szBuf, 8096,
									"The Unit [%s] in Faction [%s] has the command [%s]\nwhich can morph into the Unit [%s] but the Unit to be morphed to does not exist in this faction!",
									unitType.getName().c_str(),
									this->getName().c_str(),
									cmdType->getName().c_str(),
									morphUnit->getName().c_str());
								results.push_back(szBuf);
							}
						}
					}
				}
			}

			const int maxMorphsAllowed = 6;
			if (morphCommandCount > maxMorphsAllowed) {
				char szBuf[8096] = "";
				snprintf(szBuf, 8096,
					"The Unit [%s] in Faction [%s] has more than %d morph commands which is too many to display in the UI!",
					unitType.getName().c_str(), this->getName().c_str(),
					maxMorphsAllowed);
				results.push_back(szBuf);
			}

			// Check every unit's unit requirements to validate that for every unit-requirements
			// we have the units required in the faction.
			for (int j = 0; j < unitType.getUnitReqCount(); ++j) {
				const UnitType *unitType2 = unitType.getUnitReq(j);
				if (unitType2 != NULL) {
					// Now lets find the required unit
					bool foundUnit = false;
					for (int l = 0; l < (int) unitTypes.size() && foundUnit == false;
						++l) {
						UnitType & unitType3 = unitTypes[l];

						if (unitType2->getName() == unitType3.getName()) {
							foundUnit = true;
							break;
						}
					}

					if (foundUnit == false) {
						char szBuf[8096] = "";
						snprintf(szBuf, 8096,
							"The Unit [%s] in Faction [%s] has the required Unit [%s]\nbut the required unit does not exist in this faction!",
							unitType.getName().c_str(),
							this->getName().c_str(),
							unitType2->getName().c_str());
						results.push_back(szBuf);
					}
				}
			}

			// Now check that at least 1 other unit can produce, build or morph this unit
			bool foundUnit = false;
			for (int l = 0; l < (int) unitTypes.size() && foundUnit == false;
				++l) {
				UnitType & unitType2 = unitTypes[l];

				for (int j = 0;
					j < unitType2.getCommandTypeCount() && foundUnit == false;
					++j) {
					const CommandType *cmdType = unitType2.getCommandType(j);
					if (cmdType != NULL) {
						// Check if this is a produce command
						if (cmdType->getClass() == ccProduce) {
							const ProduceCommandType *produce =
								dynamic_cast <const ProduceCommandType *>(cmdType);
							if (produce != NULL) {
								const UnitType *produceUnit = produce->getProducedUnit();

								if (produceUnit != NULL &&
									unitType.getId() != unitType2.getId() &&
									unitType.getName() == produceUnit->getName()) {
									foundUnit = true;
									break;
								}
							}
						}
						// Check if this is a build command
						if (cmdType->getClass() == ccBuild) {
							const BuildCommandType *build =
								dynamic_cast <const BuildCommandType *>(cmdType);
							if (build == NULL) {
								throw game_runtime_error("build == NULL");
							}
							for (int k = 0;
								k < build->getBuildingCount() && foundUnit == false;
								++k) {
								const UnitType *buildUnit = build->getBuilding(k);

								if (buildUnit != NULL &&
									unitType.getId() != unitType2.getId() &&
									unitType.getName() == buildUnit->getName()) {
									foundUnit = true;
									break;
								}
							}
							if (foundUnit == true) {
								break;
							}
						}
						// Check if this is a morph command
						if (cmdType->getClass() == ccMorph) {
							const MorphCommandType *morph =
								dynamic_cast <const MorphCommandType *>(cmdType);
							if (morph == NULL) {
								throw game_runtime_error("morph == NULL");
							}
							const UnitType *morphUnit = morph->getMorphUnit();

							if (morphUnit != NULL &&
								unitType.getId() != unitType2.getId() &&
								unitType.getName() == morphUnit->getName()) {
								foundUnit = true;
								break;
							}
						}

						// Check if this is an attack command with spawned units on attack
						if (cmdType->getClass() == ccAttack) {
							const AttackCommandType *act =
								dynamic_cast <const AttackCommandType *>(cmdType);
							if (act != NULL && act->getAttackSkillType() != NULL
								&& act->getAttackSkillType()->getSpawnUnit() != ""
								&& act->getAttackSkillType()->getSpawnUnitCount() > 0) {

								if (unitType.getId() != unitType2.getId() &&
									unitType.getName() ==
									act->getAttackSkillType()->getSpawnUnit()) {
									foundUnit = true;
									break;
								}
							}
						}
					}
				}
			}

			if (foundUnit == false) {
				//printf("Problem for unit [%s] unitTypes.size() = " SIZE_T_SPECIFIER "\n",unitType.getName().c_str(),unitTypes.size());

				char szBuf[8096] = "";
				snprintf(szBuf, 8096,
					"The Unit [%s] in Faction [%s] has no other units that can produce, build or morph into it in this faction!",
					unitType.getName().c_str(), this->getName().c_str());
				results.push_back(szBuf);
			}

			// Ensure that all attack skill types have valid values
			if (unitType.hasSkillClass(scAttack) == true) {
				for (int j = 0; j < unitType.getSkillTypeCount(); ++j) {
					const SkillType *st = unitType.getSkillType(j);
					if (st != NULL
						&& dynamic_cast <const AttackSkillType *>(st) != NULL) {
						const AttackSkillType *ast =
							dynamic_cast <const AttackSkillType *>(st);
						if (ast != NULL && ast->getAttackVar() < 0) {
							char szBuf[8096] = "";
							snprintf(szBuf, 8096,
								"The Unit [%s] in Faction [%s] has the skill [%s] with an INVALID attack var value which is < 0 [%d]!",
								unitType.getName().c_str(),
								this->getName().c_str(), ast->getName().c_str(),
								ast->getAttackVar());
							results.push_back(szBuf);
						}
					}
				}
			}
			// end

			// Check if the unit has both be_built and harvest skills, this may cause issues
			// with the AI
			if (unitType.hasSkillClass(scBeBuilt) == true
				&& unitType.hasSkillClass(scHarvest) == true) {
				char szBuf[8096] = "";
				snprintf(szBuf, 8096,
					"The Unit [%s] in Faction [%s] has both a bebuilt and harvest skill which will cause AI problems for CPU players!",
					unitType.getName().c_str(), this->getName().c_str());
				results.push_back(szBuf);
			}
			// end

			// Check if the unit has harvest skills but not move, meaning they cannot
			// harvest the resource
			if (unitType.hasSkillClass(scHarvest) == true
				&& unitType.hasSkillClass(scMove) == false) {
				char szBuf[8096] = "";
				snprintf(szBuf, 8096,
					"The Unit [%s] in Faction [%s] has a harvest skill but no move skill so it cannot harvest!",
					unitType.getName().c_str(), this->getName().c_str());
				results.push_back(szBuf);
			}
			// end

		}

		return results;
	}

	std::vector < std::string >
		FactionType::validateFactionTypeResourceTypes(vector < ResourceType >
			&resourceTypes) {
		std::vector < std::string > results;

		for (int i = 0; i < (int) unitTypes.size(); ++i) {
			UnitType & unitType = unitTypes[i];

			// Check every unit's required resources to validate that for every resource-requirements
			// we have a resource in the faction.
			for (int j = 0; j < unitType.getCostCount(); ++j) {
				const Resource *r = unitType.getCost(j);
				if (r != NULL && r->getType() != NULL) {
					bool foundResourceType = false;
					// Now lets find a matching faction resource type for the unit
					for (int k = 0; k < (int) resourceTypes.size(); ++k) {
						ResourceType & rt = resourceTypes[k];

						if (r->getType()->getName() == rt.getName()) {
							foundResourceType = true;
							break;
						}
					}

					if (foundResourceType == false) {
						char szBuf[8096] = "";
						snprintf(szBuf, 8096,
							"The Unit [%s] in Faction [%s] has the resource req [%s]\nbut there are no such resources in this tech!",
							unitType.getName().c_str(),
							this->getName().c_str(),
							r->getType()->getName().c_str());
						results.push_back(szBuf);
					}
				}
			}

			// Check every unit's stored resources to validate that for every resources-stored
			// we have a resource in the faction.
			for (int j = 0; j < unitType.getStoredResourceCount(); ++j) {
				const Resource *r = unitType.getStoredResource(j);
				if (r != NULL && r->getType() != NULL) {
					bool foundResourceType = false;
					// Now lets find a matching faction resource type for the unit
					for (int k = 0; k < (int) resourceTypes.size(); ++k) {
						ResourceType & rt = resourceTypes[k];

						if (r->getType()->getName() == rt.getName()) {
							foundResourceType = true;
							break;
						}
					}

					if (foundResourceType == false) {
						char szBuf[8096] = "";
						snprintf(szBuf, 8096,
							"The Unit [%s] in Faction [%s] has the stored resource [%s]\nbut there are no such resources in this tech!",
							unitType.getName().c_str(),
							this->getName().c_str(),
							r->getType()->getName().c_str());
						results.push_back(szBuf);
					}
				}
			}

			for (int j = 0; j < unitType.getCommandTypeCount(); ++j) {
				const CommandType *cmdType = unitType.getCommandType(j);
				if (cmdType != NULL) {
					// Ensure for each harvest type command that the resource
					// exist in this faction
					if (cmdType->getClass() == ccHarvest) {
						const HarvestCommandType *harvest =
							dynamic_cast <const HarvestCommandType *>(cmdType);
						if (harvest == NULL) {
							throw game_runtime_error("harvest == NULL");
						}
						for (int k = 0; k < harvest->getHarvestedResourceCount(); ++k) {
							const ResourceType *harvestResource =
								harvest->getHarvestedResource(k);

							bool foundResourceType = false;
							// Now lets find a matching faction resource type for the unit
							for (int k = 0; k < (int) resourceTypes.size(); ++k) {
								ResourceType & rt = resourceTypes[k];

								if (harvestResource->getName() == rt.getName()) {
									foundResourceType = true;
									break;
								}
							}

							if (foundResourceType == false) {
								char szBuf[8096] = "";
								snprintf(szBuf, 8096,
									"The Unit [%s] in Faction [%s] has the command [%s] which can harvest the resource [%s]\nbut there are no such resources in this tech!",
									unitType.getName().c_str(),
									this->getName().c_str(),
									cmdType->getName().c_str(),
									harvestResource->getName().c_str());
								results.push_back(szBuf);
							}
						}
					}
				}
			}
		}

		return results;
	}

	std::vector < std::string >
		FactionType::validateFactionTypeUpgradeTypes() {
		std::vector < std::string > results;

		// For each upgrade type make sure there is at least 1 unit that can produce
		// the upgrade
		for (int i = 0; i < (int) upgradeTypes.size(); ++i) {
			const UpgradeType & upgradeType = upgradeTypes[i];

			// First find a unit with a command type to upgrade to this Upgrade type
			bool foundUnit = false;
			for (int j = 0; j < (int) unitTypes.size() && foundUnit == false;
				++j) {
				UnitType & unitType = unitTypes[j];
				for (int k = 0;
					k < unitType.getCommandTypeCount() && foundUnit == false; ++k) {
					const CommandType *cmdType = unitType.getCommandType(k);
					if (cmdType != NULL) {
						// Ensure for each build type command that the build units
						// exist in this faction
						if (cmdType->getClass() == ccUpgrade) {
							const UpgradeCommandType *upgrade =
								dynamic_cast <const UpgradeCommandType *>(cmdType);
							if (upgrade != NULL) {
								const UpgradeType *upgradeType2 =
									upgrade->getProducedUpgrade();

								if (upgradeType2 != NULL
									&& upgradeType.getName() == upgradeType2->getName()) {
									foundUnit = true;
									break;
								}
							}
						}
					}
				}
			}

			if (foundUnit == false) {
				char szBuf[8096] = "";
				snprintf(szBuf, 8096,
					"The Upgrade Type [%s] in Faction [%s] has no Unit able to produce this upgrade in this faction!",
					upgradeType.getName().c_str(),
					this->getName().c_str());
				results.push_back(szBuf);
			}
		}

		return results;
	}

	// ==================== get ====================

	const UnitType *FactionType::getUnitType(const string & name) const {
		for (int i = 0; i < (int) unitTypes.size(); i++) {
			if (unitTypes[i].getName(false) == name) {
				return &unitTypes[i];
			}
		}

		printf("In [%s::%s Line: %d] scanning [%s] size = " SIZE_T_SPECIFIER
			"\n", extractFileFromDirectoryPath(__FILE__).c_str(),
			__FUNCTION__, __LINE__, name.c_str(), unitTypes.size());
		for (int i = 0; i < (int) unitTypes.size(); i++) {
			printf("In [%s::%s Line: %d] scanning [%s] idx = %d [%s]\n",
				extractFileFromDirectoryPath(__FILE__).c_str(),
				__FUNCTION__, __LINE__, name.c_str(), i,
				unitTypes[i].getName(false).c_str());
		}

		if (SystemFlags::getSystemSettingType(SystemFlags::debugSystem).
			enabled)
			SystemFlags::OutputDebug(SystemFlags::debugSystem,
				"In [%s::%s Line: %d] scanning [%s] size = %d\n",
				extractFileFromDirectoryPath(__FILE__).
				c_str(), __FUNCTION__, __LINE__,
				name.c_str(), unitTypes.size());
		for (int i = 0; i < (int) unitTypes.size(); i++) {
			if (SystemFlags::getSystemSettingType(SystemFlags::debugSystem).
				enabled)
				SystemFlags::OutputDebug(SystemFlags::debugSystem,
					"In [%s::%s Line: %d] scanning [%s] idx = %d [%s]\n",
					extractFileFromDirectoryPath(__FILE__).
					c_str(), __FUNCTION__, __LINE__,
					name.c_str(), i,
					unitTypes[i].getName(false).c_str());
		}

		throw game_runtime_error("Unit type not found: [" + name +
			"] in faction type [" + this->name + "]",
			true);
	}

	//const UnitType *FactionType::getUnitTypeById(int id) const{
	//    for(int i=0; i < (int)unitTypes.size();i++){
	//              if(unitTypes[i].getId() == id) {
	//            return &unitTypes[i];
	//              }
	//    }
	//
	//    printf("In [%s::%s Line: %d] scanning [%d] size = " SIZE_T_SPECIFIER "\n",extractFileFromDirectoryPath(__FILE__).c_str(),__FUNCTION__,__LINE__,id,unitTypes.size());
	//    for(int i=0; i < (int)unitTypes.size();i++){
	//      printf("In [%s::%s Line: %d] scanning [%s] idx = %d [%s][%d]\n",extractFileFromDirectoryPath(__FILE__).c_str(),__FUNCTION__,__LINE__,name.c_str(),i,unitTypes[i].getName(false).c_str(),unitTypes[i].getId());
	//    }
	//
	//    if(SystemFlags::getSystemSettingType(SystemFlags::debugSystem).enabled) SystemFlags::OutputDebug(SystemFlags::debugSystem,"In [%s::%s Line: %d] scanning [%s] size = %d\n",extractFileFromDirectoryPath(__FILE__).c_str(),__FUNCTION__,__LINE__,name.c_str(),unitTypes.size());
	//    for(int i=0; i < (int)unitTypes.size();i++){
	//      if(SystemFlags::getSystemSettingType(SystemFlags::debugSystem).enabled) SystemFlags::OutputDebug(SystemFlags::debugSystem,"In [%s::%s Line: %d] scanning [%s] idx = %d [%s]\n",extractFileFromDirectoryPath(__FILE__).c_str(),__FUNCTION__,__LINE__,name.c_str(),i,unitTypes[i].getName(false).c_str());
	//    }
	//
	//      throw game_runtime_error("Unit type not found: [" + intToStr(id) + "] in faction type [" + this->name + "]",true);
	//}

	const UpgradeType *FactionType::getUpgradeType(const string & name) const {
		for (int i = 0; i < (int) upgradeTypes.size(); i++) {
			if (upgradeTypes[i].getName() == name) {
				return &upgradeTypes[i];
			}
		}

		printf("In [%s::%s Line: %d] scanning [%s] size = " SIZE_T_SPECIFIER
			"\n", extractFileFromDirectoryPath(__FILE__).c_str(),
			__FUNCTION__, __LINE__, name.c_str(), unitTypes.size());
		for (int i = 0; i < (int) upgradeTypes.size(); i++) {
			printf("In [%s::%s Line: %d] scanning [%s] idx = %d [%s]\n",
				extractFileFromDirectoryPath(__FILE__).c_str(),
				__FUNCTION__, __LINE__, name.c_str(), i,
				upgradeTypes[i].getName().c_str());
		}

		if (SystemFlags::getSystemSettingType(SystemFlags::debugSystem).
			enabled)
			SystemFlags::OutputDebug(SystemFlags::debugSystem,
				"In [%s::%s Line: %d] scanning [%s] size = %d\n",
				extractFileFromDirectoryPath(__FILE__).
				c_str(), __FUNCTION__, __LINE__,
				name.c_str(), unitTypes.size());
		for (int i = 0; i < (int) upgradeTypes.size(); i++) {
			if (SystemFlags::getSystemSettingType(SystemFlags::debugSystem).
				enabled)
				SystemFlags::OutputDebug(SystemFlags::debugSystem,
					"In [%s::%s Line: %d] scanning [%s] idx = %d [%s]\n",
					extractFileFromDirectoryPath(__FILE__).
					c_str(), __FUNCTION__, __LINE__,
					name.c_str(), i,
					upgradeTypes[i].getName().c_str());
		}

		throw game_runtime_error("Upgrade type not found: [" + name +
			"] in faction type [" + this->name + "]",
			true);
	}

	int FactionType::getStartingResourceAmount(const ResourceType *
		resourceType) const {
		for (int i = 0; i < (int) startingResources.size(); ++i) {
			if (startingResources[i].getType() == resourceType) {
				return startingResources[i].getAmount();
			}
		}
		return 0;
	}

	void FactionType::deletePixels() {
		for (int i = 0; i < (int) unitTypes.size(); ++i) {
			UnitType & unitType = unitTypes[i];
			Texture2D *texture = unitType.getMeetingPointImage();
			if (texture != NULL) {
				texture->deletePixels();
			}
		}
	}

	bool FactionType::factionUsesResourceType(const ResourceType * rt) const {
		bool factionUsesResourceType = false;
		if (rt != NULL) {
			for (unsigned int j = 0;
				factionUsesResourceType == false
				&& j < (unsigned int) this->getUnitTypeCount(); ++j) {
				const UnitType *ut = this->getUnitType(j);
				for (int k = 0;
					factionUsesResourceType == false && k < ut->getCostCount();
					++k) {
					const Resource *costResource = ut->getCost(k);
					//printf("#1 factionUsesResourceType, unit [%s] resource [%s] cost [%s]\n",ut->getName().c_str(),rt->getName().c_str(),costResource->getType()->getName().c_str());

					if (costResource != NULL && costResource->getType() != NULL &&
						costResource->getType()->getName() == rt->getName()) {
						factionUsesResourceType = true;
						break;
					}
				}
				if (factionUsesResourceType == false) {
					for (unsigned int k = 0;
						factionUsesResourceType == false
						&& k < (unsigned int) ut->getCommandTypeCount(); ++k) {
						const CommandType *commandType = ut->getCommandType(k);
						if (commandType != NULL
							&& commandType->getClass() == ccHarvest) {
							const HarvestCommandType *hct =
								dynamic_cast <const HarvestCommandType *>(commandType);
							if (hct != NULL && hct->getHarvestedResourceCount() > 0) {
								for (unsigned int l = 0;
									factionUsesResourceType == false
									&& l <
									(unsigned int) hct->getHarvestedResourceCount(); ++l) {
									//printf("#2 factionUsesResourceType, unit [%s] resource [%s] harvest [%s]\n",ut->getName().c_str(),rt->getName().c_str(),hct->getHarvestedResource(l)->getName().c_str());

									if (hct->getHarvestedResource(l)->getName() ==
										rt->getName()) {
										factionUsesResourceType = true;
										break;
									}
								}
							}
						}
					}
				}
			}
		}
		return factionUsesResourceType;
	}

	std::string FactionType::toString()const {
		std::string result = "Faction Name: " + name + "\n";

		result +=
			"Unit Type List count = " + intToStr(this->getUnitTypeCount()) +
			"\n";
		for (int i = 0; i < (int) unitTypes.size(); i++) {
			result += unitTypes[i].toString() + "\n";
		}

		result +=
			"Upgrade Type List count = " +
			intToStr(this->getUpgradeTypeCount()) + "\n";
		for (int i = 0; i < (int) upgradeTypes.size(); i++) {
			result +=
				"index: " + intToStr(i) + " " +
				upgradeTypes[i].getReqDesc(false) + "\n";
		}

		return result;
	}

	string FactionType::getName(bool translatedValue) const {
		if (translatedValue == false)
			return name;

		Lang & lang = Lang::getInstance();
		return lang.getTechTreeString("FactionName_" + name, name.c_str());
	}


} //end namespace
