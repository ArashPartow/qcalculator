/**
 *  QCalc - Extensible programming calculator
 *  Copyright (C) 2023  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef QCALC_ADDONMANAGER_HPP
#define QCALC_ADDONMANAGER_HPP

#include <set>
#include <string>
#include <functional>
#include <atomic>
#include <map>
#include <io/archive.hpp>

#include "addon.hpp"

struct AddonBundleEntry {
    std::string packagePath;
    int version;
};

class AddonManager {
public:
    struct InstallBundleEntry {
        std::string packagePath;
        int version;
    };

    static Archive createInstallableBundle(std::vector<InstallBundleEntry> entries);

    typedef std::function<void(const std::string &, const std::string &)> Listener;

    AddonManager() = default;

    AddonManager(std::string addonDirectory,
                 Listener onAddonLoadFail,
                 Listener onAddonUnloadFail);

    ~AddonManager();

    void reloadModules();

    const std::map<std::string, Addon> &getAvailableAddons() const;

    std::map<std::string, Addon> &getAvailableAddons();

    /**
     * @param inputAddons The set of addon module names which should currently be loaded.
     */
    void setActiveAddons(const std::set<std::string> &inputAddons);

    /**
     * @return The set of currently loaded module names.
     */
    std::set<std::string> getActiveAddons();

    /**
     * Install addons from a addon bundle.
     * A addon bundle must be an archive format supported by libarchive
     * and contain the file
     * "addon_bundle.json" of the following format:
     *
     * {
     *      "addons": [
     *          // Addon declaration, add one declaration for each addon that the user can enable, disable or uninstall any time through the gui
     *          {
     *              // The path to the addon package in the archive, the package must contain a python module with the same name as the package containing the load() / unload() callbacks
     *              "package": "MyDirectory/OtherDirectory/MyAddon/", // Must contain MyAddon.py
     *
     *              // The version displayed to the user (optional)
     *              "version": 1
     *          },
     *          ...
     *      ],
     *
     *      // The version of the addon install bundle format
     *      "bundleVersion": 0
     * }
     *
     * @param sourceFile
     * @param questionDialog
     */
    size_t installAddonBundle(std::istream &sourceFile,
                              const std::function<bool(const std::string &, const std::string &,
                                                       std::vector<std::string> &)> &installDialog);


    void uninstallAddon(const std::string &moduleName);

private:
    void readAddons();

    std::string addonDir;

    std::map<std::string, Addon> addons;
    std::set<std::string> activeAddons;
    std::set<std::string> addonLibraryPaths;

    Listener onAddonLoadFail;
    Listener onAddonUnloadFail;
};

#endif //QCALC_ADDONMANAGER_HPP
