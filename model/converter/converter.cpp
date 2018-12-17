/*
 *  Picsou | Keep track of your expenses !
 *  Copyright (C) 2018  koromodako
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "converter.h"
#include "converters.h"
#include "utils/macro.h"

#include <QList>
#include <QPair>
#include <QJsonObject>

#define makeConverterPair(version, converter) \
    QPair<SemVer, db_converter_t>((version), (converter))

bool Converter::convert(QJsonDocument *doc, SemVer from, PicsouUIService *ui_svc)
{
    LOG_IN("doc="<<doc<<",from="<<from);
    /* initialize convesion list */
    bool convert;
    QList<QPair<SemVer, db_converter_t>> converter_list;
    converter_list.append(makeConverterPair(SemVer(1, 0, 0), convert_100_110));
    converter_list.append(makeConverterPair(SemVer(1, 1, 0), convert_110_200));
    /* apply conversions */
    LOG_DEBUG("attempting conversion.");
    convert=false;
    for(const auto &converter : converter_list) {
        LOG_DEBUG(converter.first.to_str()<<" == "<<from.to_str()<<" : "<<(converter.first==from));
        if(converter.first==from) {
            convert=true;
        }
        if(convert) {
            LOG_DEBUG("applying converter for version: "<<converter.first.to_str());
            converter.second(doc, ui_svc);
        }
    }
    LOG_BOOL_RETURN(convert);
}

#undef makeConverterPair
