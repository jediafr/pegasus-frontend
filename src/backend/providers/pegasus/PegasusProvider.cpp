// Pegasus Frontend
// Copyright (C) 2017-2018  Mátyás Mustoha
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.


#include "PegasusProvider.h"

#include "LocaleUtils.h"
#include "Paths.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>


namespace providers {
namespace pegasus {

PegasusProvider::PegasusProvider(QObject* parent)
    : Provider(parent)
{
    load_game_dir_list();
}

void PegasusProvider::load_game_dir_list()
{
    constexpr int LINE_MAX_LEN = 4096;

    for (QString& path : paths::configDirs()) {
        path += QStringLiteral("/game_dirs.txt");

        QFile config_file(path);
        if (!config_file.open(QFile::ReadOnly | QFile::Text))
            continue;

        qInfo().noquote() << tr_log("Found `%1`").arg(path);

        QTextStream stream(&config_file);
        QString line;
        while (stream.readLineInto(&line, LINE_MAX_LEN)) {
            if (!line.startsWith('#'))
                add_game_dir(line);
        }
    }
}

void PegasusProvider::add_game_dir(const QString& dir_path)
{
    const QFileInfo entry(dir_path);
    if (!entry.exists() || !entry.isDir()) {
        qWarning().noquote() << tr_log("Game directory `%1` not found, ignored").arg(dir_path);
        return;
    }

    m_game_dirs << entry.canonicalFilePath();
}

void PegasusProvider::findLists(HashMap<QString, modeldata::Game>& games,
                                HashMap<QString, modeldata::Collection>& collections,
                                HashMap<QString, std::vector<QString>>& collection_childs)
{
    m_game_dirs.removeDuplicates();
    collection_finder.find_in_dirs(m_game_dirs, games, collections, collection_childs,
                                   [this](int game_count){ emit gameCountChanged(game_count); });
}

void PegasusProvider::findStaticData(HashMap<QString, modeldata::Game>& games,
                                     const HashMap<QString, modeldata::Collection>& collections,
                                     const HashMap<QString, std::vector<QString>>& collection_childs)
{
    metadata_finder.enhance_in_dirs(m_game_dirs, games, collections, collection_childs);
}

} // namespace pegasus
} // namespace providers
